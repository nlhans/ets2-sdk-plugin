// 
// Ets2SdkClient
// Ets2SdkDataAlt.cs
// 22:51
using System;
using System.Text;
using Ets2SdkClient.Object;

namespace Ets2SdkClient {
    /// <summary>
    /// Convert class
    /// </summary>
    public class SCSSdkConvert {
        private byte[] _data;
        private int _offset;
        private readonly int[] _offsetAreas = new[] {0,40,280,320,1400,1600,2000,2200,2400,4200,4400 };
        private int _offsetArea;
        private const int StringSize = 64;
        private const int WheelSize = 16;
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

            
            NextOffsetArea();
            #endregion









            return retData;
        }

        private bool GetBool() {
            var temp = _data[_offset];
            _offset++;
            return temp>0;
        }
        // actually no Uint conversion... 
        //TODO: check if it is every time correct uint
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
            _offset = _offsetAreas[_offsetArea];
        }

        private string GetString(int length = StringSize) {
            return Encoding.UTF8.GetString(GetSubArray(length)).Replace('\0', ' ').Trim();
        }


      
    }
}