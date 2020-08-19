#include<string.h> //strcmp
#include<stdlib.h> //exit

//file output
#include<fcntl.h>
#include<unistd.h> //pread pwrite
#include<sys/stat.h>

#include<sys/socket.h> //recv
#include<pthread.h>

#include"Util.h"

extern CLArgs args;
extern int errno;

/*
 * Function Declaraions
 */

// Someone sending a message
void writeMessage(parse_t info, int clientDesc, char* buffer);
// Someone requesting their messages
void getMessages(int clientSock, char* from, char* buffer);

/*
 * Function Definitions
 */

void recvMessage(int clientSock) {
	//
	char buffer[BUFFER_SIZE];
	ssize_t bytesRead = recv(clientSock, buffer, HEADER_SIZE, 0);
	if (bytesRead < 0)
		return;

    //demarshall the request
	parse_t deconstruct;
	if (!demarshall(buffer, &deconstruct)) {
        if (args.logging) {
            int errdesc = open(ERR_FILE, O_WRONLY | O_APPEND);
    		dprintf(errdesc, "==========\nDoesn't demarshall\n");
            write(errdesc, buffer, bytesRead);
            close(errdesc);
        }
		return;
	}

    //do the action requested
	if (strncmp("NULL", deconstruct.to, ID_SIZE) == 0)
		getMessages(clientSock, deconstruct.from, buffer);
	else
		writeMessage(deconstruct, clientSock, buffer);
}

/*
 * Someone sends a message
 */

void writeMessage(parse_t info, int clientDesc, char* buffer) {
	int fdesc = open(info.to, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fdesc < 0) {
		perror("Threads::writeMessage::openFile");
        if (args.logging) {
            strerror_r(errno, buffer, BUFFER_SIZE);
            int errdesc = open(ERR_FILE, O_WRONLY | O_APPEND);
            dprintf(errdesc, "==========\nCannot open (or create) %s: %s\n",
                info.from, buffer);
            close(errdesc);
        }
		exit(1);
	}

    //This ack is because it expects the header, then the message body in 2
    //separate things (also such that the buffer can be reused)
	dprintf(clientDesc, "Recieved");

    //get full message
	ssize_t bytesRead = recv(clientDesc, buffer, BUFFER_SIZE, 0);
	if (bytesRead < 0) {
		return;
	}

    //write response
	dprintf(fdesc, "From: %s Length: %04X\n", info.from, info.length);
	write(fdesc, buffer, bytesRead);
	printf("\x1b[94m[+] To: %s From: %s Length: %04X\x1b[0m\n",
		info.to, info.from, info.length);

	close(fdesc);
}

/*
 * Someone requested their messages
 */

void getMessages(int clientSock, char* from, char* buffer) {
	printf("\x1b[32m[-] Request From: %s\x1b[0m\n", from);

	// open file
	int fdesc = open(from, O_RDONLY);
	if (fdesc < 0) {
		return;
	}

	// Send how many bytes are going to be sent
	struct stat stats;
	if (fstat(fdesc, &stats) != 0) {
		if (args.logging) {
            strerror_r(errno, buffer, BUFFER_SIZE);

            int errdesc = open(ERR_FILE, O_WRONLY | O_APPEND);
            dprintf(errdesc, "==========\nFstat error: %s\n", buffer);
            close(errdesc);
        }
		return;
	}

	dprintf(clientSock, "%lu", stats.st_size);

    //This is necessary because otherwise the client might get size and messages
    //in the same buffer (which is not how it's designed)
    recv(clientSock, buffer, BUFFER_SIZE, 0);

	//send bytes over
	ssize_t bytesRead = 0;
	do {
		bytesRead = read(fdesc, buffer, BUFFER_SIZE);
		send(clientSock, buffer, bytesRead, 0);
	} while(bytesRead != 0);
	close(fdesc);

	//clean up
    if (args.remove)
	   remove(from);
}