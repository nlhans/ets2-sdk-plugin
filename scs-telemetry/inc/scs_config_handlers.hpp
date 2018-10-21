#ifndef SCS_CFG_HANDLERS_H
#define SCS_CFG_HANDLERS_H

// SDK
#include "scssdk_telemetry.h"
#include "eurotrucks2/scssdk_eut2.h"
#include "eurotrucks2/scssdk_telemetry_eut2.h"

typedef struct scsConfigHandler_s
{
	char const *id;
	void (*handle) (const scs_named_value_t* current);
} scsConfigHandler_t;

#define scsConfigHandle(n) void handle##n (const scs_named_value_t* current)

// Define prototypes for all the various handlers

scsConfigHandle(FuelWarningFactor);
scsConfigHandle(AdblueCapacity);
scsConfigHandle(AdblueWarningFactor);

scsConfigHandle(AirPressureEmergency);
scsConfigHandle(AirPressureWarning);

scsConfigHandle(OilPressureWarning);
scsConfigHandle(WaterTemperatureWarning);
scsConfigHandle(BatteryVoltageWarning);
scsConfigHandle(WheelCount);
scsConfigHandle(WheelPosition);
scsConfigHandle(WheelSteerable);
scsConfigHandle(WheelSimulated);
scsConfigHandle(WheelRadius);
scsConfigHandle(WheelPowered);
scsConfigHandle(WheelLiftable);
scsConfigHandle(SelectorCount);
scsConfigHandle(ShifterType);
scsConfigHandle(RetarderStepCount);
scsConfigHandle(CabinPosition);
scsConfigHandle(HeadPosition);
scsConfigHandle(HookPosition);

scsConfigHandle(Id);
scsConfigHandle(CargoId);

scsConfigHandle(TruckMake);
scsConfigHandle(TruckMakeId);
scsConfigHandle(TruckModel);


scsConfigHandle(FuelCapacity);
scsConfigHandle(RpmLimit);
scsConfigHandle(FGearCount);
scsConfigHandle(RGearCount);

scsConfigHandle(GearDifferential);
scsConfigHandle(GearForwardRatio);
scsConfigHandle(GearReverseRatio);


scsConfigHandle(JobIncome);
scsConfigHandle(JobDeadline);
scsConfigHandle(TrailerMass);
scsConfigHandle(TrailerId);
scsConfigHandle(TrailerName);
scsConfigHandle(CitySrc);
scsConfigHandle(CityDst);
scsConfigHandle(CompSrc);
scsConfigHandle(CompDst);

scsConfigHandle(CitySrcId);
scsConfigHandle(CityDstId);
scsConfigHandle(CompSrcId);
scsConfigHandle(CompDstId);

bool handleCfg(const scs_named_value_t* info);

#endif