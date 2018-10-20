#pragma warning disable 1570

namespace Ets2SdkClient.Object {
    public partial class SCSTelemetry {
        /// <summary>
        ///     Trailer Values
        /// </summary>
        public class Trailer {
            /// <summary>
            ///     Is the trailer connected to the truck
            /// </summary>
            public bool Attached { get; internal set; }

            /// <summary>
            ///     Wear of the chassis accessory as &lt;0;1&gt;
            /// </summary>
            public float Damage { get; internal set; }

            public Wheels Wheelvalues { get; internal set; }
            public Cargo CargoValues { get; internal set; }
            public Acceleration AccelerationValues { get; internal set; }

            /// <summary>
            ///     Name for internal use by code.
            ///     Limited to C-identifier characters and dots.
            /// </summary>
            public string Chassis { get; internal set; }

            /// <summary>
            ///     Id of the cargo for internal use by code.
            ///     Limited to C-identifier characters and dots
            /// </summary>
            public string Id { get; internal set; }

            /// <summary>
            ///     Name of the cargo for display purposes.
            ///     Localized using the current in-game language.
            /// </summary>
            public string Name { get; internal set; }

            /// <summary>
            ///     States of the Wheels
            /// </summary>
            public class Wheels {
                /// About: Velocity
                /// Positive velocity corresponds to forward movement
                /// 
                /// About: Steering
                /// Value is from <0.25,0.25> range in counterclockwise direction
                /// when looking from top (e.g. 0.25 corresponds to left and -0.25 corresponds to right)
                /// 
                /// Set to zero for non-steered wheels
                /// 
                /// About: Rotation
                /// Value is from
                /// <0.0,1.0) range in which value increase corresponds to forward movement
                ///     About: Lift
                ///     For use with simple lifted/ non-lifted test or logical visualization of the lifting progress.
                ///  
                ///     - Value of 0 corresponds to non-lifted axle.
                ///     - Value of 1 corresponds to fully lifted axle.
                ///     Set to zero or not provided for non-liftable axles.
                ///     About: LiftOffset
                ///     Might have non-linear relation to lift ratio.
                ///     Set to zero or not provided for non-liftable axles.
                
                /// <summary>
                ///     Substance below the wheel
                /// </summary>
                public uint[] Substance { get; internal set; }

                /// <summary>
                ///     Vertical displacement of the wheel from its axis in meters
                /// </summary>
                public float[] SuspDeflection { get; internal set; }

                /// <summary>
                ///     Angular velocity of the wheel in rotations per second
                /// </summary>
                /// <!----> **INFORMATION** <!---->
                /// Positive velocity corresponds to forward movement
                /// <!----> **INFORMATION** <!---->
                public float[] Velocity { get; internal set; }

                /// <summary>
                ///     Steering rotation of the wheel in rotations
                /// </summary>
                /// <!----> **INFORMATION** <!---->
                /// Value is from &lt;0.25,0.25&gt; range in counterclockwise direction when looking from top (e.g. 0.25 corresponds to left and -0.25 corresponds to right)
                /// 
                /// Set to zero for non-steered wheels
                /// <!----> **INFORMATION** <!---->
                public float[] Steering { get; internal set; }

                /// <summary>
                ///     Rolling rotation of the wheel in rotations
                /// </summary>
                /// <!----> **INFORMATION** <!---->
                /// Value is from &lt;0.0,1.0) range in which value increase corresponds to forward movement
                /// <!----> **INFORMATION** <!---->
                public float[] Rotation { get; internal set; }

                /// <summary>
                ///     Is the wheel in contact with ground?
                /// </summary>
                public bool[] OnGround { get; internal set; }
            }

            /// <summary>
            ///     Cargo Values
            /// </summary>
            public class Cargo {
                /// <summary>
                ///     Mass in kilograms
                /// </summary>
                public float Mass { get; internal set; }

                /// <summary>
                ///     Name of cargo accessory for internal use by code.
                ///     Limited to C-identifier characters and dots.
                /// </summary>
                public string Name { get; internal set; }
            }

            public class Acceleration {
                /// <summary>
                ///     Represents vehicle space linear velocity of the trailer measured in m/s
                /// </summary>
                public FVector LinearVelocity { get; internal set; }

                /// <summary>
                ///     Represents vehicle space angular velocity of the trailer measured in rotation/s
                /// </summary>
                public FVector AngularVelocity { get; internal set; }

                /// <summary>
                ///     Represents vehicle space linear acceleration of the trailer measured in m/s^2
                /// </summary>
                public FVector LinearAcceleration { get; internal set; }

                /// <summary>
                ///     Represents vehicle space angular acceleration of the trailer measured in rotations/s^2
                /// </summary>
                public FVector AngularAcceleration { get; internal set; }
            }
        }
    }
}