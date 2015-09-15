#ifndef ETS2_TELEMETRY_COMMON_HPP
#define ETS2_TELEMETRY_COMMON_HPP

// This file contains "Common definitions" for this ETS2 telemetry plug-in.
// This includes:
// - Debug logging detail options
// - Shared memory map struct layout
// - [..]

#define ETS2_PLUGIN_REVID					5

#define ETS2_PLUGIN_LOGGING_ON				0
#define ETS2_PLUGIN_LOGGING_SHAREDMEMORY	0
#define ETS2_PLUGIN_FILENAME_PREFIX "C:\ets2telem_"

#if ETS2_PLUGIN_LOGGING_ON == 1
#define SDK_ENABLE_LOGGING
#endif

#define ETS2_PLUGIN_MMF_NAME TEXT("Local\\SimTelemetryETS2")
#define ETS2_PLUGIN_MMF_SIZE (16*1024)

#define TRUCK_STRING_OFFSET 15*1024
#define TRAILER_STRING_OFFSET TRUCK_STRING_OFFSET+64

typedef struct ets2TelemetryMap_s
{
	unsigned int time;
	unsigned int paused;


	struct
	{
		unsigned int ets2_telemetry_plugin_revision;
		unsigned int ets2_version_major;
		unsigned int ets2_version_minor;
	} tel_revId;

	// All variables per revision are packed into 1 struct.
	// Newer revisions must contain identical struct layouts/lengths, even if variabeles become deprecated.
	// Replaced/new variabeles should be added in seperate structs
	struct
	{
		bool engine_enabled;
		bool trailer_attached;

		// vehicle dynamics
		float speed;
		
		float accelerationX;
		float accelerationY;
		float accelerationZ;
	
		float coordinateX;
		float coordinateY;
		float coordinateZ;
	
		float rotationX;
		float rotationY;
		float rotationZ;
	
		// drivetrain essentials
		int gear;
		int gears;
		int gearRanges;
		int gearRangeActive;

		float engineRpm;
		float engineRpmMax;
		
		float fuel;
		float fuelCapacity;
		float fuelRate;				// ! Not working
		float fuelAvgConsumption;
		
		// user input
		float userSteer;
		float userThrottle;
		float userBrake;
		float userClutch;
		
		float gameSteer;
		float gameThrottle;
		float gameBrake;
		float gameClutch;
	
		// truck & trailer
		float truckWeight;
		float trailerWeight;
		
		int modelType[2];
		int trailerType[2];			// ! deprecated

	} tel_rev1;

	struct
	{
		long time_abs;
		int gears_reverse;

		// Trailer ID & display name
		float trailerMass;
		char trailerId[64];
		char trailerName[64];
		
		// Job information
		int jobIncome;
		int time_abs_delivery;
		char citySrc[64];
		char cityDst[64];
		char compSrc[64];
		char compDst[64];

	} tel_rev2;	

	struct
	{
		int retarderBrake;
		int shifterSlot;
		int shifterToggle;
		int fill;

		bool cruiseControl;
		bool wipers;

		bool parkBrake;
		bool motorBrake;

		bool electricEnabled;
		bool engineEnabled;

		bool blinkerLeftActive;
		bool blinkerRightActive;
		bool blinkerLeftOn;
		bool blinkerRightOn;

		bool lightsParking;
		bool lightsBeamLow;
		bool lightsBeamHigh;
		bool lightsAuxFront;
		bool lightsAuxRoof;
		bool lightsBeacon;
		bool lightsBrake;
		bool lightsReverse;

		bool batteryVoltageWarning;
		bool airPressureWarning;
		bool airPressureEmergency;
		bool adblueWarning;
		bool oilPressureWarning;
		bool waterTemperatureWarning;

		float airPressure;
		float brakeTemperature;
		int fuelWarning;
		float adblue;
		float adblueConsumption;
		float oilPressure;
		float oilTemperature;
		float waterTemperature;
		float batteryVoltage;
		float lightsDashboard;
		float wearEngine;
		float wearTransmission;
		float wearCabin;
		float wearChassis;
		float wearWheels;
		float wearTrailer;
		float truckOdometer;
		float cruiseControlSpeed;

		// General info about the truck etc;
		char truckMake[64];
		char truckMakeId[64];
		char truckModel[64];


	} tel_rev3;

	struct
	{
		float speedLimit;
		float routeDistance;
		float routeTime;
		float fuelRange;
		float gearRatiosForward[24];
		float gearRatiosReverse[8];
		float gearDifferential;
		int gearDashboard;
	} tel_rev4; // added in sdk1.5

	struct
	{
		bool onJob;
		bool jobFinished;
	} tel_rev5;

} ets2TelemetryMap_t;

#endif