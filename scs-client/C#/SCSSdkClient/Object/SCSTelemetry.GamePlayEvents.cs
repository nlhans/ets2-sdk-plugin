#pragma warning disable 1570

namespace SCSSdkClient.Object
{
    public partial class SCSTelemetry
    {
        /// <summary>
        ///     Gameplay Events that can be fired from the sdk (starting for game version 1.35+)
        ///     first implementation is that the values will be set, but never deleted
        ///     another change listener for bool values that will be changed will say if there are new values
        /// </summary>
        public class GamePlayEvents {


            public GamePlayEvents() {
                JobCancelled = new Cancelled();
                JobDelivered = new Delivered();
                FinedEvent = new Fined();
                TollgateEvent = new Tollgate();
                TrainEvent = new Transport();
                FerryEvent = new Transport();
            }
            public Cancelled JobCancelled;
            public Delivered JobDelivered;
            public Fined FinedEvent;
            public Tollgate TollgateEvent;
            public Transport FerryEvent;
            public Transport TrainEvent;



            public class Cancelled {
                public long Penalty;
            }

            public class Delivered {
                public Delivered() {
                    DeliveryTime = new Time();
                }
                public long Revenue;
                public int EarnedXp;
                public float CargoCamage;
                public float DistanceKm;
                public Time DeliveryTime;
                public bool AutoParked;
                public bool AutoLoaded;
            }

            public class Fined {
                public Offence Offence;
                public long Amount;
            }

            public class Tollgate {
                public long PayAmount;
            }

            public class Transport {
                public long PayAmount;
                public string SourceName;
                public string TargetName;
                public string SourceId;
                public string TargetId;
            }

        }
    }
}