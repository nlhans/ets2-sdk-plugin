/**
 * @brief Simple logger.
 *
 * Writes the output into file inside the current directory.
 */

// Windows stuff.

#define WINVER 0x0500
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

// SDK

#include "scssdk_telemetry.h"
#include "eurotrucks2/scssdk_eut2.h"
#include "eurotrucks2/scssdk_telemetry_eut2.h"

// Plug-in
#include "ets2-telemetry-common.hpp"
#include "sharedmemory.hpp"
#include "scs_config_handlers.hpp"

#define UNUSED(x)

/**
 * These macro's are a shortcut to register channels inside the scs_telemetry_init function
 * They require the channel definition name (without prefix SCS_TELEMETRY_), type and destination.
 * Not all channel types are implemented; the handler function for a type should be created like so:
 * telemetry_store_[Type](const scs_string_t name, const scs_u32_t index, const scs_value_t *const value, const scs_context_t context)
 *
 * RegisterSpecificChannel allows for your own handler name, without the telemetry_store_ prefix.
 */
#define registerChannel(name, type, to) version_params->register_for_channel(SCS_TELEMETRY_##name, SCS_U32_NIL, SCS_VALUE_TYPE_##type, SCS_TELEMETRY_CHANNEL_FLAG_no_value, telemetry_store_##type, &( to ));
#define registerSpecificChannel(name, type, handler, to) version_params->register_for_channel(SCS_TELEMETRY_##name, SCS_U32_NIL, SCS_VALUE_TYPE_##type, SCS_TELEMETRY_CHANNEL_FLAG_no_value, handler, &( to ));

SharedMemory *telemMem;
ets2TelemetryMap_t *telemPtr;
const wchar_t* ets2MmfName = ETS2_PLUGIN_MMF_NAME;

static bool onJob;

/**
 * @brief Last timestamp we received.
 */
scs_timestamp_t last_timestamp = static_cast<scs_timestamp_t>(-1);
scs_timestamp_t timestamp;

#ifdef SDK_ENABLE_LOGGING

FILE *log_file = NULL;
#endif


SCSAPI_VOID telemetry_frame_start(const scs_event_t UNUSED(event), const void *const event_info, const scs_context_t UNUSED(context))
{
	static int clearJobTicker = 0;
	const struct scs_telemetry_frame_start_t *const info = static_cast<const scs_telemetry_frame_start_t *>(event_info);

	// The following processing of the timestamps is done so the output
	// from this plugin has continuous time, it is not necessary otherwise.

	// When we just initialized itself, assume that the time started
	// just now.

	if (last_timestamp == static_cast<scs_timestamp_t>(-1)) {
		last_timestamp = info->paused_simulation_time;
	}

	// The timer might be sometimes restarted (e.g. after load) while
	// we want to provide continuous time on our output.

	if (info->flags & SCS_TELEMETRY_FRAME_START_FLAG_timer_restart) {
		last_timestamp = 0;
	}

	// Advance the timestamp by delta since last frame.

	timestamp += (info->paused_simulation_time - last_timestamp);
	last_timestamp = info->paused_simulation_time;

	/* Copy over the game timestamp to our telemetry memory */
	if (telemPtr != NULL)
	{
		telemPtr->time = (unsigned int) timestamp;

		// Do a non-convential periodic update of this field:
		telemPtr->tel_rev3.cruiseControl = (telemPtr->tel_rev3.cruiseControlSpeed > 0) ? true : false;

		// Check if job could be finished ; if so empty the job field info
		if (telemPtr->tel_rev5.onJob == true && telemPtr->tel_rev1.trailer_attached == false && telemPtr->tel_rev4.routeDistance <= 0.1f && telemPtr->tel_rev4.routeDistance >= 0.0f)
		{
			// if was carrying cargo and not anymore with navigation distance close to zero; 
			// then we assume the job has finished
			// we allow some frames (see ticker) for the client to retrieve data
			telemPtr->tel_rev5.onJob = false;

			telemPtr->tel_rev5.jobFinished = true;
			clearJobTicker = 0;
		}
		else if (telemPtr->tel_rev5.jobFinished)
		{
			clearJobTicker ++;

			if (clearJobTicker > 10)
			{
				telemPtr->tel_rev2.jobIncome = 0;
				telemPtr->tel_rev2.time_abs_delivery = 0;
				telemPtr->tel_rev2.trailerMass = 0;
			
				memset(telemPtr->tel_rev2.trailerId, 0, 64);
				memset(telemPtr->tel_rev2.trailerName, 0, 64);

				memset(telemPtr->tel_rev2.citySrc, 0, 64); // TODO: put 64-byte into global define
				memset(telemPtr->tel_rev2.cityDst, 0, 64);
				memset(telemPtr->tel_rev2.compSrc, 0, 64);
				memset(telemPtr->tel_rev2.compDst, 0, 64);

				telemPtr->tel_rev5.jobFinished = false;
			}
		}
		else
		{
			if (telemPtr->tel_rev2.jobIncome != 0 && telemPtr->tel_rev1.trailer_attached)
			{
				telemPtr->tel_rev5.onJob = true;
			}
		}

	}

}

SCSAPI_VOID telemetry_pause(const scs_event_t event, const void *const UNUSED(event_info), const scs_context_t UNUSED(context))
{
	if (telemPtr != NULL)
	{
		telemPtr->paused = (event == SCS_TELEMETRY_EVENT_paused) ? 1 : 0;
	}
}

SCSAPI_VOID telemetry_configuration(const scs_event_t event, const void *const event_info, const scs_context_t UNUSED(context))
{
	// This method prints all available attributes of the truck.
	// On configuration change, this function is called.
    const struct scs_telemetry_configuration_t *const info = static_cast<const scs_telemetry_configuration_t *>(event_info);

#ifdef SDK_ENABLE_LOGGING
	fprintf(log_file,"----\n");
#endif

    for (const scs_named_value_t *current = info->attributes; current->name; ++current)
	{
#ifdef SDK_ENABLE_LOGGING
		fprintf(log_file, "Name: %s / Val: ", current->name);
		switch(current->value.type)
		{
			case SCS_VALUE_TYPE_bool:
				fprintf(log_file, "%c (bool)", ((current->value.value_bool.value) ? '1':'0'));
				break;

			case SCS_VALUE_TYPE_s32:
				fprintf(log_file, "%l (s32)", current->value.value_s32.value);
				break;

			case SCS_VALUE_TYPE_u32:
				fprintf(log_file, "%lu (u32)", current->value.value_u32.value);
				break;

			case SCS_VALUE_TYPE_u64:
				fprintf(log_file, "%lu (u64)", current->value.value_u64.value);
				break;

			case SCS_VALUE_TYPE_float:
				fprintf(log_file, "%f (float)", current->value.value_float.value);
				break;

			case SCS_VALUE_TYPE_double:
				fprintf(log_file, "%f (double)", current->value.value_double.value);
				break;

				/*
			case SCS_VALUE_TYPE_fvector:
				fprintf(log_file, "%s (string)", current->value.value_string.value);
				break;

			case SCS_VALUE_TYPE_dvector:
				fprintf(log_file, "%s (string)", current->value.value_string.value);
				break;

			case SCS_VALUE_TYPE_euler:
				fprintf(log_file, "%s (string)", current->value.value_string.value);
				break;

			case SCS_VALUE_TYPE_fplacement:
				fprintf(log_file, "%s (string)", current->value.value_string.value);
				break;

			case SCS_VALUE_TYPE_dplacement:
				fprintf(log_file, "%s (string)", current->value.value_string.value);
				break;*/

			case SCS_VALUE_TYPE_string:
				fprintf(log_file, "%s (string)", current->value.value_string.value);
				break;

			default:
				fprintf(log_file, "???? (%d)", current->value.type);
				break;
		}

		fprintf(log_file, "\r\n");

		fflush(log_file);
#endif
		handleCfg(current);
	}
}

/******* STORING OF SEVERAL SCS DATA TYPES *******/
SCSAPI_VOID telemetry_store_float(const scs_string_t name, const scs_u32_t index, const scs_value_t *const value, const scs_context_t context)
{
	if(!value) return;
	assert(value);
	assert(value->type == SCS_VALUE_TYPE_float);
	assert(context);
	*static_cast<float *>(context) = value->value_float.value;
}

SCSAPI_VOID telemetry_store_s32(const scs_string_t name, const scs_u32_t index, const scs_value_t *const value, const scs_context_t context)
{
	if(!value) return;
	assert(value);
	assert(value->type == SCS_VALUE_TYPE_s32);
	assert(context);
	*static_cast<int *>(context) = value->value_s32.value;
}

SCSAPI_VOID telemetry_store_u32(const scs_string_t name, const scs_u32_t index, const scs_value_t *const value, const scs_context_t context)
{
	if(!value) return;
	assert(value);
	assert(value->type == SCS_VALUE_TYPE_u32);
	assert(context);
	*static_cast<unsigned int *>(context) = value->value_u32.value;
}

SCSAPI_VOID telemetry_store_bool(const scs_string_t name, const scs_u32_t index, const scs_value_t *const value, const scs_context_t context)
{
	if(!context) return;
	/*assert(value);
	assert(value->type == SCS_VALUE_TYPE_bool);*/
	//assert(context);
	if (value)
	{
		if (value->value_bool.value == 0)
		{
			*static_cast<bool *>(context) = false;
		}
		else
		{
			*static_cast<bool *>(context) = true;
		}
	}
	else
	{
		*static_cast<bool *>(context) = false;
	}
}

SCSAPI_VOID telemetry_store_fvector(const scs_string_t name, const scs_u32_t index, const scs_value_t *const value, const scs_context_t context)
{
	if(!value) return;
	assert(value);
	assert(value->type == SCS_VALUE_TYPE_fvector);
	assert(context);
	*(static_cast<float *>(context)+0) = value->value_fvector.x;
	*(static_cast<float *>(context)+1) = value->value_fvector.y;
	*(static_cast<float *>(context)+2) = value->value_fvector.z;
}

SCSAPI_VOID telemetry_store_dplacement(const scs_string_t name, const scs_u32_t index, const scs_value_t *const value, const scs_context_t context)
{
	if(!value) return;
	assert(value);
	assert(value->type == SCS_VALUE_TYPE_dplacement);
	assert(context);

	// Messy hack to store the acceleration and orientation values into our telemetry struct
	// It is neccesary that these are put together, otherwise it may overwrite over values.
	*(static_cast<float *>(context)+0) = (float)value->value_dplacement.position.x;
	*(static_cast<float *>(context)+1) = (float)value->value_dplacement.position.y;
	*(static_cast<float *>(context)+2) = (float)value->value_dplacement.position.z;

	*(static_cast<float *>(context)+3) = value->value_dplacement.orientation.heading;
	*(static_cast<float *>(context)+4) = value->value_dplacement.orientation.pitch;
	*(static_cast<float *>(context)+5) = value->value_dplacement.orientation.roll;
}


/**
 * @brief Telemetry API initialization function.
 *
 * See scssdk_telemetry.h
 */

SCSAPI_RESULT scs_telemetry_init(const scs_u32_t version, const scs_telemetry_init_params_t *const params)
{
	// We currently support only one version.
	if (version != SCS_TELEMETRY_VERSION_1_00) {
		return SCS_RESULT_unsupported;
	}

	const scs_telemetry_init_params_v100_t *const version_params = static_cast<const scs_telemetry_init_params_v100_t *>(params);

    if (version_params == NULL)
    {
        return SCS_RESULT_generic_error;
    }

	/*** ACQUIRE SHARED MEMORY BUFFER ***/
	telemMem = new SharedMemory(ets2MmfName, ETS2_PLUGIN_MMF_SIZE);

    if (telemMem == NULL)
    {
        return SCS_RESULT_generic_error;
    }

	if (telemMem->Hooked() == false)
	{
		return SCS_RESULT_generic_error;
	}

#ifdef SDK_ENABLE_LOGGING
    log_file = fopen("telemetry.log", "wt");
#endif
	telemPtr = (ets2TelemetryMap_t*) (telemMem->GetBuffer());

	if (telemPtr == NULL)
    {
        return SCS_RESULT_generic_error;
	}

	memset(telemPtr, 0, ETS2_PLUGIN_MMF_SIZE);

	/*** INITIALIZE TELEMETRY MAP TO DEFAULT ***/
	telemPtr->paused = 1;
	telemPtr->time = 0;

	telemPtr->tel_revId.ets2_telemetry_plugin_revision = ETS2_PLUGIN_REVID;
	telemPtr->tel_revId.ets2_version_major = SCS_GET_MAJOR_VERSION(version_params->common.game_version);
	telemPtr->tel_revId.ets2_version_minor = SCS_GET_MINOR_VERSION(version_params->common.game_version);

	// Model & trailer type are stored in configuration event.
	// TODO: Invent a better way of sharing strings between plug-in and client application.
	telemPtr->tel_rev1.modelType[0] = TRUCK_STRING_OFFSET;
	telemPtr->tel_rev1.trailerType[0] = TRAILER_STRING_OFFSET;
	telemPtr->tel_rev1.modelType[1] = 0;
	telemPtr->tel_rev1.trailerType[1] = 0;

	/*** REGISTER GAME EVENTS (Pause/Unpause/Start/Time) ***/
	const bool events_registered =
		(version_params->register_for_event(SCS_TELEMETRY_EVENT_frame_start, telemetry_frame_start, NULL) == SCS_RESULT_ok) &&
		(version_params->register_for_event(SCS_TELEMETRY_EVENT_paused, telemetry_pause, NULL) == SCS_RESULT_ok) &&
		(version_params->register_for_event(SCS_TELEMETRY_EVENT_started, telemetry_pause, NULL) == SCS_RESULT_ok);

	// Register configuration event, because it sends data like truck make, etc.
	version_params->register_for_event(SCS_TELEMETRY_EVENT_configuration, telemetry_configuration, NULL);

	if (!events_registered)
	{
		return SCS_RESULT_generic_error;
	}

	/*** REGISTER ALL TELEMETRY CHANNELS TO OUR SHARED MEMORY MAP ***/
	registerChannel(TRUCK_CHANNEL_electric_enabled, bool, telemPtr->tel_rev1.engine_enabled);
	registerChannel(CHANNEL_game_time, u32, telemPtr->tel_rev2.time_abs);
	registerChannel(TRAILER_CHANNEL_connected, bool, telemPtr->tel_rev1.trailer_attached);

	registerChannel(TRUCK_CHANNEL_speed, float, telemPtr->tel_rev1.speed);
	registerChannel(TRUCK_CHANNEL_local_linear_acceleration, fvector, telemPtr->tel_rev1.accelerationX);
	registerChannel(TRUCK_CHANNEL_world_placement, dplacement, telemPtr->tel_rev1.coordinateX);
	
	registerChannel(TRUCK_CHANNEL_engine_gear, s32, telemPtr->tel_rev1.gear);
	registerChannel(TRUCK_CHANNEL_displayed_gear, s32, telemPtr->tel_rev4.gearDashboard);

	registerChannel(TRUCK_CHANNEL_engine_rpm, float, telemPtr->tel_rev1.engineRpm);

	registerChannel(TRUCK_CHANNEL_fuel, float, telemPtr->tel_rev1.fuel);
	// TODO: Calculate fuel rate
	registerChannel(TRUCK_CHANNEL_fuel_average_consumption, float, telemPtr->tel_rev1.fuelAvgConsumption);

	registerChannel(TRUCK_CHANNEL_input_steering, float, telemPtr->tel_rev1.userSteer);
	registerChannel(TRUCK_CHANNEL_input_throttle, float, telemPtr->tel_rev1.userThrottle);
	registerChannel(TRUCK_CHANNEL_input_brake, float, telemPtr->tel_rev1.userBrake);
	registerChannel(TRUCK_CHANNEL_input_clutch, float, telemPtr->tel_rev1.userClutch);

	registerChannel(TRUCK_CHANNEL_effective_steering, float, telemPtr->tel_rev1.gameSteer);
	registerChannel(TRUCK_CHANNEL_effective_throttle, float, telemPtr->tel_rev1.gameThrottle);
	registerChannel(TRUCK_CHANNEL_effective_brake, float, telemPtr->tel_rev1.gameBrake);
	registerChannel(TRUCK_CHANNEL_effective_clutch, float, telemPtr->tel_rev1.gameClutch);

	// Auxilliary stuff:
	registerChannel(TRUCK_CHANNEL_retarder_level, u32, telemPtr->tel_rev3.retarderBrake);
	registerChannel(TRUCK_CHANNEL_hshifter_slot, u32, telemPtr->tel_rev3.shifterSlot);
	registerChannel(TRUCK_CHANNEL_hshifter_selector, bool, telemPtr->tel_rev3.shifterToggle);

	// Booleans
	registerChannel(TRUCK_CHANNEL_wipers, bool, telemPtr->tel_rev3.wipers);
	registerChannel(TRUCK_CHANNEL_parking_brake, bool, telemPtr->tel_rev3.parkBrake);
	registerChannel(TRUCK_CHANNEL_motor_brake, bool, telemPtr->tel_rev3.motorBrake);
	registerChannel(TRUCK_CHANNEL_electric_enabled, bool, telemPtr->tel_rev3.electricEnabled);
	registerChannel(TRUCK_CHANNEL_engine_enabled, bool, telemPtr->tel_rev3.engineEnabled);
	registerChannel(TRUCK_CHANNEL_lblinker, bool, telemPtr->tel_rev3.blinkerLeftActive);
	registerChannel(TRUCK_CHANNEL_rblinker, bool, telemPtr->tel_rev3.blinkerRightActive);
	registerChannel(TRUCK_CHANNEL_light_lblinker, bool, telemPtr->tel_rev3.blinkerLeftOn);
	registerChannel(TRUCK_CHANNEL_light_rblinker, bool, telemPtr->tel_rev3.blinkerRightOn);
	registerChannel(TRUCK_CHANNEL_light_parking, bool, telemPtr->tel_rev3.lightsParking);
	registerChannel(TRUCK_CHANNEL_light_low_beam, bool, telemPtr->tel_rev3.lightsBeamLow);
	registerChannel(TRUCK_CHANNEL_light_high_beam, bool, telemPtr->tel_rev3.lightsBeamHigh);
	registerChannel(TRUCK_CHANNEL_light_aux_front, bool, telemPtr->tel_rev3.lightsAuxFront);
	registerChannel(TRUCK_CHANNEL_light_aux_roof, bool, telemPtr->tel_rev3.lightsAuxRoof);
	registerChannel(TRUCK_CHANNEL_light_beacon, bool, telemPtr->tel_rev3.lightsBeacon);
	registerChannel(TRUCK_CHANNEL_light_brake, bool, telemPtr->tel_rev3.lightsBrake);
	registerChannel(TRUCK_CHANNEL_light_reverse, bool, telemPtr->tel_rev3.lightsReverse);
	registerChannel(TRUCK_CHANNEL_battery_voltage_warning, bool, telemPtr->tel_rev3.batteryVoltageWarning);
	registerChannel(TRUCK_CHANNEL_brake_air_pressure_warning, bool, telemPtr->tel_rev3.airPressureWarning);
	registerChannel(TRUCK_CHANNEL_brake_air_pressure_emergency, bool, telemPtr->tel_rev3.airPressureEmergency);
	registerChannel(TRUCK_CHANNEL_adblue_warning, bool, telemPtr->tel_rev3.adblueWarning);
	registerChannel(TRUCK_CHANNEL_oil_pressure_warning, bool, telemPtr->tel_rev3.oilPressureWarning);
	registerChannel(TRUCK_CHANNEL_water_temperature_warning, bool, telemPtr->tel_rev3.waterTemperatureWarning);

	// Floats
	registerChannel(TRUCK_CHANNEL_brake_air_pressure, float, telemPtr->tel_rev3.airPressure);
	registerChannel(TRUCK_CHANNEL_brake_temperature, float, telemPtr->tel_rev3.brakeTemperature);
	registerChannel(TRUCK_CHANNEL_fuel_warning, bool, telemPtr->tel_rev3.fuelWarning);
	registerChannel(TRUCK_CHANNEL_adblue, float, telemPtr->tel_rev3.adblue);
	registerChannel(TRUCK_CHANNEL_adblue_average_consumption, float, telemPtr->tel_rev3.adblueConsumption);
	registerChannel(TRUCK_CHANNEL_oil_pressure, float, telemPtr->tel_rev3.oilPressure);
	registerChannel(TRUCK_CHANNEL_oil_temperature, float, telemPtr->tel_rev3.oilTemperature);
	registerChannel(TRUCK_CHANNEL_water_temperature, float, telemPtr->tel_rev3.waterTemperature);
	registerChannel(TRUCK_CHANNEL_battery_voltage, float, telemPtr->tel_rev3.batteryVoltage);
	registerChannel(TRUCK_CHANNEL_dashboard_backlight, float, telemPtr->tel_rev3.lightsDashboard);
	registerChannel(TRUCK_CHANNEL_wear_engine, float, telemPtr->tel_rev3.wearEngine);
	registerChannel(TRUCK_CHANNEL_wear_transmission, float, telemPtr->tel_rev3.wearTransmission);
	registerChannel(TRUCK_CHANNEL_wear_cabin, float, telemPtr->tel_rev3.wearCabin);
	registerChannel(TRUCK_CHANNEL_wear_chassis, float, telemPtr->tel_rev3.wearChassis);
	registerChannel(TRUCK_CHANNEL_wear_wheels, float, telemPtr->tel_rev3.wearWheels);
	registerChannel(TRAILER_CHANNEL_wear_chassis, float, telemPtr->tel_rev3.wearTrailer);
	registerChannel(TRUCK_CHANNEL_odometer, float, telemPtr->tel_rev3.truckOdometer);
	registerChannel(TRUCK_CHANNEL_cruise_control, float, telemPtr->tel_rev3.cruiseControlSpeed);
	registerChannel(TRUCK_CHANNEL_navigation_speed_limit, float, telemPtr->tel_rev4.speedLimit);
	registerChannel(TRUCK_CHANNEL_navigation_distance, float, telemPtr->tel_rev4.routeDistance);
	registerChannel(TRUCK_CHANNEL_navigation_time, float, telemPtr->tel_rev4.routeTime);
	registerChannel(TRUCK_CHANNEL_fuel_range, float, telemPtr->tel_rev4.fuelRange);

	// Set the structure with defaults.

	timestamp = static_cast<scs_timestamp_t>(0);
	last_timestamp = static_cast<scs_timestamp_t>(-1);

	return SCS_RESULT_ok;
}

/**
 * @brief Telemetry API deinitialization function.
 *
 * See scssdk_telemetry.h
 */
SCSAPI_VOID scs_telemetry_shutdown(void)
{
	// Close MemoryMap
	if (telemMem != NULL)
	{
		telemMem->Close();
	}
#ifdef SDK_ENABLE_LOGGING
    fclose(log_file);
#endif
}

// Telemetry api.

BOOL APIENTRY DllMain(
	HMODULE module,
	DWORD  reason_for_call,
	LPVOID reseved
)
{
	if (reason_for_call == DLL_PROCESS_DETACH) {

		// Close MemoryMap
		if (telemMem != NULL)
		{
			telemMem->Close();
		}

	}
	return TRUE;
}
