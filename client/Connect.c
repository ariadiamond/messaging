#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h> //inet_pton htons
#include<string.h> //memset
#include<stdlib.h>

#include<stdio.h>
#include<unistd.h>

#include"Util.h"

int createClientSock(uint16_t port) {
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
	if (!marshall(headerInfo, header))
		return;

	send(clientDesc, header, HEADER_SIZE, 0);
	recv(clientDesc, header, HEADER_SIZE, 0); //ack, however this gets stuck
		//if it never gets a response
	send(clientDesc, buffer, headerInfo.length, 0);
	close(clientDesc);
}


void getMessages(int clientDesc, char* from) {
	char buffer[BUFFER_SIZE];
	dprintf(clientDesc, "To: NULL\nFrom: %s\nVersion: %04X\nLength: 0000\n", from, VERSION);
	ssize_t bytesRead = recv(clientDesc, buffer, BUFFER_SIZE, 0);
	size_t lengths[1];
	sscanf(buffer, "%lu", lengths);
	if (lengths[0] == 0) {
		printf("You don't have any new messages\n");
		return;
	}
	size_t received = 0;
	while (received < lengths[0]) {
		bytesRead = recv(clientDesc, buffer, BUFFER_SIZE, 0);
		if (bytesRead < 1) {
			//error
			exit(1);
		}
		write(STDIN_FILENO, buffer, bytesRead);
		received += bytesRead;
	}

	close(clientDesc);
}
