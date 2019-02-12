using System;
using System.Runtime.Remoting.Lifetime;
using System.Threading;
using System.Windows.Forms.VisualStyles;
using SCSSdkClient.Object;
//TODO: possible idea: check if ets is running and if not change updaterate to infinity (why most of the user may not quit the application while ets is running)
namespace SCSSdkClient {
    public delegate void TelemetryData(SCSTelemetry data, bool newTimestamp);

    /// <summary>
    ///     Handle the SCSSdkTelemetry.
    ///     Currently IDisposable. Was implemented because of an error 
    /// </summary>
    public class SCSSdkTelemetry : IDisposable {
        private const string DefaultSharedMemoryMap = "Local\\SimTelemetrySCS";
        private const int DefaultUpdateInterval = 25;

        private Timer _updateTimer;

        private uint lastTime = 0xFFFFFFFF;


#if LOGGING
        public void Dispose() {
            _updateTimer?.Dispose();
            Log.SaveShutdown();
        }
#else
         public void Dispose() => _updateTimer?.Dispose();

#endif
        

    private SharedMemory SharedMemory;
        private bool wasFinishingJob;

        private bool wasOnJob;
        private bool wasConnected;

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
        public event EventHandler TrailerConnected;
        public event EventHandler TrailerDisconnected;

        public void pause() {
            _updateTimer.Change(Timeout.Infinite, Timeout.Infinite);
        }

        public void resume() {
            var tsInterval = new TimeSpan(0, 0, 0, 0, UpdateInterval);
            _updateTimer.Change(tsInterval, tsInterval);
        }

        /// <summary>
        ///     Set up SCS telemetry provider.
        ///     Connects to shared memory map, sets up timebase.
        /// </summary>
        /// <param name="map">Memory Map location</param>
        /// <param name="interval">Timebase interval</param>
        private void Setup(string map, int interval) {
#if LOGGING
            AppDomain.CurrentDomain.UnhandledException += CurrentDomain_UnhandledException;
            Log.Write("Start of the Telemetry");
            Log.Write("Set up everything and start the updatetimer");
#endif

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
#if LOGGING
            Log.Write("Every thing is set up correctly and the timer was started");
#endif

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

            if (wasConnected != scsTelemetry.SpecialEventsValues.TrailerConnected)
            {
                wasConnected = scsTelemetry.SpecialEventsValues.TrailerConnected;
                if (scsTelemetry.SpecialEventsValues.TrailerConnected)
                {
                    TrailerConnected?.Invoke(this, new EventArgs());
                } else {
                    TrailerDisconnected?.Invoke(this, new EventArgs());
                }
            }

            
            lastTime = time;
        }
#if LOGGING
        private static void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e) {
            try {
                var ex = (Exception) e.ExceptionObject;
                Console.WriteLine("Fatal unhandled Exception... pls create an issue and add the log file on github");
                Log.Write(ex.ToString());
                Log.SaveShutdown();
            } catch (Exception ex) {
                try {
                    Console.WriteLine("Can't write log / or close it after an unhandled Exception: "+ex);
                } finally {
                    Environment.Exit(142);
                }
            }
        }
#endif
    }
}