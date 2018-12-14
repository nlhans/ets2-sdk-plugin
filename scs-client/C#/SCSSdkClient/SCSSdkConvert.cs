// 
// Ets2SdkClient
// Ets2SdkDataAlt.cs
// 22:51
using System;
using System.Text;
using SCSSdkClient.Object;

namespace SCSSdkClient {
    /// <summary>
    /// Convert class
    /// </summary>
    public class SCSSdkConvert {
        private byte[] _data;
        private int _offset;
        private readonly int[] _offsetAreas = new[] {0,40,280,320,1400,1600,2000,2200,2400,4200,4400 };
        private int _offsetArea;
        private const int StringSize = 64;
        public static  int WheelSize = 16;
        /// <summary>
        /// Convert the Shared Memory Byte data structure in a C# object
        /// </summary>
        /// <param name="structureDataBytes">
        /// byte array from the shared memory
        /// </param>
        /// <returns>
        /// C# object with game data of the shared memory
        /// </returns>
        public SCSTelemetry Convert(byte[] structureDataBytes) {
            _offsetArea=0;
            SetOffset();


            _data = structureDataBytes;
            var retData = new SCSTelemetry();

            #region FIRST ZONE 
            retData.Timestamp = GetUint();
            retData.Paused = GetBool();

            
            NextOffsetArea();
            #endregion

            #region SECOND ZONE
            retData.DllVersion = GetUint();
            retData.GameVersion.Major = GetUint();
            retData.GameVersion.Minor = GetUint();
            retData.Game = GetUint().ToEnum<SCSGame>();
            retData.TelemetryVersion.Major = GetUint();
            retData.TelemetryVersion.Minor = GetUint();

            var temp = GetUint();
            retData.TruckValues.ConstantsValues.MotorValues.ForwardGearCount = GetUint();
            retData.TruckValues.ConstantsValues.MotorValues.ReverseGearCount = GetUint();
            retData.TruckValues.ConstantsValues.MotorValues.RetarderStepCount = GetUint();
            retData.TruckValues.ConstantsValues.WheelsValues.Count = GetUint();
            retData.TruckValues.ConstantsValues.MotorValues.SelectorCount = GetUint();
            retData.JobValues.DeliveryTime.Value = GetUint();
            retData.SetGameTime(temp);

            retData.TruckValues.CurrentValues.MotorValues.GearValues.HShifterSlot = GetUint();
            retData.TruckValues.CurrentValues.MotorValues.BrakeValues.RetarderLevel = GetUint();
            retData.TruckValues.CurrentValues.LightsValues.AuxFront = GetUint().ToEnum<AuxLevel>();
            retData.TruckValues.CurrentValues.LightsValues.AuxRoof = GetUint().ToEnum<AuxLevel>();
            retData.TrailerValues.Wheelvalues.Substance = GetUintArray(WheelSize);
            retData.TruckValues.CurrentValues.WheelsValues.Substance = GetUintArray(WheelSize);
            
            NextOffsetArea();
            #endregion

            #region THIRD ZONE

            retData.CommonValues.NextRestStop.Value = GetInt();

            retData.TruckValues.CurrentValues.MotorValues.GearValues.Selected = GetInt();
            retData.TruckValues.CurrentValues.DashboardValues.GearDashboards = GetInt();


            NextOffsetArea();
            #endregion

            #region 4TH ZONE

            retData.CommonValues.Scale = GetFloat();
            retData.TruckValues.ConstantsValues.CapacityValues.Fuel = GetFloat();
            retData.TruckValues.ConstantsValues.WarningFactorValues.Fuel = GetFloat();
            retData.TruckValues.ConstantsValues.CapacityValues.AdBlue = GetFloat();
            retData.TruckValues.ConstantsValues.WarningFactorValues.AdBlue = GetFloat();
            retData.TruckValues.ConstantsValues.WarningFactorValues.AirPressure = GetFloat();
            retData.TruckValues.ConstantsValues.WarningFactorValues.AirPressureEmergency = GetFloat();
            retData.TruckValues.ConstantsValues.WarningFactorValues.OilPressure = GetFloat();
            retData.TruckValues.ConstantsValues.WarningFactorValues.WaterTemperature = GetFloat();
            retData.TruckValues.ConstantsValues.WarningFactorValues.BatteryVoltage = GetFloat();
            retData.TruckValues.ConstantsValues.MotorValues.EngineRpmMax = GetFloat();
            retData.TruckValues.ConstantsValues.MotorValues.DifferentialRation = GetFloat();
            retData.TrailerValues.CargoValues.Mass = GetFloat();
            retData.TruckValues.ConstantsValues.WheelsValues.Radius = GetFloatArray(WheelSize);
            retData.TruckValues.ConstantsValues.MotorValues.GearRatiosForward = GetFloatArray(24);
            retData.TruckValues.ConstantsValues.MotorValues.GearRatiosReverse = GetFloatArray(8);


            retData.TruckValues.CurrentValues.DashboardValues.Speed.Value = GetFloat();
            retData.TruckValues.CurrentValues.DashboardValues.RPM = GetFloat();
            retData.ControlValues.InputValues.Steering = GetFloat();
            retData.ControlValues.InputValues.Throttle = GetFloat();
            retData.ControlValues.InputValues.Brake = GetFloat();
            retData.ControlValues.InputValues.Clutch = GetFloat();
            retData.ControlValues.GameValues.Steering = GetFloat();
            retData.ControlValues.GameValues.Throttle = GetFloat();
            retData.ControlValues.GameValues.Brake = GetFloat();
            retData.ControlValues.GameValues.Clutch = GetFloat();
            retData.TruckValues.CurrentValues.DashboardValues.CruiseControlSpeed.Value = GetFloat();
            retData.TruckValues.CurrentValues.MotorValues.BrakeValues.AirPressure = GetFloat();
            retData.TruckValues.CurrentValues.MotorValues.BrakeValues.Temperature = GetFloat();
            retData.TruckValues.CurrentValues.DashboardValues.FuelValue.Amount = GetFloat();
            retData.TruckValues.CurrentValues.DashboardValues.FuelValue.AverageConsumption = GetFloat();
            retData.TruckValues.CurrentValues.DashboardValues.FuelValue.Range = GetFloat();
            retData.TruckValues.CurrentValues.DashboardValues.AdBlue = GetFloat();
            retData.TruckValues.CurrentValues.DashboardValues.OilPressure = GetFloat();
            retData.TruckValues.CurrentValues.DashboardValues.OilTemperature = GetFloat();
            retData.TruckValues.CurrentValues.DashboardValues.WaterTemperature = GetFloat();
            retData.TruckValues.CurrentValues.DashboardValues.BatteryVoltage = GetFloat();
            retData.TruckValues.CurrentValues.LightsValues.DashboardBacklight = GetFloat();
            retData.TruckValues.CurrentValues.DamageValues.Engine = GetFloat();
            retData.TruckValues.CurrentValues.DamageValues.Transmission = GetFloat();
            retData.TruckValues.CurrentValues.DamageValues.Cabin = GetFloat();
            retData.TruckValues.CurrentValues.DamageValues.Chassis = GetFloat();
            retData.TruckValues.CurrentValues.DamageValues.WheelsAvg = GetFloat();
            retData.TrailerValues.Damage = GetFloat();


            retData.TruckValues.CurrentValues.DashboardValues.Odometer = GetFloat();
            retData.NavigationValues.NavigationDistance = GetFloat();
            retData.NavigationValues.NavigationTime = GetFloat();
            retData.NavigationValues.SpeedLimit.Value = GetFloat();
            retData.TrailerValues.Wheelvalues.SuspDeflection = GetFloatArray(WheelSize);
            retData.TruckValues.CurrentValues.WheelsValues.SuspDeflection = GetFloatArray(WheelSize);
            retData.TrailerValues.Wheelvalues.Velocity = GetFloatArray(WheelSize);
            retData.TruckValues.CurrentValues.WheelsValues.Velocity = GetFloatArray(WheelSize);
            retData.TrailerValues.Wheelvalues.Steering = GetFloatArray(WheelSize);
            retData.TruckValues.CurrentValues.WheelsValues.Steering = GetFloatArray(WheelSize);
            retData.TrailerValues.Wheelvalues.Rotation = GetFloatArray(WheelSize);
            retData.TruckValues.CurrentValues.WheelsValues.Rotation = GetFloatArray(WheelSize);
            retData.TruckValues.CurrentValues.WheelsValues.Lift = GetFloatArray(WheelSize);
            retData.TruckValues.CurrentValues.WheelsValues.LiftOffset = GetFloatArray(WheelSize);


            NextOffsetArea();
            #endregion

            #region 5Th ZONE

            retData.TruckValues.ConstantsValues.WheelsValues.Steerable = GetBoolArray(WheelSize);
            retData.TruckValues.ConstantsValues.WheelsValues.Simulated = GetBoolArray(WheelSize);
            retData.TruckValues.ConstantsValues.WheelsValues.Powered = GetBoolArray(WheelSize);
            retData.TruckValues.ConstantsValues.WheelsValues.Liftable = GetBoolArray(WheelSize);

            retData.TrailerValues.Attached = GetBool();
            retData.TruckValues.CurrentValues.MotorValues.BrakeValues.ParkingBrake = GetBool();
            retData.TruckValues.CurrentValues.MotorValues.BrakeValues.MotorBrake = GetBool();
            retData.TruckValues.CurrentValues.DashboardValues.WarningValues.AirPressure = GetBool();
            retData.TruckValues.CurrentValues.DashboardValues.WarningValues.AirPressureEmergency = GetBool();

            retData.TruckValues.CurrentValues.DashboardValues.WarningValues.FuelW = GetBool();
            retData.TruckValues.CurrentValues.DashboardValues.WarningValues.AdBlue = GetBool();
            retData.TruckValues.CurrentValues.DashboardValues.WarningValues.OilPressure = GetBool();
            retData.TruckValues.CurrentValues.DashboardValues.WarningValues.WaterTemperature = GetBool();
            retData.TruckValues.CurrentValues.DashboardValues.WarningValues.BatteryVoltage = GetBool();
            retData.TruckValues.CurrentValues.ElectricEnabled = GetBool();
            retData.TruckValues.CurrentValues.EngineEnabled = GetBool();
            retData.TruckValues.CurrentValues.DashboardValues.Wipers = GetBool();
            retData.TruckValues.CurrentValues.LightsValues.BlinkerLeftActive = GetBool();
            retData.TruckValues.CurrentValues.LightsValues.BlinkerRightActive = GetBool();
            retData.TruckValues.CurrentValues.LightsValues.BlinkerLeftOn = GetBool();
            retData.TruckValues.CurrentValues.LightsValues.BlinkerRightOn = GetBool();
            retData.TruckValues.CurrentValues.LightsValues.Parking = GetBool();
            retData.TruckValues.CurrentValues.LightsValues.BeamLow = GetBool();
            retData.TruckValues.CurrentValues.LightsValues.BeamHigh = GetBool();
            retData.TruckValues.CurrentValues.LightsValues.Beacon = GetBool();
            retData.TruckValues.CurrentValues.LightsValues.Brake = GetBool();
            retData.TruckValues.CurrentValues.LightsValues.Reverse = GetBool();
            retData.TruckValues.CurrentValues.DashboardValues.CruiseControl = GetBool();
            retData.TrailerValues.Wheelvalues.OnGround = GetBoolArray(WheelSize);
            retData.TruckValues.CurrentValues.WheelsValues.OnGround = GetBoolArray(WheelSize);
            retData.TruckValues.CurrentValues.MotorValues.GearValues.HShifterSelector = GetBoolArray(2);


            NextOffsetArea();
            #endregion

            #region 6TH ZONE

            retData.TruckValues.Positioning.Cabin = GetFVector();
            retData.TruckValues.Positioning.Head = GetFVector();
            retData.TruckValues.Positioning.Hook = GetFVector();
            var tempPos = new SCSTelemetry.FVector[WheelSize];
            for (var j = 0; j < WheelSize; j++) {
                tempPos[j] = new SCSTelemetry.FVector {X = GetFloat()};
            }
            for (var j = 0; j < WheelSize; j++)
            {
                tempPos[j].Y = GetFloat();
            }
            for (var j = 0; j < WheelSize; j++)
            {
                tempPos[j].Z = GetFloat();
            }

            retData.TruckValues.ConstantsValues.WheelsValues.PositionValues = tempPos;

            retData.TrailerValues.AccelerationValues.LinearVelocity = GetFVector();
            retData.TruckValues.CurrentValues.AccelerationValues.LinearVelocity = GetFVector();
            retData.TrailerValues.AccelerationValues.AngularVelocity = GetFVector();
            retData.TruckValues.CurrentValues.AccelerationValues.AngularVelocity = GetFVector();
            retData.TrailerValues.AccelerationValues.LinearAcceleration = GetFVector();
            retData.TruckValues.CurrentValues.AccelerationValues.LinearAcceleration = GetFVector();
            retData.TrailerValues.AccelerationValues.AngularAcceleration = GetFVector();
            retData.TruckValues.CurrentValues.AccelerationValues.AngularAcceleration = GetFVector();
            retData.TruckValues.CurrentValues.AccelerationValues.CabinAngularVelocity = GetFVector();
            retData.TruckValues.CurrentValues.AccelerationValues.CabinAngularAcceleration = GetFVector();
            
            NextOffsetArea();
            #endregion

            #region 7TH ZONE

            retData.TruckValues.Positioning.CabinOffset = GetFPlacement();
            retData.TruckValues.Positioning.HeadOffset = GetFPlacement();


            NextOffsetArea();
            #endregion

            #region 8TH ZONE 

            retData.SetTruckPosition(GetDPlacement());
            retData.TrailerValues.Position = GetDPlacement();

            

            NextOffsetArea();
            #endregion

            #region 9TH ZONE

            retData.TruckValues.ConstantsValues.BrandId = GetString();
            retData.TruckValues.ConstantsValues.Brand = GetString();
            retData.TruckValues.ConstantsValues.Id = GetString();
            retData.TrailerValues.Chassis = GetString();
            retData.TrailerValues.CargoValues.Name = GetString();
            retData.TruckValues.ConstantsValues.Name = GetString();
            retData.TrailerValues.Id = GetString();
            retData.TrailerValues.Name = GetString();
            retData.JobValues.CityDestinationId = GetString();
            retData.JobValues.CityDestination = GetString();
            retData.JobValues.CompanyDestinationId = GetString();
            retData.JobValues.CompanyDestination = GetString();
            retData.JobValues.CitySourceId = GetString();
            retData.JobValues.CitySource = GetString();
            retData.JobValues.CompanySourceId = GetString();
            retData.JobValues.CompanySource = GetString();
            var tempShift = GetString(16);
            if (tempShift?.Length > 0) {
                retData.TruckValues.ConstantsValues.MotorValues.ShifterTypeValue = tempShift.ToEnum<ShifterType>();
            }
            

            NextOffsetArea();
            #endregion

            #region 10TH ZONE

            retData.JobValues.Income = GetLong();
            
            NextOffsetArea();
            #endregion

            #region 11TH ZONE

            retData.SpecialEventsValues.OnJob = GetBool();
            retData.SpecialEventsValues.JobFinished = GetBool();

            #endregion



            return retData;
        }

        private bool GetBool() {
            var temp = _data[_offset];
            _offset++;
            return temp>0;
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
        private double GetDouble()
        {
            while (_offset % 4 != 0)
            {
                _offset++;
            }
            var temp = new[] { _data[_offset], _data[_offset + 1], _data[_offset + 2], _data[_offset + 3], _data[_offset + 4], _data[_offset +5], _data[_offset + 6], _data[_offset + 7] };
            _offset += 8;
            return BitConverter.ToDouble(temp, 0);
        }
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
       

        private void NextOffsetArea() {
            _offsetArea++;
            SetOffset();
        }

        private void SetOffset() {
            // Debug Fix?
            if (_offsetArea >= _offsetAreas.Length) {
                return;

            }

            _offset = _offsetAreas[_offsetArea];
        }

        private string GetString(int length = StringSize) {
            return Encoding.UTF8.GetString(GetSubArray(length)).Replace('\0', ' ').Trim();
        }

        private uint[] GetUintArray(int length) {
            var res = new uint[length];
            for (var i = 0; i < length; i++) {
                res[i] = GetUint();
            }

            return res;
        }
        private float[] GetFloatArray(int length)
        {
            var res = new float[length];
            for (var i = 0; i < length; i++)
            {
                res[i] = GetFloat();
            }

            return res;
        }
        private bool[] GetBoolArray(int length)
        {
            var res = new bool[length];
            for (var i = 0; i < length; i++)
            {
                res[i] = GetBool();
            }

            return res;
        }

        private SCSTelemetry.FVector GetFVector() {
            return  new SCSTelemetry.FVector{ X = GetFloat(), Y = GetFloat(), Z = GetFloat() };
        }
        private SCSTelemetry.DVector GetDVector()
        {
            return new SCSTelemetry.DVector { X = GetDouble(), Y = GetDouble(), Z = GetDouble() };
        }
        private SCSTelemetry.Euler GetEuler() {
            return  new SCSTelemetry.Euler{ Heading = GetFloat(), Pitch = GetFloat(), Roll = GetFloat() };
        }
        private SCSTelemetry.Euler GetDEuler()
        {
            return new SCSTelemetry.Euler { Heading = (float)GetDouble(), Pitch = (float)GetDouble(), Roll = (float)GetDouble() };
        }
        private SCSTelemetry.FPlacement GetFPlacement() {
            return new SCSTelemetry.FPlacement{Position =  GetFVector(), Orientation = GetEuler()};
        }
        private SCSTelemetry.DPlacement GetDPlacement()
        {
            return new SCSTelemetry.DPlacement { Position = GetDVector(), Orientation = GetDEuler() };
        }

        private long GetLong() {
           
            var temp = new[] { _data[_offset], _data[_offset + 1], _data[_offset + 2], _data[_offset + 3], _data[_offset + 4], _data[_offset + 5], _data[_offset + 6], _data[_offset + 7] };
            _offset += 8;
            return BitConverter.ToInt64(temp, 0);
        }
    }
}