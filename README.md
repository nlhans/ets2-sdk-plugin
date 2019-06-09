
  <a href="https://rencloud.github.io/scs-sdk-plugin/docs/" title="Documentation">
    <img alt="" src="https://img.shields.io/badge/documentation-09.05-green.svg?style=for-the-badge" />
  </a>
 
  <a href="https://discord.gg/JDqkZZd" title="Discord">
    <img alt="" src="https://img.shields.io/badge/Discord-blue.svg?style=for-the-badge" />
  </a>

 
fork of [nlhans](https://github.com/nlhans/ets2-sdk-plugin) work

# **1.35/SDK10**


**New Revision -> not compatible with old files. SM and C# object changed a lot, because of a lot new values. see update.md for more information**
 
# SCS Telemetry for EuroTruckSimulator 2 and AmericanTruckSimulator

SCS has kindly released a SDK that allows developers and users to stream telemetry data from the game to any 3rd party applications. An example program was provided (and often used) which enabled streaming data by using text files stored on the users harddisk. This puts unnecessary stress on the users harddrive (not the mention the number of re-writes that would hurt SSDs), and moreover requires the user to manually configure the telemetry data source.

This SDK plug-in transports the telemetry stream via a Memory Mapped File. This is a special Windows (file)stream which resides completely in RAM and can be read from multiple applications. 


## Installation

Actually you need to build this branch yourself. I will add a release later. Stay tuned for more information.

Installation is easy inside Euro Truck Simulator 2. Place the acquired DLL inside bin/win_x64/plugins/ of your ETS2/ATS installation. It is possible the plugins directory doesn't exists yet (as with every default installation). In that case you need to create the plugins folder. Place the DLL inside the plugins folder. 

You will now notice that each time ETS2/ATS now starts it prompts the SDK has been activated. Unfortunately you have to press OK to this message every time, but it's a small price to pay for the added features that are possible via the SDK. 

## Developers Information

### Documentation (not completed at the moment)
There is also an Documentation. It tells a lot about the values. An installation, build , etc. guide will also follow. Should there still be questions, feature request or other changes visit the discord server linked at the top.


Sadly the usage of the documentation generating syntax leads to a lot of `warnings`, while compiling the c# part. That's because the documentation use `<` and `>`. But that's no valid xml. Therefore the compiler cry's :cry: and give the warning about invalid xml. With the help of `#pragma` it wont show up. 

### Overview 
This plug-in stores it's data inside a Memory Mapped File, or "Shared Memory". This allows it to operate without any access to harddrive, or configuration hassle by the user to locate the memory map.

### Telemetry fields and the c# object
The following telemetry fields are supported, structure is like the c# object. Starting with sdk 1.10, game patch 1.35 and ETS2 1.14, ATS 1.01 code for some part of the need different versions of the sdk. The plugin handles this. If a game lower than 1.35 is used, only the values without (1.14/1.01) are possible:

	Basic Game Independent Values:
		- Telemetry Timestamp (not the in-game time, only for usage in code, see documentation for more information #todo add link)
		- Paused, game state
		- SCSGame identifier as enum, currently ets2/ats/unknown
		- GameVersion and Game Telemetry Version (major.minor)
		- Dll version (usage in code)
		- TelemetryVersion
		
		Substances 

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
					- SlotGear
					- SlotHandlePosition
					- SlotSelectors

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
				- LicensePlate (1.14/1.01)
				- LicensePlateCountryId (1.14/1.01)
				- LicensePlateCountry (1.14/1.01)


			Current Values (Values that change a lot):
				- Electric Enabled
				- Engine Enabled
				Motor Values:
					Gear Values:
						- HShifterSlot
						- Selected
						- HShifterSelector

					Brake Values:
						- RetarderLevel
						- Air Pressure
						- Temperature 
						- Parking Brake
						- Motor Brake					
				
				Dashboard:
					Fuel Values:
						- Amount
						- Average Consumption
						- Range (estimated range with current amount of fuel)

					Warnings:
						- Air Pressure
						- Air Pressure Emergency
						- Fuel warning
						- Adblue
						- oil pressure
						- adblue
						- water temperature
						- battery voltage

					- Gear Dashboards
					- Speed (m/s,km/h,mph)
					- CruiseControlSpeed (m/s,km/h,mph)
					- Adblue amount
					- Oil Pressure
					- Oil Temperature
					- Water Temperature
					- BatteryVoltage
					- RPM
					- Odometer
					- Wipers
					- Cruise Control ("special field", same like `CruiseControlspeed == 0`)
					  
				Acceleration:
					- Linear Velocity
					- Angular Velocity
					- Linear Acceleration
					- Angular Acceleration
					- Cabin Angular Velocity
					- Cabin Angular Acceleration					

				Lights:
					- Aux Front (enum for 3 states)
					- Aux Roof (enum for 3 states)
					- Dashboard Backlight
					- Blinker Left Active
					- Blinker Right Active
					- Blinker Left On
					- Blinker Right On
					- Parking
					- Beam Low
					- Beam High
					- Beacon
					- Brake
					- Reverse

				Wheels:
					- Substance
					- SuspDeflection
					- Velocity
					- Steering
					- Rotation
					- Lift
					- Lift Offset
					- on ground
					- position

				Damage:
					- Engine
					- Transmission
					- Cabin
					- chassis
					- wheels (avg. of all wheels)

				- Position (position in world space with position and orientation)

			
			Positioning:
				- Cabin (vehicle space)
				- Head (cabin space)
				- Hook (vehicle space)
				- Head Offset
				- Cabin Offset
				- Contains "more fields" see at the bottom of the list


		Trailer Values (will be set to 0,false, etc. if you have no trailer, while on job or with trailer ownership detached wont reset the values)[0-9] (array starting with 1.14/1.01 so 0 for 1 trailer or version lower than that):
			- Attached			 
			- Hook
			- Position
			Wheel Values:
				- Substance
				- SuspDeflection
				- Velocity
				- Steering
				- Rotation
				- On Ground
				- Lift
				- LiftOffset

			WheelsConstants:
					- Count
					- Radius
					- Simulated
					- Powered
					- Liftable
					- Steerable
			
			~~CargoValues~~(moved to job values) 
			
			Acceleration:
				- Linear Velocity
				- Angular Velocity
				- Linear Acceleration
				- Angular Acceleration

			DamageValues: 
				- Cargo (1.14/1.01)
				- Wheels
				- Chassis

			- Chassis (code)
			- Id (code)
			- Name
			- CargoAccessoryId
			- BodyType (1.14/1.01)
			- BrandId (1.14/1.01)
			- Brand (1.14/1.01)
			- Name (1.14/1.01)
			- ChainType (1.14/1.01)
			- LicensePlate (1.14/1.01)
			- LicensePlateCountryId (1.14/1.01)
			- LicensePlateCountry (1.14/1.01)
		
		Job Values(will be reset after the job finished flag is disappeared):
			- Delivery Time (time object -> in-game minutes and datetime object)
			- Remaining Delivery Time (calculated)
			- CargoLoaded (1.14/1.01)
			- SpecialJob (1.14/1.01)
			- Market (1.14/1.01)
			- City Destination Id (code)
			- City Destination
			- Company Destination Id (code)
			- Company Destination
			- City Source Id (code)
			- City Source 
			- Company Source Id (code)
			- Company Source
			- Income 
		 
		 	Cargo Values:
				- Mass 
				- Name (code)
				- Id (1.14/1.01)
				- UnitCount (1.14/1.01)
				- UnitMass (1.14/1.01)
				- CargoDamage (1.14/1.01)

		Control Values:
			User Input:
				- Steering
				- Throttle
				- Brake
				- Clutch

			Game Values:
				- Steering
				- Throttle
				- Brake
				- Clutch

		Navigation Values:
			- Navigation Distance
			- Navigation Time
			- Speed Limit
		
		SpecialEvents:
			- On Job 
			- Job Finished (flag that disappears after some time)
			- ~~TrailerConnected~~ (removed)
			- ~~TrailerDisconnected (Both trailer events use the same flag, trailerConnected, from the memory)~~ (removed)
			- Job Cancelled (1.14/1.01) (may not work atm?)
			- Job Delivered (1.14/1.01)
			- Fined (1.14/1.01)
			- Tollgate (1.14/1.01)
			- Ferry (1.14/1.01)
			- Train (1.14/1.01)
			 
	    GameplayEvents (1.14/1.01): 
  		    Cancelled:
  			    - Penalty
	        Delivered:
				- AutoLoaded
				- AutoParked
				- CargoDamage
				- DeliveryTime
				- DistanceKm
				- EarnedXp
				- Revenue
			Fined:
				- Amount
				- Offence 
		    Tollgate:
				- PayAmount
			Transport:
				- PayAmount
				- SourceId
				- SourceName
				- TargetId
				- TargedName  


Also there are a few more fields you can use:

	Truck.Positioning:
		- Head position in Cabin Space
		- Head position in Vehicle Space
		- Head position in World Space
		- Head Position in World Space 
		- Cabin Position in World Space
		- Hook Position in World Space
	Methods:
		- In-game Minutes to Date(Time)
		- Add 2 FVectors
		- Add a FVector and a DVector
		- Rotate: Rotates specified vector by specified orientation 

May I forgot something or there is a missing version information. When you found missing values or something else create an issue that would be great.

The fields are updated as fast as ETS2/ATS can and will do, as this is how the SDK has been designed by SCS. When a telemetry value has changed the SDK will immediately call a handler. This plug-in implements this handler which stores the data to the right field inside the data structure.
There is no "sample ticker" yet. This must be done at the client side, by regularly checking if the timestamp has been updated.

## Client Implementations

### C#
Actually I'm not fully happy with the actual demo. But I didn't reached my plan that works like the old one. Later I will change the current demo so that they will be a lot times better.

### Other
For other languages you need to create/find a library that can open and read MemoryMapped files. The data storage format is binary and can be found in "scs-telemetry/inc/scs-telemetry-common.hpp". The shared memory map name is "Local\SCSTelemetry". I will add some more documentary in this header later.
