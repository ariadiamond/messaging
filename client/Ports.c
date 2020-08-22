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