#include<string.h> //memset
#include<stdlib.h> //exit
#include<unistd.h> //sleep

//Socket stuff
#include<sys/socket.h>
#include<netdb.h>

#include<pthread.h>

#include"Util.h"


int createServerSock(size_t port) {
	//Set up the socket properties
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	socklen_t addrLen = sizeof(serverAddr);

	//Create the socket
	int serverSock = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSock < 0) {
		perror("Server::socket()");
		exit(1);
	}

	//
	int enable = 1;
	int retVal = setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
	if (retVal < 0) {
		perror("Server::setsockopt()");
		exit(1);
	}

	//Bind to the port
	retVal = bind(serverSock, (struct sockaddr*) &serverAddr, addrLen);
	if (retVal < 0) {
		perror("Server::bind()");
		exit(1);
	}

	//Listen for connections
	retVal = listen(serverSock, SOMAXCONN);
	if (retVal < 0) {
		perror("Server::Listen()");
		exit(1);
	}

	return serverSock;
}

void runner(int sockDesc) {
	struct sockaddr clientAddr;
	socklen_t addrLen;
	while (true) {
		int cdesc[1] = {accept(sockDesc, &clientAddr, &addrLen)};
		if (cdesc[0] < 0)
			continue;
		pthread_t thread;
		pthread_create(&thread, NULL, threadRunner, (void*) cdesc);
		sleep(1);
	}
}
