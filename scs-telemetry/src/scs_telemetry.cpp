//Windows stuff.

#define WINVER 0x0500
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <cassert>
#include <cstdarg>

// SDK

#include "scssdk_telemetry.h"
#include "eurotrucks2/scssdk_eut2.h"
#include "eurotrucks2/scssdk_telemetry_eut2.h"
#include "amtrucks/scssdk_ats.h"
#include "amtrucks/scssdk_telemetry_ats.h"

// Plug-in
#include "scs-telemetry-common.hpp"
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
#define registerChannelIndex(name, type, to, index) version_params->register_for_channel(SCS_TELEMETRY_##name, index, SCS_VALUE_TYPE_##type, SCS_TELEMETRY_CHANNEL_FLAG_no_value, telemetry_store_##type, &( to ));
#define registerSpecificChannel(name, type, handler, to) version_params->register_for_channel(SCS_TELEMETRY_##name, SCS_U32_NIL, SCS_VALUE_TYPE_##type, SCS_TELEMETRY_CHANNEL_FLAG_no_value, handler, &( to ));

SharedMemory* telem_mem;
scsTelemetryMap_t* telem_ptr;
/**
 * \brief Name/Location of the Shared Memory
 */
const wchar_t* ets2_mmf_name = SCS_PLUGIN_MMF_NAME;

static bool on_job;

scs_log_t game_log = nullptr;
//Used for Logging ingame
// use for values
//char buff[100];
	//(buff, sizeof(buff), "%f", value->value_dplacement.position.x);	 
//log_line(SCS_LOG_TYPE_warning, buff);

void log_line(const scs_log_type_t type, const char *const text, ...)
{
	if (!game_log) {
		return;
	}
	char formated[1000];

	va_list args;
	va_start(args, text);
	vsnprintf_s(formated, sizeof(formated), _TRUNCATE, text, args);
	formated[sizeof(formated) - 1] = 0;
	va_end(args);

	game_log(type, formated);
}

/**
 * @brief Last timestamp we received.
 */
scs_timestamp_t last_timestamp = static_cast<scs_timestamp_t>(-1);
scs_timestamp_t timestamp;
// Function: telemetry_frame_start
// Register telemetry values
SCSAPI_VOID telemetry_frame_start(const scs_event_t UNUSED(event), const void*const event_info,
                                  const scs_context_t UNUSED(context)) {
    static auto clear_job_ticker = 0;
    const auto info = static_cast<const scs_telemetry_frame_start_t *>(event_info);

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
    if (telem_ptr != nullptr) {
        telem_ptr->time = static_cast<unsigned int>(timestamp);

        // Do a non-convential periodic update of this field:
        telem_ptr->truck_b.cruiseControl = telem_ptr->truck_f.cruiseControlSpeed > 0;

        // Check if job could be finished ; if so empty the job field info
        if (telem_ptr->special_b.onJob && !telem_ptr->truck_b.trailer_attached && telem_ptr
                                                                                  ->truck_f.routeDistance
            <= 0.1f && telem_ptr->truck_f.routeDistance >= 0.0f) {
            //TODO: PROBLEM IF YOU SET A ROUTE DIRECTLY AFTER DETACHED
            // if was carrying cargo and not anymore with navigation distance close to zero; 
            // then we assume the job has finished
            // we allow some frames (see ticker) for the client to retrieve data
            telem_ptr->special_b.onJob = false;

            telem_ptr->special_b.jobFinished = true;
            clear_job_ticker = 0;
        }
        else if (telem_ptr->special_b.jobFinished) {
            clear_job_ticker ++;

            if (clear_job_ticker > 10) {
                telem_ptr->config_o.jobIncome = 0;
                telem_ptr->config_ui.time_abs_delivery = 0;
                telem_ptr->config_f.trailerMass = 0;

               
                memset(telem_ptr->config_s.trailerId, 0, stringsize);
                memset(telem_ptr->config_s.trailerName, 0, stringsize);
				memset(telem_ptr->config_s.compDstId, 0, stringsize);
				memset(telem_ptr->config_s.compSrcId, 0, stringsize);
				memset(telem_ptr->config_s.cityDstId, 0, stringsize);
				memset(telem_ptr->config_s.citySrcId, 0, stringsize);
                memset(telem_ptr->config_s.citySrc, 0, stringsize); // TODO: put 64-byte into global define
                memset(telem_ptr->config_s.cityDst, 0, stringsize);
                memset(telem_ptr->config_s.compSrc, 0, stringsize);
                memset(telem_ptr->config_s.compDst, 0, stringsize);
				memset(telem_ptr->config_s.Cargo, 0, stringsize);
                 



                telem_ptr->special_b.jobFinished = false;
            }
        }
        else {
            if (telem_ptr->config_o.jobIncome != 0 && telem_ptr->truck_b.trailer_attached) {
                telem_ptr->special_b.onJob = true;
            }
        }

    }

}

SCSAPI_VOID telemetry_pause(const scs_event_t event, const void*const UNUSED(event_info),
                            const scs_context_t UNUSED(context)) {
    if (telem_ptr != nullptr) {
        telem_ptr->paused = event == SCS_TELEMETRY_EVENT_paused;
    }
}

SCSAPI_VOID telemetry_configuration(const scs_event_t event, const void*const event_info,
                                    const scs_context_t UNUSED(context)) {
    // This method prints all available attributes of the truck.
    // On configuration change, this function is called.
    const auto info = static_cast<const scs_telemetry_configuration_t *>(
        event_info);


    for (auto current = info->attributes; current->name; ++current) {
        handleCfg(current);
    }
}

/******* STORING OF SEVERAL SCS DATA TYPES *******/
SCSAPI_VOID telemetry_store_float(const scs_string_t name, const scs_u32_t index, const scs_value_t*const value,
                                  const scs_context_t context) {
    if (!value) return;
    assert(value);
    assert(value->type == SCS_VALUE_TYPE_float);
    assert(context);
    *static_cast<float *>(context) = value->value_float.value;
}

SCSAPI_VOID telemetry_store_s32(const scs_string_t name, const scs_u32_t index, const scs_value_t*const value,
                                const scs_context_t context) {
    if (!value) return;
    assert(value);
    assert(value->type == SCS_VALUE_TYPE_s32);
    assert(context);
    *static_cast<int *>(context) = value->value_s32.value;
}

SCSAPI_VOID telemetry_store_u32(const scs_string_t name, const scs_u32_t index, const scs_value_t*const value,
                                const scs_context_t context) {
    if (!value) return;
    assert(value);
    assert(value->type == SCS_VALUE_TYPE_u32);
    assert(context);
    *static_cast<unsigned int *>(context) = value->value_u32.value;
}

SCSAPI_VOID telemetry_store_bool(const scs_string_t name, const scs_u32_t index, const scs_value_t*const value,
                                 const scs_context_t context) {
    if (!context) return;
    if (value) {
        if (value->value_bool.value == 0) {
            *static_cast<bool *>(context) = false;
        }
        else {
            *static_cast<bool *>(context) = true;
        }
    }
    else {
        *static_cast<bool *>(context) = false;
    }
}

SCSAPI_VOID telemetry_store_fvector(const scs_string_t name, const scs_u32_t index, const scs_value_t*const value,
                                    const scs_context_t context) {
    if (!value) return;
    assert(value);
    assert(value->type == SCS_VALUE_TYPE_fvector);
    assert(context);
    *(static_cast<float *>(context) + 0) = value->value_fvector.x;
    *(static_cast<float *>(context) + 1) = value->value_fvector.y;
    *(static_cast<float *>(context) + 2) = value->value_fvector.z;
}

SCSAPI_VOID telemetry_store_dplacement(const scs_string_t name, const scs_u32_t index, const scs_value_t*const value,
                                       const scs_context_t context) {
    if (!value) return;
    assert(value);
    assert(value->type == SCS_VALUE_TYPE_dplacement);
    assert(context);

    // Messy hack to store the acceleration and orientation values into our telemetry struct
    // It is neccesary that these are put together, otherwise it may overwrite over values.
    *(static_cast<double *>(context) + 0) = (value->value_dplacement.position.x);
    *(static_cast<double *>(context) + 1) = value->value_dplacement.position.y;
    *(static_cast<double *>(context) + 2) = value->value_dplacement.position.z;
    
    *(static_cast<double *>(context) + 3) = value->value_dplacement.orientation.heading;
    *(static_cast<double *>(context) + 4) = value->value_dplacement.orientation.pitch;
    *(static_cast<double *>(context) + 5) = value->value_dplacement.orientation.roll;

}

SCSAPI_VOID telemetry_store_fplacement(const scs_string_t name, const scs_u32_t index, const scs_value_t*const value,
                                       const scs_context_t context) {
    if (!value) return;
    assert(value);
    assert(value->type == SCS_VALUE_TYPE_fplacement);
    assert(context);

    // Messy hack to store the acceleration and orientation values into our telemetry struct
    // It is neccesary that these are put together, otherwise it may overwrite over values.
    *(static_cast<float *>(context) + 0) = (value->value_fplacement.position.x);
    *(static_cast<float *>(context) + 1) = value->value_fplacement.position.y;
    *(static_cast<float *>(context) + 2) = value->value_fplacement.position.z;

    *(static_cast<float *>(context) + 3) = value->value_fplacement.orientation.heading;
    *(static_cast<float *>(context) + 4) = value->value_fplacement.orientation.pitch;
    *(static_cast<float *>(context) + 5) = value->value_fplacement.orientation.roll;
}


/**
 * @brief Telemetry API initialization function.
 *
 * See scssdk_telemetry.h
 */

SCSAPI_RESULT scs_telemetry_init(const scs_u32_t version, const scs_telemetry_init_params_t*const params) {
    // We currently support only one version.
    if (version != SCS_TELEMETRY_VERSION_1_00) {
        return SCS_RESULT_unsupported;
    }

    const auto version_params = static_cast<const scs_telemetry_init_params_v100_t *>(
        params);
    game_log = version_params->common.log;
    if (version_params == nullptr) {
        return SCS_RESULT_generic_error;
    }

    /*** ACQUIRE SHARED MEMORY BUFFER ***/
    telem_mem = new SharedMemory(ets2_mmf_name, SCS_PLUGIN_MMF_SIZE);

    if (telem_mem == nullptr) {
        return SCS_RESULT_generic_error;
    }

    if (!telem_mem->Hooked()) {
        return SCS_RESULT_generic_error;
    }

    telem_ptr = static_cast<scsTelemetryMap_t*>(telem_mem->GetBuffer());

    if (telem_ptr == nullptr) {
        return SCS_RESULT_generic_error;
    }

    memset(telem_ptr, 0, SCS_PLUGIN_MMF_SIZE);

    /*** INITIALIZE TELEMETRY MAP TO DEFAULT ***/
    telem_ptr->paused = true;
    telem_ptr->time = 0;
    // Get SCS Game Version and Set Plugin Version
    telem_ptr->scs_values.telemetry_plugin_revision = PLUGIN_REVID;
    telem_ptr->scs_values.version_major = SCS_GET_MAJOR_VERSION(version_params->common.game_version);
    telem_ptr->scs_values.version_minor = SCS_GET_MINOR_VERSION(version_params->common.game_version);

    // Set Game ID 
    if (strcmp(version_params->common.game_id, SCS_GAME_ID_EUT2) == 0) {
        telem_ptr->scs_values.game = 1;
        telem_ptr->scs_values.telemetry_version_game_major = SCS_GET_MAJOR_VERSION(
            SCS_TELEMETRY_EUT2_GAME_VERSION_CURRENT);
        telem_ptr->scs_values.telemetry_version_game_major = SCS_GET_MINOR_VERSION(
            SCS_TELEMETRY_EUT2_GAME_VERSION_CURRENT);
    }
    else if (strcmp(version_params->common.game_id, SCS_GAME_ID_ATS) == 0) {
        telem_ptr->scs_values.game = 2;
        telem_ptr->scs_values.telemetry_version_game_major = SCS_GET_MAJOR_VERSION(
            SCS_TELEMETRY_ATS_GAME_VERSION_CURRENT);
        telem_ptr->scs_values.telemetry_version_game_major = SCS_GET_MINOR_VERSION(
            SCS_TELEMETRY_ATS_GAME_VERSION_CURRENT);
    }
    else {
        // unknown game
        
        log_line(SCS_LOG_TYPE_error, "Unknown Game SDK will not work correctly");
        telem_ptr->scs_values.game = 0;
        telem_ptr->scs_values.telemetry_version_game_major = 0;
        telem_ptr->scs_values.telemetry_version_game_major = 0;
    }


    // Model & trailer type are stored in configuration event.


    /*** REGISTER GAME EVENTS (Pause/Unpause/Start/Time) ***/
    const auto events_registered =
        (version_params->register_for_event(SCS_TELEMETRY_EVENT_frame_start, telemetry_frame_start, nullptr) ==
            SCS_RESULT_ok) &&
        (version_params->register_for_event(SCS_TELEMETRY_EVENT_paused, telemetry_pause, nullptr) == SCS_RESULT_ok) &&
        (version_params->register_for_event(SCS_TELEMETRY_EVENT_started, telemetry_pause, nullptr) == SCS_RESULT_ok);

    // Register configuration event, because it sends data like truck make, etc.
    version_params->register_for_event(SCS_TELEMETRY_EVENT_configuration, telemetry_configuration, nullptr);

    if (!events_registered) {
        return SCS_RESULT_generic_error;
    }

    /*** REGISTER ALL TELEMETRY CHANNELS TO OUR SHARED MEMORY MAP ***/ 
    registerChannel(CHANNEL_game_time, u32, telem_ptr->common_ui.time_abs);
    registerChannel(TRAILER_CHANNEL_connected, bool, telem_ptr->truck_b.trailer_attached);

    registerChannel(TRUCK_CHANNEL_speed, float, telem_ptr->truck_f.speed);
    registerChannel(TRUCK_CHANNEL_local_linear_acceleration, fvector, telem_ptr->truck_fv.accelerationX);
    registerChannel(TRUCK_CHANNEL_local_linear_velocity, fvector, telem_ptr->truck_fv.lv_accelerationX);
    registerChannel(TRUCK_CHANNEL_local_angular_acceleration, fvector, telem_ptr->truck_fv.aa_accelerationX);
    registerChannel(TRUCK_CHANNEL_local_angular_velocity, fvector, telem_ptr->truck_fv.av_accelerationX);

    registerChannel(TRUCK_CHANNEL_world_placement, dplacement, telem_ptr->truck_dp.coordinateX);

    registerChannel(TRUCK_CHANNEL_engine_gear, s32, telem_ptr->truck_i.gear);
    registerChannel(TRUCK_CHANNEL_displayed_gear, s32, telem_ptr->truck_i.gearDashboard);

    registerChannel(TRUCK_CHANNEL_engine_rpm, float, telem_ptr->truck_f.engineRpm);

    registerChannel(TRUCK_CHANNEL_fuel, float, telem_ptr->truck_f.fuel);
    registerChannel(TRUCK_CHANNEL_fuel_average_consumption, float, telem_ptr->truck_f.fuelAvgConsumption);

    registerChannel(TRUCK_CHANNEL_input_steering, float, telem_ptr->truck_f.userSteer);
    registerChannel(TRUCK_CHANNEL_input_throttle, float, telem_ptr->truck_f.userThrottle);
    registerChannel(TRUCK_CHANNEL_input_brake, float, telem_ptr->truck_f.userBrake);
    registerChannel(TRUCK_CHANNEL_input_clutch, float, telem_ptr->truck_f.userClutch);

    registerChannel(TRUCK_CHANNEL_effective_steering, float, telem_ptr->truck_f.gameSteer);
    registerChannel(TRUCK_CHANNEL_effective_throttle, float, telem_ptr->truck_f.gameThrottle);
    registerChannel(TRUCK_CHANNEL_effective_brake, float, telem_ptr->truck_f.gameBrake);
    registerChannel(TRUCK_CHANNEL_effective_clutch, float, telem_ptr->truck_f.gameClutch);

    // Auxilliary stuff:
    registerChannel(TRUCK_CHANNEL_retarder_level, u32, telem_ptr->truck_ui.retarderBrake);
    registerChannel(TRUCK_CHANNEL_hshifter_slot, u32, telem_ptr->truck_ui.shifterSlot);
    for (auto i = scs_u32_t(0); i < scs_u32_t(2); i++) {
        registerChannelIndex(TRUCK_CHANNEL_hshifter_selector, bool, telem_ptr->truck_b.shifterToggle[i], i);
    }
    // Booleans
    registerChannel(TRUCK_CHANNEL_wipers, bool, telem_ptr->truck_b.wipers);
    registerChannel(TRUCK_CHANNEL_parking_brake, bool, telem_ptr->truck_b.parkBrake);
    registerChannel(TRUCK_CHANNEL_motor_brake, bool, telem_ptr->truck_b.motorBrake);
    registerChannel(TRUCK_CHANNEL_electric_enabled, bool, telem_ptr->truck_b.electricEnabled);
    registerChannel(TRUCK_CHANNEL_engine_enabled, bool, telem_ptr->truck_b.engineEnabled);
    registerChannel(TRUCK_CHANNEL_lblinker, bool, telem_ptr->truck_b.blinkerLeftActive);
    registerChannel(TRUCK_CHANNEL_rblinker, bool, telem_ptr->truck_b.blinkerRightActive);
    registerChannel(TRUCK_CHANNEL_light_lblinker, bool, telem_ptr->truck_b.blinkerLeftOn);
    registerChannel(TRUCK_CHANNEL_light_rblinker, bool, telem_ptr->truck_b.blinkerRightOn);
    registerChannel(TRUCK_CHANNEL_light_parking, bool, telem_ptr->truck_b.lightsParking);
    registerChannel(TRUCK_CHANNEL_light_low_beam, bool, telem_ptr->truck_b.lightsBeamLow);
    registerChannel(TRUCK_CHANNEL_light_high_beam, bool, telem_ptr->truck_b.lightsBeamHigh);
    registerChannel(TRUCK_CHANNEL_light_aux_front, u32, telem_ptr->truck_ui.lightsAuxFront);
    registerChannel(TRUCK_CHANNEL_light_aux_roof, u32, telem_ptr->truck_ui.lightsAuxRoof);
    registerChannel(TRUCK_CHANNEL_light_beacon, bool, telem_ptr->truck_b.lightsBeacon);
    registerChannel(TRUCK_CHANNEL_light_brake, bool, telem_ptr->truck_b.lightsBrake);
    registerChannel(TRUCK_CHANNEL_light_reverse, bool, telem_ptr->truck_b.lightsReverse);
    registerChannel(TRUCK_CHANNEL_battery_voltage_warning, bool, telem_ptr->truck_b.batteryVoltageWarning);
    registerChannel(TRUCK_CHANNEL_brake_air_pressure_warning, bool, telem_ptr->truck_b.airPressureWarning);
    registerChannel(TRUCK_CHANNEL_brake_air_pressure_emergency, bool, telem_ptr->truck_b.airPressureEmergency);
    registerChannel(TRUCK_CHANNEL_adblue_warning, bool, telem_ptr->truck_b.adblueWarning);
    registerChannel(TRUCK_CHANNEL_oil_pressure_warning, bool, telem_ptr->truck_b.oilPressureWarning);
    registerChannel(TRUCK_CHANNEL_water_temperature_warning, bool, telem_ptr->truck_b.waterTemperatureWarning);

    // Floats
    registerChannel(TRUCK_CHANNEL_brake_air_pressure, float, telem_ptr->truck_f.airPressure);
    registerChannel(TRUCK_CHANNEL_brake_temperature, float, telem_ptr->truck_f.brakeTemperature);
    registerChannel(TRUCK_CHANNEL_fuel_warning, bool, telem_ptr->truck_b.fuelWarning);
    registerChannel(TRUCK_CHANNEL_adblue, float, telem_ptr->truck_f.adblue);
    //registerChannel(TRUCK_CHANNEL_adblue_average_consumption, float, telem_ptr->tel_rev3.adblueConsumption); // seems not to work in ets2/ATS at 19/10 skd 1.9 and actual game versions
    registerChannel(TRUCK_CHANNEL_oil_pressure, float, telem_ptr->truck_f.oilPressure);
    registerChannel(TRUCK_CHANNEL_oil_temperature, float, telem_ptr->truck_f.oilTemperature);
    registerChannel(TRUCK_CHANNEL_water_temperature, float, telem_ptr->truck_f.waterTemperature);
    registerChannel(TRUCK_CHANNEL_battery_voltage, float, telem_ptr->truck_f.batteryVoltage);
    registerChannel(TRUCK_CHANNEL_dashboard_backlight, float, telem_ptr->truck_f.lightsDashboard);
    registerChannel(TRUCK_CHANNEL_wear_engine, float, telem_ptr->truck_f.wearEngine);
    registerChannel(TRUCK_CHANNEL_wear_transmission, float, telem_ptr->truck_f.wearTransmission);
    registerChannel(TRUCK_CHANNEL_wear_cabin, float, telem_ptr->truck_f.wearCabin);
    registerChannel(TRUCK_CHANNEL_wear_chassis, float, telem_ptr->truck_f.wearChassis);
    registerChannel(TRUCK_CHANNEL_wear_wheels, float, telem_ptr->truck_f.wearWheels);
    registerChannel(TRAILER_CHANNEL_wear_chassis, float, telem_ptr->truck_f.wearTrailer);
    registerChannel(TRUCK_CHANNEL_odometer, float, telem_ptr->truck_f.truckOdometer);
    registerChannel(TRUCK_CHANNEL_cruise_control, float, telem_ptr->truck_f.cruiseControlSpeed);
    registerChannel(TRUCK_CHANNEL_navigation_speed_limit, float, telem_ptr->truck_f.speedLimit);
    registerChannel(TRUCK_CHANNEL_navigation_distance, float, telem_ptr->truck_f.routeDistance);
    registerChannel(TRUCK_CHANNEL_navigation_time, float, telem_ptr->truck_f.routeTime);
    registerChannel(TRUCK_CHANNEL_fuel_range, float, telem_ptr->truck_f.fuelRange);


    registerChannel(TRAILER_CHANNEL_world_placement, dplacement, telem_ptr->truck_dp.trailer_coordinateX);

    registerChannel(TRAILER_CHANNEL_local_linear_velocity, fvector, telem_ptr->truck_fv.trailer_lv_accelerationX);
    registerChannel(TRAILER_CHANNEL_local_angular_velocity, fvector, telem_ptr->truck_fv.trailer_la_accelerationX);
    registerChannel(TRAILER_CHANNEL_local_linear_acceleration, fvector, telem_ptr->truck_fv.trailer_la_accelerationX
    );
    registerChannel(TRAILER_CHANNEL_local_angular_acceleration, fvector, telem_ptr->truck_fv.
        trailer_aa_accelerationX
    );

    for (auto i = scs_u32_t(0); i < WHEEL_SIZE; i++) {
        registerChannelIndex(TRAILER_CHANNEL_wheel_on_ground, bool, telem_ptr->truck_b.trailer_wheelOnGround[i],i
        );
        registerChannelIndex(TRAILER_CHANNEL_wheel_substance, u32, telem_ptr->truck_ui.trailer_wheelSubstance[i], i
        );
        registerChannelIndex(TRAILER_CHANNEL_wheel_velocity, float, telem_ptr->truck_f.trailer_wheelVelocity[i], i
        );
        registerChannelIndex(TRAILER_CHANNEL_wheel_steering, float, telem_ptr->truck_f.trailer_wheelSteering[i], i
        );
        registerChannelIndex(TRAILER_CHANNEL_wheel_rotation, float, telem_ptr->truck_f.trailer_wheelRotation[i], i
        );
        registerChannelIndex(TRAILER_CHANNEL_wheel_susp_deflection, float, telem_ptr->truck_f.
            trailer_wheelSuspDeflection[i], i);
        registerChannelIndex(TRUCK_CHANNEL_wheel_on_ground, bool, telem_ptr->truck_b.truck_wheelOnGround[i], i);
        registerChannelIndex(TRUCK_CHANNEL_wheel_substance, u32, telem_ptr->truck_ui.truck_wheelSubstance[i], i);
        registerChannelIndex(TRUCK_CHANNEL_wheel_velocity, float, telem_ptr->truck_f.truck_wheelVelocity[i], i);
        registerChannelIndex(TRUCK_CHANNEL_wheel_steering, float, telem_ptr->truck_f.truck_wheelSteering[i], i);
        registerChannelIndex(TRUCK_CHANNEL_wheel_rotation, float, telem_ptr->truck_f.truck_wheelRotation[i], i);
        registerChannelIndex(TRUCK_CHANNEL_wheel_susp_deflection, float, telem_ptr->truck_f.
            truck_wheelSuspDeflection[i], i);
        registerChannelIndex(TRUCK_CHANNEL_wheel_lift, float, telem_ptr->truck_f.truck_wheelLift[i], i);
        registerChannelIndex(TRUCK_CHANNEL_wheel_lift_offset, float, telem_ptr->truck_f.truck_wheelLiftOffset[i], i
        );
    }

    registerChannel(TRUCK_CHANNEL_cabin_offset, fplacement, telem_ptr->truck_fp.cabinOffsetX);
    registerChannel(TRUCK_CHANNEL_cabin_angular_velocity, fvector, telem_ptr->truck_fv.cabinAVX);
    registerChannel(TRUCK_CHANNEL_cabin_angular_acceleration, fvector, telem_ptr->truck_fv.cabinAAX);
	registerChannel(CHANNEL_next_rest_stop, s32, telem_ptr->common_i.restStop);
	registerChannel(CHANNEL_local_scale, float, telem_ptr->common_f.scale);
    registerChannel(TRUCK_CHANNEL_head_offset, fplacement, telem_ptr->truck_fp.headOffsetX);
    // MISSING: (Will be added on wish) see also on config handler
    // actually should nothing miss here (1.9)

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
SCSAPI_VOID scs_telemetry_shutdown(void) {
    // Close MemoryMap
    if (telem_mem != nullptr) {
        telem_mem->Close();
    }
}

// Telemetry api.

BOOL APIENTRY DllMain(
    HMODULE module,
    DWORD reason_for_call,
    LPVOID reseved
) {
    if (reason_for_call == DLL_PROCESS_DETACH) {

        // Close MemoryMap
        if (telem_mem != nullptr) {
            telem_mem->Close();
        }

    }
    return TRUE;
}
