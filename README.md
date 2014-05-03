#Euro Truck Simulator 2 Telemetry

SCS has kindly released a SDK that allows developers and users to stream telemetry data from the game to any 3rd party applications. An example program was provided (and often used) which enabled streaming data by using text files stored on the users harddisk. This puts unnecessary stress on the users harddrive (not the mention the number of re-writes that would hurt SSDs), and moreover requires the user to manually configure the telemetry data source.

This SDK plug-in transports the telemetry stream via a Memory Mapped File. This is a special Windows (file)stream which resides completely in RAM and can be read from multiple applications. 

##Installation

Installation is easy inside Euro Truck Simulator 2. Place the acquired DLL inside bin/win_x86/plugins/ of your ETS2 installation. It is possible the plugins directory doesn't exists yet (as with every default installation). In that case you need to create the plugins folder. Place the DLL inside the plugins folder. 

At this moment the plug-in has not reached it's final release state, and as of yet there are no release builds found. To try out the plug-in you will need to compile it yourself using Visual Studio. 

You will now notice that each time ETS2 now starts it prompts the SDK has been activated. Unfortunately you have to press OK to this message every time, but it's a small price to pay for the added features that are possible via the SDK. 

##Developers Information

###Overview
This plug-in stores it's data inside a Memory Mapped File, or "Shared Memory". It reserves a file inside memory, and overwrites the current telemetry to this piece of memory. The values are stored in a binary fashion.

The exact binary layout of the data can be found inside "ets2-telemetry/inc/ets2-telemetry-common.hpp". The data structure ets2TelemetryMap_t contains all the fields that are planned to be supported in revision 1. Additional revisions will be added after the revision 1 struct, where it is remained intact. This should allow for maximum backwards compatibility.

Currently when writing this readme document, the DLL is in an alpha stage (revision 0). Some fields are still being tested and some are not implemented yet. 

The following telemetry fields, however, are fully functional:

 * Truck model ID
 * Cargo model ID
 
 * Pause indication
 * Timestamp
 
 * Engine RPM (float; rpm)
 * Maximum Engine RPM (float)
 * Speed (float; m/s)
 * Gear (-1 = R, 0 = N, 1=1st, etc.) (signed integer)
 * No. of gears	(int)
 * Fuel (float; litres)
 * Fuel capacity
 
 * Acceleration
 * Rotation
 * Coordinate (X/Y & Height)
 
 * User Driving inputs
 * Effective game inputs (in-game cruise control).
 
The following fields are not completely functional due to SDK problems:

 * Engine Running flag (SDK "engine enabled" is bugged, "electric enabled" works, but is only valid with automatic engine/electric start option turned ON)
 * Trailer Attached status (SDK always reports True when on mission, in free roam it's False)
 
Other fields may require testing or implementing:
 
 * Truck & trailer weight (not available from SDK)
 * Gear range (requires storage of gear selector map)
 * Fuel average consumption (as by on-board truck computer)
 
The fields are updated as fast as ETS2 can and will do, as this is how the SDK has been designed by SCS. When a telemetry value has changed the SDK will immediately call a handler. This plug-in implements this handler which stores the data to the right field inside the data structure.
There is no "sample ticker" yet. This must be done at the client side, by regularly checking if the timestamp has been updated.

###Telemetry Client Implementations
A simple C# implementation can be found in one closely related project of mine; inside SimShift: https://github.com/nlhans/SimShift/tree/master/SimShift/SimShift/Data

For other languages you may need to create/find a library that can open and read MemoryMapped files. As found in "ets2-telemetry/inc/ets2-telemetry-common.hpp", the low-level Windows path is "Local\SimTelemetryETS2". The data structure of the file can also be found in this header file.
