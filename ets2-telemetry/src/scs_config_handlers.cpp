#include <string.h>

#include "scs_config_handlers.hpp"

// Plug-in
#include "ets2-telemetry-common.hpp"
#include "sharedmemory.hpp"

extern SharedMemory *telemMem;
extern ets2TelemetryMap_t *telemPtr;

const scsConfigHandler_t scsConfigTable[] = {
	
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_brand_id, handleTruckMakeId },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_brand, handleTruckMake },

	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_id, handleId },

	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_accessory_id, handleCargoId },

	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_name, handleTruckModel },
	
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_fuel_capacity, handleFuelCapacity },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_fuel_warning_factor, handleFuelWarningFactor},

	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_adblue_capacity, handleAdblueCapacity},
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_adblue_warning_factor, handleAdblueWarningFactor},

	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_air_pressure_warning, handleAirPressureWarning},
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_air_pressure_emergency, handleAirPressureEmergency},

	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_oil_pressure_warning, handleOilPressureWarning},
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_water_temperature_warning, handleWaterTemperatureWarning},
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_battery_voltage_warning, handleBatteryVoltageWarning},

	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_rpm_limit, handleRpmLimit },

	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_forward_gear_count, handleFGearCount },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_reverse_gear_count, handleRGearCount },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_differential_ratio, handleGearDifferential },

	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_retarder_step_count, handleRetarderStepCount },

	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_forward_ratio, handleGearForwardRatio },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_reverse_ratio, handleGearReverseRatio },

	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_cabin_position, handleCabinPosition},
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_head_position, handleHeadPosition},
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_hook_position, handleHookPosition},

	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_count,handleWheelCount},
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_position,handleWheelPosition},
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_steerable,handleWheelSteerable},
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_simulated,handleWheelSimulated},
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_radius,handleWheelRadius},
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_powered,handleWheelPowered},
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_liftable,handleWheelLiftable},

	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_selector_count, handleSelectorCount},
	// some slot missing actually when there is interest in i will add them

	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_shifter_type, handleShifterType},

	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_id, handleTrailerId },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo, handleTrailerName },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_mass, handleTrailerMass },

	
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_city_id, handleCityDstId },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_city, handleCityDst },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_company_id, handleCompDstId },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_company, handleCompDst },	
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_city_id, handleCitySrcId },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_city, handleCitySrc },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_company_id, handleCompSrcId },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_company, handleCompSrc },

	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_income, handleJobIncome },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_delivery_time, handleJobDeadline },

};

#define NO_OF_CFGS ( sizeof(scsConfigTable)/sizeof(scsConfigHandler_t) )

bool handleCfg(const scs_named_value_t* current)
{
	auto i = 0;

	for (i = 0; i < NO_OF_CFGS; i++)
	{
		if (strcmp(scsConfigTable[i].id, current->name) == 0)
		{
			// Equal ID's; then handle this configuration
			if (scsConfigTable[i].handle)
				scsConfigTable[i].handle(current);

			return true;
		}
	}
	return false;
}

scsConfigHandle(Id)
{
	char * strPtr;

	// ID is shared between vehicle & chassis.
	// So examples could be: vehicle.scania_r and chassis.trailer.overweighl_w
	if (current->value.value_string.value[0] == 'v')
	{
		// Vehicle ID
		// vehicle.scania_r
		strPtr = static_cast<char*>(telemMem->getPtrAt(telemPtr->tel_rev1.modelType[0]));
		strcpy(strPtr, current->value.value_string.value);
		telemPtr->tel_rev1.modelType[1] = strlen(current->value.value_string.value);
				
	}
}
scsConfigHandle(FuelWarningFactor) {
	telemPtr->tel_rev3.substances = current->value.value_float.value;
}
scsConfigHandle(AdblueCapacity) {
	telemPtr->tel_unsorted.adblueCapacity = current->value.value_float.value;
}
scsConfigHandle(AdblueWarningFactor) {
	telemPtr->tel_unsorted.adblueWarningFacto = current->value.value_float.value;
}
scsConfigHandle(AirPressureWarning) {
	telemPtr->tel_unsorted.airPressureWarning = current->value.value_float.value;
}
scsConfigHandle(AirPressureEmergency) {
	telemPtr->tel_unsorted.airPressurEmergency = current->value.value_float.value;
}
scsConfigHandle(OilPressureWarning) {
	telemPtr->tel_unsorted.oilPressureWarning = current->value.value_float.value;
}
scsConfigHandle(WaterTemperatureWarning) {
	telemPtr->tel_unsorted.waterTemperatureWarning = current->value.value_float.value;
}
scsConfigHandle(BatteryVoltageWarning) {
	telemPtr->tel_unsorted.batteryVoltageWarning = current->value.value_float.value;
}
scsConfigHandle(RetarderStepCount) {
	telemPtr->tel_unsorted.retarderStepCount = current->value.value_u32.value;
}
scsConfigHandle(CabinPosition) {
	telemPtr->tel_unsorted.cabinPositionX = current->value.value_fvector.x;
	telemPtr->tel_unsorted.cabinPositionY = current->value.value_fvector.y;
	telemPtr->tel_unsorted.cabinPositionZ = current->value.value_fvector.z;
}
scsConfigHandle(HeadPosition) {
	telemPtr->tel_unsorted.headPositionX = current->value.value_fvector.x;
	telemPtr->tel_unsorted.headPositionY = current->value.value_fvector.y;
	telemPtr->tel_unsorted.headPositionZ = current->value.value_fvector.z;
}

scsConfigHandle(HookPosition) {
	telemPtr->tel_unsorted.hookPositionX = current->value.value_fvector.x;
	telemPtr->tel_unsorted.hookPositionY = current->value.value_fvector.y;
	telemPtr->tel_unsorted.hookPositionZ = current->value.value_fvector.z;
}

scsConfigHandle(WheelCount) {
	telemPtr->tel_unsorted.wheelCount = current->value.value_u32.value;
}

scsConfigHandle(WheelPosition) {
	if (telemPtr)
	{
		int position = current->index;
		auto ratio = current->value.value_fvector;

		if (position < 14)
		{
			telemPtr->tel_unsorted.wheelPositionX[position] = ratio.x;
			telemPtr->tel_unsorted.wheelPositionY[position] = ratio.y;
			telemPtr->tel_unsorted.wheelPositionZ[position] = ratio.z;
		}
	}
}
scsConfigHandle(WheelSteerable) {
	if (telemPtr)
	{
		int position = current->index;
		auto ratio = current->value.value_bool;

		if (position < 18)
		{
			telemPtr->tel_unsorted.wheelSteerable[position] = ratio.value; 
		}
	}
}
scsConfigHandle(WheelSimulated) {
	if (telemPtr)
	{
		int position = current->index;
		auto ratio = current->value.value_bool;

		if (position < 18)
		{
			telemPtr->tel_unsorted.wheelSimulated[position] = ratio.value;
		}
	}
}
scsConfigHandle(WheelRadius) {
	if (telemPtr)
	{
		int position = current->index;
		auto ratio = current->value.value_float;

		if (position < 18)
		{
			telemPtr->tel_unsorted.wheelRadius[position] = ratio.value;
		}
	}
}
scsConfigHandle(WheelPowered) {
	if (telemPtr)
	{
		int position = current->index;
		auto ratio = current->value.value_bool;

		if (position < 18)
		{
			telemPtr->tel_unsorted.wheelPowered[position] = ratio.value;
		}
	}
}
scsConfigHandle(WheelLiftable) {
	if (telemPtr)
	{
		int position = current->index;
		auto ratio = current->value.value_bool;

		if (position < 18)
		{
			telemPtr->tel_unsorted.wheelLiftable[position] = ratio.value;
		}
	}
}
scsConfigHandle(SelectorCount) {
	telemPtr->tel_unsorted.selectorCount = current->value.value_u32.value;
}
scsConfigHandle(ShifterType)
{
	if (telemPtr)
	{
		strncpy(telemPtr->tel_unsorted.shifterType, current->value.value_string.value, 10);
	}
}

scsConfigHandle(TruckMake)
{
	if (telemPtr)
	{
		strncpy(telemPtr->tel_rev3.truckMake, current->value.value_string.value, 64);
	}
}
scsConfigHandle(TruckMakeId)
{
	if (telemPtr)
	{
		strncpy(telemPtr->tel_rev3.truckMakeId, current->value.value_string.value, 64);
	}
}
scsConfigHandle(TruckModel)
{
	if (telemPtr)
	{
		strncpy(telemPtr->tel_rev3.truckModel, current->value.value_string.value, 64);
	}
}

scsConfigHandle(CargoId)
{
	char * strPtr;

	// Cargo ID
	// Example: cargo.overweighl_w.kvn
	// Cargo type overweighl_w.kvn can be found in def/cargo/
	strPtr = static_cast<char*>(telemMem->getPtrAt(telemPtr->tel_rev1.trailerType[0]));
	strcpy(strPtr, current->value.value_string.value);
	telemPtr->tel_rev1.trailerType[1] = strlen(current->value.value_string.value);\
}

scsConfigHandle(FuelCapacity)
{
	// Fuel capacity
	// Float
	telemPtr->tel_rev1.fuelCapacity = current->value.value_float.value;
}

scsConfigHandle(RpmLimit)
{
	// RPM Limit (often 2500)
	// Float
	telemPtr->tel_rev1.engineRpmMax = current->value.value_float.value;
}

scsConfigHandle(FGearCount)
{
	// No. of drive gears
	// u32
	telemPtr->tel_rev1.gears = current->value.value_u32.value;
}

scsConfigHandle(RGearCount)
{
	// No. of reverse gears
	// u32
	telemPtr->tel_rev2.gears_reverse = current->value.value_u32.value;
}

scsConfigHandle(JobIncome)
{
	if (telemPtr)
	{
		telemPtr->tel_rev2.jobIncome = current->value.value_u64.value;
	}
}

scsConfigHandle(JobDeadline)
{
	if (telemPtr)
	{
		telemPtr->tel_rev2.time_abs_delivery = current->value.value_u32.value;
	}
}

scsConfigHandle(TrailerMass)
{
	if (telemPtr)
	{
		telemPtr->tel_rev2.trailerMass = current->value.value_float.value;
	}
}

scsConfigHandle(TrailerId)
{
	if (telemPtr)
	{
		strncpy(telemPtr->tel_rev2.trailerId, current->value.value_string.value, 64);
	}
}

scsConfigHandle(TrailerName)
{
	if (telemPtr)
	{
		strncpy(telemPtr->tel_rev2.trailerName, current->value.value_string.value, 64);
	}
}

scsConfigHandle(CitySrc)
{
	if (telemPtr)
	{
		strncpy(telemPtr->tel_rev2.citySrc, current->value.value_string.value, 64);
	}
}
scsConfigHandle( CityDstId)
{
	if (telemPtr)
	{
		strncpy(telemPtr->tel_unsorted.cityDstId, current->value.value_string.value, 64);
	}
}
scsConfigHandle(CompDstId)
{
	if (telemPtr)
	{
		strncpy(telemPtr->tel_unsorted.compDstId, current->value.value_string.value, 64);
	}
}
scsConfigHandle(CitySrcId)
{
	if (telemPtr)
	{
		strncpy(telemPtr->tel_unsorted.citySrcId, current->value.value_string.value, 64);
	}
}
scsConfigHandle(CompSrcId)
{
	if (telemPtr)
	{
		strncpy(telemPtr->tel_unsorted.compSrcId, current->value.value_string.value, 64);
	}
}

scsConfigHandle(CityDst)
{
	if (telemPtr)
	{
		strncpy(telemPtr->tel_rev2.cityDst, current->value.value_string.value, 64);
	}
}

scsConfigHandle(CompSrc)
{
	if (telemPtr)
	{
		strncpy(telemPtr->tel_rev2.compSrc, current->value.value_string.value, 64);
	}
}

scsConfigHandle(CompDst)
{
	if (telemPtr)
	{
		strncpy(telemPtr->tel_rev2.compDst, current->value.value_string.value, 64);
	}
}

scsConfigHandle(GearDifferential)
{
	if (telemPtr)
	{
		telemPtr->tel_rev4.gearDifferential = current->value.value_float.value;
	}
}

scsConfigHandle(GearForwardRatio)
{
	if (telemPtr)
	{
		int gear = current->index;
		float ratio = current->value.value_float.value;

		if (gear < 24)
		{
			telemPtr->tel_rev4.gearRatiosForward[gear] = ratio;
		}
	}
}

scsConfigHandle(GearReverseRatio)
{
	if (telemPtr)
	{
		int gear = current->index;
		float ratio = current->value.value_float.value;

		if (gear < 24)
		{
			telemPtr->tel_rev4.gearRatiosReverse[gear] = ratio;
		}
	}
}
