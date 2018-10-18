// 
// Ets2SdkClient
// Ets2SdkDataAlt.cs
// 22:51
using System;
using System.Net.Http.Headers;
using System.Text;
using System.Windows.Forms;

namespace Ets2SdkClient {
    public class Ets2SdkDataAlt {
        private byte[] data;
        public Ets2Telemetry Convert(byte[] structureDataBytes) {
            this.data = structureDataBytes;
            var retData = new Ets2Telemetry();
            retData.Time = GetUint(0);
            retData.Paused = GetUint(4)>0;

            retData.Version.SdkPlugin = GetUint(8);
            retData.Version.GameMajor = GetUint(12);
            retData.Version.GameMinor = GetUint(16);
            retData.Version.Game = GetUint(20);
            retData.Version.GameTelemetryMajor = GetUint(24);
            retData.Version.GameTelemetryMinor = GetUint(28);
            
            // Original a byte array with 4 cells(20-23). check function check if flag[1]>0 same we do here, may check later what else we can do here
            retData.Job.TrailerAttached = data[21]>0;

            var speed = GetFloat(24);
            retData.Physics.Speed = retData.Drivetrain.Speed = speed;
            retData.Physics.SpeedKmh = retData.Drivetrain.SpeedKmh = speed * 3.6f;
            retData.Physics.SpeedMph = retData.Drivetrain.SpeedMph = retData.Physics.SpeedKmh / 1.6f;

            retData.Physics.AccelerationX = GetFloat(28);
            retData.Physics.AccelerationY = GetFloat(32);
            retData.Physics.AccelerationZ = GetFloat(36);

            retData.Physics.CoordinateX = GetFloat(40);
            retData.Physics.CoordinateY = GetFloat(44);
            retData.Physics.CoordinateZ = GetFloat(48);

            retData.Physics.RotationX = GetFloat(52);
            retData.Physics.RotationY = GetFloat(56);
            retData.Physics.RotationZ = GetFloat(60);

            retData.Drivetrain.GearRange = GetInt(64);
            retData.Drivetrain.GearsForward = GetInt(68);
            retData.Drivetrain.GearRanges = GetInt(72);
            retData.Drivetrain.GearRange = GetInt(76);

            retData.Drivetrain.EngineRpm = GetFloat(80);
            retData.Drivetrain.EngineRpmMax = GetFloat(84);

            retData.Drivetrain.Fuel = GetFloat(88);
            retData.Drivetrain.FuelMax = GetFloat(92);
            retData.Drivetrain.FuelRate = GetFloat(96);
            retData.Drivetrain.FuelAvgConsumption = GetFloat(100);

            retData.Controls.UserSteer = GetFloat(104);
            retData.Controls.UserThrottle = GetFloat(108);
            retData.Controls.UserBrake = GetFloat(112);
            retData.Controls.UserClutch = GetFloat(116);

            retData.Controls.GameSteer = GetFloat(120);
            retData.Controls.GameThrottle = GetFloat(124);
            retData.Controls.GameBrake = GetFloat(128);
            retData.Controls.GameClutch = GetFloat(132);
            
            retData.Drivetrain.Weight = GetFloat(136);
            retData.Job.TrailerWeight = GetFloat(140);

            var modelOffset = GetInt(144);
            var modelLength = GetInt(148);
            var trailerOffset = GetInt(152);
            var trailerLength = GetInt(156);


            retData.AbsolutTime = GetInt(160);
            retData.Drivetrain.GearsReverse = GetInt(164);

            retData.Job.Mass = GetFloat(168);
            retData.Job.TrailerId = Encoding.UTF8.GetString(GetSubArray(172,64)).Replace('\0', ' ').Trim();
            retData.Job.TrailerName = Encoding.UTF8.GetString(GetSubArray(236,64)).Replace('\0', ' ').Trim();
            retData.Job.Cargo = retData.Job.TrailerName; // Deprecated

            retData.Job.Income = GetInt(300);
            retData.Job.Deadline = GetInt(304);

            retData.Job.CitySource = Encoding.UTF8.GetString(GetSubArray(308,64)).Replace('\0', ' ').Trim();
            retData.Job.CityDestination = Encoding.UTF8.GetString(GetSubArray(372,64)).Replace('\0', ' ').Trim();

            retData.Job.CompanySource = Encoding.UTF8.GetString(GetSubArray(436,64)).Replace('\0', ' ').Trim();
            retData.Job.CompanyDestination = Encoding.UTF8.GetString(GetSubArray(500,64)).Replace('\0', ' ').Trim();

            retData.Drivetrain.Retarder = GetInt(564);
            retData.Drivetrain.ShifterSlot = GetInt(568);
            retData.Drivetrain.ShifterToggle = GetInt(572);
            // 576 Fill ? 

            retData.Drivetrain.CruiseControl = data[580]==1;
            retData.Axilliary.Wipers = data[581] == 1;
            retData.Drivetrain.ParkingBrake = data[582] == 1;
            retData.Drivetrain.MotorBrake = data[583] == 1;
            retData.Drivetrain.ElectricEnabled = data[584] == 1;
            retData.Drivetrain.EngineEnabled  = data[585] == 1;
            retData.Lights.BlinkerLeftActive  = data[586] == 1;
            retData.Lights.BlinkerRightActive  = data[587] == 1;
            retData.Lights.BlinkerLeftOn  = data[588] == 1;
            retData.Lights.BlinkerRightOn  = data[589] == 1;
            retData.Lights.ParkingLights  = data[590] == 1;
            retData.Lights.LowBeams  = data[591] == 1;
            retData.Lights.HighBeams  = data[592] == 1;
            retData.Lights.FrontAux = GetInt(596);
            retData.Lights.RoofAux  = GetInt(600);
            retData.Lights.Beacon  = data[604] == 1;
            retData.Lights.BrakeLight  = data[605] == 1;
            retData.Lights.ReverseLight  = data[606] == 1;
            retData.Axilliary.BatteryVoltageWarning  = data[607] == 1;
            retData.Axilliary.AirPressureWarning  = data[608] == 1;
            retData.Axilliary.AirPressureEmergency  = data[609] == 1;
            retData.Axilliary.AdblueWarning  = data[610] == 1;
            retData.Axilliary.OilPressureWarning  = data[611] == 1;
            retData.Axilliary.WaterTemperatureWarning  = data[612] == 1;

            retData.Drivetrain.AirPressure = GetFloat(616);
            retData.Drivetrain.BrakeTemperature = GetFloat(620);
            retData.Drivetrain.FuelWarning = GetInt(624);
            retData.Drivetrain.Adblue = GetFloat(628);
            retData.Drivetrain.AdblueConsumption = GetFloat(632); // Seems not to exists but exists in header
            retData.Drivetrain.OilPressure = GetFloat(636);
            retData.Drivetrain.OilTemperature = GetFloat(640);
            retData.Drivetrain.WaterTemperature = GetFloat(644);
            retData.Drivetrain.BatteryVoltage = GetFloat(648);
            retData.Lights.LightsDashboard = GetFloat(652);
            retData.Damage.WearEnigne = GetFloat(656);
            retData.Damage.WearTransmission = GetFloat(660);
            retData.Damage.WearCabin = GetFloat(664);
            retData.Damage.WearChassis = GetFloat(668);
            retData.Damage.WearWheels = GetFloat(672);
            retData.Damage.WearTrailer = GetFloat(676);
            retData.Drivetrain.TruckOdometer = GetFloat(680);
            retData.Drivetrain.CruiseControlSpeed = GetFloat(684);

            retData.Manufacturer = Encoding.UTF8.GetString(GetSubArray(688,64)).Replace('\0', ' ').Trim();
            retData .ManufacturerId  = Encoding.UTF8.GetString(GetSubArray(752,64)).Replace('\0', ' ').Trim();
            retData.TruckId = Encoding.UTF8.GetString(GetSubArray(modelOffset,modelLength));
            retData.Job.TrailerModel =  Encoding.UTF8.GetString(GetSubArray(trailerOffset,trailerLength));
            retData.Truck  = Encoding.UTF8.GetString(GetSubArray(816,64)).Replace('\0', ' ').Trim();

            retData.Job.SpeedLimit = GetFloat(880);
            retData.Job.NavigationDistanceLeft = GetFloat(884);
            retData.Job.NavigationTimeLeft = GetFloat(888);
            retData.Drivetrain.FuelRate = GetFloat(892);

            retData.Drivetrain.GearRatiosForward = new float[24];
            for (var i = 0; i < 24; i++) {
                retData.Drivetrain.GearRatiosForward[i] = GetFloat(896 + (i * 4));
            }

            retData.Drivetrain.GearRatiosReverse = new float[8];
            for (var i = 0; i < 8; i++) {
                retData.Drivetrain.GearRatiosReverse[i] = GetFloat(992 + (i * 4));
            }

            retData.Drivetrain.GearRatioDifferential = GetFloat(1024);
            retData.Drivetrain.GearDashboard = GetInt(1028);

            retData.Job.OnJob = data[1032] == 1;
            retData.Job.JobFinished = data[1033] == 1;

            return retData;
        }

        //  have to check but it seems that it is 0001 = 1 not 1000 = 1
        private uint GetUint(int index) {
            return (uint)((data[index+3] << 24) | (data[index+2] << 16) | (data[index+1] << 8) | (data[index]));
        } 
        private float GetFloat(int index) {
            var temp = new[]{data[index ], data[index + 1], data[index + 2], data[index + 3]};
            return BitConverter.ToSingle(temp,0);
        } 
        //TODO: Check if this is also correct
        private int GetInt(int index) {
            return (data[index+3] << 24) | (data[index+2] << 16) | (data[index+1] << 8) | (data[index]);
        }

        private byte[] GetSubArray(int startIndex, int length) {
            var ret = new byte[length];
            for (var i = 0; i < length; i++) {
                ret[i] = data[startIndex + i];
            }
            return ret;
        }
    }
}