#include<stdio.h>  //sscanf
#include<stdlib.h> //malloc
#include<string.h> //strcpy strtok

#include"Parse.h"


//TODO error checking
parse_t demarshall(char* buffer) {
	parse_t items;
	char** savePtr = 0;
	char* line;
	char val[ID_LEN];
	line = strtok_r(buffer, "\n", savePtr);
	sscanf(line, "From: %s", val);
	strncpy(items.from, val, ID_LEN);

	line = strtok_r(NULL, "\n", savePtr);
	sscanf(line, "To: %s", val);
	strncpy(items.to, val, ID_LEN);

	uint16_t messageSize[1];
	line = strtok_r(NULL, "\n", savePtr);
	sscanf(line, "Length: %hu", messageSize);
	items.length = messageSize[0];

	return items;
}

char* marshall(parse_t items) {
	// Assuming length is 32bits
	char* message = (char*) malloc(sizeof(char) * 34);

	snprintf(message, 34, "To: %s\nFrom: %s\nLength: %hu\n",
													items.to, items.from, items.length);
	return message;

}