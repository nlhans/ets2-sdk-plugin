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
            retData.Version.Ets2Major = GetUint(12);
            retData.Version.Ets2Minor = GetUint(16);
            
            // Original a byte array with 4 cells(20-23). check function check if flag[1]>0 same we do here, may check later what else we can do here
            retData.Job.TrailerAttached = GetUint(21)>0;

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
            retData.Lights.FrontAux  = data[593] == 1;
            retData.Lights.RoofAux  = data[594] == 1;
            retData.Lights.Beacon  = data[595] == 1;
            retData.Lights.BrakeLight  = data[596] == 1;
            retData.Lights.ReverseLight  = data[597] == 1;
            retData.Axilliary.BatteryVoltageWarning  = data[598] == 1;
            retData.Axilliary.AirPressureWarning  = data[599] == 1;
            retData.Axilliary.AirPressureEmergency  = data[600] == 1;
            retData.Axilliary.AdblueWarning  = data[601] == 1;
            retData.Axilliary.OilPressureWarning  = data[602] == 1;
            retData.Axilliary.WaterTemperatureWarning  = data[603] == 1;

            retData.Drivetrain.AirPressure = GetFloat(604);
            retData.Drivetrain.BrakeTemperature = GetFloat(608);
            retData.Drivetrain.FuelWarning = GetInt(612);
            retData.Drivetrain.Adblue = GetFloat(616);
            retData.Drivetrain.AdblueConsumption = GetFloat(620);
            retData.Drivetrain.OilPressure = GetFloat(624);
            retData.Drivetrain.OilTemperature = GetFloat(628);
            retData.Drivetrain.WaterTemperature = GetFloat(632);
            retData.Drivetrain.BatteryVoltage = GetFloat(636);
            retData.Lights.LightsDashboard = GetFloat(640);
            retData.Damage.WearEnigne = GetFloat(644);
            retData.Damage.WearTransmission = GetFloat(648);
            retData.Damage.WearCabin = GetFloat(652);
            retData.Damage.WearChassis = GetFloat(656);
            retData.Damage.WearWheels = GetFloat(660);
            retData.Damage.WearTrailer = GetFloat(664);
            retData.Drivetrain.TruckOdometer = GetFloat(668);
            retData.Drivetrain.CruiseControlSpeed = GetFloat(672);

            retData.Manufacturer = Encoding.UTF8.GetString(GetSubArray(676,64)).Replace('\0', ' ').Trim();
            retData .ManufacturerId  = Encoding.UTF8.GetString(GetSubArray(740,64)).Replace('\0', ' ').Trim();
            retData.TruckId = Encoding.UTF8.GetString(GetSubArray(modelOffset,modelLength));
            retData.Job.TrailerModel =  Encoding.UTF8.GetString(GetSubArray(trailerOffset,trailerLength));
            retData.Truck  = Encoding.UTF8.GetString(GetSubArray(804,64)).Replace('\0', ' ').Trim();

            retData.Job.SpeedLimit = GetFloat(868);
            retData.Job.NavigationDistanceLeft = GetFloat(872);
            retData.Job.NavigationTimeLeft = GetFloat(876);
            retData.Drivetrain.FuelRate = GetFloat(880);

            retData.Drivetrain.GearRatiosForward = new float[24];
            for (var i = 0; i < 24; i++) {
                retData.Drivetrain.GearRatiosForward[i] = GetFloat(884 + (i * 4));
            }

            retData.Drivetrain.GearRatiosReverse = new float[8];
            for (var i = 0; i < 8; i++) {
                retData.Drivetrain.GearRatiosReverse[i] = GetFloat(980 + (i * 4));
            }

            retData.Drivetrain.GearRatioDifferential = GetFloat(1012);
            retData.Drivetrain.GearDashboard = GetInt(1016);

            retData.Job.OnJob = data[1020] == 1;
            retData.Job.JobFinished = data[1021] == 1;

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
            return -(data[index+3] << 24) | (data[index+2] << 16) | (data[index+1] << 8) | (data[index]);
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