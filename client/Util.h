#ifndef _UTIL_H_
#define _UTIL_H_

#include<stdint.h>
#include<stdbool.h>

#include"../shared/Shared.h"

#define BUFFER_SIZE 4096
#define VERSION 0x0004
#define USER_CONFIG ".userconfig"


typedef struct Info_t {
	int cdesc;
  char* address;
  uint16_t port;
	char name[ID_SIZE + 1];
	char key;
	uint32_t seed;
	char buffer[BUFFER_SIZE + 1];
} Info;



//MessageHelper.c
parse_t createMessage(Info* info);
size_t prettyPrint(Info* info);

//Ports.c
int createClientSock(char* address, uint16_t port);
bool verify(Info* info);

//Message.c
bool sendMessages(Info* info);
bool passMessage(Info* info, parse_t headerInfo);
bool getMessages(Info* info);


#endif
