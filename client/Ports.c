#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h> //inet_pton htons

#include<string.h> //memset
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

#include"Util.h"

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
	//get the seed
	recv(info->cdesc, info->buffer, 35, 0);
	info->seed = atoi(info->buffer);
	if (info->seed <= 0)
		return false;

	//encrypt with the seed and key
	xorShift(&info->seed);
	strncpy(info->buffer, info->name, ID_SIZE);
	strcpy((info->buffer + ID_SIZE), "Hello friendo!");
	seedByteXor((info->buffer + ID_SIZE), 15, info->key, &info->seed);
	char* hexed = byteToHex((info->buffer + ID_SIZE), 15);
	strncpy(info->buffer + ID_SIZE, hexed, 30);

	//send encrypted message
	if (send(info->cdesc, info->buffer, 35, 0) <= 0) {
		dprintf(STDERR_FILENO, "Could not send message\n");
		return false;
	}

	//get a smiley to check if the server validated
	if (recv(info->cdesc, info->buffer, 2, 0) != 2) {
		dprintf(STDERR_FILENO, "did not send back smiley\n");
		return false;
	}
	if (strncmp(info->buffer, ":)", 2) != 0)
		return false;

	return true;
}