using System;
using System.Threading;
using SCSSdkClient.Object;

namespace SCSSdkClient {
    public delegate void TelemetryData(SCSTelemetry data, bool newTimestamp);

    public class SCSSdkTelemetry {
        private const string DefaultSharedMemoryMap = "Local\\SimTelemetrySCS";
        private const int DefaultUpdateInterval = 25;

        private Timer _updateTimer;

        private uint lastTime = 0xFFFFFFFF;
        private SharedMemory SharedMemory;
        private bool wasFinishingJob;

        private bool wasOnJob;

        public SCSSdkTelemetry() => Setup(DefaultSharedMemoryMap, DefaultUpdateInterval);

        public SCSSdkTelemetry(string map) => Setup(map, DefaultUpdateInterval);

        public SCSSdkTelemetry(int interval) => Setup(DefaultSharedMemoryMap, interval);

        public SCSSdkTelemetry(string map, int interval) => Setup(map, interval);

        public string Map { get; private set; }
        public int UpdateInterval { get; private set; }
        public Exception Error { get; private set; }

        public event TelemetryData Data;

        public event EventHandler JobStarted;
        public event EventHandler JobFinished;

        /// <summary>
        ///     Set up SCS telemetry provider.
        ///     Connects to shared memory map, sets up timebase.
        /// </summary>
        /// <param name="map">Memory Map location</param>
        /// <param name="interval">Timebase interval</param>
        private void Setup(string map, int interval) {
            Map = map;
            UpdateInterval = interval;

            SharedMemory = new SharedMemory();
            SharedMemory.Connect(map);

            if (!SharedMemory.Hooked) {
                Error = SharedMemory.HookException;
                return;
            }

            var tsInterval = new TimeSpan(0, 0, 0, 0, interval);

            _updateTimer = new Timer(_updateTimer_Elapsed, null, tsInterval, tsInterval);
        }

        private void _updateTimer_Elapsed(object sender) {
            var scsTelemetry = SharedMemory.Update<SCSTelemetry>();
            var time = scsTelemetry.Timestamp;
            Data?.Invoke(scsTelemetry, time != lastTime);

            // Job close & start events
            if (wasFinishingJob != scsTelemetry.SpecialEventsValues.JobFinished) {
                wasFinishingJob = scsTelemetry.SpecialEventsValues.JobFinished;
                if (scsTelemetry.SpecialEventsValues.JobFinished) {
                    JobFinished?.Invoke(this, new EventArgs());
                }
            }

            if (wasOnJob != scsTelemetry.SpecialEventsValues.OnJob) {
                wasOnJob = scsTelemetry.SpecialEventsValues.OnJob;
                if (scsTelemetry.SpecialEventsValues.OnJob) {
                    JobStarted?.Invoke(this, new EventArgs());
                }
            }

            lastTime = time;
        }
    }
}