using System;
using System.Windows.Forms;
using Newtonsoft.Json;
using SCSSdkClient.Object;

namespace SCSSdkClient.Demo {
    /// <inheritdoc />
    public partial class SCSSdkClientDemo : Form {
        /// <summary>
        /// The SCSSdkTelemetry object
        /// </summary>
        public SCSSdkTelemetry Telemetry;

        /// <inheritdoc />
        public SCSSdkClientDemo() {
            InitializeComponent();
            Telemetry = new SCSSdkTelemetry();
            Telemetry.Data += Telemetry_Data;
            Telemetry.JobFinished += TelemetryOnJobFinished;
            Telemetry.JobStarted += TelemetryOnJobStarted;
            Telemetry.TrailerConnected += TelemetryTrailerConnected;
            Telemetry.TrailerDisconnected += TelemetryTrailerDisconnected;
            if (Telemetry.Error != null) {
                lbGeneral.Text =
                    "General info:\r\nFailed to open memory map " +
                    Telemetry.Map +
                    " - on some systems you need to run the client (this app) with elevated permissions, because e.g. you're running Steam/ETS2 with elevated permissions as well. .NET reported the following Exception:\r\n" +
                    Telemetry.Error.Message +
                    "\r\n\r\nStacktrace:\r\n" +
                    Telemetry.Error.StackTrace;
            } 
        }

        private void TelemetryOnJobFinished(object sender, EventArgs args) =>
            MessageBox.Show("Job finished.");

        private void TelemetryOnJobStarted(object sender, EventArgs e) =>
            MessageBox.Show("Just started job OR loaded game with active.");
        private void TelemetryTrailerConnected(object sender, EventArgs e) =>
            MessageBox.Show("A Trailer is now connected to you");
        private void TelemetryTrailerDisconnected(object sender, EventArgs e) =>
            MessageBox.Show("A Trailer is now not more connected to you");

        private void Telemetry_Data(SCSTelemetry data, bool updated) {
           
            try {
                if (InvokeRequired) {
                    Invoke(new TelemetryData(Telemetry_Data), data, updated);
                    return;
                }

                lbGeneral.Text = "General info:\n "+
                                 "\tSDK Version:\n" +
                                 $"\t\t\t{data.DllVersion}\n" +
                                 "\tGame:\n " +
                                 $"\t\t\t{data.Game}\n" +
                                 "\tGame Version:\n" +
                                 $"\t\t\t{data.GameVersion}\n" +
                                 "\tTelemetry Version:\n" +
                                 $"\t\t\t{data.TelemetryVersion}\n" +
                                 "\tTimeStamp:\n" +
                                 $"\t\t\t{data.Timestamp}\n" +
                                 "\tGame Paused:\n" +
                                 $"\t\t\t{data.Paused}\n" +
                                 "\tOn Job:\n" +
                                 $"\t\t\t{data.SpecialEventsValues.OnJob}\n" +
                                 "\tJob Finished:\n" +
                                 $"\t\t\t{data.SpecialEventsValues.JobFinished}\n" +
                                 "\tTrailer Connected:\n" +
                                 $"\t\t\t{data.SpecialEventsValues.TrailerConnected}\n";
               
                common.Text = JsonConvert.SerializeObject(data.CommonValues, Formatting.Indented);
                truck.Text = JsonConvert.SerializeObject(data.TruckValues, Formatting.Indented);
                trailer.Text = JsonConvert.SerializeObject(data.TrailerValues, Formatting.Indented);
                job.Text = JsonConvert.SerializeObject(data.JobValues, Formatting.Indented);
                control.Text = JsonConvert.SerializeObject(data.ControlValues, Formatting.Indented);
                navigation.Text = JsonConvert.SerializeObject(data.NavigationValues, Formatting.Indented);
                substances.Text = JsonConvert.SerializeObject(data.Substances, Formatting.Indented);

                 
            } catch (Exception ex) {
                // ignored atm i found no proper way to shut the telemetry down and down call this anymore when this or another thing is already disposed
                Console.WriteLine("Telemetry was closed: "+ ex); 
            }
        }

        private void SCSSdkClientDemo_FormClosing(object sender, FormClosingEventArgs e) {
            Telemetry.pause(); // that line make it possible, but not every application wants to ask the user to quit, need to see if i can change that, when not use the try catch and IGNORE it (nothing changed )
            if (MessageBox.Show("Are you sure you want to quit?", "My Application", MessageBoxButtons.YesNo) ==
                DialogResult.No) {
                e.Cancel = true;
                Telemetry.resume();
                return;
            }
              
            Telemetry.Dispose();
        }
         
    }
}