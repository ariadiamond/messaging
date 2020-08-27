#include<string.h> //strcmp
#include<stdlib.h> //exit
#include<stdio.h> //remove
#include<errno.h>

//file output
#include<fcntl.h>
#include<unistd.h> //pread pwrite
#include<sys/stat.h>

#include<sys/socket.h> //recv
#include<pthread.h>

#include"Util.h"
#include"../shared/Crypt.h"


extern CLArgs args;
extern int errno;

/*
 * Function Declaraions
 */

// Someone sending a message
bool writeMessage(parse_t info, int clientDesc, char* buffer);
// Someone requesting their messages
bool getMessages(int clientSock, char* from, char* buffer);
// Someone registering
bool registerUser(int clientSock, char* from, char* buffer);

/*
 * Function Definitions
 */

bool recvMessage(int clientSock) {
	//
	char buffer[BUFFER_SIZE + 1];
	ssize_t bytesRead = recv(clientSock, buffer, HEADER_SIZE, 0);
	if (bytesRead <= 0)
		return false;

	//demarshall the request
	parse_t deconstruct;
	if (!demarshall(buffer, &deconstruct)) {
		if (args.logging) {
			int errdesc = open(ERR_FILE, O_WRONLY | O_APPEND);
			dprintf(errdesc,"\n\nDoesn't demarshall\n");
			write(errdesc, buffer, bytesRead);
			dprintf(errdesc, "\n");
			close(errdesc);
		}
		return false;
	}

	//do the action requested
	if (strncmp("NULL", deconstruct.to, ID_SIZE) == 0)
		return getMessages(clientSock, deconstruct.from, buffer);
	else if (strncmp("CREA", deconstruct.to, ID_SIZE) == 0)
		return registerUser(clientSock, deconstruct.from, buffer);
	else
		return writeMessage(deconstruct, clientSock, buffer);
}

/*
 * Someone sends a message
 */

bool writeMessage(parse_t info, int clientDesc, char* buffer) {
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
		exit(1); //exits because it's a big error if you cannot write
	}

	//putting the marshalled header because it is easier after requests
	write(fdesc, buffer, HEADER_SIZE);


	//This ack is because it expects the header, then the message body in 2
	//separate messages (also such that the buffer can be reused)
	dprintf(clientDesc, "Recieved");

	//get full message
	ssize_t bytesRead = recv(clientDesc, buffer, BUFFER_SIZE, 0);
	if (bytesRead <= 0)
		return false;

	//write response
	write(fdesc, buffer, bytesRead);
	printf("\x1b[94m[+] To: %s From: %s Length: %04X\x1b[0m\n",
		info.to, info.from, info.length);

	close(fdesc);
	return true;
}

/*
 * Someone requested their messages
 */

bool getMessages(int clientSock, char* from, char* buffer) {
	printf("\x1b[32m[-] Request From: %s\x1b[0m\n", from);

	// open file
	int fdesc = open(from, O_RDONLY);
	if (fdesc < 0) {
		return false;
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
		return false;
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
	return true;
}

bool keyContains(char* from) {
	int kdesc = open(KEY_FILE, O_RDONLY);
	ssize_t bytesRead = 0;
	const int ENTRY_SIZE = 8;
	char buffer[ENTRY_SIZE + 1];
	do {
		bytesRead = read(kdesc, buffer, ENTRY_SIZE);
		char name[ID_SIZE + 1];
		if (sscanf(buffer, "%s", name) != 1) {
			dprintf(STDERR_FILENO, "Improperly formatted key file\n");
			exit(1);
		}
		if (strncmp(name, from, ID_SIZE) == 0) {
			return true;
		}
	} while (bytesRead > 0);
	return false;
}


bool registerUser(int clientSock, char* from, char* buffer) {
	if (keyContains(from)) {
		//already has a key
		return false;
	}
	uint32_t seed = xorShift(args.seed);
	dprintf(clientSock, "%u", seed);
	recv(clientSock, buffer, BUFFER_SIZE, 0);

	uint32_t key = 0;
	if (sscanf(buffer, "%u", &key) != 1) {
		//not correct format
		return false;
	}
	seed = seed - key;
	char k1 = (seed & 0xFF000000) >> 24;
	char k2 = (seed & 0x00FF0000) >> 16;
	char k3 = (seed & 0x0000FF00) >> 8;
	char k4 = seed  & 0x000000FF;
	if (k1 != k2 || k1 != k3 || k1 != k4) {
		//inconsistent key
		return false;
	}

	int kdesc = open(KEY_FILE, O_WRONLY | O_APPEND);
	dprintf(kdesc, "%s: %c\n", from, k1);
	close(kdesc);
	return true;
}
