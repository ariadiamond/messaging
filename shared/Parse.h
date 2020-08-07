#ifndef _PARSE_H_
#define _PARSE_H_

/*
 * Include
 */

#include<stdint.h>

/*
 * Define
 */

#define ID_LEN 4
#define BUFFER_SIZE 4096

/*
 * parse_t struct
 */

typedef struct parse_t {
	char from[ID_LEN];
	char to[ID_LEN];
	uint16_t length; //I'm only doing max 4096 byte messages
} parse_t;


/*
 * Functions
 */

parse_t demarshall(char* buffer);
char* marshall(parse_t items);

#endif
