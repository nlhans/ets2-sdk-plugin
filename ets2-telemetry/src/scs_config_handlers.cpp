#include <string.h>

#include "scs_config_handlers.hpp"

// Plug-in
#include "ets2-telemetry-common.hpp"
#include "sharedmemory.hpp"

extern SharedMemory *telemMem;
extern ets2TelemetryMap_t *telemPtr;

const scsConfigHandler_t scsConfigTable[] = {
	
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_id, handleId },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_accessory_id, handleCargoId },

	// Truck technical info
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_fuel_capacity, handleFuelCapacity },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_rpm_limit, handleRpmLimit },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_forward_gear_count, handleFGearCount },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_reverse_gear_count, handleRGearCount },

	// Job information
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_income, handleJobIncome },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_delivery_time, handleJobDeadline },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_mass, handleTrailerMass },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_id, handleTrailerId },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo, handleTrailerName },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_city, handleCitySrc },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_city, handleCityDst },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_company, handleCompSrc },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_company, handleCompDst },
};

#define NO_OF_CFGS ( sizeof(scsConfigTable)/sizeof(scsConfigHandler_t) )

bool handleCfg(const scs_named_value_t* current)
{
	int i = 0;

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
