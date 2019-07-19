namespace SCSSdkClient {
    /// <summary>
    ///     Offence type
    /// </summary>
    public enum Offence {
        ///About: Values
        ///
        /// NoValue         - No Value from the sdk
        /// Crash           - Crash with another vehicle
        /// Avoid_sleeping  - driver did not sleep
        /// Wrong_way       - drive on the wrong side of the street
        /// Speeding_camera - drives to fast at a camera
        /// No_lights       - drives without lights on
        /// Red_signal      - ignores a red signal
        /// Avoid-Weighting - ignore weighting
        /// Speeding        - drives to fast
        /// Illegal_trailer - carries a trailer that is not allowed in this area
        /// Generic         - some other generic fine



        /// <summary>
        ///     No Value from the sdk
        /// </summary>
        NoValue,

        /// <summary>
        ///     Crash with another vehicle
        /// </summary>
        Crash,

        /// <summary>
        ///     driver did not sleep
        /// </summary>
        Avoid_sleeping,

        /// <summary>
        ///     drive on the wrong side of the street
        /// </summary>
        Wrong_way,

        /// <summary>
        ///     drives to fast at a camera
        /// </summary>
        Speeding_camera,

        /// <summary>
        ///     drives without lights on
        /// </summary>
        No_lights,

        /// <summary>
        ///     ignores a red signal
        /// </summary>
        Red_signal,

        /// <summary>
        ///     ignore weighting
        /// </summary>
        Avoid_weighting,

        /// <summary>
        ///     drives to fast
        /// </summary>
        Speeding,

        /// <summary>
        ///     carries a trailer that is not allowed in this area
        /// </summary>
        Illegal_trailer,

        /// <summary>
        ///     some other generic fine
        /// </summary>
        Generic
    }
}