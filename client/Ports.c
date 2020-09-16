#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h> //inet_pton htons

#include<string.h> //memset
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<time.h>

#include"Util.h"

const uint8_t MSG_LEN = 20;
const char MSG[21] = {"I just want a hug :/"};

int createClientSock(char* address, uint16_t port) {
	//TCP IPv4
	int clientDesc = socket(AF_INET, SOCK_STREAM, 0);

	//configure socket options
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);

	//converts human readable addresses to machine readable addresses
	if (inet_pton(AF_INET, address, &(serverAddr.sin_addr)) != 1) {
		perror("Ports::createClientSock::inet_pton");
		exit(1);
	}

	//creates connection
	if (connect(clientDesc, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) != 0) {
		perror("Ports::createClientSock::connect");
		exit(1);
	}
	return clientDesc;
}

bool verify(Info* info) {
	//get the seed and put it in the buffer
	info->seed = (uint32_t) time(NULL);
	uint32_t seed = info->seed ^ ((info->key << 24)
		| (info->key << 16) | (info->key << 8) | (info->key));
		//to not send the seed in the clear, it is xored with the key
	ssize_t bytesRead = sprintf(info->buffer + (2 * MSG_LEN), "%u", seed);

	//encrypt the message
	strncpy(info->buffer, MSG, MSG_LEN + 1);
	seedByteXor(info->buffer, MSG_LEN, info->key, &info->seed);
	char* hex = byteToHex(info->buffer, MSG_LEN);
	strncpy(info->buffer, hex, (2 * MSG_LEN));
	free(hex);


	bytesRead += (2 * MSG_LEN); //we need bytesRead because the seed is not
								//constant length
	send(info->cdesc, info->buffer, bytesRead, 0);

	//check if we got a smiley face (aka the server found the key)
	recv(info->cdesc, info->buffer, 2, 0);
	if (strncmp(info->buffer, ":)", 2) == 0)
		return true;

	//things did not work
	dprintf(STDERR_FILENO, "Cannot verify\n");
	return false;
}