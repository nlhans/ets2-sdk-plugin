# Euro Truck Simulator 2 Telemetry

[![Join the chat at https://gitter.im/nlhans/ets2-sdk-plugin](https://badges.gitter.im/nlhans/ets2-sdk-plugin.svg)](https://gitter.im/nlhans/ets2-sdk-plugin?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

SCS has kindly released a SDK that allows developers and users to stream telemetry data from the game to any 3rd party applications. An example program was provided (and often used) which enabled streaming data by using text files stored on the users harddisk. This puts unnecessary stress on the users harddrive (not the mention the number of re-writes that would hurt SSDs), and moreover requires the user to manually configure the telemetry data source.

This SDK plug-in transports the telemetry stream via a Memory Mapped File. This is a special Windows (file)stream which resides completely in RAM and can be read from multiple applications. 

## Installation

Grab the latest release from the Github Release page: https://github.com/nlhans/ets2-sdk-plugin/releases 

Installation is easy inside Euro Truck Simulator 2. Place the acquired DLL inside bin/win_x86/plugins/ of your ETS2 installation. It is possible the plugins directory doesn't exists yet (as with every default installation). In that case you need to create the plugins folder. Place the DLL inside the plugins folder. 

You will now notice that each time ETS2 now starts it prompts the SDK has been activated. Unfortunately you have to press OK to this message every time, but it's a small price to pay for the added features that are possible via the SDK. 

## Developers Information

### Overview
This plug-in stores it's data inside a Memory Mapped File, or "Shared Memory". This allows it to operate without any access to harddrive, or configuration hassle by the user to locate the memory map.

The following telemetry fields are supported:

 * Truck model name & ID
 * Trailer model name & ID
 
 * Pause indicator
 * In game timestamp
 
 * Engine Running flag
 * Trailer Attached status
 
 * Engine RPM (float; rpm)
 * Maximum Engine RPM (float)
 * Speed (float; m/s)
 * Gear (-1 = R, 0 = N, 1=1st, etc.) (signed integer)
 * No. of forward and reverse gears
 * Fuel (float; litres)
 * Fuel capacity
 
 * Acceleration
 * Rotation
 * Coordinate (X/Y & Height)
 
 * User inputs
 * Game inputs (e.g. with in-game cruise control).
 
 * In-game cruise control read out
 * Various other truck readouts like:
    - air pressure
    - brake temperature
    - truck odometer
    - water temperature
	- wipers
	- damage info
    - indicators
	- light information (low/high beam, beacon)
	- brake information (e.g. parking brake or retarder)
 
 * Job information including:
	- City Source
	- City Destination
	- Cargo company Source 
	- Cargo company Destinatoin
	- Cargo model ID
	- Cargo name
	- Cargo weight (kg)
	All strings are encoded in UTF8 instead of ASCII.
 
Other fields may require testing or implementing:
 
 * Gear range (requires storage of gear selector map)
 * Fuel average consumption (as by on-board truck computer)
 
The fields are updated as fast as ETS2 can and will do, as this is how the SDK has been designed by SCS. When a telemetry value has changed the SDK will immediately call a handler. This plug-in implements this handler which stores the data to the right field inside the data structure.
There is no "sample ticker" yet. This must be done at the client side, by regularly checking if the timestamp has been updated.

## Client Implementations

### C Sharp / .NET
A C# 4.0 demo application is provided inside the ets2-client folder. The demo application includes a Ets2SdkClient DLL and a Windows GUI application for viewing the output of the DLL. 

### Other
For other languages you need to create/find a library that can open and read MemoryMapped files. The data storage format is binary and can be found in "ets2-telemetry/inc/ets2-telemetry-common.hpp". The shared memory map name is "Local\SimTelemetryETS2".
