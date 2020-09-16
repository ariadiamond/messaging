#include<stdio.h>
#include<stdlib.h>
#include<unistd.h> //write
#include<string.h>

#include<sys/socket.h>

#include"Util.h"

//this is solely to make Pseudoclient easier
bool sendMessages(Info* info) {
	parse_t headerInfo = createMessage(info);
	return passMessage(info, headerInfo);
}

bool passMessage(Info* info, parse_t headerInfo) {
	if (headerInfo.version == 3 || headerInfo.version == 4) { //encrypt message according to v3
		seedByteXor(info->buffer, headerInfo.length, info->key, &info->seed);
		char* hexed = byteToHex(info->buffer, headerInfo.length);
		strncpy(info->buffer, hexed, headerInfo.length * 2);
		free(hexed);
		headerInfo.length *= 2;
	}
	//marshall message
	char header[HEADER_SIZE + 1];
	if (!marshall(headerInfo, header))
		return false;

	//send message
	send(info->cdesc, header, HEADER_SIZE, 0);
	recv(info->cdesc, header, HEADER_SIZE, 0); //ack, however this gets stuck
		//if it never gets a response TODO
	send(info->cdesc, info->buffer, headerInfo.length, 0);
	return true;
}


bool getMessages(Info* info) {
	//send request
	memset(info->buffer, 0, 20); //this is to make sure parsing the number works
	dprintf(info->cdesc, "To: NULL\nFrom: %s\nVersion: %04X\nLength: 0000\n",
		info->name, VERSION);


	if (recv(info->cdesc, info->buffer, BUFFER_SIZE, 0) < 1) {
		perror("Message::getMessages::recv");
		return false;
	}
	dprintf(info->cdesc, "Recieved");

	//parse response
	size_t lengths[1];
	sscanf(info->buffer, "%lu", lengths);
	if (*lengths == 0) {
		printf("You don't have any new messages\n");
		return true;
	}

	//Receive all the messages
	size_t bytesRecv = 0;
	while(bytesRecv < *lengths) {
		size_t bytesRead = prettyPrint(info);
		if (bytesRead <= 0) //error message printed by prettyPrint()
			return false;
		bytesRecv += bytesRead;
	}
	return true;
}
