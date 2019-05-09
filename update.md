# Update log

## Rev 10 (Game Version 1.35)
A lot changes here: update to SDK Version 1.10
- shared memory files size increased (16kb to 32kb)
- structure of the shared memory file changed a lot through new order and new attributes
- implement gameplay events from the api
- added versions support (game sdk version) eg. for ets2 it is actually 1.14 and 1.13 is still working. With a later update 1.13 will may deleted than there could be errors. So should work for 1.27 up to 1.35 for now. Possible also lower versions. 
- mostly added the new cool stuff of the sdk for now more (linux sup, logging, ...) will come in a later update
- find and fixes a few bugs will creating new content (hopefully no new bugs)
- notice: job canceled and tolgate seems not to work atm hopefully later 
- demo was updated but it's not working good with the 10 trailer updated so avoid open that page
- new events to register (tolgate, train, ferry, fine, job delivered,... ) and some removed (trailer connected, disconnected) but you could create your own connected/disconnected listener if you need it
- a lot of other small thinks. i will start early to write update notes the next time
- some documentation for new functions, values etc. exists already 
- note in experimental 1.35 of ets2 (more not tested atm) some events like job cancelled or tollgate does not work