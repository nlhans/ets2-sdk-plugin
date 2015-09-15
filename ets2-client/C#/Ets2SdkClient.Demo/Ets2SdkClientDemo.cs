using System;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Ets2SdkClient.Demo
{
    public partial class Ets2SdkClientDemo : Form
    {
        public Ets2SdkTelemetry Telemetry;

        public Ets2SdkClientDemo()
        {
            InitializeComponent();

            Telemetry = new Ets2SdkTelemetry();
            Telemetry.Data += Telemetry_Data;

            Telemetry.JobFinished += TelemetryOnJobFinished;
            Telemetry.JobStarted += TelemetryOnJobStarted;

            if (Telemetry.Error != null)
            {
                lbGeneral.Text =
                    "General info:\r\nFailed to open memory map " + Telemetry.Map +
                        " - on some systems you need to run the client (this app) with elevated permissions, because e.g. you're running Steam/ETS2 with elevated permissions as well. .NET reported the following Exception:\r\n" +
                        Telemetry.Error.Message + "\r\n\r\nStacktrace:\r\n" + Telemetry.Error.StackTrace;
            }
        }

        private void TelemetryOnJobFinished(object sender, EventArgs args)
        {
            MessageBox.Show("Job finished, or at least unloaded nearby cargo destination.");
        }

        private void TelemetryOnJobStarted(object sender, EventArgs e)
        {
            MessageBox.Show("Just started job OR loaded game with active.");
        }

        private void Telemetry_Data(Ets2Telemetry data, bool updated)
        {
            try
            {
                if (this.InvokeRequired)
                {
                    this.Invoke(new TelemetryData(Telemetry_Data), new object[2] { data, updated });
                    return;
                }

                lbGeneral.Text = "General info:\r\n SDK Version: " + data.Version.SdkPlugin + "\r\n Reported game Version: " +
                                 data.Version.Ets2Major + "." + data.Version.Ets2Minor + "\r\n\r\nTruck: " + data.Truck + " (" + data.TruckId + ")\r\nManufacturer: " + data.Manufacturer + "(" + data.ManufacturerId + ")" +
                                 "\r\nGame Timestamp: " + data.Time + "\r\nPaused? " + data.Paused;

                // Do some magic trickery to display ALL info:
                var grps = new object[]
                       {
                           data.Drivetrain, data.Physics, data.Controls, data.Axilliary, data.Damage, data.Lights, data.Job
                       };

                foreach (var grp in grps)
                {
                    // Find the right tab page:
                    var grpName = grp.GetType().Name;
                    if (grpName.StartsWith("_"))
                        grpName = grpName.Substring(1);

                    var tabPage = default(TabPage);
                    var tabFound = false;

                    for (int k = 0; k < telemetryInfo.TabCount; k++)
                    {
                        if (telemetryInfo.TabPages[k].Text == grpName)
                        {
                            tabPage = telemetryInfo.TabPages[k];
                            tabFound = true;
                        }
                    }
                    if (!tabFound)
                    {
                        tabPage = new CustomTabPage(grpName);
                        telemetryInfo.TabPages.Add(tabPage);
                    }

                    // All properties;
                    var props = grp.GetType().GetProperties().OrderBy(x => x.Name);
                    var labels = new StringBuilder();
                    var vals = new StringBuilder();
                    foreach (var prop in props)
                    {
                        labels.AppendLine(prop.Name + ":");
                        object val = prop.GetValue(grp, null);
                        if (val is float[])
                        {
                            vals.AppendLine(string.Join(", ", (val as float[]).Select(x=> x.ToString("0.000"))));
                        }
                        else
                        {
                            vals.AppendLine(val.ToString());
                        }
                    }

                    tabPage.Controls.Clear();
                    var lbl1 = new Label { Location = new Point(3, 3), Size = new Size(200, tabPage.Height - 6) };
                    var lbl2 = new Label { Location = new Point(203, 3), Size = new Size(1000, tabPage.Height - 6) };
                    lbl1.Text = labels.ToString();
                    lbl2.Text = vals.ToString();
                    lbl2.AutoSize = false;
                    tabPage.Controls.Add(lbl1);
                    tabPage.Controls.Add(lbl2);
                }
            }
            catch
            {
            }
        }
    }
}