using System;
using System.Runtime.CompilerServices;
using System.Text;

namespace Ets2SdkClient {
    /// <summary>
    /// Telemetry Data off SCS SDK's
    /// </summary>
    ///
    /// <!----> **Currency** <!---->
    ///     ATS use US Dolars as internal currency
    ///     ETS2 use Euro as internal currency
    /// <!----> **Currency** <!---->
    ///
    /// 
    public class SCSTelemetry
    {
        public uint Time { get; internal set; }
        public int AbsolutTime { get; internal set; }
        public DateTime GameTime => SecondsToDate(AbsolutTime);

        public bool Paused { get; internal set; }

        public string Truck { get; internal set; }
        public string TruckId { get; internal set; }

        public string Manufacturer { get; internal set; }
        public string ManufacturerId { get; internal set; }
        public float Scale { get; internal set; }
        public int RestStop { get; internal set; }
        public float FuelWarningFactor { get; internal set; }

        public class _Physics
        {
            public float Speed { get; internal set; }
            public float SpeedKmh { get; internal set; }
            public float SpeedMph { get; internal set; }

            public float AccelerationX { get; internal set; }
            public float AccelerationY { get; internal set; }
            public float AccelerationZ { get; internal set; }

            public float CoordinateX { get; internal set; }
            public float CoordinateY { get; internal set; }
            public float CoordinateZ { get; internal set; }

            public float RotationX { get; internal set; }
            public float RotationY { get; internal set; }
            public float RotationZ { get; internal set; }
        }


        public class _Drivetrain
        {
            public float TruckOdometer { get; internal set; }

            public bool EngineEnabled { get; internal set; }
            public bool ElectricEnabled { get; internal set; }

            public int Gear { get; internal set; }
            public float[] GearRatiosForward { get; internal set; }
            public float[] GearRatiosReverse { get; internal set; }
            public float GearRatioDifferential { get; internal set; }
            public int GearDashboard { get; internal set; }
            public int GearsForward { get; internal set; }
            public int GearsReverse { get; internal set; }
            public int GearRange { get; internal set; }
            public int GearRanges { get; internal set; }

            public float EngineRpm { get; internal set; }
            public float EngineRpmMax { get; internal set; }

            public float Speed { get; internal set; }
            public float SpeedKmh { get; internal set; }
            public float SpeedMph { get; internal set; }

            public float Fuel { get; internal set; }
            public float FuelRange { get; internal set; }
            public float FuelMax { get; internal set; }

            // Deprecated: was never actually functional
            public float FuelWarning { get; internal set; }
            public bool FuelWarningLight { get; internal set; }
            public float FuelRate { get; internal set; }
            /// <summary>
            /// 
            /// </summary>
            /// <!----> **ATS Information** <!---->
            ///     The fuel_average_consumption is currently mostly static and depends
            ///     on presence of the trailer and skills of the driver instead
            ///     of the workload of the engine.
            /// <!----> **ATS Information** <!---->
            public float FuelAvgConsumption { get; internal set; }

            public int Retarder { get; internal set; }

            public int ShifterSlot { get; internal set; }
            public int ShifterToggle { get; internal set; }

            public bool CruiseControl { get; internal set; }
            public float CruiseControlSpeed { get; internal set; }
            public float CruiseControlSpeedKmh { get; internal set; }
            public float CruiseControlSpeedMph { get; internal set; }
            public bool ParkingBrake { get; internal set; }
            public bool MotorBrake { get; internal set; }

            /// <summary>
            /// 
            /// </summary>
            /// <!----> **Information** <!---->
            ///     Not simulated. They are very loosely approximated.
            /// <!----> **Information** <!---->
            public float OilPressure { get; internal set; }
            /// <summary>
            /// 
            /// </summary>
            /// <!----> **Information** <!---->
            ///     Not simulated. They are very loosely approximated.
            /// <!----> **Information** <!---->
            public float OilTemperature { get; internal set; }
            /// <summary>
            /// 
            /// </summary>
            /// <!----> **Information** <!---->
            ///     Not simulated. They are very loosely approximated.
            /// <!----> **Information** <!---->
            public float WaterTemperature { get; internal set; }

            /// <summary>
            /// 
            /// </summary>
            /// <!----> **ATS Information** <!---->
            ///     No Support currently
            /// <!----> **ATS Information** <!---->
            public float Adblue { get; internal set; }
            /// <summary>
            /// 
            /// </summary>
            /// <!----> **ATS Information** <!---->
            ///     No Support currently
            /// <!----> **ATS Information** <!---->
            public float AdblueConsumption { get; internal set; }
            public float AirPressure { get; internal set; }
            /// <summary>
            /// 
            /// </summary>
            /// <!----> **Information** <!---->
            ///     Not simulated. They are very loosely approximated.
            /// <!----> **Information** <!---->
            public float BrakeTemperature { get; internal set; }

            public float Weight { get; internal set; }
            /// <summary>
            /// 
            /// </summary>
            /// <!----> **Information** <!---->
            ///     Not simulated. They are very loosely approximated.
            /// <!----> **Information** <!---->
            public float BatteryVoltage { get; internal set; }


            public float AdblueCapacity { get; internal set; }
            public float AdblueWarningFactor { get; internal set; }

            public float AirPressureWarning { get; internal set; }
            public float AirPressureEmergency { get; internal set; }

            public float OilPressureWarning { get; internal set; }
            public float WaterTemperatureWarning { get; internal set; }
            public float BatteryVoltageWarning { get; internal set; }

            public int RetarderStepCount { get; internal set; }
            public float CabinPositionX { get; internal set; }
            public float CabinPositionY { get; internal set; }
            public float CabinPositionZ { get; internal set; }

            public float HeadPositionX { get; internal set; }
            public float HeadPositionY { get; internal set; }
            public float HeadPositionZ { get; internal set; }

            public float HookPositionX { get; internal set; }
            public float HookPositionY { get; internal set; }
            public float HookPositionZ { get; internal set; }
            public int WheelCount { get; internal set; }

            public float[] WheelPositionX { get; internal set; }
            public float[] WheelPositionY { get; internal set; }
            public float[] WheelPositionZ { get; internal set; }


            public bool[] WheelSteerable { get; internal set; }
            public bool[] WheelSimulated { get; internal set; }
            public float[] WheelRadius { get; internal set; }
            public bool[] WheelPowered { get; internal set; }
            public bool[] WheelLiftable { get; internal set; }



        }

        public class _Controls
        {
            public float UserSteer { get; internal set; }
            public float UserThrottle { get; internal set; }
            public float UserBrake { get; internal set; }
            public float UserClutch { get; internal set; }

            public float GameSteer { get; internal set; }
            public float GameThrottle { get; internal set; }
            public float GameBrake { get; internal set; }
            public float GameClutch { get; internal set; }
        }

        public class _Job
        {
            public bool OnJob { get; internal set; }
            public bool JobFinished { get; internal set; }

            public bool TrailerAttached { get; internal set; }
            public float Mass { get; internal set; }
            public string TrailerId { get; internal set; }
            public string TrailerName { get; internal set; }
            public string Cargo { get; internal set; }
            public int Income { get; internal set; }
            public int Deadline { get; internal set; }
            public string CitySource { get; internal set; }
            public string CityDestination { get; internal set; }
            public string CompanySource { get; internal set; }
            public string CompanyDestination { get; internal set; }
            public float NavigationDistanceLeft { get; internal set; } // meter
            public float NavigationTimeLeft { get; internal set; }
            public float SpeedLimit { get; internal set; } // meter/second
            public float TrailerWeight { get; internal set; }

            public string TrailerModel { get; internal set; }
        }

        public class _Axilliary
        {
            public bool Wipers { get; internal set; }

            public bool BatteryVoltageWarning { get; internal set; }
            public bool AirPressureWarning { get; internal set; }
            public bool AirPressureEmergency { get; internal set; }
            /// <summary>
            /// 
            /// </summary>
            /// <!----> **ATS Information** <!---->
            ///     No Support currently
            /// <!----> **ATS Information** <!---->
            public bool AdblueWarning { get; internal set; }
            public bool OilPressureWarning { get; internal set; }
            public bool WaterTemperatureWarning { get; internal set; }

            public int SelectorCount { get; internal set; }
            public string ShifterType { get;internal set; } //TODO: ENUM
            public string cityDestinationId { get; internal set; }
            public string citySourceId { get; internal set; }
            public string compDestinationId { get; internal set; }
            public string compSourceId { get; internal set; }
            public float trailer_coordinateX { get; internal set; }
            public float trailer_coordinateY { get; internal set; }
            public float trailer_coordinateZ { get; internal set; }

            public float trailer_RotationeX { get; internal set; }
            public float trailer_RotationeY { get; internal set; }
            public float trailer_RotationeZ { get; internal set; }

            public float trailer_lv_accelerationX { get; internal set; }
            public float trailer_lv_accelerationY { get; internal set; }
            public float trailer_lv_accelerationZ { get; internal set; }

            public float trailer_av_accelerationX { get; internal set; }
            public float trailer_av_accelerationY { get; internal set; }
            public float trailer_av_accelerationZ { get; internal set; }

            public float trailer_la_accelerationX { get; internal set; }
            public float trailer_la_accelerationY { get; internal set; }
            public float trailer_la_accelerationZ { get; internal set; }

            public float trailer_aa_accelerationX { get; internal set; }
            public float trailer_aa_accelerationY { get; internal set; }
            public float trailer_aa_accelerationZ { get; internal set; }

            public float[] trailer_wheelSuspDeflection { get; internal set; }

            public bool[] trailer_wheelOnGround { get; internal set; }
            public int[] trailer_wheelSubstance { get; internal set; }
            public float[] trailer_wheelVelocity { get; internal set; }
            public float[] trailer_wheelSteering { get; internal set; }
            public float[] trailer_wheelRotation { get; internal set; }

            public float lv_accelerationX { get; internal set; }
            public float lv_accelerationY { get; internal set; }
            public float lv_accelerationZ { get; internal set; }

            public float av_accelerationX { get; internal set; }
            public float av_accelerationY { get; internal set; }
            public float av_accelerationZ { get; internal set; }

            public float aa_accelerationX { get; internal set; }
            public float aa_accelerationY { get; internal set; }
            public float aa_accelerationZ { get; internal set; }

            public float cabinOffsetX { get; internal set; }
            public float cabinOffsetY { get; internal set; }
            public float cabinOffsetZ { get; internal set; }

            public float cabinOffsetRotationX { get; internal set; }
            public float cabinOffsetRotationY { get; internal set; }
            public float cabinOffsetRotationZ { get; internal set; }

            public float cabinAVX { get; internal set; }
            public float cabinAVY { get; internal set; }
            public float cabinAVZ { get; internal set; }
            public float cabinAAX { get; internal set; }
            public float cabinAAY { get; internal set; }
            public float cabinAAZ { get; internal set; }


            public float HeadOffsetX { get; internal set; }
            public float HeadOffsetY { get; internal set; }
            public float HeadOffsetZ { get; internal set; }

            public float HeadOffsetRotationX { get; internal set; }
            public float HeadOffsetRotationY { get; internal set; }
            public float HeadOffsetRotationZ { get; internal set; }

            public float[] TruckWheelSuspDeflection { get; internal set; }

            public bool[] TruckWheelOnGround { get; internal set; }
            public int[] TruckWheelSubstance { get; internal set; }
            public float[] TruckWheelVelocity { get; internal set; }
            public float[] TruckWheelSteering { get; internal set; }
            public float[] TruckWheelRotation { get; internal set; }
            public float[] TruckWheelLift { get; internal set; }
            public float[] TruckWheelLiftOffset { get; internal set; }


        }

        public class _Damage
        {
            public float WearEnigne { get; internal set; }
            public float WearTransmission { get; internal set; }
            public float WearCabin { get; internal set; }
            public float WearChassis { get; internal set; }
            public float WearWheels { get; internal set; }
            public float WearTrailer { get; internal set; }
        }

        public class _Lights
        {
            public bool BlinkerLeftActive { get; internal set; }
            public bool BlinkerRightActive { get; internal set; }
            public bool BlinkerLeftOn { get; internal set; }
            public bool BlinkerRightOn { get; internal set; }
            public bool ParkingLights { get; internal set; }
            public bool LowBeams { get; internal set; }
            public bool HighBeams { get; internal set; }
            public int FrontAux { get; internal set; }
            public int RoofAux { get; internal set; }
            public bool BrakeLight { get; internal set; }
            public bool ReverseLight { get; internal set; }
            public float LightsDashboard { get; internal set; }
            public bool Beacon { get; internal set; }
        }

        public class _Version
        {
            public uint SdkPlugin { get; internal set; }
            public uint GameMajor { get; internal set; }
            public uint GameMinor { get; internal set; }
            public uint Game { get; internal set; }
            public uint GameTelemetryMajor { get; internal set; }
            public uint GameTelemetryMinor { get; internal set; }
        }

        public _Version Version { get; protected set; }
        public _Controls Controls { get; protected set; }
        public _Physics Physics { get; protected set; }

        public _Drivetrain Drivetrain { get; protected set; }
        public _Job Job { get; protected set; }

        public _Axilliary Axilliary { get; protected set; }
        public _Damage Damage { get; protected set; }
        public _Lights Lights { get; protected set; }

        internal static DateTime SecondsToDate(int seconds)
        {
            if (seconds < 0) {
                seconds = 0;
            }
            return new DateTime((long)seconds * 10000000, DateTimeKind.Utc);
        }
        public SCSTelemetry(Ets2SdkData raw, Ets2SdkUnmanaged rawUnmanaged)
        {
            Time = raw.time;
            Paused = (raw.paused > 0);
            AbsolutTime = raw.timeAbsolute; 


            TruckId = rawUnmanaged.TruckModel;
            Truck = Encoding.UTF8.GetString(raw.truckModel).Replace('\0', ' ').Trim();
            Manufacturer = Encoding.UTF8.GetString(raw.truckMake).Replace('\0', ' ').Trim();
            ManufacturerId= Encoding.UTF8.GetString(raw.truckMakeId).Replace('\0', ' ').Trim();

            Version = new _Version();
            Controls = new _Controls();
            Physics = new _Physics();
            Drivetrain = new _Drivetrain();
            Job = new _Job();
            Axilliary = new _Axilliary();
            Damage = new _Damage();
            Lights = new _Lights();

            // Version
            Version.GameMajor = raw.ets2_version_major;
            Version.GameMinor = raw.ets2_version_minor;
            Version.SdkPlugin = raw.ets2_telemetry_plugin_revision;
            
            // Controls
            Controls.GameBrake = raw.gameBrake;
            Controls.GameClutch = raw.gameClutch;
            Controls.GameSteer = raw.gameSteer;
            Controls.GameThrottle = raw.gameThrottle;

            Controls.UserBrake = raw.userBrake;
            Controls.UserClutch = raw.userClutch;
            Controls.UserSteer = raw.userSteer;
            Controls.UserThrottle = raw.userThrottle;

            // Physics
            Physics.AccelerationX = raw.accelerationX;
            Physics.AccelerationY = raw.accelerationY;
            Physics.AccelerationZ = raw.accelerationZ;

            Physics.CoordinateX = raw.coordinateX;
            Physics.CoordinateY = raw.coordinateY;
            Physics.CoordinateZ = raw.coordinateZ;

            Physics.RotationX = raw.rotationX;
            Physics.RotationY = raw.rotationY;
            Physics.RotationZ = raw.rotationZ;

            Physics.Speed = raw.speed;
            Physics.SpeedKmh = raw.speed*3.6f;
            Physics.SpeedMph = Physics.SpeedKmh/1.6f;

            // Drivetrain
            Drivetrain.Adblue = raw.adblue;
            Drivetrain.AdblueConsumption = raw.adblueConsumption;
            Drivetrain.AirPressure = raw.airPressure;
            Drivetrain.BrakeTemperature = raw.brakeTemperature;
            Drivetrain.CruiseControl = raw.GetBool(Ets2SdkBoolean.CruiseControl);
            Drivetrain.CruiseControlSpeed = raw.cruiseControlSpeed;
            Drivetrain.CruiseControlSpeedKmh = raw.cruiseControlSpeed * 3.6f;
            Drivetrain.CruiseControlSpeedMph = Drivetrain.CruiseControlSpeedKmh/ 1.6f;
            Drivetrain.ElectricEnabled = raw.GetBool(Ets2SdkBoolean.ElectricEnabled);
            Drivetrain.EngineEnabled = raw.GetBool(Ets2SdkBoolean.EngineEnabled);
            Drivetrain.EngineRpm = raw.engineRpm;
            Drivetrain.EngineRpmMax = raw.engineRpmMax;

            Drivetrain.Speed = raw.speed;
            Drivetrain.SpeedKmh = raw.speed * 3.6f;
            Drivetrain.SpeedMph = Physics.SpeedKmh / 1.6f;

            Drivetrain.Fuel = raw.fuel;
            Drivetrain.FuelRange = raw.fuelRange;
            Drivetrain.FuelAvgConsumption = raw.fuelAvgConsumption;
            Drivetrain.FuelMax = raw.fuelCapacity;
            Drivetrain.FuelRate = raw.fuelRate;
            Drivetrain.FuelWarningLight = raw.fuelWarning != 0; 

            Drivetrain.Gear = raw.gear;
            Drivetrain.GearRange = raw.gearRangeActive;
            Drivetrain.GearRanges = raw.gearRanges;
            Drivetrain.GearsForward = raw.gears;
            Drivetrain.GearsReverse = raw.gearsReverse;

            // Note all gears
            var gearFw = raw.gearRatioForward;
            var gearRw = raw.gearRatioReverse;

            Array.Resize<float>(ref gearFw, Drivetrain.GearsForward);
            Array.Resize<float>(ref gearRw, Drivetrain.GearsReverse);

            Drivetrain.GearRatiosForward = gearFw;
            Drivetrain.GearRatiosReverse = gearRw;
            Drivetrain.GearRatioDifferential = raw.gearRatioDifferential;

            Drivetrain.GearDashboard =raw.gearDashboard;

            Drivetrain.MotorBrake = raw.GetBool(Ets2SdkBoolean.MotorBrake);
            Drivetrain.OilPressure = raw.oilPressure;
            Drivetrain.OilTemperature = raw.oilTemperature;
            Drivetrain.ParkingBrake = raw.GetBool(Ets2SdkBoolean.ParkBrake);
            Drivetrain.Retarder = raw.retarderBrake;
            Drivetrain.ShifterSlot = raw.shifterSlot;
            Drivetrain.ShifterToggle = raw.shifterToggle;
            Drivetrain.TruckOdometer = raw.truckOdometer;
            Drivetrain.WaterTemperature = raw.waterTemperature;

            // Job info:
            Job.CityDestination = Encoding.UTF8.GetString(raw.jobCityDestination).Replace('\0',' ').Trim();
            Job.CitySource = Encoding.UTF8.GetString(raw.jobCitySource).Replace('\0', ' ').Trim();
            Job.CompanyDestination = Encoding.UTF8.GetString(raw.jobCompanyDestination).Replace('\0', ' ').Trim();
            Job.CompanySource = Encoding.UTF8.GetString(raw.jobCompanySource).Replace('\0', ' ').Trim();
            Job.Deadline = raw.jobDeadline;
            Job.Income = raw.jobIncome;
            Job.Mass = raw.trailerMass;
            Job.TrailerAttached = raw.GetBool(Ets2SdkBoolean.TrailerAttached);//TODO
            Job.TrailerId = Encoding.UTF8.GetString(raw.trailerId).Replace('\0', ' ').Trim();
            Job.TrailerName = Encoding.UTF8.GetString(raw.trailerName).Replace('\0', ' ').Trim();
            Job.Cargo = Job.TrailerName; // trailerModel is actually deprecated

            Job.NavigationDistanceLeft = raw.routeDistance;
            Job.NavigationTimeLeft = raw.routeTime;
            Job.SpeedLimit = raw.speedLimit;

            Job.OnJob = raw.onJob != 0;
            Job.JobFinished = raw.jobFinished != 0;

            // Axilliary flags
            Axilliary.AdblueWarning = raw.GetBool(Ets2SdkBoolean.AdblueWarning);
            Axilliary.AirPressureEmergency = raw.GetBool(Ets2SdkBoolean.AirPressureEmergency);
            Axilliary.AirPressureWarning = raw.GetBool(Ets2SdkBoolean.AirPressureWarning);
            Axilliary.BatteryVoltageWarning = raw.GetBool(Ets2SdkBoolean.BatteryVoltageWarning);
            Axilliary.OilPressureWarning = raw.GetBool(Ets2SdkBoolean.OilPressureWarning);
            Axilliary.WaterTemperatureWarning = raw.GetBool(Ets2SdkBoolean.WaterTemperatureWarning);
            Axilliary.Wipers = raw.GetBool(Ets2SdkBoolean.Wipers);

            // Damage
            Damage.WearCabin = raw.wearCabin;
            Damage.WearChassis = raw.wearChassis;
            Damage.WearEnigne = raw.wearEngine;
            Damage.WearTrailer = raw.wearTrailer;
            Damage.WearTransmission = raw.wearTransmission;
            Damage.WearWheels = raw.wearWheels;

            // Lights
            Lights.Beacon = raw.GetBool(Ets2SdkBoolean.LightsBeacon);
            Lights.BlinkerLeftActive = raw.GetBool(Ets2SdkBoolean.BlinkerLeftActive);// TODO
            Lights.BlinkerLeftOn = raw.GetBool(Ets2SdkBoolean.BlinkerLeftOn);// TODO
            Lights.BlinkerRightActive = raw.GetBool(Ets2SdkBoolean.BlinkerRightActive);// TODO
            Lights.BlinkerRightOn = raw.GetBool(Ets2SdkBoolean.BlinkerRightOn);// TODO
            Lights.BrakeLight = raw.GetBool(Ets2SdkBoolean.LightsBrake);// TODO
          //  Lights.FrontAux = raw.GetBool(Ets2SdkBoolean.LightsAuxFront);// TODO
            Lights.HighBeams = raw.GetBool(Ets2SdkBoolean.LightsBeamHigh);// TODO
            Lights.LightsDashboard = raw.lightsDashboard;
            Lights.LowBeams = raw.GetBool(Ets2SdkBoolean.LightsBeamLow);// TODO
            Lights.ParkingLights = raw.GetBool(Ets2SdkBoolean.LightsParking);// TODO
            Lights.ReverseLight = raw.GetBool(Ets2SdkBoolean.LightsReverse);// TODO
            //Lights.RoofAux = raw.GetBool(Ets2SdkBoolean.LightsAuxRoof);// TODO
        }

        public SCSTelemetry()
        {
            Version = new _Version();
            Physics = new _Physics();
            Drivetrain = new _Drivetrain();
            Job = new _Job();
            Controls = new _Controls();
            Axilliary = new _Axilliary();
            Lights = new _Lights();
            Damage = new _Damage();
        }
    }

    public class Ets2SdkUnmanaged
    {
        public string TruckModel { get; set; }
        public string TrailerModel { get; set; }
    }
}