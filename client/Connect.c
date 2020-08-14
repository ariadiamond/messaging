#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h> //inet_pton htons
#include<string.h> //memset

#include<stdio.h>

#include"Util.h"

int clientSock(uint16_t port) {
	int clientDesc = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);

	inet_pton(AF_INET, "127.0.0.1", &(serverAddr.sin_addr));

	connect(clientDesc, (struct sockaddr*) &serverAddr, sizeof(serverAddr));

	return clientDesc;
}

void sendMessage(int clientDesc, char* from) {
	char buffer[BUFFER_SIZE];
	parse_t headerInfo = createMessage(buffer, from);

	char header[HEADER_SIZE + 1];
	snprintf(header, HEADER_SIZE, "To: %s\nFrom: %s\nLength: %04X\n",
		headerInfo.to, headerInfo.from, headerInfo.length);
	header[HEADER_SIZE] = 0;

	send(clientDesc, header, HEADER_SIZE, 0);
	recv(clientDesc, header, HEADER_SIZE, 0); //ack, however this gets stuck
		//if it never gets a response
	send(clientDesc, buffer, headerInfo.length, 0);
}
