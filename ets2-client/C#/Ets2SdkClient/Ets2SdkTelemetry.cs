using System;
using System.Text;
using System.Threading;
using System.Timers;

using Timer = System.Threading.Timer;

namespace Ets2SdkClient {
    public delegate void TelemetryData(Ets2Telemetry data, bool newTimestamp);

    public class Ets2SdkTelemetry
    {
        private const string DefaultSharedMemoryMap = "Local\\SimTelemetryETS22";
        private const int DefaultUpdateInterval = 25;

        private Timer _updateTimer;
        private SharedMemory SharedMemory;

        public string Map { get; private set; }
        public int UpdateInterval { get; private set; }
        public Exception Error { get; private set; } 

        private uint lastTime = 0xFFFFFFFF;

        public event TelemetryData Data;

        public event EventHandler JobStarted;
        public event EventHandler JobFinished;

        private bool wasOnJob;
        private bool wasFinishingJob;

        public Ets2SdkTelemetry()
        {
            Setup(DefaultSharedMemoryMap, DefaultUpdateInterval);
        }

        public Ets2SdkTelemetry(string map)
        {
            Setup(map, DefaultUpdateInterval);
        }

        public Ets2SdkTelemetry(int interval)
        {
            Setup(DefaultSharedMemoryMap, interval);
        }

        public Ets2SdkTelemetry(string map, int interval)
        {
            Setup(map, interval);
        }
        
        /// <summary>
        /// Set up ETS2 telemetry provider.
        /// Connects to shared memory map, sets up timebase.
        /// </summary>
        /// <param name="map">Memory Map location</param>
        /// <param name="interval">Timebase interval</param>
        private void Setup(string map, int interval)
        {
            Map = map;
            UpdateInterval = interval;

            SharedMemory = new SharedMemory();
            SharedMemory.Connect(map);

            if (!SharedMemory.Hooked)
            {
                Error = SharedMemory.HookException;
                return;
            }
            var tsInterval = new TimeSpan(0, 0, 0,0,interval);

            _updateTimer = new Timer( _updateTimer_Elapsed, null,tsInterval,tsInterval);
             
        }

        void _updateTimer_Elapsed(object sender)
        {
            /*var ets2RawData = SharedMemory.Update<Ets2SdkData>();

            var ets2RawUnmanaged = new Ets2SdkUnmanaged();
            ets2RawUnmanaged.TrailerModel = Encoding.UTF8.GetString(SharedMemory.RawData, ets2RawData.trailerOffset, ets2RawData.trailerLength);
            ets2RawUnmanaged.TruckModel = Encoding.UTF8.GetString(SharedMemory.RawData, ets2RawData.modelOffset, ets2RawData.modelLength);

            var ets2telemetry = new Ets2Telemetry(ets2RawData, ets2RawUnmanaged);*/
            var ets2telemetry = SharedMemory.Update<Ets2Telemetry>();
            var time = ets2telemetry.Time;
            if (Data != null)
                Data(ets2telemetry, time != lastTime);

            // Job close & start events
            if (wasFinishingJob != ets2telemetry.Job.JobFinished)
            {
                wasFinishingJob = ets2telemetry.Job.JobFinished;
                if (ets2telemetry.Job.JobFinished)
                    JobFinished(this, new EventArgs());

            }
            if (wasOnJob != ets2telemetry.Job.OnJob)
            {
                wasOnJob = ets2telemetry.Job.OnJob;
                if (ets2telemetry.Job.OnJob)
                    JobStarted(this, new EventArgs());

            }

            lastTime = time;

        }
    }
}