using System.Runtime.InteropServices;

namespace Ets2SdkClient
{
    [StructLayout(LayoutKind.Explicit)]
    public struct Ets2SdkData
    {
        [FieldOffset(0)]
        public uint time;
        [FieldOffset(4)]
        public uint paused;

        [FieldOffset(8)]
        public uint ets2_telemetry_plugin_revision;
        [FieldOffset(12)]
        public uint ets2_version_major;
        [FieldOffset(16)]
        public uint ets2_version_minor;

        // ***** REVISION 1 ****** //

        [FieldOffset(20)]
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
        public byte[] flags;

        // vehicle dynamics

        [FieldOffset(24)]
        public float speed;
        [FieldOffset(28)]
        public float accelerationX;
        [FieldOffset(32)]
        public float accelerationY;
        [FieldOffset(36)]
        public float accelerationZ;


        [FieldOffset(40)]
        public float coordinateX;
        [FieldOffset(44)]
        public float coordinateY;
        [FieldOffset(48)]
        public float coordinateZ;


        [FieldOffset(52)]
        public float rotationX;
        [FieldOffset(56)]
        public float rotationY;
        [FieldOffset(60)]
        public float rotationZ;

        // drivetrain essentials

        [FieldOffset(64)]
        public int gear;
        [FieldOffset(68)]
        public int gears;
        [FieldOffset(72)]
        public int gearRanges;
        [FieldOffset(76)]
        public int gearRangeActive;

        [FieldOffset(80)]
        public float engineRpm;
        [FieldOffset(84)]
        public float engineRpmMax;

        [FieldOffset(88)]
        public float fuel;
        [FieldOffset(92)]
        public float fuelCapacity;
        [FieldOffset(96)]
        public float fuelRate;
        [FieldOffset(100)]
        public float fuelAvgConsumption;

        // user input

        [FieldOffset(104)]
        public float userSteer;
        [FieldOffset(108)]
        public float userThrottle;
        [FieldOffset(112)]
        public float userBrake;
        [FieldOffset(116)]
        public float userClutch;


        [FieldOffset(120)]
        public float gameSteer;
        [FieldOffset(124)]
        public float gameThrottle;
        [FieldOffset(128)]
        public float gameBrake;
        [FieldOffset(132)]
        public float gameClutch;

        // truck & trailer

        [FieldOffset(136)]
        public float truckWeight;
        [FieldOffset(140)]
        public float trailerWeight;

        [FieldOffset(144)]
        public int modelOffset;
        [FieldOffset(148)]
        public int modelLength;

        [FieldOffset(152)]
        public int trailerOffset;
        [FieldOffset(156)]
        public int trailerLength;


        // ***** REVISION 2 ****** //
        [FieldOffset(160)]
        public int timeAbsolute;
        [FieldOffset(164)]
        public int gearsReverse;

        [FieldOffset(168)]
        public float trailerMass;
        [FieldOffset(172)]
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 64)]
        public byte[] trailerId;
        [FieldOffset(236)]
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 64)]
        public byte[] trailerName;

        [FieldOffset(300)]
        public int jobIncome;
        [FieldOffset(304)]
        public int jobDeadline;

        [FieldOffset(308)]
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 64)]
        public byte[] jobCitySource;
        [FieldOffset(372)]
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 64)]
        public byte[] jobCityDestination;

        [FieldOffset(436)]
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 64)]
        public byte[] jobCompanySource;
        [FieldOffset(500)]
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 64)]
        public byte[] jobCompanyDestination;

        // ***** REVISION 3 ****** //
        [FieldOffset(564)]
        public int retarderBrake;
        [FieldOffset(568)]
        public int shifterSlot;
        [FieldOffset(572)]
        public int shifterToggle;
        //[FieldOffset(576)]
        //public int fill;

        [FieldOffset(580)]
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 24)]
        public byte[] aux;
        [FieldOffset(604)]
        public float airPressure;
        [FieldOffset(608)]
        public float brakeTemperature;
        [FieldOffset(612)]
        public int fuelWarning;
        [FieldOffset(616)]
        public float adblue;
        [FieldOffset(620)]
        public float adblueConsumption;
        [FieldOffset(624)]
        public float oilPressure;
        [FieldOffset(628)]
        public float oilTemperature;
        [FieldOffset(632)]
        public float waterTemperature;
        [FieldOffset(636)]
        public float batteryVoltage;
        [FieldOffset(640)]
        public float lightsDashboard;
        [FieldOffset(644)]
        public float wearEngine;
        [FieldOffset(648)]
        public float wearTransmission;
        [FieldOffset(652)]
        public float wearCabin;
        [FieldOffset(656)]
        public float wearChassis;
        [FieldOffset(660)]
        public float wearWheels;
        [FieldOffset(664)]
        public float wearTrailer;
        [FieldOffset(668)]
        public float truckOdometer;
        [FieldOffset(672)]
        public float cruiseControlSpeed;

        [FieldOffset(676)]
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 64)]
        public byte[] truckMake;
        [FieldOffset(740)]
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 64)]
        public byte[] truckMakeId;
        [FieldOffset(804)]
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 64)]
        public byte[] truckModel;

        // ***** REVISION 4 ****** //
        [FieldOffset(868)]
        public float speedLimit;

        [FieldOffset(872)]
        public float routeDistance;

        [FieldOffset(876)]
        public float routeTime;

        [FieldOffset(880)]
        public float fuelRange;

        [FieldOffset(884)]
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 24)]
        public float[] gearRatioForward;

        [FieldOffset(980)]
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
        public float[] gearRatioReverse;

        [FieldOffset(1012)]
        public float gearRatioDifferential;

        [FieldOffset(1016)]
        public int gearDashboard;

        [FieldOffset(1020)] public byte onJob;
        [FieldOffset(1021)] public byte jobFinished;

        public bool GetBool(Ets2SdkBoolean i)
        {
            if (i == Ets2SdkBoolean.TrailerAttached)
                return flags[1] > 0;
            return aux[(int)i] > 0;
        }
    }
}