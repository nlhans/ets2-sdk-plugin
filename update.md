# Update log

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
