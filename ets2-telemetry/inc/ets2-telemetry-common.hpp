#ifndef ETS2_TELEMETRY_COMMON_HPP
#define ETS2_TELEMETRY_COMMON_HPP

// This file contains "Common definitions" for this ETS2 telemetry plug-in.
// This includes:
// - Debug logging detail options
// - Shared memory map struct layout
// - [..]

#define ETS2_PLUGIN_REVID					2

#define ETS2_PLUGIN_LOGGING_ON				0
#define ETS2_PLUGIN_LOGGING_SHAREDMEMORY	1
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

} ets2TelemetryMap_t;

#endif