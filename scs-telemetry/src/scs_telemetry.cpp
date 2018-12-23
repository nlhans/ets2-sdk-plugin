//Windows stuff.

#define WINVER 0x0500
#define WIN32_WINNT 0x0500
#include <Windows.h>
#include <cassert>
#include <cstdarg>
#include <algorithm>
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
#define REGISTER_CHANNEL(name, type, to) version_params->register_for_channel(SCS_TELEMETRY_##name, SCS_U32_NIL, SCS_VALUE_TYPE_##type, SCS_TELEMETRY_CHANNEL_FLAG_no_value, telemetry_store_##type, &( to ));
#define REGISTER_CHANNEL_INDEX(name, type, to, index) version_params->register_for_channel(SCS_TELEMETRY_##name, index, SCS_VALUE_TYPE_##type, SCS_TELEMETRY_CHANNEL_FLAG_no_value, telemetry_store_##type, &( to ));
#define REGISTER_SPECIFIC_CHANNEL(name, type, handler,to) version_params->register_for_channel(SCS_TELEMETRY_##name, SCS_U32_NIL, SCS_VALUE_TYPE_##type, SCS_TELEMETRY_CHANNEL_FLAG_no_value, handler, &( to ));

SharedMemory* telem_mem;
scsTelemetryMap_t* telem_ptr;

// const: scs_mmf_name
// Name/Location of the Shared Memory
const wchar_t* scs_mmf_name = SCS_PLUGIN_MMF_NAME;

// ptr: game_log
// Used to write to the game log
scs_log_t game_log = nullptr;

// About: Game log
//
// - Use function log_line(const scs_log_type_t type, const char*const text,...) to write to the in game console log with choosen message type
// - or use log_line(const char*const text, ...) to write to the in game console log with error type (more for debugging purpose)

// use for values
// char buff[100];
// snprintf(buff, sizeof(buff), "%f", value->value_dplacement.position.x);	 
// log_line(SCS_LOG_TYPE_warning, buff);

// Function: log_line
// Used to write to the in game console log
void log_line(const scs_log_type_t type, const char*const text, ...) {
    if (!game_log) {
        return;
    }
    char formated[1000];

    va_list args;
    va_start(args, text);
    vsnprintf_s(formated, sizeof formated, _TRUNCATE, text, args);
    formated[sizeof formated - 1] = 0;
    va_end(args);

    game_log(type, formated);
}

// Function: log_line
// Used to write to the in game console log as error (debugging)
void log_line(const char*const text, ...) {
    if (!game_log) {
        return;
    }
    char formated[1000];

    va_list args;
    va_start(args, text);
    vsnprintf_s(formated, sizeof formated, _TRUNCATE, text, args);
    formated[sizeof formated - 1] = 0;
    va_end(args);

    game_log(SCS_LOG_TYPE_error, formated);
}


// Function: log_configs
// It print every config event that appears to the in game log
// careful, create a lot of logs so that fast parts are not readable anymore in the log window
void log_configs(const scs_telemetry_configuration_t* info) { 
    log_line("Configuration: %s", info->id);
    for (auto current = info->attributes; current->name; ++current) {

        if (current->index != SCS_U32_NIL) {
           // log_line("[%u]", static_cast<unsigned>(current->index));
        }
        switch (current->value.type) {
        case SCS_VALUE_TYPE_INVALID: {
            log_line(" %s none", current->name);
            break;
        }
        case SCS_VALUE_TYPE_bool: {
            log_line(" %s bool = %s", current->name, current->value.value_bool.value ? "true" : "false");
            break;
        }
        case SCS_VALUE_TYPE_s32: {
            log_line(" %s s32 = %d", current->name, static_cast<int>(current->value.value_s32.value));
            break;
        }
        case SCS_VALUE_TYPE_u32: {
            log_line(" %s u32 = %u", current->name, static_cast<unsigned>(current->value.value_u32.value));
            break;
        }
        case SCS_VALUE_TYPE_u64: {
            log_line(" %s u64 = %" SCS_PF_U64, current->name, current->value.value_u64.value);
            break;
        }
        case SCS_VALUE_TYPE_float: {
            log_line(" %s float = %f", current->name, current->value.value_float.value);
            break;
        }
        case SCS_VALUE_TYPE_double: {
            log_line(" %s double = %f", current->name, current->value.value_double.value);
            break;
        }
        case SCS_VALUE_TYPE_fvector: {
            log_line(
                " %s fvector = (%f,%f,%f)", current->name,
                current->value.value_fvector.x,
                current->value.value_fvector.y,
                current->value.value_fvector.z
            );
            break;
        }
        case SCS_VALUE_TYPE_dvector: {
            log_line(
                " %s dvector = (%f,%f,%f)", current->name,
                current->value.value_dvector.x,
                current->value.value_dvector.y,
                current->value.value_dvector.z
            );
            break;
        }
        case SCS_VALUE_TYPE_euler: {
            log_line(
                " %s euler = h:%f p:%f r:%f", current->name,
                current->value.value_euler.heading * 360.0f,
                current->value.value_euler.pitch * 360.0f,
                current->value.value_euler.roll * 360.0f
            );
            break;
        }
        case SCS_VALUE_TYPE_fplacement: {
            log_line(
                " %s fplacement = (%f,%f,%f) h:%f p:%f r:%f", current->name,
                current->value.value_fplacement.position.x,
                current->value.value_fplacement.position.y,
                current->value.value_fplacement.position.z,
                current->value.value_fplacement.orientation.heading * 360.0f,
                current->value.value_fplacement.orientation.pitch * 360.0f,
                current->value.value_fplacement.orientation.roll * 360.0f
            );
            break;
        }
        case SCS_VALUE_TYPE_dplacement: {
            log_line(
                " %s dplacement = (%f,%f,%f) h:%f p:%f r:%f", current->name,
                current->value.value_dplacement.position.x,
                current->value.value_dplacement.position.y,
                current->value.value_dplacement.position.z,
                current->value.value_dplacement.orientation.heading * 360.0f,
                current->value.value_dplacement.orientation.pitch * 360.0f,
                current->value.value_dplacement.orientation.roll * 360.0f
            );
            break;
        }
        case SCS_VALUE_TYPE_string: {
            log_line(" %s string = %s", current->name, current->value.value_string.value);
            break;
        }
        default: {
            log_line(" %s unknown", current->name);
            break;
        }

        }

    }
}

/**
 * @brief Last timestamp we received.
 */
scs_timestamp_t last_timestamp = static_cast<scs_timestamp_t>(-1);
scs_timestamp_t timestamp;
static auto clear_job_ticker = 0;

// Function: set_job_values_zero
// set every job (cargo) values to 0/empty string
void set_job_values_zero() {
	telem_ptr->config_o.jobIncome = 0;
	telem_ptr->config_ui.time_abs_delivery = 0;
	telem_ptr->config_f.cargoMass = 0;
	memset(telem_ptr->config_s.compDstId, 0, stringsize);
	memset(telem_ptr->config_s.compSrcId, 0, stringsize);
	memset(telem_ptr->config_s.cityDstId, 0, stringsize);
	memset(telem_ptr->config_s.citySrcId, 0, stringsize);
	memset(telem_ptr->config_s.citySrc, 0, stringsize);
	memset(telem_ptr->config_s.cityDst, 0, stringsize);
	memset(telem_ptr->config_s.compSrc, 0, stringsize);
	memset(telem_ptr->config_s.compDst, 0, stringsize);
	memset(telem_ptr->config_s.cargoId, 0, stringsize);
	memset(telem_ptr->config_s.cargoAcessoryId, 0, stringsize);
	memset(telem_ptr->config_s.cargo, 0, stringsize);
}
// Function: set_trailer_values_zero
// set every trailer value 0/empty string 
void set_trailer_values_zero() {
    
	telem_ptr->truck_f.wearTrailer = 0;
	std::fill(telem_ptr->truck_ui.trailer_wheelSubstance, telem_ptr->truck_ui.trailer_wheelSubstance+16, 0u);
	std::fill(telem_ptr->truck_f.trailer_wheelSuspDeflection, telem_ptr->truck_f.trailer_wheelSuspDeflection + 16, 0.0f);
	std::fill(telem_ptr->truck_f.trailer_wheelVelocity, telem_ptr->truck_f.trailer_wheelVelocity + 16, 0.0f);
	std::fill(telem_ptr->truck_f.trailer_wheelSteering, telem_ptr->truck_f.trailer_wheelSteering + 16, 0.0f);
	std::fill(telem_ptr->truck_f.trailer_wheelRotation, telem_ptr->truck_f.trailer_wheelRotation + 16, 0.0f);
	std::fill(telem_ptr->truck_b.trailer_wheelOnGround, telem_ptr->truck_b.trailer_wheelOnGround + 16, false);

	telem_ptr->truck_fv.trailer_lv_accelerationX = 0;
	telem_ptr->truck_fv.trailer_lv_accelerationY = 0;
	telem_ptr->truck_fv.trailer_lv_accelerationZ = 0;

	telem_ptr->truck_fv.trailer_av_accelerationX = 0;
	telem_ptr->truck_fv.trailer_av_accelerationY = 0;
	telem_ptr->truck_fv.trailer_av_accelerationZ = 0;

	telem_ptr->truck_fv.trailer_la_accelerationX = 0;
	telem_ptr->truck_fv.trailer_la_accelerationY = 0;
	telem_ptr->truck_fv.trailer_la_accelerationZ = 0;

	telem_ptr->truck_fv.trailer_aa_accelerationX = 0;
	telem_ptr->truck_fv.trailer_aa_accelerationY = 0;
	telem_ptr->truck_fv.trailer_aa_accelerationZ = 0;

	telem_ptr->config_fv.trailerHookPositionX = 0;
	telem_ptr->config_fv.trailerHookPositionY = 0;
	telem_ptr->config_fv.trailerHookPositionZ = 0;

	telem_ptr->truck_dp.trailer_coordinateX = 0;
	telem_ptr->truck_dp.trailer_coordinateY = 0;
	telem_ptr->truck_dp.trailer_coordinateZ = 0;

	telem_ptr->truck_dp.trailer_rotationX = 0;
	telem_ptr->truck_dp.trailer_rotationY = 0;
	telem_ptr->truck_dp.trailer_rotationZ = 0;

	telem_ptr->config_ui.trailerWheelCount = 0; 

	std::fill(telem_ptr->config_f.trailerWheelRadius, telem_ptr->config_f.trailerWheelRadius + 16, 0.0f);
	std::fill(telem_ptr->config_b.trailerWheelSimulated, telem_ptr->config_b.trailerWheelSimulated + 16, false);
	std::fill(telem_ptr->config_b.trailerWheelLiftable, telem_ptr->config_b.trailerWheelLiftable + 16, false);
	std::fill(telem_ptr->config_b.trailerWheelPowered, telem_ptr->config_b.trailerWheelPowered + 16, false);
	std::fill(telem_ptr->config_b.trailerWheelSteerable, telem_ptr->config_b.trailerWheelSteerable + 16, false);
	std::fill(telem_ptr->config_fv.trailerWheelPositionX, telem_ptr->config_fv.trailerWheelPositionX + 16, 0.0f);
	std::fill(telem_ptr->config_fv.trailerWheelPositionY, telem_ptr->config_fv.trailerWheelPositionY + 16, 0.0f);
	std::fill(telem_ptr->config_fv.trailerWheelPositionZ, telem_ptr->config_fv.trailerWheelPositionZ + 16, 0.0f);

	memset(telem_ptr->config_s.trailerId, 0, stringsize);
}

// Function: telemetry_frame_start
// Register telemetry values
SCSAPI_VOID telemetry_frame_start(const scs_event_t UNUSED(event), const void*const event_info,
                                    scs_context_t UNUSED(context)) {

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

    timestamp += info->paused_simulation_time - last_timestamp;
    last_timestamp = info->paused_simulation_time;

    /* Copy over the game timestamp to our telemetry memory */
    if (telem_ptr != nullptr) {
        telem_ptr->time = static_cast<unsigned int>(timestamp);

        // Do a non-convential periodic update of this field:
        telem_ptr->truck_b.cruiseControl = telem_ptr->truck_f.cruiseControlSpeed > 0;

        if (telem_ptr->special_b.jobFinished) {
            clear_job_ticker++;

            if (clear_job_ticker > 10) {
				set_job_values_zero();
                telem_ptr->special_b.jobFinished = false;
            }
        }
    }

}

// Function: telemetry_pause
// called if the game fires the event start/pause. Used to set the paused value
SCSAPI_VOID telemetry_pause(const scs_event_t event, const void*const UNUSED(event_info),
                             scs_context_t UNUSED(context)) {
    if (telem_ptr != nullptr) {
        telem_ptr->paused = event == SCS_TELEMETRY_EVENT_paused;
    }
}

// Function: telemetry_configuration
// called if the game fires the event configuration. Used to handle all the configuration values
SCSAPI_VOID telemetry_configuration(const scs_event_t event, const void*const event_info,
                                     scs_context_t UNUSED(context)) {
    // On configuration change, this function is called.
    const auto info = static_cast<const scs_telemetry_configuration_t *>(
        event_info);

    // check which type the event has
	configType type = {};
    if(strcmp(info->id,"substances")==0) {
		type = substances;
    }else if (strcmp(info->id, "controls") == 0) {
		type = controls;
	}else if (strcmp(info->id, "hshifter") == 0) {
		type = hshifter;
	}else if (strcmp(info->id, "truck") == 0) {
		type = truck;
	}else if (strcmp(info->id, "trailer") == 0) {
		type = trailer;
	}else if (strcmp(info->id, "job") == 0) {
		type = job;
	}else {
		log_line(SCS_LOG_TYPE_warning, "Something went wrong with this %s",info->id);
	}

    // uncomment to log every config, should work but with function not tested ^^`
    //log_configs(info); 

    // attribute is a pointer array that is never null so ... i have no clue how to check it on another way than this
    // if for loop can't loop it is empty so simple 
    auto is_empty = true;

    for ( auto current = info->attributes; current->name; ++current) {
        if(!handleCfg(current, type)) {
            // actually only for testing/debug purpose, so should there be a message in game with that line there is missed something
			log_line("attribute not handled id: %i attribute: %s", type, current->name);
        } 
        is_empty = false;
    }
    // if id of config is "job" but without element and we are on a job -> we finished it now
    if (type==job && is_empty && telem_ptr->special_b.onJob) {
        telem_ptr->special_b.onJob = false;
        telem_ptr->special_b.jobFinished = true;
        clear_job_ticker = 0;
    }
    else if (!telem_ptr->special_b.onJob && type == job && !is_empty) {
        // oh hey no job but now we have fields in this array so we start a new job
        telem_ptr->special_b.onJob = true;
    }
    // no trailer which is connected with us? than delete information of the sdk and say there is no connected trailer
    if(type==trailer && is_empty) {
		set_trailer_values_zero();
		telem_ptr->special_b.trailerConnected = false;
    }else if(type == trailer && !is_empty && !telem_ptr->special_b.trailerConnected) {
        // there exist trailer information and actually we say there is no connected trailer. That can't be true anymore
        // so say we are connected to a trailer
		telem_ptr->special_b.trailerConnected = true;
    }
}

/******* STORING OF SEVERAL SCS DATA TYPES *******/
SCSAPI_VOID telemetry_store_float(const scs_string_t name, const scs_u32_t index, const scs_value_t*const value,
                                    scs_context_t  context) {
    if (!value) return;
    assert(value);
    assert(value->type == SCS_VALUE_TYPE_float);
    assert(context);
    *static_cast<float *>(context) = value->value_float.value;
}

SCSAPI_VOID telemetry_store_s32(const scs_string_t name, const scs_u32_t index, const scs_value_t*const value,
                                 scs_context_t context) {
    if (!value) return;
    assert(value);
    assert(value->type == SCS_VALUE_TYPE_s32);
    assert(context);
    *static_cast<int *>(context) = value->value_s32.value;
}

SCSAPI_VOID telemetry_store_u32(const scs_string_t name, const scs_u32_t index, const scs_value_t*const value,
                                 scs_context_t context) {
    if (!value) return;
    assert(value);
    assert(value->type == SCS_VALUE_TYPE_u32);
    assert(context);
    *static_cast<unsigned int *>(context) = value->value_u32.value;
}

SCSAPI_VOID telemetry_store_bool(const scs_string_t name, const scs_u32_t index, const scs_value_t*const value,
                                  scs_context_t context) {
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
                                     scs_context_t context) {
    if (!value) return;
    assert(value);
    assert(value->type == SCS_VALUE_TYPE_fvector);
    assert(context);
    *(static_cast<float *>(context) + 0) = value->value_fvector.x;
    *(static_cast<float *>(context) + 1) = value->value_fvector.y;
    *(static_cast<float *>(context) + 2) = value->value_fvector.z;
}

SCSAPI_VOID telemetry_store_dplacement(const scs_string_t name, const scs_u32_t index, const scs_value_t*const value,
                                        scs_context_t context) {
    if (!value) return;
    assert(value);
    assert(value->type == SCS_VALUE_TYPE_dplacement);
    assert(context);

    // Messy hack to store the acceleration and orientation values into our telemetry struct
    // It is neccesary that these are put together, otherwise it may overwrite over values.
    *(static_cast<double *>(context) + 0) = value->value_dplacement.position.x;
    *(static_cast<double *>(context) + 1) = value->value_dplacement.position.y;
    *(static_cast<double *>(context) + 2) = value->value_dplacement.position.z;

    *(static_cast<double *>(context) + 3) = value->value_dplacement.orientation.heading;
    *(static_cast<double *>(context) + 4) = value->value_dplacement.orientation.pitch;
    *(static_cast<double *>(context) + 5) = value->value_dplacement.orientation.roll;

}

SCSAPI_VOID telemetry_store_fplacement(const scs_string_t name, const scs_u32_t index, const scs_value_t*const value,
                                        scs_context_t context) {
    if (!value) return;
    assert(value);
    assert(value->type == SCS_VALUE_TYPE_fplacement);
    assert(context);

    // Messy hack to store the acceleration and orientation values into our telemetry struct
    // It is neccesary that these are put together, otherwise it may overwrite over values.
    *(static_cast<float *>(context) + 0) = value->value_fplacement.position.x;
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

    const auto version_params = static_cast<const scs_telemetry_init_params_v100_t *>(params);  // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
    game_log = version_params->common.log;
    if (version_params == nullptr) {
        return SCS_RESULT_generic_error;
    }

    /*** ACQUIRE SHARED MEMORY BUFFER ***/
    telem_mem = new SharedMemory(scs_mmf_name, SCS_PLUGIN_MMF_SIZE);

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
        telem_ptr->scs_values.telemetry_version_game_minor = SCS_GET_MINOR_VERSION(
            SCS_TELEMETRY_EUT2_GAME_VERSION_CURRENT);
    }
    else if (strcmp(version_params->common.game_id, SCS_GAME_ID_ATS) == 0) {
        telem_ptr->scs_values.game = 2;
        telem_ptr->scs_values.telemetry_version_game_major = SCS_GET_MAJOR_VERSION(
            SCS_TELEMETRY_ATS_GAME_VERSION_CURRENT);
        telem_ptr->scs_values.telemetry_version_game_minor = SCS_GET_MINOR_VERSION(
            SCS_TELEMETRY_ATS_GAME_VERSION_CURRENT);
    }
    else {
        // unknown game

        log_line(SCS_LOG_TYPE_error, "Unknown Game SDK will not work correctly");
        telem_ptr->scs_values.game = 0;
        telem_ptr->scs_values.telemetry_version_game_major = 0;
        telem_ptr->scs_values.telemetry_version_game_minor = 0;
    }


    // Model & trailer type are stored in configuration event.


    /*** REGISTER GAME EVENTS (Pause/Unpause/Start/Time) ***/
    const auto events_registered =
        version_params->register_for_event(SCS_TELEMETRY_EVENT_frame_start, telemetry_frame_start, nullptr) ==
        SCS_RESULT_ok &&
        version_params->register_for_event(SCS_TELEMETRY_EVENT_paused, telemetry_pause, nullptr) == SCS_RESULT_ok &&
        version_params->register_for_event(SCS_TELEMETRY_EVENT_started, telemetry_pause, nullptr) == SCS_RESULT_ok;

    // Register configuration event, because it sends data like truck make, etc.
    version_params->register_for_event(SCS_TELEMETRY_EVENT_configuration, telemetry_configuration, nullptr);

    if (!events_registered) {
        return SCS_RESULT_generic_error;
    }

    /*** REGISTER ALL TELEMETRY CHANNELS TO OUR SHARED MEMORY MAP ***/
    REGISTER_CHANNEL(CHANNEL_game_time, u32, telem_ptr->common_ui.time_abs);
    REGISTER_CHANNEL(TRAILER_CHANNEL_connected, bool, telem_ptr->truck_b.trailer_attached);

    REGISTER_CHANNEL(TRUCK_CHANNEL_speed, float, telem_ptr->truck_f.speed);
    REGISTER_CHANNEL(TRUCK_CHANNEL_local_linear_acceleration, fvector, telem_ptr->truck_fv.accelerationX);
    REGISTER_CHANNEL(TRUCK_CHANNEL_local_linear_velocity, fvector, telem_ptr->truck_fv.lv_accelerationX);
    REGISTER_CHANNEL(TRUCK_CHANNEL_local_angular_acceleration, fvector, telem_ptr->truck_fv.aa_accelerationX);
    REGISTER_CHANNEL(TRUCK_CHANNEL_local_angular_velocity, fvector, telem_ptr->truck_fv.av_accelerationX);

    REGISTER_CHANNEL(TRUCK_CHANNEL_world_placement, dplacement, telem_ptr->truck_dp.coordinateX);

    REGISTER_CHANNEL(TRUCK_CHANNEL_engine_gear, s32, telem_ptr->truck_i.gear);
    REGISTER_CHANNEL(TRUCK_CHANNEL_displayed_gear, s32, telem_ptr->truck_i.gearDashboard);

    REGISTER_CHANNEL(TRUCK_CHANNEL_engine_rpm, float, telem_ptr->truck_f.engineRpm);

    REGISTER_CHANNEL(TRUCK_CHANNEL_fuel, float, telem_ptr->truck_f.fuel);
    REGISTER_CHANNEL(TRUCK_CHANNEL_fuel_average_consumption, float, telem_ptr->truck_f.fuelAvgConsumption);

    REGISTER_CHANNEL(TRUCK_CHANNEL_input_steering, float, telem_ptr->truck_f.userSteer);
    REGISTER_CHANNEL(TRUCK_CHANNEL_input_throttle, float, telem_ptr->truck_f.userThrottle);
    REGISTER_CHANNEL(TRUCK_CHANNEL_input_brake, float, telem_ptr->truck_f.userBrake);
    REGISTER_CHANNEL(TRUCK_CHANNEL_input_clutch, float, telem_ptr->truck_f.userClutch);

    REGISTER_CHANNEL(TRUCK_CHANNEL_effective_steering, float, telem_ptr->truck_f.gameSteer);
    REGISTER_CHANNEL(TRUCK_CHANNEL_effective_throttle, float, telem_ptr->truck_f.gameThrottle);
    REGISTER_CHANNEL(TRUCK_CHANNEL_effective_brake, float, telem_ptr->truck_f.gameBrake);
    REGISTER_CHANNEL(TRUCK_CHANNEL_effective_clutch, float, telem_ptr->truck_f.gameClutch);

    // Auxilliary stuff:
    REGISTER_CHANNEL(TRUCK_CHANNEL_retarder_level, u32, telem_ptr->truck_ui.retarderBrake);
    REGISTER_CHANNEL(TRUCK_CHANNEL_hshifter_slot, u32, telem_ptr->truck_ui.shifterSlot);
    for (auto i = scs_u32_t(0); i < scs_u32_t(2); i++) {
        REGISTER_CHANNEL_INDEX(TRUCK_CHANNEL_hshifter_selector, bool, telem_ptr->truck_b.shifterToggle[i], i);
    }
    // Booleans
    REGISTER_CHANNEL(TRUCK_CHANNEL_wipers, bool, telem_ptr->truck_b.wipers);
    REGISTER_CHANNEL(TRUCK_CHANNEL_parking_brake, bool, telem_ptr->truck_b.parkBrake);
    REGISTER_CHANNEL(TRUCK_CHANNEL_motor_brake, bool, telem_ptr->truck_b.motorBrake);
    REGISTER_CHANNEL(TRUCK_CHANNEL_electric_enabled, bool, telem_ptr->truck_b.electricEnabled);
    REGISTER_CHANNEL(TRUCK_CHANNEL_engine_enabled, bool, telem_ptr->truck_b.engineEnabled);
    REGISTER_CHANNEL(TRUCK_CHANNEL_lblinker, bool, telem_ptr->truck_b.blinkerLeftActive);
    REGISTER_CHANNEL(TRUCK_CHANNEL_rblinker, bool, telem_ptr->truck_b.blinkerRightActive);
    REGISTER_CHANNEL(TRUCK_CHANNEL_light_lblinker, bool, telem_ptr->truck_b.blinkerLeftOn);
    REGISTER_CHANNEL(TRUCK_CHANNEL_light_rblinker, bool, telem_ptr->truck_b.blinkerRightOn);
    REGISTER_CHANNEL(TRUCK_CHANNEL_light_parking, bool, telem_ptr->truck_b.lightsParking);
    REGISTER_CHANNEL(TRUCK_CHANNEL_light_low_beam, bool, telem_ptr->truck_b.lightsBeamLow);
    REGISTER_CHANNEL(TRUCK_CHANNEL_light_high_beam, bool, telem_ptr->truck_b.lightsBeamHigh);
    REGISTER_CHANNEL(TRUCK_CHANNEL_light_aux_front, u32, telem_ptr->truck_ui.lightsAuxFront);
    REGISTER_CHANNEL(TRUCK_CHANNEL_light_aux_roof, u32, telem_ptr->truck_ui.lightsAuxRoof);
    REGISTER_CHANNEL(TRUCK_CHANNEL_light_beacon, bool, telem_ptr->truck_b.lightsBeacon);
    REGISTER_CHANNEL(TRUCK_CHANNEL_light_brake, bool, telem_ptr->truck_b.lightsBrake);
    REGISTER_CHANNEL(TRUCK_CHANNEL_light_reverse, bool, telem_ptr->truck_b.lightsReverse);
    REGISTER_CHANNEL(TRUCK_CHANNEL_battery_voltage_warning, bool, telem_ptr->truck_b.batteryVoltageWarning);
    REGISTER_CHANNEL(TRUCK_CHANNEL_brake_air_pressure_warning, bool, telem_ptr->truck_b.airPressureWarning);
    REGISTER_CHANNEL(TRUCK_CHANNEL_brake_air_pressure_emergency, bool, telem_ptr->truck_b.airPressureEmergency);
    REGISTER_CHANNEL(TRUCK_CHANNEL_adblue_warning, bool, telem_ptr->truck_b.adblueWarning);
    REGISTER_CHANNEL(TRUCK_CHANNEL_oil_pressure_warning, bool, telem_ptr->truck_b.oilPressureWarning);
    REGISTER_CHANNEL(TRUCK_CHANNEL_water_temperature_warning, bool, telem_ptr->truck_b.waterTemperatureWarning);

    // Floats
    REGISTER_CHANNEL(TRUCK_CHANNEL_brake_air_pressure, float, telem_ptr->truck_f.airPressure);
    REGISTER_CHANNEL(TRUCK_CHANNEL_brake_temperature, float, telem_ptr->truck_f.brakeTemperature);
    REGISTER_CHANNEL(TRUCK_CHANNEL_fuel_warning, bool, telem_ptr->truck_b.fuelWarning);
    REGISTER_CHANNEL(TRUCK_CHANNEL_adblue, float, telem_ptr->truck_f.adblue);
    //registerChannel(TRUCK_CHANNEL_adblue_average_consumption, float, telem_ptr->tel_rev3.adblueConsumption); // seems not to work in ets2/ATS at 19/10 skd 1.9 and actual game versions
    REGISTER_CHANNEL(TRUCK_CHANNEL_oil_pressure, float, telem_ptr->truck_f.oilPressure);
    REGISTER_CHANNEL(TRUCK_CHANNEL_oil_temperature, float, telem_ptr->truck_f.oilTemperature);
    REGISTER_CHANNEL(TRUCK_CHANNEL_water_temperature, float, telem_ptr->truck_f.waterTemperature);
    REGISTER_CHANNEL(TRUCK_CHANNEL_battery_voltage, float, telem_ptr->truck_f.batteryVoltage);
    REGISTER_CHANNEL(TRUCK_CHANNEL_dashboard_backlight, float, telem_ptr->truck_f.lightsDashboard);
    REGISTER_CHANNEL(TRUCK_CHANNEL_wear_engine, float, telem_ptr->truck_f.wearEngine);
    REGISTER_CHANNEL(TRUCK_CHANNEL_wear_transmission, float, telem_ptr->truck_f.wearTransmission);
    REGISTER_CHANNEL(TRUCK_CHANNEL_wear_cabin, float, telem_ptr->truck_f.wearCabin);
    REGISTER_CHANNEL(TRUCK_CHANNEL_wear_chassis, float, telem_ptr->truck_f.wearChassis);
    REGISTER_CHANNEL(TRUCK_CHANNEL_wear_wheels, float, telem_ptr->truck_f.wearWheels);
    REGISTER_CHANNEL(TRAILER_CHANNEL_wear_chassis, float, telem_ptr->truck_f.wearTrailer);
    REGISTER_CHANNEL(TRUCK_CHANNEL_odometer, float, telem_ptr->truck_f.truckOdometer);
    REGISTER_CHANNEL(TRUCK_CHANNEL_cruise_control, float, telem_ptr->truck_f.cruiseControlSpeed);
    REGISTER_CHANNEL(TRUCK_CHANNEL_navigation_speed_limit, float, telem_ptr->truck_f.speedLimit);
    REGISTER_CHANNEL(TRUCK_CHANNEL_navigation_distance, float, telem_ptr->truck_f.routeDistance);
    REGISTER_CHANNEL(TRUCK_CHANNEL_navigation_time, float, telem_ptr->truck_f.routeTime);
    REGISTER_CHANNEL(TRUCK_CHANNEL_fuel_range, float, telem_ptr->truck_f.fuelRange);


    REGISTER_CHANNEL(TRAILER_CHANNEL_world_placement, dplacement, telem_ptr->truck_dp.trailer_coordinateX);

    REGISTER_CHANNEL(TRAILER_CHANNEL_local_linear_velocity, fvector, telem_ptr->truck_fv.trailer_lv_accelerationX);
    REGISTER_CHANNEL(TRAILER_CHANNEL_local_angular_velocity, fvector, telem_ptr->truck_fv.trailer_la_accelerationX);
    REGISTER_CHANNEL(TRAILER_CHANNEL_local_linear_acceleration, fvector, telem_ptr->truck_fv.trailer_la_accelerationX
    );
    REGISTER_CHANNEL(TRAILER_CHANNEL_local_angular_acceleration, fvector, telem_ptr->truck_fv.
        trailer_aa_accelerationX
    );

    for (auto i = scs_u32_t(0); i < WHEEL_SIZE; i++) {
        REGISTER_CHANNEL_INDEX(TRAILER_CHANNEL_wheel_on_ground, bool, telem_ptr->truck_b.trailer_wheelOnGround[i],i
        );
        REGISTER_CHANNEL_INDEX(TRAILER_CHANNEL_wheel_substance, u32, telem_ptr->truck_ui.trailer_wheelSubstance[i], i
        );
        REGISTER_CHANNEL_INDEX(TRAILER_CHANNEL_wheel_velocity, float, telem_ptr->truck_f.trailer_wheelVelocity[i], i
        );
        REGISTER_CHANNEL_INDEX(TRAILER_CHANNEL_wheel_steering, float, telem_ptr->truck_f.trailer_wheelSteering[i], i
        );
        REGISTER_CHANNEL_INDEX(TRAILER_CHANNEL_wheel_rotation, float, telem_ptr->truck_f.trailer_wheelRotation[i], i
        );
        REGISTER_CHANNEL_INDEX(TRAILER_CHANNEL_wheel_susp_deflection, float, telem_ptr->truck_f.
            trailer_wheelSuspDeflection[i], i);
        REGISTER_CHANNEL_INDEX(TRUCK_CHANNEL_wheel_on_ground, bool, telem_ptr->truck_b.truck_wheelOnGround[i], i);
        REGISTER_CHANNEL_INDEX(TRUCK_CHANNEL_wheel_substance, u32, telem_ptr->truck_ui.truck_wheelSubstance[i], i);
        REGISTER_CHANNEL_INDEX(TRUCK_CHANNEL_wheel_velocity, float, telem_ptr->truck_f.truck_wheelVelocity[i], i);
        REGISTER_CHANNEL_INDEX(TRUCK_CHANNEL_wheel_steering, float, telem_ptr->truck_f.truck_wheelSteering[i], i);
        REGISTER_CHANNEL_INDEX(TRUCK_CHANNEL_wheel_rotation, float, telem_ptr->truck_f.truck_wheelRotation[i], i);
        REGISTER_CHANNEL_INDEX(TRUCK_CHANNEL_wheel_susp_deflection, float, telem_ptr->truck_f.
            truck_wheelSuspDeflection[i], i);
        REGISTER_CHANNEL_INDEX(TRUCK_CHANNEL_wheel_lift, float, telem_ptr->truck_f.truck_wheelLift[i], i);
        REGISTER_CHANNEL_INDEX(TRUCK_CHANNEL_wheel_lift_offset, float, telem_ptr->truck_f.truck_wheelLiftOffset[i], i
        );
    }

    REGISTER_CHANNEL(TRUCK_CHANNEL_cabin_offset, fplacement, telem_ptr->truck_fp.cabinOffsetX);
    REGISTER_CHANNEL(TRUCK_CHANNEL_cabin_angular_velocity, fvector, telem_ptr->truck_fv.cabinAVX);
    REGISTER_CHANNEL(TRUCK_CHANNEL_cabin_angular_acceleration, fvector, telem_ptr->truck_fv.cabinAAX);
    REGISTER_CHANNEL(CHANNEL_next_rest_stop, s32, telem_ptr->common_i.restStop);
    REGISTER_CHANNEL(CHANNEL_local_scale, float, telem_ptr->common_f.scale);
    REGISTER_CHANNEL(TRUCK_CHANNEL_head_offset, fplacement, telem_ptr->truck_fp.headOffsetX);
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
SCSAPI_VOID scs_telemetry_shutdown() {
    // Close MemoryMap
    if (telem_mem != nullptr) {
        telem_mem->Close();
    }
}

// Telemetry api.

// ReSharper disable once CppInconsistentNaming
BOOL APIENTRY DllMain(
    HMODULE module,
    DWORD reason_for_call,
    LPVOID reseved
) {
    if (reason_for_call == DLL_PROCESS_DETACH) {

        scs_telemetry_shutdown();

    }
    return TRUE;
}
