#include <cstring>

#include "scs_config_handlers.hpp"

// Plug-in
#include "scs-telemetry-common.hpp"
#include "sharedmemory.hpp"


extern SharedMemory* telem_mem;
extern scsTelemetryMap_t* telem_ptr;
extern void log_line(scs_log_type_t type, const char*const text, ...);

const scsConfigHandler_t scs_config_table[] = {

        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_brand_id, handleTruckMakeId},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_brand, handleTruckMake},

        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_id, handleId},

        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_accessory_id, handleCargoId},

        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_name, handleTruckModel},

        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_fuel_capacity, handleFuelCapacity},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_fuel_warning_factor, handleFuelWarningFactor},

        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_adblue_capacity, handleAdblueCapacity},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_adblue_warning_factor, handleAdblueWarningFactor},

        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_air_pressure_warning, handleAirPressureWarning},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_air_pressure_emergency, handleAirPressureEmergency},

        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_oil_pressure_warning, handleOilPressureWarning},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_water_temperature_warning, handleWaterTemperatureWarning},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_battery_voltage_warning, handleBatteryVoltageWarning},

        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_rpm_limit, handleRpmLimit},

        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_forward_gear_count, handleFGearCount},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_reverse_gear_count, handleRGearCount},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_differential_ratio, handleGearDifferential},

        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_retarder_step_count, handleRetarderStepCount},

        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_forward_ratio, handleGearForwardRatio},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_reverse_ratio, handleGearReverseRatio},

        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_cabin_position, handleCabinPosition},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_head_position, handleHeadPosition},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_hook_position, handleHookPosition},

        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_count, handleWheelCount},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_position, handleWheelPosition},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_steerable, handleWheelSteerable},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_simulated, handleWheelSimulated},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_radius, handleWheelRadius},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_powered, handleWheelPowered},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_liftable, handleWheelLiftable},

        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_selector_count, handleSelectorCount},
    // some slot config's missing actually when there is interest in i will add them

        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_shifter_type, handleShifterType},

        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_id, handleTrailerId},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo, handleTrailerName},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_mass, handleTrailerMass},


        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_city_id, handleCityDstId},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_city, handleCityDst},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_company_id, handleCompDstId},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_company, handleCompDst},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_city_id, handleCitySrcId},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_city, handleCitySrc},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_company_id, handleCompSrcId},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_company, handleCompSrc},

        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_income, handleJobIncome},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_delivery_time, handleJobDeadline},

};

#define NO_OF_CFGS ( sizeof(scsConfigTable)/sizeof(scsConfigHandler_t) )

bool handleCfg(const scs_named_value_t* current) {
    for (auto i : scs_config_table) {
        if (strcmp(i.id, current->name) == 0) {
            // Equal ID's; then handle this configuration
            if (i.handle)
                i.handle(current);

            return true;
        }
    }
    return false;
}

scsConfigHandle(Id) {
    // TODO : check if every thing is working like expected
    // ID is shared between vehicle & chassis.
    // So examples could be: vehicle.scania_r and chassis.trailer.overweighl_w
    if (current->value.value_string.value[0] == 'v') {
        // Vehicle ID
        // vehicle.scania_r
        if (telem_ptr) {
            strncpy_s(telem_ptr->config_s.Vehicle, current->value.value_string.value, stringsize);
        }
        log_line(SCS_LOG_TYPE_warning, "Vehicle Handling");
        log_line(SCS_LOG_TYPE_warning, current->value.value_string.value);

    }
    else {
        auto dot = false;
        for (auto* element = current->value.value_string.value; *element; ++element) {
            if (*element == '.') {
                dot = true;
                break;
            }
        }
        if (dot) {
            if (telem_ptr) {
                strncpy_s(telem_ptr->config_s.Chassis, current->value.value_string.value, stringsize);
                log_line(SCS_LOG_TYPE_warning, "Chassis Chandling");
                log_line(SCS_LOG_TYPE_warning, current->value.value_string.value);

            }
        }


    }
}

scsConfigHandle(FuelWarningFactor) {
    telem_ptr->config_f.fuelWarningFactor = current->value.value_float.value;
}

scsConfigHandle(AdblueCapacity) {
    telem_ptr->config_f.adblueCapacity = current->value.value_float.value;
}

scsConfigHandle(AdblueWarningFactor) {
    telem_ptr->config_f.adblueWarningFactor = current->value.value_float.value;
}

scsConfigHandle(AirPressureWarning) {
    telem_ptr->config_f.airPressureWarning = current->value.value_float.value;
}

scsConfigHandle(AirPressureEmergency) {
    telem_ptr->config_f.airPressurEmergency = current->value.value_float.value;
}

scsConfigHandle(OilPressureWarning) {
    telem_ptr->config_f.oilPressureWarning = current->value.value_float.value;
}

scsConfigHandle(WaterTemperatureWarning) {
    telem_ptr->config_f.waterTemperatureWarning = current->value.value_float.value;
}

scsConfigHandle(BatteryVoltageWarning) {
    telem_ptr->config_f.batteryVoltageWarning = current->value.value_float.value;
}

scsConfigHandle(RetarderStepCount) {
    telem_ptr->config_ui.retarderStepCount = current->value.value_u32.value;
}

scsConfigHandle(CabinPosition) {
    telem_ptr->config_fv.cabinPositionX = current->value.value_fvector.x;
    telem_ptr->config_fv.cabinPositionY = current->value.value_fvector.y;
    telem_ptr->config_fv.cabinPositionZ = current->value.value_fvector.z;
}

scsConfigHandle(HeadPosition) {
    telem_ptr->config_fv.headPositionX = current->value.value_fvector.x;
    telem_ptr->config_fv.headPositionY = current->value.value_fvector.y;
    telem_ptr->config_fv.headPositionZ = current->value.value_fvector.z;
}

scsConfigHandle(HookPosition) {
    telem_ptr->config_fv.hookPositionX = current->value.value_fvector.x;
    telem_ptr->config_fv.hookPositionY = current->value.value_fvector.y;
    telem_ptr->config_fv.hookPositionZ = current->value.value_fvector.z;
}

scsConfigHandle(WheelCount) {
    telem_ptr->config_ui.wheelCount = current->value.value_u32.value;
}

scsConfigHandle(WheelPosition) {
    if (telem_ptr) {
        const auto position = current->index;
        const auto ratio = current->value.value_fvector;

        if (position < WHEEL_SIZE) {
            telem_ptr->config_fv.wheelPositionX[position] = ratio.x;
            telem_ptr->config_fv.wheelPositionY[position] = ratio.y;
            telem_ptr->config_fv.wheelPositionZ[position] = ratio.z;
        }
    }
}

scsConfigHandle(WheelSteerable) {
    if (telem_ptr) {
        const auto position = current->index;
        const auto ratio = current->value.value_bool;

        if (position < WHEEL_SIZE) {
            telem_ptr->config_b.wheelSteerable[position] = ratio.value;
        }
    }
}

scsConfigHandle(WheelSimulated) {
    if (telem_ptr) {
        const auto position = current->index;
        const auto ratio = current->value.value_bool;

        if (position < WHEEL_SIZE) {
            telem_ptr->config_b.wheelSimulated[position] = ratio.value;
        }
    }
}

scsConfigHandle(WheelRadius) {
    if (telem_ptr) {
        const auto position = current->index;
        const auto ratio = current->value.value_float;

        if (position < WHEEL_SIZE) {
            telem_ptr->config_f.wheelRadius[position] = ratio.value;
        }
    }
}

scsConfigHandle(WheelPowered) {
    if (telem_ptr) {
        const auto position = current->index;
        const auto ratio = current->value.value_bool;

        if (position < WHEEL_SIZE) {
            telem_ptr->config_b.wheelPowered[position] = ratio.value;
        }
    }
}

scsConfigHandle(WheelLiftable) {
    if (telem_ptr) {
        const auto position = current->index;
        const auto ratio = current->value.value_bool;

        if (position < WHEEL_SIZE) {
            telem_ptr->config_b.wheelLiftable[position] = ratio.value;
        }
    }
}

scsConfigHandle(SelectorCount) {
    telem_ptr->config_ui.selectorCount = current->value.value_u32.value;
}

scsConfigHandle(ShifterType) {
    if (telem_ptr) {
        strncpy_s(telem_ptr->config_s.shifterType, current->value.value_string.value, 10);
    }
}

scsConfigHandle(TruckMake) {
    if (telem_ptr) {
        strncpy_s(telem_ptr->config_s.truckMake, current->value.value_string.value, stringsize);
    }
}

scsConfigHandle(TruckMakeId) {
    if (telem_ptr) {
        strncpy_s(telem_ptr->config_s.truckMakeId, current->value.value_string.value, stringsize);
    }
}

scsConfigHandle(TruckModel) {
    if (telem_ptr) {
        strncpy_s(telem_ptr->config_s.truckModel, current->value.value_string.value, stringsize);
    }
}

scsConfigHandle(CargoId) {


    // Cargo ID
    // Example: cargo.overweighl_w.kvn
    // Cargo type overweighl_w.kvn can be found in def/cargo/
    if (telem_ptr) {
        strncpy_s(telem_ptr->config_s.Cargo, current->value.value_string.value, stringsize);
    }
}

scsConfigHandle(FuelCapacity) {
    // Fuel capacity
    // Float
    telem_ptr->config_f.fuelCapacity = current->value.value_float.value;
}

scsConfigHandle(RpmLimit) {
    // RPM Limit (often 2500)
    // Float
    telem_ptr->config_f.engineRpmMax = current->value.value_float.value;
}

scsConfigHandle(FGearCount) {
    // No. of drive gears
    // u32
    telem_ptr->config_ui.gears = current->value.value_u32.value;
}

scsConfigHandle(RGearCount) {
    // No. of reverse gears
    // u32
    telem_ptr->config_ui.gears_reverse = current->value.value_u32.value;
}

scsConfigHandle(JobIncome) {
    if (telem_ptr) {
        telem_ptr->config_o.jobIncome = current->value.value_u64.value;
    }
}

scsConfigHandle(JobDeadline) {
    if (telem_ptr) {
        telem_ptr->config_ui.time_abs_delivery = current->value.value_u32.value;
    }
}

scsConfigHandle(TrailerMass) {
    if (telem_ptr) {
        telem_ptr->config_f.trailerMass = current->value.value_float.value;
    }
}

scsConfigHandle(TrailerId) {
    if (telem_ptr) {
        strncpy_s(telem_ptr->config_s.trailerId, current->value.value_string.value, stringsize);
    }
}

scsConfigHandle(TrailerName) {
    if (telem_ptr) {
        strncpy_s(telem_ptr->config_s.trailerName, current->value.value_string.value, stringsize);
    }
}

scsConfigHandle(CitySrc) {
    if (telem_ptr) {
        strncpy_s(telem_ptr->config_s.citySrc, current->value.value_string.value, stringsize);
    }
}

scsConfigHandle( CityDstId) {
    if (telem_ptr) {
        strncpy_s(telem_ptr->config_s.cityDstId, current->value.value_string.value, stringsize);
    }
}

scsConfigHandle(CompDstId) {
    if (telem_ptr) {
        strncpy_s(telem_ptr->config_s.compDstId, current->value.value_string.value, stringsize);
    }
}

scsConfigHandle(CitySrcId) {
    if (telem_ptr) {
        strncpy_s(telem_ptr->config_s.citySrcId, current->value.value_string.value, stringsize);
    }
}

scsConfigHandle(CompSrcId) {
    if (telem_ptr) {
        strncpy_s(telem_ptr->config_s.compSrcId, current->value.value_string.value, stringsize);
    }
}

scsConfigHandle(CityDst) {
    if (telem_ptr) {
        strncpy_s(telem_ptr->config_s.cityDst, current->value.value_string.value, stringsize);
    }
}

scsConfigHandle(CompSrc) {
    if (telem_ptr) {
        strncpy_s(telem_ptr->config_s.compSrc, current->value.value_string.value, stringsize);
    }
}

scsConfigHandle(CompDst) {
    if (telem_ptr) {
        strncpy_s(telem_ptr->config_s.compDst, current->value.value_string.value, stringsize);
    }
}

scsConfigHandle(GearDifferential) {
    if (telem_ptr) {
        telem_ptr->config_f.gearDifferential = current->value.value_float.value;
    }
}

scsConfigHandle(GearForwardRatio) {
    if (telem_ptr) {
        const auto gear = current->index;
        const auto ratio = current->value.value_float.value;

        if (gear < 24) {
            telem_ptr->config_f.gearRatiosForward[gear] = ratio;
        }
    }
}

scsConfigHandle(GearReverseRatio) {
    if (telem_ptr) {
        const auto gear = current->index;
        const auto ratio = current->value.value_float.value;

        if (gear < 24) {
            telem_ptr->config_f.gearRatiosReverse[gear] = ratio;
        }
    }
}
