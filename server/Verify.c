#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<stdio.h>

#include"Util.h"

extern CLArgs args;


char getKey(char* name) {
	//setup
	int fdesc = open(KEY_FILE, O_RDONLY); //I hope this works!
	const int ENTRY_LEN = 8;
	char buffer[ENTRY_LEN + 1];

	//iterate through each k,v pair to check if the name works
	while (true) {
		if (read(fdesc, buffer, ENTRY_LEN) != ENTRY_LEN) {
			close(fdesc);
			return 0; //if the key doesn't exist
		}

		buffer[ENTRY_LEN] = 0;
		if (strncmp(buffer, name, ID_SIZE) == 0)
			break;
	}
	close(fdesc);

	return buffer[6];
}

bool verifyName(ClientInfo* client) {
	//grab the seed for this client and update it for the next
	xorShift(&args.seed);
	client->seed = args.seed;
	args.seed += 193;

	//send the seed to the client and skip to the next
	dprintf(client->cdesc, "%u", client->seed);
	xorShift(&client->seed);

	char buffer[36];
	if (recv(client->cdesc, buffer, 35, 0) <= 0)
		return false;

	//parse the name
	buffer[35] = 0; //null terminate to protect with sscanf
	char name[36]; //has to be this length because possible buffer overflow
	if (sscanf(buffer, "%s", name) != 1)
		return false;
	strncpy(client->name, name, ID_SIZE);
	client->name[ID_SIZE + 1] = 0;

	//grab key for the client
	client->key = getKey(client->name);
	if (client->key == 0)
		return false;

	//encrypt with the key and seed pair to check if valid key
	char compare[31];
	strncpy(compare, "Hello friendo!", 15); //it's 15 to include the \0
	seedByteXor(compare, 15, client->key, &client->seed);
	char* hexed = byteToHex(compare, 15);
	if (strncmp(hexed, (buffer + ID_SIZE), 30) != 0)
		return false;

	//tell the client things worked :)
	strcpy(buffer, ":)");
	send(client->cdesc, buffer, 2, 0);
	return true;
}