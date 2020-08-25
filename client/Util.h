#ifndef _UTIL_H_
#define _UTIL_H_

#include<stdint.h>
#include<stdbool.h>

#include"../shared/Parse.h"
#include"../shared/Convert.h"
#include"../shared/Crypt.h"

#define BUFFER_SIZE 4096
#define VERSION 0x0002
#define USER_CONFIG ".userconfig"

//MessageHelper.c
parse_t createMessage(char* buffer, char* from);
size_t prettyPrint(int clientDesc, char* buffer);

//Ports.c
int createClientSock(uint16_t port);

//Message.c
void sendMessages(int clientDesc, char* from);
void passMessage(int clientDesc, parse_t headerInfo, char* buffer);
void getMessages(int clientDesc, char* from);


#endif
