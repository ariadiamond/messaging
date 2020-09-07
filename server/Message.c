#include<string.h> //strcmp
#include<stdlib.h> //exit
#include<stdio.h> //remove
#include<errno.h>

//file output
#include<fcntl.h>
#include<unistd.h> //pread pwrite
#include<sys/stat.h>

#include<sys/socket.h> //recv
//#include<pthread.h>

#include"Util.h"
#include"../shared/Crypt.h"


extern CLArgs args;

/*
 * Function Declaraions
 */

// Someone sending a message
bool writeMessage(ClientInfo* client, parse_t info);
// Someone requesting their messages
bool getMessages(ClientInfo* client);

/*
 * Function Definitions
 */

bool recvMessage(ClientInfo* client) {
	ssize_t bytesRead = recv(client->cdesc, client->buffer, HEADER_SIZE, 0);
	if (bytesRead <= 0)
		return false;

	//demarshall the request
	parse_t deconstruct;
	if (!demarshall(client->buffer, &deconstruct)) {
		if (args.logging) {
			int errdesc = open(ERR_FILE, O_WRONLY | O_APPEND);
			dprintf(errdesc,"\n\nDoesn't demarshall\n");
			write(errdesc, client->buffer, bytesRead);
			dprintf(errdesc, "\n");
			close(errdesc);
		}
		return false;
	}
	if (strncmp(client->name, deconstruct.from, ID_SIZE) != 0) {
		return false; //name does not match
	}

	//do the action requested
	if (strncmp("NULL", deconstruct.to, ID_SIZE) == 0)
		return getMessages(client);
	else
		return writeMessage(client, deconstruct);
}

/*
 * Someone sends a message
 */

bool writeMessage(ClientInfo* client, parse_t info) {
	int fdesc = open(info.to, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fdesc < 0) {
		perror("Threads::writeMessage::openFile");
		if (args.logging) {
			strerror_r(errno, client->buffer, BUFFER_SIZE);
			int errdesc = open(ERR_FILE, O_WRONLY | O_APPEND);
			dprintf(errdesc, "==========\nCannot open (or create) %s: %s\n",
				info.from, client->buffer);
			close(errdesc);
		}
		exit(1); //exits because it's a big error if you cannot write
	}

	//putting the marshalled header because it is easier after requests

	write(fdesc, client->buffer, HEADER_SIZE);

	//This ack is because it expects the header, then the message body in 2
	//separate messages (also such that the buffer can be reused)
	dprintf(client->cdesc, "Recieved");

	//get full message
	ssize_t bytesRead = recv(client->cdesc, client->buffer, BUFFER_SIZE, 0);
	if (bytesRead <= 0)
		return false;

	//write response
	if (info.version == 3) {
		char* byte = hexToByte(client->buffer, info.length);
		seedByteXor(byte, info.length / 2, client->key, &client->seed);
		write(fdesc, byte, info.length / 2);
	}

	printf("\x1b[94m[+] To: %s From: %s Length: %04X\x1b[0m\n",
		info.to, info.from, info.length);

	close(fdesc);
	return true;
}

/*
 * Someone requested their messages
 */

bool getMessages(ClientInfo* client) {
	printf("\x1b[32m[-] Request From: %s\x1b[0m\n", client->name);

	// open file
	int fdesc = open(client->name, O_RDONLY);
	if (fdesc < 0) {
		switch(errno) {
			case ENOENT: //if there are no messages
				dprintf(client->cdesc, "0");
				return true;
			default: //something else, this is not good TODO: error log
				return false;
		}
	}

	// Send how many bytes are going to be sent
	struct stat stats;
	if (fstat(fdesc, &stats) != 0) {
		if (args.logging) {
			strerror_r(errno, client->buffer, BUFFER_SIZE);

			int errdesc = open(ERR_FILE, O_WRONLY | O_APPEND);
			dprintf(errdesc, "==========\nFstat error: %s\n", client->buffer);
			close(errdesc);
		}
		return false;
	}

	dprintf(client->cdesc, "%lu", stats.st_size);

	//This is necessary because otherwise the client might get size and messages
	//in the same buffer (which is not how it's designed)
	recv(client->cdesc, client->buffer, BUFFER_SIZE, 0);

	//send bytes over
	//this complexity is necessary (ie reading each individual message) because
	//otherwise there is a mismatch with how the client decrypts the messages
	//and how they are encoded (see messages that are not mod 4 == 0).
	while (true) {
		//get the length and send over the header
		if (read(fdesc, client->buffer, HEADER_SIZE) != HEADER_SIZE)
			break;
		client->buffer[HEADER_SIZE] = 0;
		parse_t info;
		demarshall(client->buffer, &info);
		seedByteXor(client->buffer, HEADER_SIZE, client->key, &client->seed);
		char* hex = byteToHex(client->buffer, HEADER_SIZE);
		send(client->cdesc, hex, HEADER_SIZE * 2, 0);
		free(hex);

		//send over the message
		if (read(fdesc, client->buffer, info.length / 2) != info.length / 2)
			return false;
		seedByteXor(client->buffer, info.length / 2, client->key, &client->seed);
		hex = byteToHex(client->buffer, info.length / 2);
		send(client->cdesc, hex, info.length, 0);
		free(hex);

	}
	close(fdesc);

	//clean up
	if (args.remove)
		unlink(client->name);

	return true;
}
