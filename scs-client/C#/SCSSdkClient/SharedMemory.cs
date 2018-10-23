using System;
using System.IO.MemoryMappedFiles;
using SCSSdkClient.Object;

namespace SCSSdkClient {
    public class SharedMemory {
        private const uint defaultMapSize = 16 * 1024;
        private readonly SCSSdkConvert alt = new SCSSdkConvert();

        private MemoryMappedFile memoryMappedHandle;
        private MemoryMappedViewAccessor memoryMappedView;

        public bool Hooked { get; private set; }
        public Exception HookException { get; private set; }

        public byte[] RawData { get; private set; }

        public void Connect(string map) => Connect(map, defaultMapSize);

        public void Connect(string map, uint mapSize) {
            if (Hooked) {
                Disconnect();
            }

            // Reset any errors
            HookException = null;

            try {
                RawData = new byte[mapSize];

                // Open the map and create a "memory view" at the begin (byte 0)
                memoryMappedHandle = MemoryMappedFile.CreateOrOpen(map, mapSize, MemoryMappedFileAccess.ReadWrite);
                memoryMappedView = memoryMappedHandle.CreateViewAccessor(0, mapSize);

                // Mark as a success.
                Hooked = true;
            } catch (Exception e) {
                // We were unable to hook onto the map.
                Hooked = false;
                HookException = e;
            }
        }

        public void Disconnect() {
            Hooked = false;

            memoryMappedView.Dispose();
            memoryMappedHandle.Dispose();
        }

        public SCSTelemetry Update<T>() {
            Update();

            // Convert the data to our object.
            return ToObject<T>(RawData);
        }

        public void Update() {
            if (!Hooked || memoryMappedView == null) {
                return;
            }

            // Re-read data from the view.
            memoryMappedView.ReadArray(0, RawData, 0, RawData.Length);
        }

        /// <summary>
        ///     Cast a set of bytes to a managed C# object.
        /// </summary>
        /// <typeparam name="T">Managed C# object type</typeparam>
        /// <param name="structureDataBytes">Bytes array</param>
        /// <returns>Managed object from given bytes</returns>
        protected SCSTelemetry ToObject<T>(byte[] structureDataBytes) => alt.Convert(structureDataBytes);
    }
}