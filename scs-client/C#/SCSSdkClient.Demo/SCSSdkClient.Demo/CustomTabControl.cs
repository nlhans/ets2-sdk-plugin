// 
// Ets2SdkClient.Demo
// CustomTabControl.cs
// 10:52
using System.Windows.Forms;

namespace SCSSdkClient.Demo {
    public class CustomTabPage : TabPage
    {
        public CustomTabPage(string txt) : base(txt)
        {
            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
        }
        public CustomTabPage()
        {
            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
        }
    }
}