#pragma warning disable 1570

namespace SCSSdkClient.Object {
    public partial class SCSTelemetry {
        /// <summary>
        ///     Special Events the plugin deliver like onJob and JobFinished
        /// </summary>
        public class SpecialEvents {
            /// <summary>
            ///     Event flag that displays if a driver is doing a job
            /// </summary>
            /// 
            public bool OnJob { get; internal set; }

            /// <summary>
            ///     Event flag that displays (for some ticks) that a job was finished, doesn't matter how (cancelled, delivered, ...)
            ///      Deprecated for Version 1.35 of both games. Use JobCancelled and JobDelivered Event for version starting with 1.35+
            ///     But should still work
            /// </summary>
            /// 
            public bool JobFinished { get; internal set; }
            /// <summary>
            ///     Event flag that displays if the game says there is a trailer that is connected with us (so attached, detached doesn't matter for this flag).
            ///
            ///     The event flag is true for:
            ///     <list type="bullet">
            ///         <item><description>Trailer is attached</description></item>
            ///         <item><description>Trailer is detached, but you're on a job and doesn't cancel it</description></item>
            ///         <item><description>Trailer is detached, but it is your trailer and doesn't bring it back</description></item>
            ///     </list>
            ///    The event flag is false for:
            ///     <list type="bullet">
            ///         <item><description>You end/cancel a job and doesn't own the trailer</description></item>
            ///         <item><description>Bring your own trailer back, after it detached the flag should be false</description></item>
            ///     </list>
            /// </summary>
            public bool TrailerConnected { get; internal set; }

            public bool JobCancelled { get; internal set; }
            public bool JobDelivered { get; internal set; }
            public bool Fined { get; internal set; }
            public bool Tollgate { get; internal set; }
            public bool Ferry { get; internal set; }
            public bool Train { get; internal set; }
        }
    }
}