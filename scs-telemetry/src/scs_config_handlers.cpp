#include <cstring>
#include "scs_config_handlers.hpp"

// Plug-in
#include "scs-telemetry-common.hpp"
#include "sharedmemory.hpp"

#include "log.hpp"

extern SharedMemory* telem_mem;
extern scsTelemetryMap_t* telem_ptr;
#pragma region scsConfigHandler_t[]

// const: substances_config
// handle config id `substances`
const scsConfigHandler_t substances_config[] = {
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_id, handleSubstancesId}
};

// const: controls_config
// handle config id `controls`
const scsConfigHandler_t controls_config[] = {
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_shifter_type, handleControlsShifterType}
};

// const: hshifter_config
// handle config id `hshifter`
const scsConfigHandler_t hshifter_config[] = {
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_selector_count, handleHShifterSelectorCount},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_slot_gear, handleHShifterResulting},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_slot_handle_position, handleHShifterPosition},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_slot_selectors, handleHShifterBitmask}
};

// const: truck_config
// handle config id `truck` 
const scsConfigHandler_t truck_config[] = {
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_brand_id, handleTruckBrandId},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_brand, handleTruckBrand},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_id, handleTruckId},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_name, handleTruckName},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_fuel_capacity, handleTruckFuelCapacity},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_fuel_warning_factor, handleTruckFuelWarningFactor},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_adblue_capacity, handleTruckAdblueCapacity},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_adblue_warning_factor, handleTruckAdblueWarningFactor},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_air_pressure_warning, handleTruckAirPressureWarning},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_air_pressure_emergency, handleTruckAirPressureEmergency},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_oil_pressure_warning, handleTruckOilPressureWarning},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_water_temperature_warning, handleTruckWaterTemperatureWarning},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_battery_voltage_warning, handleTruckBatteryVoltageWarning},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_rpm_limit, handleTruckRpmLimit},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_forward_gear_count, handleTruckFGearCount},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_reverse_gear_count, handleTruckRGearCount},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_retarder_step_count, handleTruckRetarderStepCount},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_cabin_position, handleTruckCabinPosition},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_head_position, handleTruckHeadPosition},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_hook_position, handleTruckHookPosition},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_count, handleTruckWheelCount},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_position, handleTruckWheelPosition},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_steerable, handleTruckWheelSteerable},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_simulated, handleTruckWheelSimulated},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_radius, handleTruckWheelRadius},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_powered, handleTruckWheelPowered},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_liftable, handleTruckWheelLiftable},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_differential_ratio, handleTruckGearDifferential},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_forward_ratio, handleTruckGearForwardRatio},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_reverse_ratio, handleTruckGearReverseRatio}
};

// const: trailer_config
// handle config id `trailer`
const scsConfigHandler_t trailer_config[] = {
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_id, handleTrailerId},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_accessory_id, handleTrailerCargoAccessoryId},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_hook_position, handleTrailerHookPosition},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_count, handleTrailerWheelCount},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_position, handleTrailerWheelOffset},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_steerable, handleTrailerWheelSteerable},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_simulated, handleTrailerWheelSimulated},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_radius, handleTrailerWheelRadius},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_powered, handleTrailerWheelPowered},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_liftable, handleTrailerWheelLiftable}
};

// const: job_config
// handle config id `job`
const scsConfigHandler_t job_config[] = {
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_id, handleJobCargoId},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo, handleJobCargo},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_mass, handleJobCargoMass},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_city_id, handleJobCityDstId},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_city, handleJobCityDst},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_city_id, handleJobCitySrcId},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_city, handleJobCitySrc},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_company_id, handleJobCompDstId},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_company, handleJobCompDst},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_company_id, handleJobCompSrcId},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_company, handleJobCompSrc},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_income, handleJobIncome},
        {SCS_TELEMETRY_CONFIG_ATTRIBUTE_delivery_time, handleJobDeliveryTime},
};

const int length_configs[] = {
    sizeof substances_config / sizeof*substances_config,
    sizeof controls_config / sizeof*controls_config,
    sizeof hshifter_config / sizeof*hshifter_config,
    sizeof truck_config / sizeof*truck_config,
    sizeof trailer_config / sizeof*trailer_config,
    sizeof job_config / sizeof*job_config
};
#pragma endregion Contains all handler arrays


// Function: handleCfg
// brings the config attributes to the correct function
bool handleCfg(const scs_named_value_t* current, const configType type) {
    const scsConfigHandler_t* configs = nullptr;
    switch (type) {
    case substances:
        configs = substances_config;
        break;
    case controls:
        configs = controls_config;
        break;
    case hshifter:
        configs = hshifter_config;
        break;
    case truck:
        configs = truck_config;
        break;
    case trailer:
        configs = trailer_config;
        break;
    case job:
        configs = job_config;
        break;
    default:
        // something went wrong
        ;
    }
    auto i = configs;
    for (auto index = 0; index < length_configs[type]; index++) {
        if (strcmp(i->id, current->name) == 0) {
            if (telem_ptr) {
                // Equal ID's; then handle this configuration
                if (i->handle)
                    i->handle(current);
            }
            return true;
        }
        ++i;
    }
    return false;
}

#pragma region handleSubstances
// Function: handleSubstancesId
// handle the Substances Id and write it to the memory
// It write up to <SUBSTANCE_SIZE> substances into the memory
scsConfigHandle(Substances, Id) {
    if (current->index < SUBSTANCE_SIZE) {
#if LOGGING
		logger::out << "Substance log" << '\n';
		logger::out << "current value of that substance " <<  telem_ptr->substances.substance[current->index] << '\n';
#endif
        strncpy_s(telem_ptr->substances.substance[current->index], current->value.value_string.value, stringsize);
#if LOGGING
		logger::out << "new value of that substance " <<  telem_ptr->substances.substance[current->index] << '\n';
		logger::out << "field value of that substance " << current->value.value_string.value << '\n';
#endif
    }
}


#pragma endregion All handler of the id substances

#pragma region handleControls
// Function: handleControlsShifterType
// handle the Controls ShifterType and write it to the memory
scsConfigHandle(Controls, ShifterType) {
    strncpy_s(telem_ptr->config_s.shifterType, current->value.value_string.value, 10);
}
#pragma endregion All handler of the id controls

#pragma region  handleHShifter
// Function: handleHShifterSelectorCount
// handle the HShifter SelectorCount and write it to the memory
scsConfigHandle(HShifter, SelectorCount) {
    telem_ptr->config_ui.selectorCount = current->value.value_u32.value;
}

scsConfigHandle(HShifter, Resulting) {
    const auto gear = current->index;
    const auto value = current->value.value_s32.value;

    if (gear < 32) {
        telem_ptr->truck_i.hshifterResulting[gear] = value;
    }
}

scsConfigHandle(HShifter, Position) {
    const auto gear = current->index;
    const auto value = current->value.value_u32.value;

    if (gear < 32) {
        telem_ptr->truck_ui.hshifterPosition[gear] = value;
    }
}

scsConfigHandle(HShifter, Bitmask) {
    const auto gear = current->index;
    const auto value = current->value.value_u32.value;

    if (gear < 32) {
        telem_ptr->truck_ui.hshifterBitmask[gear] = value;
    }
}
#pragma  endregion  All handler of the id hshifter

#pragma region handleTruck
scsConfigHandle(Truck, BrandId) {
    strncpy_s(telem_ptr->config_s.truckBrandId, current->value.value_string.value, stringsize);
}

scsConfigHandle(Truck, Brand) {
    strncpy_s(telem_ptr->config_s.truckBrand, current->value.value_string.value, stringsize);
}

scsConfigHandle(Truck, Id) {
    strncpy_s(telem_ptr->config_s.truckId, current->value.value_string.value, stringsize);
}

scsConfigHandle(Truck, Name) {
    strncpy_s(telem_ptr->config_s.truckName, current->value.value_string.value, stringsize);
}

scsConfigHandle(Truck, FuelCapacity) {
    telem_ptr->config_f.fuelCapacity = current->value.value_float.value;
}

scsConfigHandle(Truck, FuelWarningFactor) {
    telem_ptr->config_f.fuelWarningFactor = current->value.value_float.value;
}

scsConfigHandle(Truck, AdblueCapacity) {
    telem_ptr->config_f.adblueCapacity = current->value.value_float.value;
}

scsConfigHandle(Truck, AdblueWarningFactor) {
    telem_ptr->config_f.adblueWarningFactor = current->value.value_float.value;
}

scsConfigHandle(Truck, AirPressureWarning) {
    telem_ptr->config_f.airPressureWarning = current->value.value_float.value;
}

scsConfigHandle(Truck, AirPressureEmergency) {
    telem_ptr->config_f.airPressurEmergency = current->value.value_float.value;
}

scsConfigHandle(Truck, OilPressureWarning) {
    telem_ptr->config_f.oilPressureWarning = current->value.value_float.value;
}

scsConfigHandle(Truck, WaterTemperatureWarning) {
    telem_ptr->config_f.waterTemperatureWarning = current->value.value_float.value;
}

scsConfigHandle(Truck, BatteryVoltageWarning) {
    telem_ptr->config_f.batteryVoltageWarning = current->value.value_float.value;
}

scsConfigHandle(Truck, RpmLimit) {
    telem_ptr->config_f.engineRpmMax = current->value.value_float.value;
}

scsConfigHandle(Truck, FGearCount) {
    telem_ptr->config_ui.gears = current->value.value_u32.value;
}

scsConfigHandle(Truck, RGearCount) {
    telem_ptr->config_ui.gears_reverse = current->value.value_u32.value;
}

scsConfigHandle(Truck, RetarderStepCount) {
    telem_ptr->config_ui.retarderStepCount = current->value.value_u32.value;
}

scsConfigHandle(Truck, CabinPosition) {
    telem_ptr->config_fv.cabinPositionX = current->value.value_fvector.x;
    telem_ptr->config_fv.cabinPositionY = current->value.value_fvector.y;
    telem_ptr->config_fv.cabinPositionZ = current->value.value_fvector.z;
}

scsConfigHandle(Truck, HeadPosition) {
    telem_ptr->config_fv.headPositionX = current->value.value_fvector.x;
    telem_ptr->config_fv.headPositionY = current->value.value_fvector.y;
    telem_ptr->config_fv.headPositionZ = current->value.value_fvector.z;
}

scsConfigHandle(Truck, HookPosition) {
    telem_ptr->config_fv.truckHookPositionX = current->value.value_fvector.x;
    telem_ptr->config_fv.truckHookPositionY = current->value.value_fvector.y;
    telem_ptr->config_fv.truckHookPositionZ = current->value.value_fvector.z;
}

scsConfigHandle(Truck, WheelCount) {
    telem_ptr->config_ui.truckWheelCount = current->value.value_u32.value;
}

scsConfigHandle(Truck, WheelPosition) {
    const auto position = current->index;
    const auto ratio = current->value.value_fvector;

    if (position < WHEEL_SIZE) {
        telem_ptr->config_fv.truckWheelPositionX[position] = ratio.x;
        telem_ptr->config_fv.truckWheelPositionY[position] = ratio.y;
        telem_ptr->config_fv.truckWheelPositionZ[position] = ratio.z;
    }
}

scsConfigHandle(Truck, WheelSteerable) {
    const auto position = current->index;
    const auto ratio = current->value.value_bool;

    if (position < WHEEL_SIZE) {
        telem_ptr->config_b.truckWheelSteerable[position] = ratio.value;
    }
}

scsConfigHandle(Truck, WheelSimulated) {
    const auto position = current->index;
    const auto ratio = current->value.value_bool;

    if (position < WHEEL_SIZE) {
        telem_ptr->config_b.truckWheelSimulated[position] = ratio.value;
    }
}

scsConfigHandle(Truck, WheelRadius) {
    const auto position = current->index;
    const auto ratio = current->value.value_float;

    if (position < WHEEL_SIZE) {
        telem_ptr->config_f.truckWheelRadius[position] = ratio.value;
    }
}

scsConfigHandle(Truck, WheelPowered) {

    const auto position = current->index;
    const auto ratio = current->value.value_bool;

    if (position < WHEEL_SIZE) {
        telem_ptr->config_b.truckWheelPowered[position] = ratio.value;
    }
}

scsConfigHandle(Truck, WheelLiftable) {
    const auto position = current->index;
    const auto ratio = current->value.value_bool;

    if (position < WHEEL_SIZE) {
        telem_ptr->config_b.truckWheelLiftable[position] = ratio.value;
    }
}

scsConfigHandle(Truck, GearDifferential) {
    telem_ptr->config_f.gearDifferential = current->value.value_float.value;
}

scsConfigHandle(Truck, GearForwardRatio) {
    const auto gear = current->index;
    const auto ratio = current->value.value_float.value;

    if (gear < 24) {
        telem_ptr->config_f.gearRatiosForward[gear] = ratio;
    }
}

scsConfigHandle(Truck, GearReverseRatio) {
    const auto gear = current->index;
    const auto ratio = current->value.value_float.value;

    if (gear < 24) {
        telem_ptr->config_f.gearRatiosReverse[gear] = ratio;
    }
}
#pragma endregion  All handler of the id truck

#pragma region handleTrailer
scsConfigHandle(Trailer, Id) {
    strncpy_s(telem_ptr->config_s.trailerId, current->value.value_string.value, stringsize);
}

scsConfigHandle(Trailer, CargoAccessoryId) {
    strncpy_s(telem_ptr->config_s.cargoAcessoryId, current->value.value_string.value, stringsize);
}

scsConfigHandle(Trailer, HookPosition) {
    telem_ptr->config_fv.trailerHookPositionX = current->value.value_fvector.x;
    telem_ptr->config_fv.trailerHookPositionY = current->value.value_fvector.y;
    telem_ptr->config_fv.trailerHookPositionZ = current->value.value_fvector.z;
}

scsConfigHandle(Trailer, WheelCount) {
    telem_ptr->config_ui.trailerWheelCount = current->value.value_u32.value;
}

scsConfigHandle(Trailer, WheelOffset) {
    // seems to be WheelPosition
    const auto position = current->index;
    const auto ratio = current->value.value_fvector;
    if (position < WHEEL_SIZE) {
        telem_ptr->config_fv.trailerWheelPositionX[position] = ratio.x;
        telem_ptr->config_fv.trailerWheelPositionY[position] = ratio.y;
        telem_ptr->config_fv.trailerWheelPositionZ[position] = ratio.z;
    }
}

scsConfigHandle(Trailer, WheelSteerable) {
    const auto position = current->index;
    const auto ratio = current->value.value_bool;

    if (position < WHEEL_SIZE) {
        telem_ptr->config_b.trailerWheelSteerable[position] = ratio.value;
    }
}

scsConfigHandle(Trailer, WheelSimulated) {
    const auto position = current->index;
    const auto ratio = current->value.value_bool;

    if (position < WHEEL_SIZE) {
        telem_ptr->config_b.trailerWheelSimulated[position] = ratio.value;
    }
}

scsConfigHandle(Trailer, WheelRadius) {
    const auto position = current->index;
    const auto ratio = current->value.value_float;

    if (position < WHEEL_SIZE) {
        telem_ptr->config_f.trailerWheelRadius[position] = ratio.value;
    }
}

scsConfigHandle(Trailer, WheelPowered) {

    const auto position = current->index;
    const auto ratio = current->value.value_bool;

    if (position < WHEEL_SIZE) {
        telem_ptr->config_b.trailerWheelPowered[position] = ratio.value;
    }
}

scsConfigHandle(Trailer, WheelLiftable) {
    const auto position = current->index;
    const auto ratio = current->value.value_bool;

    if (position < WHEEL_SIZE) {
        telem_ptr->config_b.trailerWheelLiftable[position] = ratio.value;
    }
}
#pragma endregion All handler for the id trailer


#pragma region handleJob
scsConfigHandle(Job, CargoId) {
    strncpy_s(telem_ptr->config_s.cargoId, current->value.value_string.value, stringsize);
}

scsConfigHandle(Job, Cargo) {
    strncpy_s(telem_ptr->config_s.cargo, current->value.value_string.value, stringsize);
}

scsConfigHandle(Job, CargoMass) {
    telem_ptr->config_f.cargoMass = current->value.value_float.value;
}

scsConfigHandle(Job, CityDstId) {
    strncpy_s(telem_ptr->config_s.cityDstId, current->value.value_string.value, stringsize);
}

scsConfigHandle(Job, CityDst) {
    strncpy_s(telem_ptr->config_s.cityDst, current->value.value_string.value, stringsize);
}

scsConfigHandle(Job, CitySrcId) {
    strncpy_s(telem_ptr->config_s.citySrcId, current->value.value_string.value, stringsize);
}

scsConfigHandle(Job, CitySrc) {
    strncpy_s(telem_ptr->config_s.citySrc, current->value.value_string.value, stringsize);
}


scsConfigHandle(Job, CompDstId) {
    strncpy_s(telem_ptr->config_s.compDstId, current->value.value_string.value, stringsize);
}

scsConfigHandle(Job, CompDst) {
    strncpy_s(telem_ptr->config_s.compDst, current->value.value_string.value, stringsize);
}

scsConfigHandle(Job, CompSrcId) {
    strncpy_s(telem_ptr->config_s.compSrcId, current->value.value_string.value, stringsize);
}


scsConfigHandle(Job, CompSrc) {
    strncpy_s(telem_ptr->config_s.compSrc, current->value.value_string.value, stringsize);
}

scsConfigHandle(Job, Income) {
    telem_ptr->config_ull.jobIncome = current->value.value_u64.value;
}

scsConfigHandle(Job, DeliveryTime) {
    telem_ptr->config_ui.time_abs_delivery = current->value.value_u32.value;
}
#pragma endregion All handler of the id job
