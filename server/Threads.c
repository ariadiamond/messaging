#include<string.h> //strcmp
#include<stdlib.h> //exit

//file output
#include<fcntl.h>
#include<unistd.h> //pread pwrite
#include<sys/stat.h>

#include<sys/socket.h> //recv
#include<pthread.h>

#include"Util.h"

/*
 * Function Declaraions
 */

void recvMessage(int clientSock);
// Someone sending a message
void writeMessage(parse_t info, int clientDesc, char* buffer);
// Someone requesting their messages
void getMessages(int clientSock, char* from, char* buffer);

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
	ssize_t bytesRead = recv(clientSock, buffer, HEADER_SIZE, 0);
	if (bytesRead < 0)
		return;

	parse_t deconstruct;
	if (!demarshall(buffer, &deconstruct)) {
		printf("\x1b[31mDoesn't demarshall\x1b[0m\n");
		return;
	}

	if (strncmp("NULL", deconstruct.to, ID_SIZE) == 0) //how do you make sure the client doesn't send this
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
		perror("Threads::writeMessage::open");
		exit(1);
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

/*
 * Someone requested their messages
 */

void getMessages(int clientSock, char* from, char* buffer) {
	printf("\x1b[32m[-] Request From: %s\x1b[0m\n", from);
	int fdesc = open(from, O_RDONLY);
	if (fdesc < 0) {
		return;
	}

	struct stat stats;
	int checkVal = fstat(fdesc, &stats);
	if (checkVal != 0) {
		//why though?
		return;
	}

	dprintf(clientSock, "%lu", stats.st_size);

	ssize_t bytesRead = 0;
	do {
		bytesRead = read(fdesc, buffer, BUFFER_SIZE);
		send(clientSock, buffer, bytesRead, 0);
	} while(bytesRead != 0);
	close(fdesc);

	fdesc = open(from, O_WRONLY | O_TRUNC);
	close(fdesc);
}