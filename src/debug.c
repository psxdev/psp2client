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

#include "debug.h"
#include <stdarg.h>
#include <stdio.h>

int logLevel=ERROR;
void debugNetPrintf(int level, char* format, ...) 
{
	char msgbuf[0x800];
	va_list args;
	
		if (level>logLevel)
		return;
       
	va_start(args, format);
	vsnprintf(msgbuf,2048, format, args);	
	msgbuf[2047] = 0;
	va_end(args);
	if(level>logLevel)
	{
		level=NONE;
	}
	switch(level)
	{
		case INFO:
	    	printf("[HOST][INFO]: %s",msgbuf);  
	        break;
	   	case ERROR: 
	    	printf("[HOST][ERROR]: %s",msgbuf);
	        break;
		case DEBUG:
	        printf("[HOST][DEBUG]: %s",msgbuf);
	        break;
		case NONE:
			break;
	    default:
		    printf("%s",msgbuf);
       
	}
	
}
