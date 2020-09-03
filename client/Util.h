#ifndef _UTIL_H_
#define _UTIL_H_

#include<stdint.h>
#include<stdbool.h>

#include"../shared/Parse.h"
#include"../shared/Convert.h"
#include"../shared/Crypt.h"

#define BUFFER_SIZE 4096
#define VERSION 0x0003
#define USER_CONFIG ".userconfig"

//MessageHelper.c
parse_t createMessage(char* buffer, char* from);
size_t prettyPrint(int clientDesc, char* buffer, char key, uint32_t* seed);

//Ports.c
int createClientSock(uint16_t port);
uint32_t verify(int cdesc, char key, char* from);

//Message.c
bool sendMessages(int clientDesc, char* from, char key, uint32_t* seed);
bool passMessage(int clientDesc, parse_t headerInfo, char* buffer, char key, uint32_t* seed);
bool getMessages(int clientDesc, char* from, char key, uint32_t* seed);


#endif
