#ifndef _PARSE_H_
#define _PARSE_H_

/*
 * Include
 */

#include<stdint.h>
#include<stdbool.h>

/*
 * Define
 */

#define ID_SIZE 4
#define HEADER_SIZE 34

/*
 * parse_t struct
 */

typedef struct parse_t {
	char to[ID_SIZE + 1];
	char from[ID_SIZE + 1];
	uint16_t length; //I'm only doing max 4096 byte messages
} parse_t;


/*
 * Functions
 */

bool demarshall(char* header, parse_t* parsed);
bool marshall(parse_t items, char* header);

#endif

/* TODO:
 * return int and set errno
 * more tests
 * how to deal with invalid lengths
 */
