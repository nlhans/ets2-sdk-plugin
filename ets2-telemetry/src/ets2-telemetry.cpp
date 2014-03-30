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

#define UNUSED(x)

/**
/* These macro's are a shortcut to register channels inside the scs_telemetry_init function
 * They require the channel definition name (without prefix SCS_TELEMETRY_), type and destination.
 * Not all channel types are implemented; the handler function for a type should be created like so:
 * telemetry_store_[Type](const scs_string_t name, const scs_u32_t index, const scs_value_t *const value, const scs_context_t context)
 *
 * RegisterSpecificChannel allows for your own handler name, without the telemetry_store_ prefix.
 */
#define registerChannel(name, type, to) version_params->register_for_channel(SCS_TELEMETRY_##name, SCS_U32_NIL, SCS_VALUE_TYPE_##type, SCS_TELEMETRY_CHANNEL_FLAG_none, telemetry_store_##type, &( to ));
#define registerSpecificChannel(name, type, handler, to) version_params->register_for_channel(SCS_TELEMETRY_##name, SCS_U32_NIL, SCS_VALUE_TYPE_##type, SCS_TELEMETRY_CHANNEL_FLAG_none, handler, &( to ));

SharedMemory *telemMem;
ets2TelemetryMap_t *telemPtr;
LPWSTR ets2MmfName = ETS2_PLUGIN_MMF_NAME;

/**
 * @brief Last timestamp we received.
 */
scs_timestamp_t last_timestamp = static_cast<scs_timestamp_t>(-1);
scs_timestamp_t timestamp;


FILE *log_file = NULL;


SCSAPI_VOID telemetry_frame_start(const scs_event_t UNUSED(event), const void *const event_info, const scs_context_t UNUSED(context))
{
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
		telemPtr->time = timestamp;
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
	char * strPtr;
	// This method prints all available attributes of the truck.
	// On configuration change, this function is called.
    const struct scs_telemetry_configuration_t *const info = static_cast<const scs_telemetry_configuration_t *>(event_info);
   
	fprintf(log_file,"----\n");

    for (const scs_named_value_t *current = info->attributes; current->name; ++current)
	{
		if (current->value.type == SCS_VALUE_TYPE_string)
		{
			fprintf(log_file, "Name: %s / Val: %s\n", current->name, current->value.value_string.value);
			fflush(log_file);
		}

		// This loops through all attributes of the truck.
		// Because the identifiers are strings, we must use strcmp.
		// ID is shared between vehicle & chassis.
		if (strcmp(SCS_TELEMETRY_CONFIG_ATTRIBUTE_id, current->name) == 0)
		{
			if (current->value.value_string.value != NULL)
			{
				// vehicle.scania_r
				if (current->value.value_string.value[0] == 'v')
				{
					// Vehicle ID
					strPtr = static_cast<char*>(telemMem->getPtrAt(telemPtr->tel_rev1.modelType[0]));
					strcpy(strPtr, current->value.value_string.value);
					telemPtr->tel_rev1.modelType[1] = strlen(current->value.value_string.value);
				}
				
				// chassis.trailer.
				if (current->value.value_string.value[0] == 'c')
				{
					// Chassis ID
					strPtr = static_cast<char*>(telemMem->getPtrAt(telemPtr->tel_rev1.trailerType[0]));
					strcpy(strPtr, current->value.value_string.value);
					telemPtr->tel_rev1.trailerType[1] = strlen(current->value.value_string.value);
				}
			}
		}

	}
}

/******* STORING OF SEVERAL SCS DATA TYPES *******/
SCSAPI_VOID telemetry_store_float(const scs_string_t name, const scs_u32_t index, const scs_value_t *const value, const scs_context_t context)
{
	assert(value);
	assert(value->type == SCS_VALUE_TYPE_float);
	assert(context);
	*static_cast<float *>(context) = value->value_float.value;
}

SCSAPI_VOID telemetry_store_s32(const scs_string_t name, const scs_u32_t index, const scs_value_t *const value, const scs_context_t context)
{
	assert(value);
	assert(value->type == SCS_VALUE_TYPE_s32);
	assert(context);
	*static_cast<int *>(context) = value->value_s32.value;
}

SCSAPI_VOID telemetry_store_u32(const scs_string_t name, const scs_u32_t index, const scs_value_t *const value, const scs_context_t context)
{
	assert(value);
	assert(value->type == SCS_VALUE_TYPE_u32);
	assert(context);
	*static_cast<unsigned int *>(context) = value->value_u32.value;
}

SCSAPI_VOID telemetry_store_bool(const scs_string_t name, const scs_u32_t index, const scs_value_t *const value, const scs_context_t context)
{
	assert(value);
	assert(value->type == SCS_VALUE_TYPE_bool);
	assert(context);
	if (value->value_bool.value == 0)
	{
		*static_cast<bool *>(context) = false;
	}
	else
	{
		*static_cast<bool *>(context) = true;
	}
}

SCSAPI_VOID telemetry_store_fvector(const scs_string_t name, const scs_u32_t index, const scs_value_t *const value, const scs_context_t context)
{
	assert(value);
	assert(value->type == SCS_VALUE_TYPE_fvector);
	assert(context);
	*(static_cast<float *>(context)+0) = value->value_fvector.x;
	*(static_cast<float *>(context)+1) = value->value_fvector.y;
	*(static_cast<float *>(context)+2) = value->value_fvector.z;
}

SCSAPI_VOID telemetry_store_dplacement(const scs_string_t name, const scs_u32_t index, const scs_value_t *const value, const scs_context_t context)
{
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
	
	/*** ACQUIRE SHARED MEMORY BUFFER ***/
	telemMem = new SharedMemory(ets2MmfName, ETS2_PLUGIN_MMF_SIZE);

	if (telemMem->Hooked() == false)
	{
		return SCS_RESULT_generic_error;
	}
	
    log_file = fopen("telemetry.log", "wt");
	telemPtr = (ets2TelemetryMap_t*) (telemMem->GetBuffer());
	memset(telemPtr, 0, ETS2_PLUGIN_MMF_SIZE);

	/*** INITIALIZE TELEMETRY MAP TO DEFAULT ***/
	telemPtr->paused = 1;
	telemPtr->time = 0;

	telemPtr->tel_revId.ets2_telemetry_plugin_revision = ETS2_PLUGIN_REVID;
	telemPtr->tel_revId.ets2_version_major = SCS_GET_MAJOR_VERSION(version_params->common.game_version);
	telemPtr->tel_revId.ets2_version_minor = SCS_GET_MINOR_VERSION(version_params->common.game_version);
	
	// Model & trailer type are stored in configuration event.
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
	registerChannel(TRUCK_CHANNEL_engine_enabled, bool, telemPtr->tel_rev1.engine_enabled);
	registerChannel(TRAILER_CHANNEL_connected, bool, telemPtr->tel_rev1.trailer_attached);

	registerChannel(TRUCK_CHANNEL_speed, float, telemPtr->tel_rev1.speed);
	registerChannel(TRUCK_CHANNEL_local_linear_acceleration, fvector, telemPtr->tel_rev1.accelerationX);
	registerChannel(TRUCK_CHANNEL_world_placement, dplacement, telemPtr->tel_rev1.coordinateX);
	
	// TODO: Add truck position (world placement)
	//version_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_world_placement, SCS_U32_NIL, SCS_VALUE_TYPE_euler, SCS_TELEMETRY_CHANNEL_FLAG_no_value, telemetry_store_orientation, &telemetry);
	// TODO: Add truck rotation (hills)

	registerChannel(TRUCK_CHANNEL_engine_gear, s32, telemPtr->tel_rev1.gear);

	/*** TODO: Move to attribute parser ***/
	registerChannel(CONFIG_ATTRIBUTE_forward_gear_count, float, telemPtr->tel_rev1.gears);
	registerChannel(CONFIG_ATTRIBUTE_selector_count, u32, telemPtr->tel_rev1.gearRanges);
	registerChannel(CONFIG_ATTRIBUTE_slot_gear, s32, telemPtr->tel_rev1.gearActive);

	registerChannel(TRUCK_CHANNEL_engine_rpm, float, telemPtr->tel_rev1.engineRpm);
	registerChannel(CONFIG_ATTRIBUTE_rpm_limit, float, telemPtr->tel_rev1.engineRpmMax);

	registerChannel(TRUCK_CHANNEL_fuel, float, telemPtr->tel_rev1.fuel);
	registerChannel(CONFIG_ATTRIBUTE_fuel_capacity, float, telemPtr->tel_rev1.fuelCapacity);
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

	// TODO: Add shifter type detection (auto warnings?)
	// TODO: Add wheel speed (Traction Control for in snow)
	
	// TODO: Add brand/model id's
	// TODO: Add cargo accessory ID

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
    fclose(log_file);
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
