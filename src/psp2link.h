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


#ifndef __PSP2LINK_H__
#define __PSP2LINK_H__

///////////////////////
// PS2LINK FUNCTIONS //
///////////////////////

int psp2link_connect(char *hostname);

int psp2link_mainloop(int timeout);

int psp2link_disconnect(void);

#define PSP2LINK_REQUEST_OPEN		0xBABE0111
#define PSP2LINK_REQUEST_CLOSE		0xBABE0121
#define PSP2LINK_REQUEST_READ		0xBABE0131
#define PSP2LINK_REQUEST_WRITE		0xBABE0141
#define PSP2LINK_REQUEST_LSEEK		0xBABE0151
#define PSP2LINK_REQUEST_OPENDIR	0xBABE0161
#define PSP2LINK_REQUEST_CLOSEDIR	0xBABE0171
#define PSP2LINK_REQUEST_READDIR	0xBABE0181
#define PSP2LINK_REQUEST_REMOVE		0xBABE0191
#define PSP2LINK_REQUEST_MKDIR		0xBABE01A1
#define PSP2LINK_REQUEST_RMDIR		0xBABE01B1

int psp2link_request_open(void *packet);

int psp2link_request_close(void *packet);

int psp2link_request_read(void *packet);

int psp2link_request_write(void *packet);

int psp2link_request_lseek(void *packet);

int psp2link_request_opendir(void *packet);

int psp2link_request_closedir(void *packet);

int psp2link_request_readdir(void *packet);

int psp2link_request_remove(void *packet);

int psp2link_request_mkdir(void *packet);

int psp2link_request_rmdir(void *packet);

////////////////////////////////
// PS2LINK RESPONSE FUNCTIONS //
////////////////////////////////

#define PSP2LINK_RESPONSE_OPEN		0xBABE0112
#define PSP2LINK_RESPONSE_CLOSE		0xBABE0122
#define PSP2LINK_RESPONSE_READ		0xBABE0132
#define PSP2LINK_RESPONSE_WRITE		0xBABE0142
#define PSP2LINK_RESPONSE_LSEEK		0xBABE0152
#define PSP2LINK_RESPONSE_OPENDIR	0xBABE0162
#define PSP2LINK_RESPONSE_CLOSEDIR	0xBABE0172
#define PSP2LINK_RESPONSE_READDIR	0xBABE0182
#define PSP2LINK_RESPONSE_REMOVE	0xBABE0192
#define PSP2LINK_RESPONSE_MKDIR		0xBABE01A2
#define PSP2LINK_RESPONSE_RMDIR		0xBABE01B2

int psp2link_response_open(int result);

int psp2link_response_close(int result);

int psp2link_response_read(int result, int size);

int psp2link_response_write(int result);

int psp2link_response_lseek(int result);

int psp2link_response_opendir(int result);

int psp2link_response_closedir(int result);

int psp2link_response_readdir(int result, unsigned int mode, unsigned int attr, unsigned int size, unsigned short *ctime, unsigned short *atime, unsigned short *mtime,  char *name);

int psp2link_response_remove(int result);

int psp2link_response_mkdir(int result);

int psp2link_response_rmdir(int result);

//////////////////////////////
// PSP2LINK THREAD FUNCTIONS //
//////////////////////////////

void *psp2link_thread_console(void *thread_id);

void *psp2link_thread_request(void *thread_id);

#endif

