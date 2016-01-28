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

#ifndef __UTILITY_H__
#define __UTILITY_H__

#define PACKED __attribute__((packed))

///////////////////////
// UTILITY FUNCTIONS //
///////////////////////


int fix_flags(int flags);
int fix_pathname(char *pathname);
int fix_argv(char *destination, char **argv);
void arg_prepend_host(char *new, char *old);
void win_to_unix(char *pathname);



int print_usage(void);

#endif
