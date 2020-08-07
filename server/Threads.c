#include<pthread.h>

#include<stdlib.h> //exit

//file output
#include<fcntl.h>
#include<unistd.h>
//stdio //dprintf

#include<sys/socket.h> //recv

#include"Util.h"

/*
 * Function Declaraions
 */
void writeMessage(parse_t info, int clientDesc, char* buffer);
void recvMessage(int clientSock);

/*
 * Function Definitions
 */

void* threadRunner(void* parg) {
	int cdesc = ((int*) parg)[0];
	recvMessage(cdesc);
	//Do you want to ack?
	close(cdesc);
	return NULL;
}

void recvMessage(int clientSock) {
	char buffer[BUFFER_SIZE];
	size_t bytesRead = recv(clientSock, buffer, BUFFER_SIZE, 0);
	parse_t deconstruct = demarshall(buffer);
	writeMessage(deconstruct, clientSock, buffer);
}


void writeMessage(parse_t info, int clientDesc, char* buffer) {
	int fdesc = open(info.to, O_WRONLY | O_APPEND);
	if (fdesc < 0) {
		switch(errno) {
			case ENOENT:
				fdesc = open(info.to, O_WRONLY | O_CREAT, 0644);
				if (fdesc != -1) //this warning is intentional
					break;
			default:
				perror("Threads::writeMessage::open");
				exit(1);
		}
	}
	dprintf(fdesc, "From: %s\nLength: %du", info.from, info.length);
	ssize_t bytesRead = recv(clientDesc, buffer, BUFFER_SIZE, 0);
	if (bytesRead < 0) {

	}
	write(fdesc, buffer, bytesRead);

	close(fdesc);
}