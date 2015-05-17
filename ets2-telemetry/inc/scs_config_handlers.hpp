#ifndef SCS_CFG_HANDLERS_H
#define SCS_CFG_HANDLERS_H

// SDK
#include "scssdk_telemetry.h"
#include "eurotrucks2/scssdk_eut2.h"
#include "eurotrucks2/scssdk_telemetry_eut2.h"

typedef struct scsConfigHandler_s
{
	char* id;
	void (*handle) (const scs_named_value_t* current);
} scsConfigHandler_t;

#define scsConfigHandle(n) void handle##n (const scs_named_value_t* current)

// Define prototypes for all the various handlers

// Truck info etc.
scsConfigHandle(Id);
scsConfigHandle(CargoId);

scsConfigHandle(TruckMake);
scsConfigHandle(TruckMakeId);
scsConfigHandle(TruckModel);

// Technical vehicle info
scsConfigHandle(FuelCapacity);
scsConfigHandle(RpmLimit);
scsConfigHandle(FGearCount);
scsConfigHandle(RGearCount);

scsConfigHandle(GearDifferential);
scsConfigHandle(GearForwardRatio);
scsConfigHandle(GearReverseRatio);

// Job info
scsConfigHandle(JobIncome);
scsConfigHandle(JobDeadline);
scsConfigHandle(TrailerMass);
scsConfigHandle(TrailerId);
scsConfigHandle(TrailerName);
scsConfigHandle(CitySrc);
scsConfigHandle(CityDst);
scsConfigHandle(CompSrc);
scsConfigHandle(CompDst);

bool handleCfg(const scs_named_value_t* info);

#endif