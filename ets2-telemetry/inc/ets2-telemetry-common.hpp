#ifndef ETS2_TELEMETRY_COMMON_HPP
#define ETS2_TELEMETRY_COMMON_HPP

// This file contains "Common definitions" for this ETS2 telemetry plug-in.
// This includes:
// - Debug logging detail options
// - Shared memory map struct layout
// - [..]

#define ETS2_PLUGIN_LOGGING_ON				1
#define ETS2_PLUGIN_LOGGING_SHAREDMEMORY	1
#define ETS2_PLUGIN_FILENAME_PREFIX "ets2telem_"

typedef struct ets2TelemetryMap_s
{
	unsigned int time;
	unsigned int revision;

	// All variables per revision are packed into 1 struct.
	// Newer revisions must contain identical struct layouts/lengths, even if variabeles become deprecated.
	// Replaced/new variabeles should be added in seperate structs
	struct
	{
		// vehicle dynamics
		float speed;
		float acceleration;
	
		float coordinateX;
		float coordinateY;
		float coordinateZ;
	
		float rotationX;
		float rotationY;
		float rotationZ;
	
		// drivetrain essentials
		int gear;
		int gears;

		float engineRpm;
		float engineRpmMax;
		
		float fuel;
		float fuelCapacity;
		float fuelRate;
		float fuelAvgConsumption;

		// user input
		float userThrottle;
		float userBrake;
		float userClutch;

		float gameThrottle;
		float gameBrake;
		float gameClutch;
	
		// truck & trailer
		float truckWeight;

		int hasTrailer;
		float trailerWeight;

	} tel_rev1;
	

} ets2TelemetryMap_t;

#endif