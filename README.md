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
  
 
 You will see logs in terminal window output from your PlayStation Vita using libdebugnet udp log feauture and you can browse over folder in your hard disk.
 
 Sample is showing only browser example you have full io r/w access to your hard disk
 
 ```
 ./psp2client -h yourvitaip listen
 [INFO]: Client connected from xxx.xxx.xxx.xxx port: 30915
  [INFO]: sock psp2link_fileio set 50 connected 1
 [INFO]: Waiting for connection
 [INFO]: psp2link connected  1
 [DEBUG]: dir open req (host0:/usr/local)
 aqui
 [DEBUG]: dir open reply received (ret 0)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 0)
 [DEBUG]: psp2link_file: dir close req (fd: 0)
 [DEBUG]: dir close reply received (ret 0)
 ```
 
 3) ready to have a lot of fun :P
 
===================
 What next?
===================
  
  Add windows support, by now i have not free time to do it if someone want to add it contact with me.
  
  
===========================
  Credits
===========================
  
  Special thanks goes to:
  
  - ps2dev old comrades. 
  - All people collaborating in PSP2SDK: @17310, @xerpi, @frangar, @frtomtomdu80, @hykemthedemon , @SMOKE587, @Josh_Axey ... 
  
