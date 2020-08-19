#include<unistd.h> //read

#include<stdio.h>
#include<string.h> //strncpy strncmp

#include"Util.h"

bool validName(char* name) {
	if (strncmp(name, "NULL", ID_SIZE) == 0) //Protects getting messages
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
	ssize_t bytesRead = read(STDIN_FILENO, buffer, BUFFER_SIZE);
	info.length = bytesRead;
	return info;
}
