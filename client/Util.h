#ifndef _UTIL_H_
#define _UTIL_H_

#include<stdint.h>
#include<stdbool.h>

#include"../shared/Parse.h"

#define BUFFER_SIZE 4096
#define VERSION 0x0001


//Message.c
parse_t createMessage(char* buffer, char* from);

//Connect.c
int createClientSock(uint16_t port);
void sendMessage(int clientDesc, char* from);
void getMessages(int clientDesc, char* from);


#endif
