#pragma warning disable 1570

namespace SCSSdkClient.Object {
    public partial class SCSTelemetry {
        /// <summary>
        ///     Gameplay Events that can be fired from the sdk (starting for game version 1.35+)
        ///     first implementation is that the values will be set, but never deleted
        ///     another change listener for bool values that will be changed will say if there are new values
        /// </summary>
        public class GamePlayEvents {
            public Transport FerryEvent;
            public Fined FinedEvent;
            public Cancelled JobCancelled;
            public Delivered JobDelivered;
            public Tollgate TollgateEvent;
            public Transport TrainEvent;


            public GamePlayEvents() {
                JobCancelled = new Cancelled();
                JobDelivered = new Delivered();
                FinedEvent = new Fined();
                TollgateEvent = new Tollgate();
                TrainEvent = new Transport();
                FerryEvent = new Transport();
            }


            public class Cancelled {
                public long Penalty;
            }

            public class Delivered {
                public bool AutoLoaded;
                public bool AutoParked;
                public float CargoDamage;  // Typo fixed thanks to Patrick-van-Halm https://github.com/RenCloud/scs-sdk-plugin/pull/32
                public Time DeliveryTime;
                public float DistanceKm;
                public int EarnedXp;
                public long Revenue;

                public Delivered() => DeliveryTime = new Time();
            }

            public class Fined {
                public long Amount;
                public Offence Offence;
            }

            public class Tollgate {
                public long PayAmount;
            }

            public class Transport {
                public long PayAmount;
                public string SourceId;
                public string SourceName;
                public string TargetId;
                public string TargetName;
            }
        }
    }
}