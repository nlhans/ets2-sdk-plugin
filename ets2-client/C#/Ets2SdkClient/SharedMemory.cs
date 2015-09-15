using System;
using System.IO.MemoryMappedFiles;
using System.Runtime.InteropServices;

namespace Ets2SdkClient
{
    public class SharedMemory
    {
        private const uint defaultMapSize = 16*1024;
        
        public bool Hooked { get; private set; }
        public Exception HookException { get; private set; }

        public byte[] RawData { get; private set; }

        private MemoryMappedFile memoryMappedHandle;
        private MemoryMappedViewAccessor memoryMappedView;

        public void Connect(string map)
        {
            // Default 16kiB of map size.
            Connect(map, defaultMapSize);
        }

        public void Connect(string map, uint mapSize)
        {
            if (Hooked)
            {
                Disconnect();
            }

            // Reset any errors
            HookException = null;

            try
            {
                RawData = new byte[mapSize];

                // Open the map and create a "memory view" at the begin (byte 0)
                memoryMappedHandle = MemoryMappedFile.CreateOrOpen(map, mapSize, MemoryMappedFileAccess.ReadWrite);
                memoryMappedView = memoryMappedHandle.CreateViewAccessor(0, mapSize);

                // Mark as a success.
                Hooked = true;
            }
            catch (Exception e)
            {
                // We were unable to hook onto the map.
                Hooked = false;
                HookException = e;
            }
        }

        public void Disconnect()
        {
            Hooked = false;

            memoryMappedView.Dispose();
            memoryMappedHandle.Dispose();
        }

        public T Update<T>()
        {
            Update();

            // Convert the data to our object.
            return ToObject<T>(RawData);
        }

        public void Update()
        {
            if (!Hooked || memoryMappedView == null)
            {
                return;
            }

            // Re-read data from the view.
            memoryMappedView.ReadArray(0, RawData, 0, RawData.Length);
        }

        /// <summary>
        /// Cast a set of bytes to a managed C# object.
        /// </summary>
        /// <typeparam name="T">Managed C# object type</typeparam>
        /// <param name="structureDataBytes">Bytes array</param>
        /// <returns>Managed object from given bytes</returns>
        protected T ToObject<T>(byte[] structureDataBytes)
        {
            T createdObject = default(T);

            var memoryObjectSize = Marshal.SizeOf(typeof(T));

            // Cannot create object from array that is too small.
            if (memoryObjectSize > structureDataBytes.Length)
                return createdObject;

            // Reserve unmanaged memory, copy structureDataBytes bytes to there, and convert this unmanaged memory to a managed type.
            // Then free memory.
            var reservedMemPtr = Marshal.AllocHGlobal(memoryObjectSize);

            Marshal.Copy(structureDataBytes, 0, reservedMemPtr, memoryObjectSize);

            createdObject = (T)Marshal.PtrToStructure(reservedMemPtr, typeof(T));

            Marshal.FreeHGlobal(reservedMemPtr);
            
            return createdObject;
        }


    }
}
