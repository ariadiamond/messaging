#include<stdio.h>
#include<string.h> //strncpy
#include<stdlib.h> //exit

#include<unistd.h> //read

#include"Util.h"

/*
 * Function Definitions
 */

void menu(char* from, char key) {
	int cdesc = createClientSock(8080);
	uint32_t seed = verify(cdesc, key, from);
	if (seed == 0) {
		dprintf(STDERR_FILENO, "Could not validate with server, exiting now\n");
		return;
	}
	bool loop = true;
	while(loop) {
		//Print menu
		printf("What would you like to do?\n");
		printf("\x1b[31m1. Send a message\n");
		printf("\x1b[32m2. Check for messages\n");
		printf("\x1b[33mq. Quit\x1b[0m\n");

		//get input
		char buffer[3];
		uint8_t option;
		read(STDIN_FILENO, buffer, 2);
		if (buffer[0] == 81 || buffer[0] == 113) //Q or q
			exit(0);
		else if (buffer[0] > 48 && buffer[0] < 58) //1-10
			option = (buffer[0] - 48);
		else {
			continue; //invalid choice
		}

		//act on the input
		switch(option) {
			case 1:
				loop = sendMessages(cdesc, from, key, &seed);
				break;
			case 2:
				loop = getMessages(cdesc, from, key, &seed);
				break;
			default:
				printf("This is not an option!");
				loop = false;
		}
	} //end while
	close(cdesc);
}

/*
 * main
 */

int main(int argc, char** argv) {
	if (argc != 3) {
		printf("Usage: %s yourName key\n", argv[0]); //config file?
		exit(1);
	}

	//parsing from
	char from[ID_SIZE + 1];
	strncpy(from, argv[1], ID_SIZE);
	from[ID_SIZE] = 0;

	//menu
	menu(from, argv[2][0]);

	return 0;
}
