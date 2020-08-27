#ifndef _UTIL_H_
#define _UTIL_H_

/*
 * Includes
 */

// Standard types
#include<stdint.h>
#include<stdbool.h>

#include"../shared/Parse.h"

/*
 * Defines
 */

#define BUFFER_SIZE 4096
#define VERSION 0x0001
#define ERR_FILE "error.log"
#define KEY_FILE ".keyfile"

/*
 * Options struct
 */

typedef struct CLArgs_t {
	bool logging;
	bool remove;
    uint32_t seed;
} CLArgs;

// Runners Functions
void runner(int sockDesc);
void* threadRunner(void* parg);

// Ports Functions
int createServerSock(size_t port);

// Messages Functions
bool recvMessage(int clientSock);

#endif
