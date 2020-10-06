#ifndef _PARSE_H_
#define _PARSE_H_

/*
 * Include
 */

#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h> //is size_t in here?

/*
 * Define
 */

#define ID_SIZE 4
#define HEADER_SIZE 48

/*
 * parse_t struct
 */

typedef struct parse_t {
	char to[ID_SIZE + 1];
	char from[ID_SIZE + 1];
	uint16_t version; //this might be useful, especially with encryption method
	uint16_t length; //I'm only doing max 4096 byte messages
} parse_t;


/*
 * Functions
 */

bool demarshall(char* header, parse_t* parsed);
bool marshall(parse_t items, char* header);

//Convert.c
char* hexToByte(char* bytes, size_t numChars);
char* byteToHex(char* nex, size_t numChars);


//Encrypt.c
void ceaser(char* bytes, size_t numChars, int spin);

#endif
