#include<stdio.h>
#include<string.h> //strncpy
#include<stdlib.h> //exit

#include<fcntl.h> //open
#include<unistd.h> //read

#include<errno.h>

#include"Util.h"

/*
 * User Config
 */

typedef struct config_t {
    char from[ID_SIZE + 1];
    char* address;
    uint16_t port;
} config_t;

/*
 * Function Definitions
 */

static inline void usage() {
    if (errno == 0)
        errno = EINVAL;
    perror("Please run \x1b[30;47mmake config\x1b[0m before running the client");
    exit(1);
}

config_t parseConfig() {
    config_t items;

    //read config file
    int fdesc = open(USER_CONFIG, O_RDONLY);
    if (fdesc < 1) {
        usage();
    }
    char buffer[BUFFER_SIZE + 1];
    read(fdesc, buffer, BUFFER_SIZE);
    close(fdesc);

    //get name
    char* line = strtok(buffer, "\n");
    if (line == NULL)
        usage();
    if (sscanf(line, "Name: %s", items.from) != 1)
        usage();

    //get address
    line = strtok(NULL, "\n");
    if (line == NULL)
        usage();
    items.address = malloc(sizeof(char) * strlen(line));
    if (sscanf(line, "Address: %s", items.address) != 1)
        usage();

    //get port
    line = strtok(NULL, "\n");
    if (line == NULL)
        usage();
    if (sscanf(line, "Port: %hu", &(items.port)) != 1)
        usage();

    return items;
}



void menu(config_t config) {
    while(true) {
        //Print menu
     	printf("\nWhat would you like to do?\n");
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
        printf("\n");

        //act on the input
    	int cdesc = createClientSock(config.address, config.port); //it disconnects each time TODO?
    	switch(option) {
    		case 1:
    			sendMessage(cdesc, config.from);
    			break;
    		case 2:
    			getMessages(cdesc, config.from);
    			break;
    		default:
    			printf("This is not an option!");
    	}
        close(cdesc);
    } //end while
}

/*
 * main
 */

int main(void) {
    //config parsing
    config_t config = parseConfig();
    printf("Hi \x1b[36m%s\x1b[0m!\n", config.from);

    //menu
    menu(config);

	return 0;
}
