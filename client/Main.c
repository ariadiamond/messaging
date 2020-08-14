#include<stdio.h>
#include<string.h> //strncpy
#include<stdlib.h> //exit

#include"Util.h"

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Usage: %s yourName\n", argv[0]);
		exit(1);
	}
	char from[ID_SIZE + 1];
	strncpy(from, argv[1], ID_SIZE);
	from[ID_SIZE] = 0;
	int cdesc = clientSock(8080); //change me

	//works to here
	sendMessage(cdesc, from);

	return 0;
}
