#include<pthread.h>

#include<stdlib.h> //exit

//file output
#include<fcntl.h>
#include<unistd.h>

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
	close(cdesc);
	return NULL;
}

void recvMessage(int clientSock) {
	char buffer[BUFFER_SIZE];
	ssize_t bytesRead = recv(clientSock, buffer, BUFFER_SIZE, 0);
	if (bytesRead < 0)
		return;
	parse_t deconstruct;
	if (!demarshall(buffer, &deconstruct))
		return;
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
	dprintf(clientDesc, "Recieved"); //ack
	ssize_t bytesRead = recv(clientDesc, buffer, BUFFER_SIZE, 0);
	if (bytesRead < 0) {
		return;
	}

	dprintf(fdesc, "From: %s Length: %04X\n", info.from, info.length);
	write(fdesc, buffer, bytesRead);
	printf("\x1b[94m[+] To: %s From: %s Length: %04X\x1b[0m\n",
		info.to, info.from, info.length);

	close(fdesc);
}
