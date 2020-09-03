#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h> //inet_pton htons

#include<string.h> //memset
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

#include"Util.h"

int createClientSock(uint16_t port) {
    //TCP IPv4
	int clientDesc = socket(AF_INET, SOCK_STREAM, 0);

    //configure socket options
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);

    //converts human readable addresses to machine readable addresses
	if (inet_pton(AF_INET, "127.0.0.1", &(serverAddr.sin_addr)) != 1) {
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

uint32_t verify(int cdesc, char key, char* from) {
	char buffer[36];
	recv(cdesc, buffer, 35, 0);
	uint32_t seed = atoi(buffer);
	if (seed <= 0) {
		return 0;
	}
	seed = xorShift(seed);
	strncpy(buffer, from, ID_SIZE);
	strcpy((buffer + ID_SIZE), "Hello friendo!");
	seedByteXor((buffer + ID_SIZE), 15, key, &seed);
	char* hexed = byteToHex((buffer + ID_SIZE), 15);
	strncpy(buffer + ID_SIZE, hexed, 30);
	if (send(cdesc, buffer, 35, 0) <= 0) {
		dprintf(STDERR_FILENO, "Could not send message\n");
		return 0;
	}


	if (recv(cdesc, buffer, 2, 0) != 2) {
		dprintf(STDERR_FILENO, "did not send back smiley\n");
		return 0;
	}
	
	if (strncmp(buffer, ":)", 2) != 0)
		return 0;

	return seed;
}