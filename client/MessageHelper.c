#include<unistd.h> //read

#include<stdio.h>
#include<string.h> //strncpy strncmp

#include<sys/socket.h>

#include"Util.h"

/*
 * For sending messages
 */

bool validName(char* name) {
	if (strncmp(name, "NULL", ID_SIZE) == 0) //Protects getting messages
		return false;

	if (strncmp(name, "CREA", ID_SIZE) == 0)
		return false;

	for (uint8_t i = 0; i < ID_SIZE; i++) { //unreadable characters (and space)
		if (name[i] < 32 || name[i] > 126)
			return false;
	}
	return true;
}

parse_t createMessage(char* buffer, char* from) {
	//setup
	parse_t info;
	info.version = VERSION;
	strncpy(info.from, from, ID_SIZE);
	info.from[ID_SIZE] = 0;

	//who to send the message to
	printf("Who would you like to send a message to?\n");
	read(STDIN_FILENO, buffer, BUFFER_SIZE);
	strncpy(info.to, buffer, ID_SIZE);
	info.to[ID_SIZE] = 0;

	//I'm lazy, so recursion is how I'm handling invalid input
	if (!validName(info.to)) {
		printf("That's not a valid name\n");
		return createMessage(buffer, from);
	}

	//actual message input
	printf("What would you like to send to \x1b[35m%s\x1b[0m?\n", info.to);
	ssize_t bytesRead = read(STDIN_FILENO, buffer, BUFFER_SIZE / 2);
	info.length = bytesRead;

	return info;
}

/*
 * For getting your messages from the server
 */

size_t prettyPrint(int clientDesc, char* buffer) {
	//getting header
	//this will be helpful when versions are relevant for decryption
	if (recv(clientDesc, buffer, HEADER_SIZE, 0) != HEADER_SIZE) {
		perror("MessageHelper::prettyPrint::recv1");
		return 0;
	}
	parse_t info;
	if (!demarshall(buffer, &info)) {
		printf("%s\n", buffer);
		printf("Someone doesn't know how to format :O\n");
		return 0;
	}

	//get the body
	if (recv(clientDesc, buffer, info.length, 0) != (ssize_t) info.length) {
		perror("MessageHelper::prettyPrint::recv2");
		return 0;
	}

	//print the messages
	printf("From: \x1b[35m%s\x1b[0m\n", info.from);
	if (info.version == 2) {
		char* unhexed = hexToByte(buffer, info.length);
		byteXor(unhexed, info.length / 2, info.from[0]);
		write(STDOUT_FILENO, unhexed, info.length / 2);
	} else if (info.version == 1) {
		write(STDOUT_FILENO, buffer, info.length);
	}

	//return bytes received so we don't wait for something that will never come
	return HEADER_SIZE + info.length;
}