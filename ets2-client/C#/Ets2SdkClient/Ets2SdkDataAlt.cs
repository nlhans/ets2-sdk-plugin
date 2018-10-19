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
        private byte[] _data;
        private int _offset;
        public SCSTelemetry Convert(byte[] structureDataBytes) {
            _offset = 0;
            this._data = structureDataBytes;
            var retData = new SCSTelemetry();
            retData.Time = GetUint();
            retData.Paused = GetUint()>0;

            retData.Version.SdkPlugin = GetUint();
            retData.Version.GameMajor = GetUint();
            retData.Version.GameMinor = GetUint();
            retData.Version.Game = GetUint();
            retData.Version.GameTelemetryMajor = GetUint();
            retData.Version.GameTelemetryMinor = GetUint();


            retData.Scale=  GetFloat();
            retData.AbsolutTime = GetInt();
            retData.RestStop =  GetInt();
            retData.Job.TrailerAttached = GetByte()>0;

            var speed = GetFloat();
            retData.Physics.Speed = retData.Drivetrain.Speed = speed;
            retData.Physics.SpeedKmh = retData.Drivetrain.SpeedKmh = speed * 3.6f;
            retData.Physics.SpeedMph = retData.Drivetrain.SpeedMph = retData.Physics.SpeedKmh / 1.6f;

            retData.Physics.AccelerationX = GetFloat();
            retData.Physics.AccelerationY = GetFloat();
            retData.Physics.AccelerationZ = GetFloat();

            retData.Physics.CoordinateX = GetFloat();
            retData.Physics.CoordinateY = GetFloat();
            retData.Physics.CoordinateZ = GetFloat();

            retData.Physics.RotationX = GetFloat();
            retData.Physics.RotationY = GetFloat();
            retData.Physics.RotationZ = GetFloat();

            retData.Drivetrain.Gear = GetInt();
            retData.Drivetrain.GearsForward = GetInt();
            retData.Drivetrain.GearRanges = GetInt();
            retData.Drivetrain.GearRange = GetInt();

            retData.Drivetrain.EngineRpm = GetFloat();
            retData.Drivetrain.EngineRpmMax = GetFloat();

            retData.Drivetrain.Fuel = GetFloat();
            retData.Drivetrain.FuelMax = GetFloat();
            retData.Drivetrain.FuelRate = GetFloat(); // Not working
            retData.Drivetrain.FuelAvgConsumption = GetFloat();

            retData.Controls.UserSteer = GetFloat();
            retData.Controls.UserThrottle = GetFloat();
            retData.Controls.UserBrake = GetFloat();
            retData.Controls.UserClutch = GetFloat();

            retData.Controls.GameSteer = GetFloat();
            retData.Controls.GameThrottle = GetFloat();
            retData.Controls.GameBrake = GetFloat();
            retData.Controls.GameClutch = GetFloat();
            
            retData.Drivetrain.Weight = GetFloat();
            retData.Job.TrailerWeight = GetFloat();

            var modelOffset = GetInt();
            var modelLength = GetInt();
            var trailerOffset = GetInt();
            var trailerLength = GetInt();


            //retData.AbsolutTime = GetInt();
            retData.Drivetrain.GearsReverse = GetInt();

            retData.Job.Mass = GetFloat();
            retData.Job.TrailerId = Encoding.UTF8.GetString(GetSubArray(64)).Replace('\0', ' ').Trim();
            retData.Job.TrailerName = Encoding.UTF8.GetString(GetSubArray(64)).Replace('\0', ' ').Trim();
            retData.Job.Cargo = retData.Job.TrailerName; // Deprecated

            retData.Job.Income = GetInt();
            retData.Job.Deadline = GetInt();

            retData.Job.CitySource = Encoding.UTF8.GetString(GetSubArray(64)).Replace('\0', ' ').Trim();
            retData.Job.CityDestination = Encoding.UTF8.GetString(GetSubArray(64)).Replace('\0', ' ').Trim();

            retData.Job.CompanySource = Encoding.UTF8.GetString(GetSubArray(64)).Replace('\0', ' ').Trim();
            retData.Job.CompanyDestination = Encoding.UTF8.GetString(GetSubArray(64)).Replace('\0', ' ').Trim();

            retData.Drivetrain.Retarder = GetInt();
            retData.Drivetrain.ShifterSlot = GetInt();
            retData.Drivetrain.ShifterToggle = GetInt();

            // 576 Fill ? 
            GetInt(); // jump because off missing value at the moment

            retData.Drivetrain.CruiseControl = GetByte() == 1;
            retData.Axilliary.Wipers = GetByte() == 1;
            retData.Drivetrain.ParkingBrake = GetByte() == 1;
            retData.Drivetrain.MotorBrake = GetByte() == 1;
            retData.Drivetrain.ElectricEnabled = GetByte() == 1;
            retData.Drivetrain.EngineEnabled  = GetByte() == 1;
            retData.Lights.BlinkerLeftActive  = GetByte() == 1;
            retData.Lights.BlinkerRightActive  = GetByte() == 1;
            retData.Lights.BlinkerLeftOn  = GetByte() == 1;
            retData.Lights.BlinkerRightOn  = GetByte() == 1;
            retData.Lights.ParkingLights  = GetByte() == 1;
            retData.Lights.LowBeams  = GetByte() == 1;
            retData.Lights.HighBeams  = GetByte() == 1;
            retData.Lights.FrontAux = GetInt();
            retData.Lights.RoofAux  = GetInt();
            retData.Lights.Beacon  = GetByte() == 1;
            retData.Lights.BrakeLight  = GetByte() == 1;
            retData.Lights.ReverseLight  = GetByte() == 1;
            retData.Axilliary.BatteryVoltageWarning  = GetByte() == 1;
            retData.Axilliary.AirPressureWarning  = GetByte() == 1;
            retData.Axilliary.AirPressureEmergency  = GetByte() == 1;
            retData.Axilliary.AdblueWarning  = GetByte() == 1;
            retData.Axilliary.OilPressureWarning  = GetByte() == 1;
            retData.Axilliary.WaterTemperatureWarning  = GetByte() == 1;

            retData.Drivetrain.AirPressure = GetFloat();
            retData.Drivetrain.BrakeTemperature = GetFloat();
            retData.Drivetrain.FuelWarning = GetInt();
            retData.Drivetrain.Adblue = GetFloat();
            retData.Drivetrain.AdblueConsumption = GetFloat(); // Seems not to exists but exists in header
            retData.Drivetrain.OilPressure = GetFloat();
            retData.Drivetrain.OilTemperature = GetFloat();
            retData.Drivetrain.WaterTemperature = GetFloat();
            retData.Drivetrain.BatteryVoltage = GetFloat();
            retData.Lights.LightsDashboard = GetFloat();
            retData.Damage.WearEnigne = GetFloat();
            retData.Damage.WearTransmission = GetFloat();
            retData.Damage.WearCabin = GetFloat();
            retData.Damage.WearChassis = GetFloat();
            retData.Damage.WearWheels = GetFloat();
            retData.Damage.WearTrailer = GetFloat();
            retData.Drivetrain.TruckOdometer = GetFloat();
            retData.Drivetrain.CruiseControlSpeed = GetFloat();

            retData.Manufacturer = Encoding.UTF8.GetString(GetSubArray(64)).Replace('\0', ' ').Trim();
            retData .ManufacturerId  = Encoding.UTF8.GetString(GetSubArray(64)).Replace('\0', ' ').Trim();
            
            retData.Truck  = Encoding.UTF8.GetString(GetSubArray(64)).Replace('\0', ' ').Trim();
            retData.FuelWarningFactor = GetFloat();
            retData.Job.SpeedLimit = GetFloat();
            retData.Job.NavigationDistanceLeft = GetFloat();
            retData.Job.NavigationTimeLeft = GetFloat();
            retData.Drivetrain.FuelRange = GetFloat();

            retData.Drivetrain.GearRatiosForward = new float[24];
            for (var i = 0; i < 24; i++) {
                retData.Drivetrain.GearRatiosForward[i] = GetFloat();
            }

            retData.Drivetrain.GearRatiosReverse = new float[8];
            for (var i = 0; i < 8; i++) {
                retData.Drivetrain.GearRatiosReverse[i] = GetFloat();
            }

            retData.Drivetrain.GearRatioDifferential = GetFloat();
            retData.Drivetrain.GearDashboard = GetInt();

            retData.Job.OnJob = GetByte() == 1;
            retData.Job.JobFinished = GetByte() == 1;


            retData.TruckId = Encoding.UTF8.GetString(GetSubArray(modelOffset, modelLength)).Replace('\0', ' ').Trim();
            retData.Job.TrailerModel = Encoding.UTF8.GetString(GetSubArray(trailerOffset, trailerLength)).Replace('\0', ' ').Trim();

            retData.Drivetrain.AdblueCapacity = GetFloat();
            retData.Drivetrain.AdblueWarningFactor = GetFloat();
            retData.Drivetrain.AirPressureWarning = GetFloat();
            retData.Drivetrain.AirPressureEmergency = GetFloat();
            retData.Drivetrain.OilPressureWarning = GetFloat();
            retData.Drivetrain.WaterTemperatureWarning = GetFloat();
            retData.Drivetrain.BatteryVoltageWarning = GetFloat();

            retData.Drivetrain.RetarderStepCount = GetInt();

            retData.Drivetrain.CabinPositionX = GetFloat();
            retData.Drivetrain.CabinPositionY = GetFloat();
            retData.Drivetrain.CabinPositionZ = GetFloat();
            retData.Drivetrain.HeadPositionX = GetFloat();
            retData.Drivetrain.HeadPositionY = GetFloat();
            retData.Drivetrain.HeadPositionZ = GetFloat();
            retData.Drivetrain.HookPositionX = GetFloat();
            retData.Drivetrain.HookPositionY = GetFloat();
            retData.Drivetrain.HookPositionZ = GetFloat();
            retData.Drivetrain.WheelCount = GetInt();
            //TODO: 16 auslagern
            var wheelsize = 16;
            retData.Drivetrain.WheelPositionX = new float[wheelsize];
            for (var i = 0; i < wheelsize; i++)
            {
                retData.Drivetrain.WheelPositionX[i] = GetFloat();
            }
            retData.Drivetrain.WheelPositionY = new float[wheelsize];
            for (var i = 0; i < wheelsize; i++)
            {
                retData.Drivetrain.WheelPositionY[i] = GetFloat();
            }
            retData.Drivetrain.WheelPositionZ = new float[wheelsize];
            for (var i = 0; i < wheelsize; i++)
            {
                retData.Drivetrain.WheelPositionZ[i] = GetFloat();
            }

            retData.Drivetrain.WheelSteerable = new bool[wheelsize];
            for (var i = 0; i < wheelsize; i++)
            {
                retData.Drivetrain.WheelSteerable[i] = GetByte()>0;
            }
            retData.Drivetrain.WheelSimulated = new bool[wheelsize];
            for (var i = 0; i < wheelsize; i++)
            {
                retData.Drivetrain.WheelSimulated[i] = GetByte() >0;
            }
            retData.Drivetrain.WheelRadius = new float[wheelsize];
            for (var i = 0; i < wheelsize; i++)
            {
                retData.Drivetrain.WheelRadius[i] = GetFloat();
            }
            retData.Drivetrain.WheelPowered = new bool[wheelsize];
            for (var i = 0; i < wheelsize; i++)
            {
                retData.Drivetrain.WheelPowered[i] = GetByte() >0;
            }
            retData.Drivetrain.WheelLiftable = new bool[wheelsize];
            for (var i = 0; i < wheelsize; i++)
            {
                retData.Drivetrain.WheelLiftable[i] = GetByte() >0;
            }

            retData.Axilliary.SelectorCount = GetInt();
            retData.Axilliary.ShifterType = Encoding.UTF8.GetString(GetSubArray(16)).Replace('\0', ' ').Trim();

            retData.Axilliary.cityDestinationId = Encoding.UTF8.GetString(GetSubArray(64)).Replace('\0', ' ').Trim();
            retData.Axilliary.citySourceId = Encoding.UTF8.GetString(GetSubArray(64)).Replace('\0', ' ').Trim();
            retData.Axilliary.compDestinationId = Encoding.UTF8.GetString(GetSubArray(64)).Replace('\0', ' ').Trim();
            retData.Axilliary.compSourceId = Encoding.UTF8.GetString(GetSubArray(64)).Replace('\0', ' ').Trim();

            retData.Axilliary.trailer_coordinateX = GetFloat();
            retData.Axilliary.trailer_coordinateY = GetFloat();
            retData.Axilliary.trailer_coordinateZ = GetFloat();

            retData.Axilliary.trailer_RotationeX = GetFloat();
            retData.Axilliary.trailer_RotationeY = GetFloat();
            retData.Axilliary.trailer_RotationeZ = GetFloat();

            retData.Axilliary.trailer_lv_accelerationX = GetFloat();
            retData.Axilliary.trailer_lv_accelerationY = GetFloat();
            retData.Axilliary.trailer_lv_accelerationZ = GetFloat();

            retData.Axilliary.trailer_av_accelerationX = GetFloat();
            retData.Axilliary.trailer_av_accelerationY = GetFloat();
            retData.Axilliary.trailer_av_accelerationZ = GetFloat();

            retData.Axilliary.trailer_la_accelerationX = GetFloat();
            retData.Axilliary.trailer_la_accelerationY = GetFloat();
            retData.Axilliary.trailer_la_accelerationZ = GetFloat();

            retData.Axilliary.trailer_aa_accelerationX = GetFloat();
            retData.Axilliary.trailer_aa_accelerationY = GetFloat();
            retData.Axilliary.trailer_aa_accelerationZ = GetFloat();

          
            retData.Axilliary.trailer_wheelSuspDeflection = new float[wheelsize];
            for (var i = 0; i < wheelsize; i++)
            {
                retData.Axilliary.trailer_wheelSuspDeflection[i] = GetFloat();
            }


            retData.Axilliary.trailer_wheelOnGround = new bool[wheelsize];
            for (var i = 0; i < wheelsize; i++)
            {
                retData.Axilliary.trailer_wheelOnGround[i] = GetByte()>0;
            }
            retData.Axilliary.trailer_wheelSubstance = new int[wheelsize];
            for (var i = 0; i < wheelsize; i++)
            {
                retData.Axilliary.trailer_wheelSubstance[i] = GetInt();
            }
            retData.Axilliary.trailer_wheelVelocity = new float[wheelsize];
            for (var i = 0; i < wheelsize; i++)
            {
                retData.Axilliary.trailer_wheelVelocity[i] = GetFloat();
            }
            retData.Axilliary.trailer_wheelSteering = new float[wheelsize];
            for (var i = 0; i < wheelsize; i++)
            {
                retData.Axilliary.trailer_wheelSteering[i] = GetFloat();
            }
            retData.Axilliary.trailer_wheelRotation = new float[wheelsize];
            for (var i = 0; i < wheelsize; i++)
            {
                retData.Axilliary.trailer_wheelRotation[i] = GetFloat();
            }

            retData.Axilliary.lv_accelerationX = GetFloat();
            retData.Axilliary.lv_accelerationY = GetFloat();
            retData.Axilliary.lv_accelerationZ = GetFloat();

            retData.Axilliary.av_accelerationX = GetFloat();
            retData.Axilliary.av_accelerationY = GetFloat();
            retData.Axilliary.av_accelerationZ = GetFloat();

            retData.Axilliary.aa_accelerationX = GetFloat();
            retData.Axilliary.aa_accelerationY = GetFloat();
            retData.Axilliary.aa_accelerationZ = GetFloat();

            retData.Axilliary.cabinOffsetX = GetFloat();
            retData.Axilliary.cabinOffsetY = GetFloat();
            retData.Axilliary.cabinOffsetZ = GetFloat();

            retData.Axilliary.cabinOffsetRotationX = GetFloat();
            retData.Axilliary.cabinOffsetRotationY = GetFloat();
            retData.Axilliary.cabinOffsetRotationZ = GetFloat();

            retData.Axilliary.cabinAVX = GetFloat();
            retData.Axilliary.cabinAVY = GetFloat();
            retData.Axilliary.cabinAVZ = GetFloat();
            retData.Axilliary.cabinAAX = GetFloat();
            retData.Axilliary.cabinAAY = GetFloat();
            retData.Axilliary.cabinAAZ = GetFloat();


            retData.Axilliary.HeadOffsetX = GetFloat();
            retData.Axilliary.HeadOffsetY = GetFloat();
            retData.Axilliary.HeadOffsetZ = GetFloat();

            retData.Axilliary.HeadOffsetRotationX = GetFloat();
            retData.Axilliary.HeadOffsetRotationY = GetFloat();
            retData.Axilliary.HeadOffsetRotationZ = GetFloat();

            retData.Axilliary.TruckWheelSuspDeflection = new float[wheelsize];
            for (var i = 0; i < wheelsize; i++)
            {
                retData.Axilliary.TruckWheelSuspDeflection[i] = GetFloat();
            }

            retData.Axilliary.TruckWheelOnGround = new bool[wheelsize];
            for (var i = 0; i < wheelsize; i++)
            {
                retData.Axilliary.TruckWheelOnGround[i] = GetByte()>0;
            }

            retData.Axilliary.TruckWheelSubstance = new int[wheelsize];
            for (var i = 0; i < wheelsize; i++)
            {
                retData.Axilliary.TruckWheelSubstance[i] = GetInt();
            }

            retData.Axilliary.TruckWheelVelocity = new float[wheelsize];
            for (var i = 0; i < wheelsize; i++)
            {
                retData.Axilliary.TruckWheelVelocity[i] = GetFloat();
            }
            retData.Axilliary.TruckWheelSteering = new float[wheelsize];
            for (var i = 0; i < wheelsize; i++)
            {
                retData.Axilliary.TruckWheelSteering[i] = GetFloat();
            }
            retData.Axilliary.TruckWheelRotation = new float[wheelsize];
            for (var i = 0; i < wheelsize; i++)
            {
                retData.Axilliary.TruckWheelRotation[i] = GetFloat();
            }
            retData.Axilliary.TruckWheelLift = new float[wheelsize];
            for (var i = 0; i < wheelsize; i++)
            {
                retData.Axilliary.TruckWheelLift[i] = GetFloat();
            }
            retData.Axilliary.TruckWheelLiftOffset = new float[wheelsize];
            for (var i = 0; i < wheelsize; i++)
            {
                retData.Axilliary.TruckWheelLiftOffset[i] = GetFloat();
            }

            var tempus = Encoding.UTF8.GetString(GetSubArray(10)).Replace('\0', ' ').Trim();
            return retData;
        }

        private byte GetByte() {
            var temp = _data[_offset];
            _offset++;
            return temp;
        }
        private uint GetUint() {
            while (_offset % 4 != 0) {
                _offset++;
            }
            var temp =  (uint)((_data[_offset+3] << 24) | (_data[_offset + 2] << 16) | (_data[_offset + 1] << 8) | (_data[_offset]));
            _offset += 4;
            return temp;
        } 
        private float GetFloat() {
            while (_offset % 4 != 0)
            {
                _offset++;
            }
            var temp = new[]{_data[_offset], _data[_offset + 1], _data[_offset + 2], _data[_offset + 3]};
            _offset += 4;
            return BitConverter.ToSingle(temp,0);
        } 
        //TODO: Check if this is also correct
        private int GetInt() {
            while (_offset % 4 != 0)
            {
                _offset++;
            }
            var temp = (_data[_offset + 3] << 24) | (_data[_offset + 2] << 16) | (_data[_offset + 1] << 8) | (_data[_offset]);
            _offset += 4;
            return temp;
        }

        private byte[] GetSubArray( int length) {
            var ret = new byte[length];
            for (var i = 0; i < length; i++) {
                ret[i] = _data[_offset + i];
            }

            _offset += length;
            return ret;
        }
        private byte[] GetSubArray(int specialOffset, int length)
        {
            var ret = new byte[length];
            for (var i = 0; i < length; i++)
            {
                ret[i] = _data[specialOffset + i];
            }
            return ret;
        }
    }
}