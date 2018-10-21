fork of [nlhans](https://github.com/nlhans/ets2-sdk-plugin) work

.dll and c# object is complete new and won't work with old code. Why change? It at a lot missing values.
# SCS Telemetry for EuroTruckSimulator 2 and AmericanTruckSimulator

SCS has kindly released a SDK that allows developers and users to stream telemetry data from the game to any 3rd party applications. An example program was provided (and often used) which enabled streaming data by using text files stored on the users harddisk. This puts unnecessary stress on the users harddrive (not the mention the number of re-writes that would hurt SSDs), and moreover requires the user to manually configure the telemetry data source.

This SDK plug-in transports the telemetry stream via a Memory Mapped File. This is a special Windows (file)stream which resides completely in RAM and can be read from multiple applications. 

## Installation

Actually you need to build this branch yourself. I will add a release later. Stay tuned for more information.

Installation is easy inside Euro Truck Simulator 2. Place the acquired DLL inside bin/win_x64/plugins/ of your ETS2/ATS installation. It is possible the plugins directory doesn't exists yet (as with every default installation). In that case you need to create the plugins folder. Place the DLL inside the plugins folder. 

You will now notice that each time ETS2/ATS now starts it prompts the SDK has been activated. Unfortunately you have to press OK to this message every time, but it's a small price to pay for the added features that are possible via the SDK. 

## Developers Information

### Documentation
I created a documentation. It tells you a lot about the values. I will also add there a installation, build, etc. guide to make it simpler for all to use this sdk and may change a bit on your on.

But at the moment only C# code/value documentation exists.

Thats also why there are (if i wouldn't use '#pragma') a lot `warnings` while compiling the c# part. In the documentation i use `<` and `>`. But that's not valid xml and the compiler create this warning. So only a warning we can ignore.

### Overview 
This plug-in stores it's data inside a Memory Mapped File, or "Shared Memory". This allows it to operate without any access to harddrive, or configuration hassle by the user to locate the memory map.

The following telemetry fields are supported, structure like the c# object:

	Basic Game Independent Values:
		- Telemetry Timestamp (not the in-game time, only for usage in code, see documentation for more information #todo add link)
		- Paused, game state
		- SCSGame identifier as enum
		- GameVersion and Game Telemetry Version (major.minor)
		- Dll version (usage in code)

		Common Values:
			- Scale
			- Game Time (Time object with in-game minutes and datetime object)
			- NextRestStop (Frequency object, more a time span)
			- NextRestStopTime (Specific date, calculated)

		Truck Values (Contains 2 big parts, and a small one):
			Constants/Configs (Values that barely change):
				Motor Values:
					- Forward Gear Count
					- Reverse Gear Count
					- Retarder Step Count
					- Selector Count
					- Engine RPM Max
					- Differential Ration
					- Gear Ratios Forward
					- Gear Ratios Reverse
					- Shifter Type Value (Enum)

				Capacity Values:
					- Fuel
					- Adblue

				Warning Factors:
					- Fuel
					- Adblue
					- Air Pressure
					- Air Pressure Emergency
					- Oil Pressure
					- Water Temperature
					- Battery Voltage

				Wheels:
					- Count
					- Radius
					- Simulated
					- Powered
					- Liftable
					- Steerable

				- Brand Id (code)
				- Brand
				- Id (code)
				- Name


			Current Values (Values that change alot):
				-
			
			Positioning:
				-

		Trailer Values:
			-
		
		Job Values:
			-

		Control Values:
			-

		Navigation Values:
			-
		
		SpecialEvents:
			-


Also there are a few more fields you can use:




The fields are updated as fast as ETS2/ATS can and will do, as this is how the SDK has been designed by SCS. When a telemetry value has changed the SDK will immediately call a handler. This plug-in implements this handler which stores the data to the right field inside the data structure.
There is no "sample ticker" yet. This must be done at the client side, by regularly checking if the timestamp has been updated.

## Client Implementations



### Other
For other languages you need to create/find a library that can open and read MemoryMapped files. The data storage format is binary and can be found in "ets2-telemetry/inc/ets2-telemetry-common.hpp". The shared memory map name is "Local\SCSTelemetry".
