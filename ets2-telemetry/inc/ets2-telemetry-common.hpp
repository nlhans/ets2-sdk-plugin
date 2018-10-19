#ifndef ETS2_TELEMETRY_COMMON_HPP
#define ETS2_TELEMETRY_COMMON_HPP

// This file contains "Common definitions" for this ETS2 telemetry plug-in.
// This includes:
// - Debug logging detail options
// - Shared memory map struct layout
// - [..]

#define PLUGIN_REVID					6

#define ETS2_PLUGIN_LOGGING_ON				0
#define ETS2_PLUGIN_LOGGING_SHAREDMEMORY	0
#define ETS2_PLUGIN_FILENAME_PREFIX "C:\ets2telem_"

#if ETS2_PLUGIN_LOGGING_ON == 1
#define SDK_ENABLE_LOGGING
#endif

#define ETS2_PLUGIN_MMF_NAME TEXT("Local\\SimTelemetrySCS")
#define ETS2_PLUGIN_MMF_SIZE (16*1024)

#define TRUCK_STRING_OFFSET 15*1024
#define TRAILER_STRING_OFFSET TRUCK_STRING_OFFSET+64

typedef struct ets2TelemetryMap_s
{
	unsigned int time;
	unsigned int paused;

	// Contains Game independent values
	struct
	{
		// Telemetry Plugin Version 
		unsigned int telemetry_plugin_revision;
		// Game major version
		unsigned int version_major;
		// Game minor version
		unsigned int version_minor;
		// Game identifier
		unsigned int game; // actually 0 for unknown,1 for ets2 and 2 for ats
		// Game telemetry version major
		unsigned int telemetry_version_game_major;
		// Game telemetry version minor
		unsigned int telemetry_version_game_minor;		 
	} tel_game_values;
	struct {
		float scale;
		long time_abs;
		int restStop;
	}tel_channel;
	
	struct
	{
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
		int lightsAuxFront;
		int lightsAuxRoof;
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
		float substances;


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

	struct {
		float adblueCapacity;
		float adblueWarningFacto;
		float airPressureWarning;
		float airPressurEmergency;
		float oilPressureWarning;
		float waterTemperatureWarning;
		float batteryVoltageWarning;
		int retarderStepCount;
		float cabinPositionX;
		float cabinPositionY;
			float cabinPositionZ;
			float headPositionX;
			float headPositionY;
			float headPositionZ;
			float hookPositionX;
			float hookPositionY;
			float hookPositionZ;
			int wheelCount;
			float wheelPositionX[16];
			float wheelPositionY[16];
			float wheelPositionZ[16];

			bool wheelSteerable[16];
			bool wheelSimulated[16];
			float wheelRadius[16];
			bool wheelPowered[16];
			bool wheelLiftable[16];

			int selectorCount;
			char shifterType[16];
			char cityDstId[64];
			char citySrcId[64];
			char compDstId[64];
			char compSrcId[64];

			float trailer_coordinateX;
			float trailer_coordinateY;
			float trailer_coordinateZ;

			float trailer_rotationX;
			float trailer_rotationY;
			float trailer_rotationZ;

			float trailer_lv_accelerationX;
			float trailer_lv_accelerationY;
			float trailer_lv_accelerationZ;

			float trailer_av_accelerationX;
			float trailer_av_accelerationY;
			float trailer_av_accelerationZ;

			float trailer_la_accelerationX;
			float trailer_la_accelerationY;
			float trailer_la_accelerationZ;

			float trailer_aa_accelerationX;
			float trailer_aa_accelerationY;
			float trailer_aa_accelerationZ;

			float trailer_wheelSuspDeflection[16];

			bool trailer_wheelOnGround[16];
			int trailer_wheelSubstance[16];
			float trailer_wheelVelocity[16];
			float trailer_wheelSteering[16];
			float trailer_wheelRotation[16];

			float lv_accelerationX;
			float lv_accelerationY;
			float lv_accelerationZ;

			float av_accelerationX;
			float av_accelerationY;
			float av_accelerationZ;

			float aa_accelerationX;
			float aa_accelerationY;
			float aa_accelerationZ;

			float cabinOffsetX;
			float cabinOffsetY;
			float cabinOffsetZ;

			float cabinOffsetrotationX;
			float cabinOffsetrotationY;
			float cabinOffsetrotationZ;

			float cabinAVX;
			float cabinAVY;
			float cabinAVZ;

			float cabinAAX;
			float cabinAAY;
			float cabinAAZ;

			float headOffsetX;
			float headOffsetY;
			float headOffsetZ;

			float headOffsetrotationX;
			float headOffsetrotationY;
			float headOffsetrotationZ;

			float truck_wheelSuspDeflection[16];

			bool truck_wheelOnGround[16];
			int truck_wheelSubstance[16];
			float truck_wheelVelocity[16];
			float truck_wheelSteering[16];
			float truck_wheelRotation[16];

			float truck_wheelLift[16];
			float truck_wheelLiftOffset[16];
			char test[10];// Delete

	}tel_unsorted;

} ets2TelemetryMap_t;

#endif