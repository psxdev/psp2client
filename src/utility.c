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
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "utility.h"
#include "debug.h"

///////////////////////
// UTILITY FUNCTIONS //
///////////////////////

int fix_flags(int flags) 
{ 
	int result = 0;

	// Fix the flags.
	if (flags & 0x0001) { result |= O_RDONLY;           }
	if (flags & 0x0002) { result |= O_WRONLY | O_TRUNC; } // FIXME: Truncate is needed for some programs.
#ifndef _WIN32
	if (flags & 0x0010) { result |= O_NONBLOCK;         }
#endif
	if (flags & 0x0100) { result |= O_APPEND;           }
	if (flags & 0x0200) { result |= O_CREAT;            }
	if (flags & 0x0400) { result |= O_TRUNC;            }

#ifdef _WIN32

	// Binary mode file access.
	result |= O_BINARY;

#endif

	// End function.
	return result;

}
void win_to_unix(char *pathname)
{
	int loop0 = 0;
	for (loop0=0; loop0<strlen(pathname); loop0++) { if (pathname[loop0] == '\\') { pathname[loop0] = '/'; } }
	return;
	
}

int fix_pathname(char *pathname)
{
	char new_pathname[PSP2LINK_MAX_PATH];
	memset(new_pathname, 0, sizeof(new_pathname));
 
	if (strncmp(pathname, "host0:", 6) == 0)
	{
		if (pathname[6] != '/' && pathname[6] != '\\' && !strchr(pathname + 6, ':'))
		{
			// Get CWD
#if defined (__CYGWIN__) || defined (__MINGW32__)
			_getcwd(new_pathname, PSP2LINK_MAX_PATH);
#else
			getcwd(new_pathname, PSP2LINK_MAX_PATH);
#endif
 
			// Add slash if there's none yet
			int len = strlen(new_pathname);
			if (new_pathname[len - 1] != '/' && new_pathname[len - 1] != '\\')
				strcat(new_pathname, "/");
		}
 
		// Add path
		strcat(new_pathname, pathname + 6);
 
		// Convert \ to / for unix compatibility.
		win_to_unix(new_pathname);
 
		// Remove end-slash
		int len = strlen(new_pathname);
		if (len >= 2 && new_pathname[len - 2] != ':' && new_pathname[len - 1] == '/')
			new_pathname[len - 1] = '\0';
	}
	else
	{
		debugNetPrintf(INFO,"Path received does not include host0: %s\n", new_pathname);
	}
   
	debugNetPrintf(INFO,"%s -> %s\n", pathname, new_pathname);
	strcpy(pathname, new_pathname);
 
	return 0;
}
int fix_argv(char *destination, char **argv) 
{ 
	int loop0 = 0;

	// For each argv...
	for (loop0=0; argv[loop0]; loop0++) {
		if(loop0==0)
		{
			arg_prepend_host(destination,  argv[loop0]); 
			destination += strlen(destination);
			
		}
		else
		{
		// Copy the argv to the destination.
		memcpy(destination, argv[loop0], strlen(argv[loop0]));
		
		
		// Increment the destination pointer.
		destination += strlen(argv[loop0]);
		}
		// Null-terminate the argv.
		*destination = 0;

		// Increment the destination pointer.
		destination += 1;

	}

	// End function.
	return 0;

}

void arg_prepend_host(char *new, char *old) 
{
	char *pstr_ptr;
	if ((pstr_ptr = strstr(old, ":/")) != NULL ) 
	{
		(void)strcpy(new, old);
	} 
	else if ((pstr_ptr = strstr(old, "host0:")) != NULL ) 
	{
		pstr_ptr = strstr(pstr_ptr, ":");
		++pstr_ptr;
 	   (void)strcpy(new, "host0:");
	   (void)strcat(new, pstr_ptr);
	} 
	else if ((pstr_ptr = strstr(old, ":")) != NULL ) 
	{
		(void)strcpy(new, old);
	} 
	else 
	{
		(void)strcpy(new, "host0:");
		(void)strcat(new, old);
	}
}

int print_usage(void) 
{

	// Print out the psp2client usage string.
	printf("\n");
	printf(" Usage: psp2client [-h hostname] [-t timeout] <command> [arguments]\n\n");
	printf(" Available commands:\n\n");
	printf("   exit\n");
	printf("   execelf <filename> [arguments]\n");
	printf("   execsprx <filename> [arguments]\n");
	printf("   listen\n\n");

	// End function.
	return 0;
}
