#ifndef _SHARED_H_
#define _SHARED_H_

/*
 * Include
 */

#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h> //size_t

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


//Parse.c
bool demarshall(char* header, parse_t* parsed);
bool marshall(parse_t items, char* header);

//Crypt.c
void xorShift(uint32_t* seed);
void seedByteXor(char* bytes, size_t numBytes, char key, uint32_t* seed);
void byteXor(char* bytes, size_t numBytes, char key);

//Convert.c
char* hexToByte(char* bytes, size_t numChars);
char* byteToHex(char* hex, size_t numChars);

#endif
