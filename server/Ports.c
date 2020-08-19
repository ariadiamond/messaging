#include<string.h> //memset
#include<stdlib.h> //exit

//Socket stuff
#include<sys/socket.h>
#include<netdb.h>

#include"Util.h"


//I'm not doing error file here because how the server exits
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

	//Using TCP and reusing the address (but not port)
	int enable = 1;
	if (setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) {
		perror("Server::setsockopt()");
		exit(1);
	}

	//Bind to the port
	if (bind(serverSock, (struct sockaddr*) &serverAddr, addrLen) < 0) {
		perror("Server::bind()");
		exit(1);
	}

	//Listen for connections
	if (listen(serverSock, SOMAXCONN) < 0) {
		perror("Server::Listen()");
		exit(1);
	}

	return serverSock;
}

