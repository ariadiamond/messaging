#include<stdio.h>  //sscanf
#include<stdlib.h> //malloc
#include<string.h> //strcpy strtok

#include"Parse.h"


bool demarshall(char* header, parse_t* parsed) {
	//these block buffer overflow attacks and bus errors
	char buffer[HEADER_SIZE + 1];
	strncpy(buffer, header, HEADER_SIZE);
	buffer[HEADER_SIZE] = 0;

	//for strtok_r and sscanf
	char* savePtr;
	char* line;
	char val[HEADER_SIZE]; //to not allow for seg faults

	//parsing To
	line = strtok_r(buffer, "\n", &savePtr);
	if (line == NULL)
		return false;
	int ret = sscanf(line, "To: %s", val);
	if (ret != 1 || strlen(val) != ID_SIZE)
		return false;
	strncpy(parsed->to, val, ID_SIZE);
	parsed->to[ID_SIZE] = 0;

	//parsing From
	line = strtok_r(NULL, "\n", &savePtr);
	if (line == NULL)
		return false;
	ret = sscanf(line, "From: %s", val);
	if (ret != 1 || strlen(val) != ID_SIZE)
		return false;
	strncpy(parsed->from, val, ID_SIZE);
	parsed->from[ID_SIZE] = 0;

	//parsing version
	line = strtok_r(NULL, "\n", &savePtr);
	if (line == NULL)
		return false;
	uint16_t version[1];
	ret = sscanf(line, "Version: %hX", version);
	if (ret != 1)
		return false;
	parsed->version = version[0];

	//parsing Length
	line = strtok_r(NULL, "\n", &savePtr);
	if (line == NULL)
		return false;
	uint16_t messageSize[1];
	ret = sscanf(line, "Length: %hX", messageSize);
	if (ret != 1)
		return false;
	parsed->length = messageSize[0];

	return true;
}

bool marshall(parse_t items, char* header) {
	snprintf(header, HEADER_SIZE + 1, "To: %s\nFrom: %s\nVersion: %04X\nLength: %04X\n",
		items.to, items.from, items.version, items.length);
	header[HEADER_SIZE] = 0;
	return true;
}
