#include<stdio.h>
#include<stdlib.h>
#include<unistd.h> //write
#include<string.h>

#include<sys/socket.h>

#include"Util.h"

//this is solely to make Pseudoclient easier
bool sendMessages(int clientDesc, char* from) {
	char buffer[BUFFER_SIZE];
	parse_t headerInfo = createMessage(buffer, from);
	return passMessage(clientDesc, headerInfo, buffer);
}

bool passMessage(int clientDesc, parse_t headerInfo, char* buffer) {
	if (headerInfo.version == 2) {
		byteXor(buffer, headerInfo.length, headerInfo.from[0]);
		char* hexed = byteToHex(buffer, headerInfo.length);
		strncpy(buffer, hexed, headerInfo.length * 2);
		free(hexed);
		headerInfo.length *= 2;
	}
	//marshall message
	char header[HEADER_SIZE + 1];
	if (!marshall(headerInfo, header))
		return false;

	//send message
	send(clientDesc, header, HEADER_SIZE, 0);
	recv(clientDesc, header, HEADER_SIZE, 0); //ack, however this gets stuck
		//if it never gets a response TODO
	send(clientDesc, buffer, headerInfo.length, 0);
	return true;
}


bool getMessages(int clientDesc, char* from) {
	//send request
	char buffer[BUFFER_SIZE];
	dprintf(clientDesc, "To: NULL\nFrom: %s\nVersion: %04X\nLength: 0000\n",
		from, VERSION);


	if (recv(clientDesc, buffer, BUFFER_SIZE, 0) < 1) {
		perror("Message::getMessages::recv");
		return false;
	}
	dprintf(clientDesc, "Recieved");

	//parse response
	size_t lengths[1];
	sscanf(buffer, "%lu", lengths);
	if (*lengths == 0) {
		printf("You don't have any new messages\n");
		close(clientDesc);
		return false;
	}

	size_t bytesRecv = 0;
	while(bytesRecv < *lengths) {
		size_t bytesRead = prettyPrint(clientDesc, buffer);
		if (bytesRead <= 0) //error message printed by prettyPrint()
			return false;
		bytesRecv += bytesRead;
	}
	return true;
}
