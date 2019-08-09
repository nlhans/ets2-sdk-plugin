# Update log
## Rev 10 Update 1
- change `DefaultUpdateInterval` from 25ms to 100ms of the Shared Memory File refresh timer to avoid crashed until i (or someone) have a better solution (see issue #38)
- added value `SdkActive` that could create problems if not update both parts of the project. It's only a byte/bool but on the first place in the shared memory -> the update function won't work when not updated both without small fix (offset + 1, 2 or 3 at the begin)
- c# library will going in `paused` state when `SdkActive == false`. Means that the update rate of the shared memory is now 1 second and not higher, get back to high update rate when sdk is active again
- values are now reset when the game is closed correct (see #39). Crashes or kills of the game won't reset the values
- c# demo now has a status label that shows the current update rate of the sdk 



## Rev 10 (Game Version 1.35)
A lot changes here: update to SDK Version 1.10
- shared memory files size increased (16kb to 32kb), because it can handle up to 10 trailers... that needs already a lot of space
- structure of the shared memory file changed a lot through new order and new attributes
- implement gameplay events from the api
- added versions support (game sdk version) eg. for ets2 it is actually 1.14, but 1.13 is still working. With comming updates 1.13 will may deleted, because it could leed to errors or to many code. So should work for 1.27 up to 1.35+ for now. Possible also lower versions. 
- mostly added the new cool stuff of the sdk for now, more (e.g. linux support, logging, ...) will come in a later update
- find and fixes a few bugs will creating new content (hopefully no new bugs)
- notice: job canceled ~and tollgate~(tollgate works, but only if you pay and not your customer) seems not to work atm  
- demo was updated but it's not working good with the 10 trailer updated. Avoid to open tje trailer page or you need to restart the demo. It will freeze the ui. (To high update rate of that mutch text, when game is paused ui will also stop freezing) 
- new events to register (tollgate, train, ferry, fine, job delivered,... ) and some removed (trailer connected, disconnected) but you could create your own connected/disconnected listener if you need them again
- a lot of other small thinks. i will start early to write update notes the next time (hopefuly)
- some documentation for new functions, values etc.   
