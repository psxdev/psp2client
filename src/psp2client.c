/*
 * psp2client host tool for PSP2 providing host fileio system 
 * Copyright (C) 2003,2015 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/psxdev/psp2client
 * based on ps2vfs, ps2client, ps2link, ps2http tools. 
 * Credits goes for all people involved in ps2dev project https://github.com/ps2dev
 * This file is subject to the terms and conditions of the PSP2Client License.
 * See the file LICENSE in the main directory of this distribution for more
 * details.
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utility.h"
#include "debug.h"
#include "psp2link.h"
#ifdef _WIN32
#include "network.h"
#endif


//change to your vita ip
char hostname[256] = { "192.168.1.37" };

int timeout = -1;

int main(int argc, char **argv, char **env) 
{ 
	int loop0 = 0;

	// Turn off stdout buffering.
	setbuf(stdout, NULL);

	// Parse the environment list for optional arguments.
	for (loop0=0; env[loop0]; loop0++) 
	{

		// A hostname has been specified...
		if (strncmp(env[loop0], "PSP2HOSTNAME", 12) == 0) { strncpy(hostname, &env[loop0][13], sizeof(hostname)); }
	}
	// Check the number of arguments.
	if (argc < 2) { print_usage(); return -1; }

	// Parse the argument list for optional arguments.
	for (loop0=1; argv[loop0]; loop0++) {

		// If an optional hostname has been specified...
		if (strncmp(argv[loop0], "-h", 2) == 0) { loop0++;

			// Check to make sure the hostname value was actually supplied.
			if (argc == loop0) { debugNetPrintf(INFO,"No hostname was supplied the '-h' option.\n"); print_usage(); return -1; }

			// Set the hostname to the supplied value.
			strncpy(hostname, argv[loop0], sizeof(hostname));
	  

		}

		// Else, if an optional timeout has been specified...
		else if (strncmp(argv[loop0], "-t", 2) == 0) { loop0++;

			// Check to make sure a value was actually supplied.
			if (argc == loop0) { debugNetPrintf(INFO,"No timeout was supplied the '-t' option.\n"); print_usage(); return -1; }

			// Set the timeout to the supplied value.
			timeout = atoi(argv[loop0]);

		}

		// Else, the end of the options has been reached...
		else { break; }
	}

	// Increment the argument counters past any optional arguments.
	loop0++; argc -= loop0; argv += loop0;

	// Check to make sure a command was actually supplied.
	if (argc < 0) { debugNetPrintf(INFO,"No command was supplied.\n"); print_usage(); return -1; }
#ifdef _WIN32

	// Startup network, under windows.
	if (network_startup() < 0) 
	{ 
		debugNetPrintf(INFO,"Could not start up winsock.\n"); 
		return 1; 
	}

#endif

	// Initialization by now psp2 hostname will not be used
	if (psp2link_connect(hostname) < 0) 
	{ 
		debugNetPrintf(INFO,"Could not connect to the psp2link server. (%s)\n", hostname); 
		return -1; 
	}
	
	if (strcmp(argv[-1], "execelf") == 0) { 
		psp2link_command_execelf(argc, argv); 
	} else
	if (strcmp(argv[-1], "execsprx") == 0) { 
		psp2link_command_execsprx(argc, argv); 
	} else
	if (strcmp(argv[-1], "exit") == 0) { psp2link_command_exit();
	 } else
	if (strcmp(argv[-1], "listen") == 0) { } else
	// An unknown command was requested.
	{ debugNetPrintf(INFO,"Unknown command requested. (%s)\n", argv[-1]); print_usage(); return -1; }
	
   
	// Enter the main loop.
	psp2link_mainloop(timeout);

	// Disconnect from the psp2link server.
	psp2link_disconnect();

	// End program.
	return 0;
}
