#ifndef _UTIL_H_
#define _UTIL_H_

/*
 * Includes
 */

// Error handling
#include<errno.h>
#include<stdio.h>

// Standard types
#include<stdint.h>
#include<stdbool.h>

#include"../shared/Shared.h"

/*
 * Defines
 */

 #define BUFFER_SIZE 4096
 #define VERSION 0x0001
 #define ERR_FILE "error.log"

/*
 * Options struct
 */

typedef struct CLArgs_t {
	bool logging;
	bool remove;
} CLArgs;

// Runners Functions
void runner(int sockDesc);
void* threadRunner(void* parg);

// Ports Functions
int createServerSock(size_t port);

// Messages Functions
void recvMessage(int clientSock);

#endif
