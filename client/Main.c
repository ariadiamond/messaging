#include<stdio.h>
#include<string.h> //strncpy
#include<stdlib.h> //exit

#include<unistd.h> //read

#include"Util.h"

/*
 * Function Definitions
 */

void menu(Info info) {
	info.cdesc = createClientSock(8080);
	if (!verify(&info)) {
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
		uint8_t option;
		read(STDIN_FILENO, info.buffer, BUFFER_SIZE);
		if (info.buffer[0] == 81 || info.buffer[0] == 113) //Q or q
			exit(0);
		else if (info.buffer[0] > 48 && info.buffer[0] < 58) //1-10
			option = (info.buffer[0] - 48);
		else {
			continue; //invalid choice
		}

		//act on the input
		switch(option) {
			case 1:
				loop = sendMessages(&info);
				break;
			case 2:
				loop = getMessages(&info);
				break;
			default:
				printf("This is not an option!");
				loop = false;
		}
	} //end while
	close(info.cdesc);
}

/*
 * main
 */

int main(int argc, char** argv) {
	if (argc != 3) {
		printf("Usage: %s yourName key\n", argv[0]); //config file?
		exit(1);
	}
	Info info = {
		.key = argv[2][0] };
	//parsing from
	strncpy(info.name, argv[1], ID_SIZE);
	info.name[ID_SIZE] = 0;

	//menu
	menu(info);

	return 0;
}
