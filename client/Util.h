#ifndef _UTIL_H_
#define _UTIL_H_

#include<stdint.h>
#include<stdbool.h>

#include"../shared/Parse.h"

#define BUFFER_SIZE 4096

//Message.c
parse_t createMessage(char* buffer, char* from);

//Connect.c
int clientSock(uint16_t port);
void sendMessage(int clientDesc, char* from);

#endif
