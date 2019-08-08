using System.Collections.Generic;

#pragma warning disable 1570

namespace SCSSdkClient.Object {
    /// <summary>
    ///     Telemetry Data of SCS SDK's
    /// </summary>
    //
    public partial class SCSTelemetry {
        private const float PiTimes2 = 6.2831853071795864769252867665590058f;

        /// <summary>
        ///     initialise an SCSTelemetry object
        /// </summary>
        public SCSTelemetry() {
            GameVersion = new Version();
            TelemetryVersion = new Version();
            TruckValues = new Truck();
            JobValues = new Job();
            CommonValues = new Common();
            //TrailerValues = new Trailer();
            ControlValues = new Control();
            NavigationValues = new Navigation();
            SpecialEventsValues = new SpecialEvents();
            Substances = new List<Substance>();
            GamePlay = new GamePlayEvents();
        }


        /// About: Currency
        ///  
        /// - ATS use US Dollars as internal currency
        /// - ETS2 use Euro as internal currency
        ///  
        /// About: GameVersion
        /// Does *NOT* match the patch level of the game
        /// 
        /// About: Temperatures
        /// Aproximated for entire truck, not at the wheel level.
      
        /// <summary>
        ///     Is the sdk active (only reset when game is closed correct, crashes or kills won't set this value)
        /// </summary> 
        public bool SdkActive { get; internal set; }

        /// <summary>
        ///     Timestamp not the in game time,
        /// </summary>
        /// <seealso cref="Common.GameTime" />
        public uint Timestamp { get; internal set; }

        /// <summary>
        ///     Is the game actual paused? (Menu, F1, Map, etc. open)
        /// </summary>
        /// <example>
        ///     Driving
        ///     <code>
        /// Paused == true
        /// </code>
        ///     Menu/Map
        ///     <code>
        /// Paused == false
        /// </code>
        /// </example>
        public bool Paused { get; internal set; }

        /// <summary>
        ///     Which game data we collect?
        /// </summary>
        /// <example>
        ///     Running game is EuroTruckSimulator2
        ///     <code>
        /// Game == SCSGame.Ets2
        /// </code>
        ///     Running game is AmericanTruckSimulator
        ///     <code>
        /// Game == SCSGame.Ats
        /// </code>
        /// </example>
        /// <seealso cref="SCSGame" />
        public SCSGame Game { get; internal set; }

        /// <summary>
        ///     Version of the game for purpose of the specific api which is being initialized.
        /// </summary>
        /// <!----> **INFORMATION** <!---->
        /// Does NOT match the patch level of the game.
        /// <!----> **INFORMATION** <!---->
        public Version GameVersion { get; internal set; }


        /// <summary>
        ///     Version/Revision of the dll
        /// </summary>
        /// <value>
        ///     actually it is the revision number
        /// </value>
        public uint DllVersion { get; internal set; }

        /// <summary>
        ///     Version of the Game Telemetry
        /// </summary>
        public Version TelemetryVersion { get; internal set; }

        /// <summary>
        ///     Contains "common" values -> scale, gameTime and reststop
        /// </summary>
        public Common CommonValues { get; internal set; }

        /// <summary>
        ///     Contains values of the truck
        /// </summary>
        public Truck TruckValues { get; internal set; }

        /// <summary>
        ///     Contains values of the Trailers
        /// </summary>
        public Trailer[] TrailerValues { get; internal set; }

        /// <summary>
        ///     Contains values of the actual job
        /// </summary>
        public Job JobValues { get; internal set; }

        /// <summary>
        ///     Contains values about the user and game control
        /// </summary>
        public Control ControlValues { get; internal set; }

        /// <summary>
        ///     Navigation values
        /// </summary>
        public Navigation NavigationValues { get; internal set; }

        /// <summary>
        ///     Contains special Event Values like onJob and JobFinished
        /// </summary>
        public SpecialEvents SpecialEventsValues { get; internal set; }

        /// <summary>
        ///     Contains string values to the substances used in values like Truck.Current.Wheels.Substance
        /// </summary>
        public List<Substance> Substances { get; internal set; }

        public uint MaxTrailerCount { get; internal set; }

        public GamePlayEvents GamePlay { get; internal set; }
    }
}