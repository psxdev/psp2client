PSP2CLIENT FOR PC/MAC
=================
 
===================
 What does this do?
===================
 
  psp2client is a host tool providing host fileio system for PSP2. It is the same method that we used in ps2dev days, so basically it is the same protocol than ps2link and ps2client have been using since 2003.
  
  It provide all io request operation availables on psp2link library. Now command are not implemented but it can be easily incorporated.
  
  By now only osx and linux confirmed to run fine. 
  
==================
  How do I use it?
==================

 1) Compile and install psp2client

  You need a gcc installed in your environment 
  
  ```
  make
  make install
  ```
  

 2) Run sample provided in psp2link with rejuvenete on vita and when you see psp2link splash screen you are ready to run psp2client in your mac/linux
   
  ```
  psp2client -h ipofyourvita listen 
  ```
  
  To check command functionality, command thread on vita is listening and you can call with
  ```
  psp2client -h ipofyourvita -t 5 execelf README.md 
  ```
  
  ```
  psp2client -h ipofyourvita -t 5 exit
  ```
  
 
 You will see logs in terminal window output from your PlayStation Vita using libdebugnet udp log feauture and you can browse over folder in your hard disk.
 
 Sample is showing only browser example you have full io r/w access to your hard disk
 
 ```
 ./psp2client -h yourvitaip listen
 waiting psp2...
 error connecting
 debugnet initialized
 Copyright (C) 2010,2015 Antonio Jose Ramos Marquez aka bigboss @psxdev
 This Program is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 ready to have a lot of fun...
 [DEBUG]: [PSP2LINK] Server request thread UID: 0x4001058F
 [DEBUG]: [PSP2LINK] Created psp2link_requests_sock: 65
 [INFO]: [PSP2LINK] Waiting for connection
 [DEBUG]: Server command thread UID: 0x40010591
 [DEBUG]: Test debug level 1
 [DEBUG]: [PSP2LINK] Command Thread Started.
 [DEBUG]: [PSP2LINK] Created psp2link_commands_sock: 66
 [DEBUG]: [PSP2LINK] Command listener waiting for commands...
 waiting psp2...
 [INFO]: [PSP2LINK] Client connected from ip port: 44543
 [ERROR]: [PSP2LINK] Client reconnected
 [INFO]: [PSP2LINK] sock psp2link_fileio set 67 connected 1
 [INFO]: [PSP2LINK] Waiting for connection
 [INFO]: [PSP2LINK] psp2link connected  1
 [DEBUG]: [PSP2LINK] dir open req (host0:/usr/local/vitadev)
 waiting psp2...
 [DEBUG]: [PSP2LINK] dir open reply received (ret 0)
 [DEBUG]: [PSP2LINK] dir read req (0)
 [DEBUG]: [PSP2LINK] dir read reply received (ret 1)
 [DEBUG]: [PSP2LINK] dir read req (0)
 [DEBUG]: [PSP2LINK] dir read reply received (ret 1)
 [DEBUG]: [PSP2LINK] dir read req (0)
 [DEBUG]: [PSP2LINK] dir read reply received (ret 1) 
 [DEBUG]: [PSP2LINK] dir read req (0)
 [DEBUG]: [PSP2LINK] dir read reply received (ret 1)
 [DEBUG]: [PSP2LINK] dir read req (0)
 [DEBUG]: [PSP2LINK] dir read reply received (ret 1)
 [DEBUG]: [PSP2LINK] dir read req (0)
 [DEBUG]: [PSP2LINK] dir read reply received (ret 1)
 [DEBUG]: [PSP2LINK] dir read req (0)
 [DEBUG]: [PSP2LINK] dir read reply received (ret 1)
 [DEBUG]: [PSP2LINK] dir read req (0)
 [DEBUG]: [PSP2LINK] dir read reply received (ret 1)
 [DEBUG]: [PSP2LINK] dir read req (0)
 [DEBUG]: [PSP2LINK] dir read reply received (ret 1)
 [DEBUG]: [PSP2LINK] dir read req (0)
 [DEBUG]: [PSP2LINK] dir read reply received (ret 1)
 [DEBUG]: [PSP2LINK] dir read req (0) 
 [DEBUG]: [PSP2LINK] dir read reply received (ret 0)
 [DEBUG]: [PSP2LINK] psp2link_file: dir close req (fd: 0)
 [DEBUG]: [PSP2LINK] dir close reply received (ret 0)
 ```
 
 For command execution:
 
 ```
 psp2client -t 5 execelf README.md
 [INFO]: [PSP2LINK] Client connected from 192.168.1.3 port: 192
 [ERROR]: [PSP2LINK] Client reconnected
 [INFO]: [PSP2LINK] sock psp2link_fileio set 86 connected 1
 [INFO]: [PSP2LINK] Waiting for connection
 [DEBUG]: [PSP2LINK] commands listener received packet size (266)
 [DEBUG]: [PSP2LINK] Received command execelf argc=1 argv=host0:README.md
 [DEBUG]: [PSP2LINK] file open req (host0:README.md, 1 0)
 Opening README.md flags 0
 Open return 5
 [DEBUG]: [PSP2LINK] file open reply received (ret 5)
 [DEBUG]: [PSP2LINK] file lseek req (fd: 5)
 3382 result of lseek 0 offset 2 whence
 [DEBUG]: [PSP2LINK] psp2link_lseek_file: lseek reply received (ret 3382)
 [DEBUG]: [PSP2LINK] file lseek req (fd: 5)
 0 result of lseek 0 offset 0 whence
 [DEBUG]: [PSP2LINK] psp2link_lseek_file: lseek reply received (ret 0)
 [DEBUG]: [PSP2LINK] psp2link_read_file: Reply said there's 3382 bytes to read (wanted 3382)
 [DEBUG]: [PSP2LINK] psp2link_file: file close req (fd: 5)
 [DEBUG]: [PSP2LINK] psp2link_close_file: close reply received (ret 0)
 [DEBUG]: [PSP2LINK] commands listener waiting for next command
 
 ```
 3) ready to have a lot of fun :P
 

===================
 What next?
===================
 Add new commands. Finally run fine on all unix flavours and windows :P
===================
 Changelog
===================
  - 05/09/2016 customized debug output and adapted to my tools  not tested on windows 
  - 24/01/2016 fix windows support 
  
  
===========================
  Credits
===========================
  
  Special thanks goes to:
  
  - ps2dev old comrades. 
  - All people collaborating in PSP2SDK: @17310, @xerpi, @frangar, @frtomtomdu80, @hykemthedemon , @SMOKE587, @Josh_Axey ... 
  
