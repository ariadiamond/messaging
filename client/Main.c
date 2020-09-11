#include<stdio.h>
#include<string.h> //strncpy
#include<stdlib.h> //exit

#include<fcntl.h> //open
#include<unistd.h> //read

#include<errno.h>

#include"Util.h"

/*
 * Function Definitions
 */

static inline void usage() {
	if (errno == 0)
		errno = EINVAL;
	perror("Please run \x1b[30;47mmake config\x1b[0m before running the client");
	exit(1);
}

Info parseConfig() {
	Info info;

	//read config file
	int fdesc = open(USER_CONFIG, O_RDONLY);
	if (fdesc < 1) {
		usage();
	}

	read(fdesc, info.buffer, BUFFER_SIZE);
	close(fdesc);

	//get name
	char* line = strtok(info.buffer, "\n");
	if (line == NULL)
		usage();
	if (sscanf(line, "Name: %s", info.name) != 1)
		usage();
	info.name[ID_SIZE] = 0;

	//get key
	line = strtok(NULL, "\n");
	if (line == NULL)
		usage();
	if (sscanf(line, "Key: %c", &(info.key)) != 1)
		usage();

	//get address
	line = strtok(NULL, "\n");
	if (line == NULL)
		usage();
	info.address = malloc(sizeof(char) * strlen(line));
	if (sscanf(line, "Address: %s", info.address) != 1)
		usage();

	//get port
	line = strtok(NULL, "\n");
	if (line == NULL)
		usage();
	if (sscanf(line, "Port: %hu", &(info.port)) != 1)
		usage();

	return info;
}


void menu(Info info) {
	info.cdesc = createClientSock(info.address, info.port);
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

int main(void) {

	//config parsing
	Info config = parseConfig();
	printf("Hi \x1b[36m%s\x1b[0m!\n", config.name);

	//menu
	menu(config);
	return 0;
}
