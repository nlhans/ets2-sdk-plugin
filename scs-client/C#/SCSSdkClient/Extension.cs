using System;
using System.Runtime.CompilerServices;

namespace SCSSdkClient {
    public static class Extension {
        public static T ToEnum<T>(this uint enumInt) {
            return (T)Enum.ToObject(typeof(T), enumInt);
        }
        public static T ToEnum<T>(this string enumString)
        {
            return (T)Enum.Parse(typeof(T), enumString,ignoreCase:true);
        }

        public static string StringFormater(this string choob) {
            var result = "";
            for (var index = 0; index < choob.Length; index++) {
                var cha = choob[index];
                result += cha;
                if (cha != '\n') {
                    continue;
                }
                
                
                while (cha=='\n'||cha=='\t') {
                    index++;
                    cha = choob[index];
                    if (cha == '\n' || cha == '\t') {
                        result += cha;
                    } else {
                        result += '\t';
                        result += cha;
                    }
                   
                }

                
            }

            return result;
        }
    }
}