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


typedef struct Info_t {
	int cdesc;
	char name[ID_SIZE + 1];
	char key;
	uint32_t seed;
	char buffer[BUFFER_SIZE + 1];
} Info;



//MessageHelper.c
parse_t createMessage(Info* info);
size_t prettyPrint(Info* info);

//Ports.c
int createClientSock(uint16_t port);
bool verify(Info* info);

//Message.c
bool sendMessages(Info* info);
bool passMessage(Info* info, parse_t headerInfo);
bool getMessages(Info* info);


#endif
