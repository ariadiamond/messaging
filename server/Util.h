#ifndef _UTIL_H_
#define _UTIL_H_

/*
 * Includes
 */

// Standard types
#include<stdint.h>
#include<stdbool.h>

#include"../shared/Shared.h"


/*
 * Defines
 */

#define BUFFER_SIZE 4096
#define VERSION 0x0004
#define ERR_FILE "error.log"
#define KEY_FILE ".keyfile"

/*
 * structs
 */

//global options
typedef struct CLArgs_t {
	bool logging;
	bool remove;
} CLArgs;

//client info
typedef struct ClientInfo_t {
	const int cdesc;
	char name[ID_SIZE + 1];
	char key;
	uint32_t seed;
	char buffer[BUFFER_SIZE + 1];
} ClientInfo;


// Runners Functions
void runner(int sockDesc);
void* threadRunner(void* parg);

// Ports Functions
int createServerSock(size_t port);

// Messages Functions
bool recvMessage(ClientInfo* client);

//Verify Fucntions
char getKey(char* name);
bool verifyName(ClientInfo* client);

#endif
