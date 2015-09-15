using System;
using System.Windows.Forms;

namespace Ets2SdkClient.Demo
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Ets2SdkClientDemo());
        }
    }
}
