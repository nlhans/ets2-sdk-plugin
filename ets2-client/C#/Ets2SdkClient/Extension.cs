using System;

namespace Ets2SdkClient {
    public static class Extension {
        public static T ToEnum<T>(this uint enumInt) {
            return (T)Enum.ToObject(typeof(T), enumInt);
        }
    }
}