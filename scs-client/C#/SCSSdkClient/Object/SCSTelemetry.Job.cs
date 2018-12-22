#pragma warning disable 1570

namespace SCSSdkClient.Object {
    public partial class SCSTelemetry {
        /// <summary>
        ///     Job values. Income, destination, source, etc.
        /// </summary>
        public class Job {
            public Job()
            {
                DeliveryTime = new Time();
                RemainingDeliveryTime = new Frequency();
                CargoValues = new Cargo();
            }

            public Time DeliveryTime { get; internal set; }

            /// <summary>
            ///     Remaining in-game time until the job delivery window close
            /// </summary>
            /// <!----> **INFORMATION** <!---->
            /// Negative if the delivery is to late
            /// <!----> **INFORMATION** <!---->
            public Frequency RemainingDeliveryTime { get; protected internal set; }
            public Cargo CargoValues { get; internal set; }
            public string CityDestinationId { get; internal set; }
            public string CityDestination { get; internal set; }
            public string CompanyDestinationId { get; internal set; }
            public string CompanyDestination { get; internal set; }

            public string CitySourceId { get; internal set; }
            public string CitySource { get; internal set; }
            public string CompanySourceId { get; internal set; }
            public string CompanySource { get; internal set; }

            /// <summary>
            ///     Reward in internal game-specific currency.
            /// </summary>
            public long Income { get; internal set; }
            /// <summary>
            ///     Cargo Values
            /// </summary>
            public class Cargo
            {
                /// <summary>
                ///     Mass in kilograms
                /// </summary>
                public float Mass { get; internal set; }

                /// <summary>
                ///     Name for internal use by code.
                ///     Limited to C-identifier characters and dots.
                /// </summary>
                public string Id { get; internal set; }

                /// <summary>
                ///     Name for display purposes.
                ///     Localized using the current in-game language.
                /// </summary>
                public string Name { get; internal set; }

                /// <summary>
                ///     Name of cargo accessory for internal use by code.
                ///     Limited to C-identifier characters and dots.
                /// </summary>
                public string AccessoryId { get; internal set; }


            }


        }
    }
}