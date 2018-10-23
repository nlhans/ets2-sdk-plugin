#pragma warning disable 1570

namespace SCSSdkClient.Object {
    public partial class SCSTelemetry {
        /// <summary>
        ///     Represents a Double Placement simular to the one in the SDK with coordinate X,Y,Z and rotation X,Y,Z
        /// </summary>
        public class DPlacement {
            public DVector Position { get; internal set; }
            public Euler Orientation { get; internal set; }
        }
    }
}