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
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <pthread.h>
#ifndef _WIN32
#include <netinet/in.h>
#else
#include <windows.h>
#define sleep(x) Sleep(x * 1000)
#endif

#include "network.h"
#include "psp2link.h"
#include "utility.h"

extern int errno ;
int console_socket = -1;
int request_socket = -1;
int command_socket = -1; 

pthread_t console_thread_id;
pthread_t request_thread_id;

int psp2link_counter = 0;

// psp2link_dd is now an array of structs
struct {
	char *pathname; // remember to free when closing dir
	DIR *dir;
} psp2link_dd[10] = {
  { NULL, NULL }, { NULL, NULL }, { NULL, NULL }, { NULL, NULL }, { NULL, NULL },
  { NULL, NULL }, { NULL, NULL }, { NULL, NULL }, { NULL, NULL }, { NULL, NULL }
};

////////////////////////
// PSP2LINK FUNCTIONS //
////////////////////////

int psp2link_connect(char *hostname) 
{

	
	// Listen datagram socket port for debugnet console.
	console_socket = network_listen(0x4712, SOCK_DGRAM);

	// Create the console thread.
	if (console_socket > 0) 
	{ 
		pthread_create(&console_thread_id, NULL, psp2link_thread_console, (void *)&console_thread_id); 
	}

	// Connect to the request port.
	request_socket = network_connect(hostname, 0x4711, SOCK_STREAM);
	// request_socket = network_listen(0x4711, SOCK_STREAM);

	// Create the request thread.
	while(request_socket<0)
	{
		request_socket = network_connect(hostname, 0x4711, SOCK_STREAM);
		sleep(1);
		printf("waiting psp2...\n");
	 	
	}
	if (request_socket > 0) 
	{ 
		pthread_create(&request_thread_id, NULL, psp2link_thread_request, (void *)&request_thread_id); 
	}
  	
	// Connect to the command port future use to send commands to psp2
	command_socket = network_connect(hostname, 0x4712, SOCK_DGRAM);

	// Delay for a moment to let ps2link finish setup.
#ifdef _WIN32
	Sleep(1);
#else
	sleep(1);
#endif

	// End function.
	return 0;

}

int psp2link_mainloop(int timeout) 
{

	// Disconnect from the command port.
	if (network_disconnect(command_socket) < 0) 
	{ 
		return -1; 
	}

	// If no timeout was given, timeout immediately.
	if (timeout == 0) 
	{ 
		return 0; 
	}

	// If timeout was never, loop forever.
	if (timeout < 0) 
	{ 
		for (;;) 
		{ 
			sleep(600); 
		} 
	}

	// Increment the timeout counter until timeout is reached.
	while (psp2link_counter++ < timeout) 
	{ 
		sleep(1); 
	};

	// End function.
	return 0;
}

int psp2link_disconnect(void) 
{

	// Disconnect from the command port.
	if (network_disconnect(command_socket) < 0) { return -1; }

	// Disconnect from the request port.
	if (network_disconnect(request_socket) < 0) 
	{ 
		return -1; 
	}

	// Disconnect from console port.
	if (network_disconnect(console_socket) < 0) 
	{ 
		return -1; 
	}

	// End function.
	return 0;

}

////////////////////////////////
// PSP2LINK COMMAND FUNCTIONS //
////////////////////////////////


int psp2link_command_execelf(int argc, char **argv) {
	struct { unsigned int number; unsigned short length; int argc; char argv[256]; } PACKED command;

	// Build the command packet.
	command.number = htonl(PSP2LINK_EXECELF_CMD);
	command.length = htons(sizeof(command));
	command.argc   = htonl(argc);
	fix_argv(command.argv, argv);

	// Send the command packet.
	return network_send(command_socket, &command, sizeof(command));

}
 
int psp2link_command_execsprx(int argc, char **argv) {
	struct { unsigned int number; unsigned short length; int argc; char argv[256]; } PACKED command;

	// Build the command packet.
	command.number = htonl(PSP2LINK_EXECSPRX_CMD);
	command.length = htons(sizeof(command));
	command.argc   = htonl(argc);
	fix_argv(command.argv, argv);

	// Send the command packet.
	return network_send(command_socket, &command, sizeof(command));
}

int psp2link_command_exit(void) {
	struct { unsigned int number; unsigned short length; int argc; char argv[256]; } PACKED command;

	// Build the command packet.
	command.number = htonl(PSP2LINK_EXIT_CMD);
	command.length = htons(sizeof(command));
	
	// Send the command packet.
	return network_send(command_socket, &command, sizeof(command));
}

////////////////////////////////
// PSP2LINK REQUEST FUNCTIONS //
////////////////////////////////

int psp2link_request_open(void *packet) 
{
	struct { unsigned int number; unsigned short length; int flags; char pathname[256]; } PACKED *request = packet;
	int result = -1;
	struct stat stats;

	// Fix the arguments.
	fix_pathname(request->pathname);
	if(request->pathname[0]==0)
	{
		return psp2link_response_open(-1);
	}
	request->flags = fix_flags(ntohl(request->flags));

	printf("Opening %s flags %x\n",request->pathname,request->flags);
    
	if(((stat(request->pathname, &stats) == 0) && (!S_ISDIR(stats.st_mode))) || (request->flags & O_CREAT))
	{
		// Perform the request.
#if defined (__CYGWIN__) || defined (__MINGW32__)
		result = open(request->pathname, request->flags | O_BINARY, 0644);
#else
		result = open(request->pathname, request->flags, 0644);
#endif
	}

	// Send the response.
	printf("Open return %d\n",result);
	return psp2link_response_open(result);

}

int psp2link_request_close(void *packet) 
{
	struct { unsigned int number; unsigned short length; int fd; } PACKED *request = packet;
	int result = -1;

	// Perform the request.
	result = close(ntohl(request->fd));

	// Send the response.
	return psp2link_response_close(result);

}

int psp2link_request_read(void *packet) 
{
	struct { unsigned int number; unsigned short length; int fd; int size; } PACKED *request = packet;
	int result = -1, size = -1; 
	char buffer[65536], *bigbuffer;

	// If a big read is requested...
	if (ntohl(request->size) > sizeof(buffer)) 
	{
		// Allocate the bigbuffer.
		bigbuffer = malloc(ntohl(request->size));

		// Perform the request.
		result = size = read(ntohl(request->fd), bigbuffer, ntohl(request->size));

		// Send the response.
		psp2link_response_read(result, size);

		// Send the response data.
		network_send(request_socket, bigbuffer, size);

		// Free the bigbuffer.
		free(bigbuffer);

		// Else, a normal read is requested...
	} 
	else 
	{

		// Perform the request.
		size = read(ntohl(request->fd), buffer, ntohl(request->size));
		//int error=errno ;
		//printf("Error reading file: %s %s\n", strerror( error ),buffer);
		result=size;
		//printf("read %d bytes of file descritor %d\n",result,ntohl(request->fd));

		// Send the response.
		psp2link_response_read(result, size);

		// Send the response data.
		network_send(request_socket, buffer, size);

	}

	// End function.
	return 0;

}

int psp2link_request_write(void *packet) 
{
	struct { unsigned int number; unsigned short length; int fd; int size; } PACKED *request = packet;
	int result = -1; 
	char buffer[65536], *bigbuffer;

	// If a big write is requested...
	if (ntohl(request->size) > sizeof(buffer)) 
	{

		// Allocate the bigbuffer.
		bigbuffer = malloc(ntohl(request->size));

		// Read the request data.
		network_receive_all(request_socket, bigbuffer, ntohl(request->size));

		// Perform the request.
		result = write(ntohl(request->fd), bigbuffer, ntohl(request->size));

		// Send the response.
		psp2link_response_write(result);

		// Free the bigbuffer.
		free(bigbuffer);

	} 
	// Else, a normal write is requested...
	else 
	{

		// Read the request data.
		network_receive_all(request_socket, buffer, ntohl(request->size));

		// Perform the request.
		result = write(ntohl(request->fd), buffer, ntohl(request->size));

		// Send the response.
		psp2link_response_write(result);

	}

	// End function.
	return 0;

}

int psp2link_request_lseek(void *packet) 
{
	struct { unsigned int number; unsigned short length; int fd; int offset; int whence; } PACKED *request = packet;
	int result = -1;

	// Perform the request.
	result = lseek(ntohl(request->fd), ntohl(request->offset), ntohl(request->whence));
	printf("%d result of lseek %d offset %d whence\n",result,ntohl(request->offset), ntohl(request->whence));
	// Send the response.
	return psp2link_response_lseek(result);

}

int psp2link_request_opendir(void *packet) 
{ 
	int loop0 = 0;
	struct { unsigned int command; unsigned short length; int flags; char pathname[256]; } PACKED *request = packet;
	int result = -1;
	struct stat stats;

	// Fix the arguments.
	fix_pathname(request->pathname);
	if(request->pathname[0]==0)
	{
		return psp2link_response_opendir(-1);

	}

	if((stat(request->pathname, &stats) == 0) && (S_ISDIR(stats.st_mode)))
	{
		// Allocate an available directory descriptor.
		for (loop0=0; loop0<10; loop0++) 
		{ 
			if (psp2link_dd[loop0].dir == NULL) 
			{ 
				result = loop0; 
				break; 
			} 
		}

		// Perform the request.
		if (result != -1)
		{
			psp2link_dd[result].pathname = (char *) malloc(strlen(request->pathname) + 1);
			strcpy(psp2link_dd[result].pathname, request->pathname);
			psp2link_dd[result].dir = opendir(request->pathname);
		}
		
	}

	// Send the response.
	return psp2link_response_opendir(result);
}

int psp2link_request_closedir(void *packet) 
{
	struct { unsigned int number; unsigned short length; int dd; } PACKED *request = packet;
	int result = -1;

	// Perform the request.
	result = closedir(psp2link_dd[ntohl(request->dd)].dir);

	if(psp2link_dd[ntohl(request->dd)].pathname)
	{
		free(psp2link_dd[ntohl(request->dd)].pathname);
		psp2link_dd[ntohl(request->dd)].pathname = NULL;
	}

	// Free the directory descriptor.
	psp2link_dd[ntohl(request->dd)].dir = NULL;

	// Send the response.
	return psp2link_response_closedir(result);

}

int psp2link_request_readdir(void *packet) 
{
	DIR *dir;
	struct { unsigned int number; unsigned short length; int dd; } PACKED *request = packet;
	struct dirent *dirent; 
	struct stat stats; 
	struct tm loctime;
	unsigned int mode; 
	
	unsigned short ctime[8]; 
	unsigned short atime[8];
	unsigned short mtime[8];
	
	char tname[512];

	dir = psp2link_dd[ntohl(request->dd)].dir;

	// Perform the request.
	dirent = readdir(dir);

	// If no more entries were found...
	if (dirent == NULL) 
	{

		// Tell the user an entry wasn't found.
		return psp2link_response_readdir(0, 0, 0, 0, NULL, NULL, NULL, NULL);

	}

	// need to specify the directory as well as file name otherwise uses CWD!
	sprintf(tname, "%s/%s", psp2link_dd[ntohl(request->dd)].pathname, dirent->d_name);

	// Fetch the entry's statistics.
	stat(tname, &stats);

	// Convert the mode.
	mode = (stats.st_mode& 0xFFF);//0x01FF);//0x07);
	//printf("mode %x st_mode %04o\n",mode,stats.st_mode);
	if (S_ISDIR(stats.st_mode)) 
	{ 
		mode |= 0x1000;//0x20; 
	}
#ifndef _WIN32
	if (S_ISLNK(stats.st_mode)) 
	{ 
		mode |= 0x4000; //0x08; 
	}
#endif
	if (S_ISREG(stats.st_mode)) 
	{ 
		mode |= 0x2000;//0x10; 
	}

	
	
	// Convert the creation time.
	localtime_r(&(stats.st_ctime), &loctime);
	ctime[6] = (unsigned short)(loctime.tm_year+1900);
	ctime[5] = (unsigned short)(loctime.tm_mon + 1);
	ctime[4] = (unsigned short)loctime.tm_mday;
	ctime[3] = (unsigned short)loctime.tm_hour;
	ctime[2] = (unsigned short)loctime.tm_min;
	ctime[1] = (unsigned short)loctime.tm_sec;
	

	// Convert the access time.
	localtime_r(&(stats.st_atime), &loctime);
	atime[6] = (unsigned short)(loctime.tm_year+1900);
	atime[5] = (unsigned short)(loctime.tm_mon + 1);
	atime[4] = (unsigned short)loctime.tm_mday;
	atime[3] = (unsigned short)loctime.tm_hour;
	atime[2] = (unsigned short)loctime.tm_min;
	atime[1] = (unsigned short)loctime.tm_sec;

	// Convert the last modified time.
	localtime_r(&(stats.st_mtime), &loctime);
	mtime[6] = (unsigned short)(loctime.tm_year+1900);
	mtime[5] = (unsigned short)(loctime.tm_mon + 1);
	mtime[4] = (unsigned short)loctime.tm_mday;
	mtime[3] = (unsigned short)loctime.tm_hour;
	mtime[2] = (unsigned short)loctime.tm_min;
	mtime[1] = (unsigned short)loctime.tm_sec;

  
	// Send the response.
	return psp2link_response_readdir(1, mode, 0, stats.st_size, ctime, atime, mtime, dirent->d_name);

}

int psp2link_request_remove(void *packet) 
{
	struct { unsigned int number; unsigned short length; char name[256]; } PACKED *request = packet;
	int result = -1;

	// Fix the arguments.
	fix_pathname(request->name);

	// Perform the request.
	result = remove(request->name);

	// Send the response.
	return psp2link_response_remove(result);
}

int psp2link_request_mkdir(void *packet) 
{
	struct { unsigned int number; unsigned short length; int mode; char name[256]; } PACKED *request = packet;
	int result = -1;

	// Fix the arguments.
	fix_pathname(request->name);
	// request->flags = fix_flags(ntohl(request->flags));

	// Perform the request.
	// do we need to use mode in here: request->mode ?
  
#ifdef _WIN32
	result = mkdir(request->name);  
#else
	result = mkdir(request->name, request->mode);
#endif
  
	// Send the response.
	return psp2link_response_mkdir(result);
}

int psp2link_request_rmdir(void *packet) 
{
	struct { unsigned int number; unsigned short length; char name[256]; } PACKED *request = packet;
	int result = -1;

	// Fix the arguments.
	fix_pathname(request->name);

	// Perform the request.
	result = rmdir(request->name);

	// Send the response.
	return psp2link_response_rmdir(result);
}

/////////////////////////////////
// PSP2LINK RESPONSE FUNCTIONS //
/////////////////////////////////

int psp2link_response_open(int result) 
{
	struct { unsigned int number; unsigned short length; int result; } PACKED response;

	// Build the response packet.
	response.number = htonl(PSP2LINK_RESPONSE_OPEN);
	response.length = htons(sizeof(response));
	response.result = htonl(result);

	// Send the response packet.
	return network_send(request_socket, &response, sizeof(response));
}

int psp2link_response_close(int result) 
{
	struct { unsigned int number; unsigned short length; int result; } PACKED response;

	// Build the response packet.
	response.number = htonl(PSP2LINK_RESPONSE_CLOSE);
	response.length = htons(sizeof(response));
	response.result = htonl(result);

	// Send the response packet.
	return network_send(request_socket, &response, sizeof(response));

}

int psp2link_response_read(int result, int size) 
{
	struct { unsigned int number; unsigned short length; int result; int size; } PACKED response;

	// Build the response packet.
	response.number = htonl(PSP2LINK_RESPONSE_READ);
	response.length = htons(sizeof(response));
	response.result = htonl(result);
	response.size   = htonl(size);

	// Send the response packet.
	return network_send(request_socket, &response, sizeof(response));

}

int psp2link_response_write(int result) 
{
	struct { unsigned int number; unsigned short length; int result; } PACKED response;

	// Build the response packet.
	response.number = htonl(PSP2LINK_RESPONSE_WRITE);
	response.length = htons(sizeof(response));
	response.result = htonl(result);

	// Send the response packet.
	return network_send(request_socket, &response, sizeof(response));

}

int psp2link_response_lseek(int result) 
{
	struct { unsigned int number; unsigned short length; int result; } PACKED response;

	// Build the response packet.
	response.number = htonl(PSP2LINK_RESPONSE_LSEEK);
	response.length = htons(sizeof(response));
	response.result = htonl(result);

	// Send the response packet.
	return network_send(request_socket, &response, sizeof(response));

}

int psp2link_response_opendir(int result) 
{
	struct { unsigned int number; unsigned short length; int result; } PACKED response;

	// Build the response packet.
	response.number = htonl(PSP2LINK_RESPONSE_OPENDIR);
	response.length = htons(sizeof(response));
	response.result = htonl(result);

	// Send the response packet.
	return network_send(request_socket, &response, sizeof(response));

}

int psp2link_response_closedir(int result) 
{
	struct { unsigned int number; unsigned short length; int result; } PACKED response;

	// Build the response packet.
	response.number = htonl(PSP2LINK_RESPONSE_CLOSEDIR);
	response.length = htons(sizeof(response));
	response.result = htonl(result);

	// Send the response packet.
	return network_send(request_socket, &response, sizeof(response));

}

int psp2link_response_readdir(int result, unsigned int mode, unsigned int attr, unsigned int size, unsigned short *ctime, unsigned short *atime, unsigned short *mtime, char *name) 
{
	//struct { unsigned int number; unsigned short length; int result; unsigned int mode; unsigned int attr; unsigned int size; unsigned char ctime[8]; unsigned char atime[8]; unsigned char mtime[8]; unsigned int hisize; char name[256]; } PACKED response;
	
	struct { unsigned int number; unsigned short length; int result; unsigned int mode; unsigned int attr; unsigned int size; unsigned short ctime[8]; unsigned short atime[8]; unsigned short mtime[8]; char name[256]; } PACKED response;

	// Build the response packet.
	response.number = htonl(PSP2LINK_RESPONSE_READDIR);
	response.length = htons(sizeof(response));
	response.result = htonl(result);
	response.mode   = htonl(mode);
	response.attr   = htonl(attr);
	response.size   = htonl(size);
	if(ctime)
	{
		response.ctime[0] = 0;
		response.ctime[1] = htons(ctime[1]);
		response.ctime[2] = htons(ctime[2]);
		response.ctime[3] = htons(ctime[3]);
		response.ctime[4] = htons(ctime[4]);
		response.ctime[5] = htons(ctime[5]);
		response.ctime[6] = htons(ctime[6]);
	}
	if(atime)
	{
		response.atime[0] = 0;
		response.atime[1] = htons(atime[1]);
		response.atime[2] = htons(atime[2]);
		response.atime[3] = htons(atime[3]);
		response.atime[4] = htons(atime[4]);
		response.atime[5] = htons(atime[5]);
		response.atime[6] = htons(atime[6]);
	}
	if(mtime)
	{
		response.mtime[0] = 0;
		response.mtime[1] = htons(mtime[1]);
		response.mtime[2] = htons(mtime[2]);
		response.mtime[3] = htons(mtime[3]);
		response.mtime[4] = htons(mtime[4]);
		response.mtime[5] = htons(mtime[5]);
		response.mtime[6] = htons(mtime[6]);
	}
	
  	
#ifdef _WIN32
	if (name) { sprintf(response.name, "%s", name); }
#else
	if (name) { snprintf(response.name, 256, "%s", name); }
#endif

	// Send the response packet.
	return network_send(request_socket, &response, sizeof(response));

}

int psp2link_response_remove(int result) 
{
	struct { unsigned int number; unsigned short length; int result; } PACKED response;

	// Build the response packet.
	response.number = htonl(PSP2LINK_RESPONSE_REMOVE);
	response.length = htons(sizeof(response));
	response.result = htonl(result);

	// Send the response packet.
	return network_send(request_socket, &response, sizeof(response));
}

int psp2link_response_mkdir(int result) 
{
	struct { unsigned int number; unsigned short length; int result; } PACKED response;

	// Build the response packet.
	response.number = htonl(PSP2LINK_RESPONSE_MKDIR);
	response.length = htons(sizeof(response));
	response.result = htonl(result);

	// Send the response packet.
	return network_send(request_socket, &response, sizeof(response));
}

int psp2link_response_rmdir(int result) 
{
	struct { unsigned int number; unsigned short length; int result; } PACKED response;

	// Build the response packet.
	response.number = htonl(PSP2LINK_RESPONSE_RMDIR);
	response.length = htons(sizeof(response));
	response.result = htonl(result);

	// Send the response packet.
	return network_send(request_socket, &response, sizeof(response));
}

///////////////////////////////
// PSP2LINK THREAD FUNCTIONS //
///////////////////////////////

void *psp2link_thread_console(void *thread_id) 
{
	char buffer[1024];

	// If the socket isn't open, this thread isn't needed.
	if (console_socket < 0) { pthread_exit(thread_id); }

	// Loop forever...
	for (;;) 
	{

		// Wait for network activity.
		network_wait(console_socket, -1);

		// Receive the console buffer.
		network_receive(console_socket, buffer, sizeof(buffer));

		// Print out the console buffer.
		printf("%s", buffer);

		// Clear the console buffer.
		memset(buffer, 0, sizeof(buffer));

		// Reset the timeout counter.
		psp2link_counter = 0;

	}

	// End function.
	return NULL;

}

void *psp2link_thread_request(void *thread_id) 
{
	struct { unsigned int number; unsigned short length; char buffer[512]; } PACKED packet;

	// If the socket isn't open, this thread isn't needed.
	if (request_socket < 0) { pthread_exit(thread_id); }
	
	listen(request_socket , 5);
	// Loop forever...
	for (;;) {

		// Wait for network activity.
		network_wait(request_socket, -1);

		// Read in the request packet header.
		network_receive_all(request_socket, &packet, 6);

		// Read in the rest of the packet.
		network_receive_all(request_socket, packet.buffer, ntohs(packet.length) - 6);

		// Perform the requested action.
		switch(ntohl(packet.number))
		{
			case PSP2LINK_REQUEST_OPEN:     
				psp2link_request_open(&packet);     
				break;
			case PSP2LINK_REQUEST_CLOSE:    
				psp2link_request_close(&packet);
				break;    
			case PSP2LINK_REQUEST_READ:    
				psp2link_request_read(&packet);    
				break;
			case PSP2LINK_REQUEST_WRITE:    
				psp2link_request_write(&packet);    
				break;
			case PSP2LINK_REQUEST_LSEEK:    
				psp2link_request_lseek(&packet);   
				break;
			case PSP2LINK_REQUEST_OPENDIR:  
				psp2link_request_opendir(&packet);  
				break;
			case PSP2LINK_REQUEST_CLOSEDIR: 
				psp2link_request_closedir(&packet); 
				break;
			case PSP2LINK_REQUEST_READDIR:  
				psp2link_request_readdir(&packet);  
				break;
			case PSP2LINK_REQUEST_REMOVE:   
				psp2link_request_remove(&packet);
				break;
			case PSP2LINK_REQUEST_MKDIR:    
				psp2link_request_mkdir(&packet);  
				break;   
			case PSP2LINK_REQUEST_RMDIR:    
				psp2link_request_rmdir(&packet);  
				break;
			default:
				printf("Received unsupported request number\n");
				break;
		}
   	 	
		// Reset the timeout counter.
		psp2link_counter = 0;

	}

	// End function.
	return NULL;

}
