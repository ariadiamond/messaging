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

parse_t createMessage(Info* info) {
	//setup
	parse_t header;
	header.version = VERSION;
	strncpy(header.from, info->name, ID_SIZE);
	header.from[ID_SIZE] = 0;

	//who to send the message to
	printf("Who would you like to send a message to?\n");
	read(STDIN_FILENO, info->buffer, BUFFER_SIZE);
	strncpy(header.to, info->buffer, ID_SIZE);
	header.to[ID_SIZE] = 0;

	//I'm lazy, so recursion is how I'm handling invalid input
	if (!validName(header.to)) {
		printf("That's not a valid name\n");
		return createMessage(info);
	}

	//actual message input
	printf("What would you like to send to \x1b[35m%s\x1b[0m?\n", header.to);
	ssize_t bytesRead = read(STDIN_FILENO, info->buffer, BUFFER_SIZE / 2);
	header.length = bytesRead;

	return header;
}

/*
 * For getting your messages from the server
 */

size_t prettyPrint(Info* info) {
	//getting header
	//this will be helpful when versions are relevant for decryption
	if (recv(info->cdesc, info->buffer, HEADER_SIZE * 2, 0) != HEADER_SIZE * 2) {
		perror("MessageHelper::prettyPrint::recv1");
		return 0;
	}
	char* bytes = hexToByte(info->buffer, HEADER_SIZE * 2);
	seedByteXor(bytes, HEADER_SIZE, info->key, &info->seed);

	parse_t header;
	if (!demarshall(bytes, &header)) {
		printf("%s\n", bytes);
		printf("Someone doesn't know how to format :O\n");
		return 0;
	}

	//get the body
	if (recv(info->cdesc, info->buffer, header.length, 0) != (ssize_t) header.length) {
		perror("MessageHelper::prettyPrint::recv2");
		return 0;
	}

	//print the messages
	printf("From: \x1b[35m%s\x1b[0m\n", header.from);
	if (header.version == 3) { //decrypt according to v3
		char* unhexed = hexToByte(info->buffer, header.length);
		seedByteXor(unhexed, header.length / 2, info->key, &info->seed);
		write(STDOUT_FILENO, unhexed, header.length / 2);
	} else { //does not decrypt
		write(STDOUT_FILENO, info->buffer, header.length);
	}

	//return bytes received so we don't wait for something that will never come
	return HEADER_SIZE + (header.length / 2);
}