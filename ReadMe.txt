ThinAir v0.7.2b
---------------

WARNING :
This is only an experimental version, use it at your own risk !
There are still some bugs.

Introduction
------------
ThinAir is a tool which help you :
- create ISO image file out of a real GameCube mini-DVD,
- send ".dol" file to your GameCube via PSOLoad (included),
- stream ISO file to your GameCube.

Of course this tool should only be used for LEGAL purpose.

ThinAir server is under GNU GPL license. Full code source is available in
ThinAir archive.

Pre-requisites
--------------
- A GameCube (of course)
- A Broad Band Adapter for your GameCube
- A Mac running Mac OS X (Jaguar or Panther)
- The Phantasy Star Online game for GameCube
- A memory card for your GameCube
- At last 1.4 Go a free disc space

Your PSO should have been correctly configured for an online game
in order to use the PSO exploit (I suggest you to configure your gamecube
network with PSUL under Virtual PC - Read PSUL documentation for more information).

Installation (READ CAREFULLY)
-----------------------------
Use "Stuffit Expander" to extract ThinAir from its compressed archive file.
You can run it from this folder.

The first time you launch ThinAir, you will have to go in its menu and choose:
"Set PSOLoad permission".

Then you will be asked for your root password. This is needed only the first time because
PSOLoad is using port 53 for DNS service which is a system reserved port.

If you are not confident about giving your root password to this application, do not worry,
you could also :
- launch the terminal
- go (with the "cd" command) in the "data" folder of ThinAir
- type "./setsuid.sh" or type :
sudo chown 0:0 psoload
sudo chmod 6777 psoload
This we leads you at the same point ;-)
(of course the shell will asked you for your root password also)

PSOLoad issues (Error 256 in ThinAir)
-------------------------------------
Even if PSOLoad permissions are correctly set there could are some issues (especially under Jaguar).

If you still have the error 256 in ThinAir :
1) Disable Rendezvous service :
- Launch "Directory Access" application (could be found in /Applications/Utilities/),
- Authenticate yourself,
- Disable "Rendezvous".

You could also type under the Terminal :
sudo /System/Library/StartupItems/mDNSResponder/mDNSResponder stop
(your root password will be needed)

Disabling Rendezvous under Jaguar is mandatory to have PSOLoad working.

2) Select "Preferences" in ThinAir menu.
Then you could check "Give Mac IP to PSOLoad" to force PSOLoad to use your IP (it could help).
You could also do not let PSOLoad trying to guess your PSO version but select the good one from
the "PSO detection" list (it could also help).

Preferences are automatically saved when quitting ThinAir (a file called "thinair.prefs" will be created).

Uninstall
---------
To uninstall ThinAir it is quite simple : move to the trash its main folder.
ThinAir will not write or create any file to your disks (instead of ISO file of course :-) ).

Ripping part usage
------------------
When launched, ThinAir is showing the IP address of your Mac. Your Nintendo GameCube's
IP address will be : 192.168.1.32 and you Mac IP address should be 192.168.1.100.
1) Click on the "Choose" button to choose a filename for the ISO file you will create
from a GameCube mini-DVD.
2) Click on the "Start" button
3) Your Mac is then waiting for a PSO online game to start on your GameCube
4) Start a PSO online game on your GameCube
5) After a while, your GameCube will show a message saying that you have 10 seconds
to swap disc. 
6) Open your GameCube's lid and insert the midi-DVD you want to rip.
7) ThinAir will then start to rip your midi-DVD creating a ISO file (".gmc" file)

A full ISO file size is 1 459 978 240 bytes.

At anytime you could click on the "Stop" button, but if you do so, you have to start
all the process again.

Loading part usage
------------------
ThinAir will also allows you to send a ".dol" file to your GameCube (via PSOLoad which is included).
To do that :
1) Click on the "Loader" tab
2) Click on "Custom "dol" file"
3) Click on "Choose" button to select a ".dol" file
4) Click on Start and launch a PSO online game
then the ".dol" file will be sent to your GameCube.

You could also use the streaming server.
To do that :
1) Select "ACL 1.4b" or "Loader 1.0b" (depending on the loader you would like to run on the GameCube side)
You could also select "MGS Loader" if you would like to launch Metal Gear Solid ISO file (this game
needs a specific loader).
2) Check the "Stream "gcm" file" box and choose a ".gcm" file (or a ".iso" file)
3) Type in the "delay" field the delay you want for the streaming server. The delay is a way to control
the speed of the network traffic between your Mac and your GameCube.
The higher the delay is, the slower the traffic will be. But if you set a very low delay (like 1ms or 5ms)
perhaps the traffic will be too fast for your GameCube, you have to make some tuning. I suggest you
to try 1 ms and then increase this number if it is not working.
4) You can chose to block some files during streaming. This could be useful if you want to remove
streamed musics while playing. Please read the File To Block descriptor section below.
5) Choose to bypass or not PSOLoad. This feature allows you to directly launch the streaming server,
this feature could be useful if :
- you would like to launch PSOLoad by hands for any reason and use the ThinAir streaming server
- some games which have more than one disc could ask you to swap disc, then you have to stop
the streaming server, and start it again with the ISO file corresponding to disc 2. Then you
have to use the "Bypass PSOLoad" option because PSOLoad is not needed at this point ! (It should
work for Metal Gear Solid for instance).
6) Click on Start and launch a PSO online game.
then the loader is sent to your GameCube and the streaming server will be launch on the Mac side.

Remark :
You could send any ".dol" file with the "Custom "dol" file" option and also use the streaming
server. This could be useful if you wish to use another loader than the ones included.

Files To Block descriptor :
---------------------------
The "Block files" feature enables you to block chosen files while streaming.
Only ThinAir has this unique feature :-)

What you need first is to create a FTB (Files To Block) descriptor for the game
you would like to manage.

To do that, please follow the instructions :
- launch the Terminal
- go with the "cd" command inside the "data" folder of ThinAir
- type :
./thinair -list <filename.gcm>
with <filename.gcm> the name of your GCM file with full path.
The "-list" flag of ThinAir is used to list all the files inside a GCM file.
You will need to redirect this output to your future FTB descriptor,
to do that just type :
./thinair -list <filename.gcm> >test.tbf
A file called "test.tbf" will be created.
- Now edit this file with a text editor like "vi" or anything else and remove the 3 first lines.
- You will have then the list of all the files inside the GCM file, the goal now is to remove
all the lines representing a file that you do not want to block.
- When you file is done (i.e. : you have only the files you would like to block inside this file)
save this file and move it to the "Files to block" folder of ThinAir.
- Rename this file with a name representing something to you (and not "test").

Now when you select the GCM file to stream in ThinAir, you could also linked it with you new FTB descriptor
by using the "Block files" feature.

Remarks :
- "IkarugaPAL.ftb" is given as an example in the ThinAir archive.
- The FTP descriptor should always have its last line leaved blank.
- The FTP descriptor must not have more than 256 lines.
- Musics on GC are usually stored in ".adp" or ".adx" files.
- Blocking random files or too much ".adp" files could lead you to hazardous result :-)
- Block files feature has no incidence on the streaming performance.
- FTB descriptor for PAL game may not work for the NTSC version of the same game.

Known issues
------------
- Conflict with some UDP traffic
- Conflict with Rendezvous under Jaguar
- Possible conflict if multiple network interfaces are active at the same time
- GameCube IP address is set to 192.168.1.32
- Mac IP address should be 192.168.1.100
- Lost packets during ripping process are definitely lost ("riptool.dol" limitation)
- When PAL 50Hz display is used for the Loader, some games which are not switching
the display when launched may not work
- FTB (Files To Block) descriptor could not contain more than 256 files
- Old loaders are not affected by PAL 50Hz display option
- Blocking files is not available with GC Sim Preview loader

History
-------
12/12/2004	v0.7.2b	Add support for GC Sim Preview loader

02/05/2004	v0.7.1b	All known loaders have been added
			ThinAir now can remove garbage from GCM file (command line only)

23/04/2004	v0.7b	Block chosen files while streaming is now possible.

15/04/2004	v0.6.3b	GUI is ready for the next feature (not yet implemented)
			Rewrote the way PAL 50Hz display is managed
			MGS loader is no more affected by PAL 50Hz option
			ThinAir server is now under GNU GPL license

07/04/2004	v0.6.2b	PAL 50Hz display could be used for Ripper/Loaders

05/04/2004	v0.6.1b	Add MGS loader support
			ThinAir is now using PSOLoad v2.0 (included)

04/04/2004	v0.6b	Now PSOLoad could be more easily managed from ThinAir

03/04/2004	v0.5.3b	Add the possibility to give the Mac IP to PSOLoad

02/04/2004	v0.5.2b	Internal release

01/04/2004	v0.5.1b	Bypass PSOLoad feature added

31/03/2004	v0.5b	Remove IP flag given to PSOLoad to avoid multiple network interface conflict
			Delay value could be set for the streaming server

30/03/2004	v0.4.1b	Streaming server is now less experimental :-)

26/03/2004	v0.4b	New name : NGCIRS is now ThinAir (THinair Is Not An Iso Ripper)
			GUI redesign
			Fix some minor bugs
			".dol" loader has been added via PSOLoad (included)
			Experimental streaming server has been added

12/03/2004	v0.3b	PSOLoad's permissions are now managed from the application
			Ripping server is now more robust
			Show download speed
			Fix CPU usage problem while ripping is in progress

05/03/2004	v0.2.1b	Disable ZeroLink build option in XCode
			Fix socket initialization problem
			Fix CPU usage problem
			Add retry feature in ripping server
			Little delay added before running the rip server
			New icon

28/02/2004	v0.2b	Jaguar's "/Volumes" path is now supported

26/02/2004	v0.1b	First release