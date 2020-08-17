#include<stdio.h>
#include<string.h> //strncpy
#include<stdlib.h> //exit

#include<unistd.h> //read

#include"Util.h"

/*
 * Function Definitions
 */

 void menu(char* from) {
    while(true) {
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
     		option = 0;
     	else if (buffer[0] > 48 && buffer[0] < 58) //1-10
     		option = (buffer[0] - 48);
     	else {
     		continue; //invalid choice
        }

        //act on the input
    	int cdesc = createClientSock(8080); //it disconnects each time TODO?
    	switch(menu()) {
    		case 0:
    			exit(0); //we can do this because there isn't really cleanup
    		case 1:
    			sendMessage(cdesc, from);
    			break;
    		case 2:
    			getMessages(cdesc, from);
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

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Usage: %s yourName\n", argv[0]); //config file?
		exit(1);
	}

    //parsing from
	char from[ID_SIZE + 1];
	strncpy(from, argv[1], ID_SIZE);
	from[ID_SIZE] = 0;

    //menu
    menu(from);
    
	return 0;
}
