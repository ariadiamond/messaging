#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<stdio.h>
#include<sys/stat.h>

#include"Util.h"

extern CLArgs args;
const int ENTRY_LEN = 8;

const uint8_t MSG_LEN = 20;
const char MSG[21] = {"I just want a hug :/"};

char getKey(char* name) {
	//setup
	int fdesc = open(KEY_FILE, O_RDONLY); //I hope this works!
	char buffer[ENTRY_LEN + 1];

	//iterate through each k,v pair to check if the name works
	while (true) {
		if (read(fdesc, buffer, ENTRY_LEN) != ENTRY_LEN) {
			close(fdesc);
			return 0; //if the key doesn't exist
		}

		if (strncmp(buffer, name, ID_SIZE) == 0)
			break;
	}
	close(fdesc);

	return buffer[6];
}

//reverse lookup
bool getName(char key, char* name) {
	int fdesc = open(KEY_FILE, O_RDONLY);
	char buffer[ENTRY_LEN + 1];

	while (true) {
		if (read(fdesc, buffer, ENTRY_LEN) != ENTRY_LEN) { //we didn't find it
			close(fdesc);
			return NULL;
		}

		if (buffer[6] == key) { //we found it!
			strncpy(name, buffer, ID_SIZE);
			return true;
		}
	}
	//it should never get here
	return false;

}

char* getKeys(size_t* numKeys) {
	//this is possible because IDs and keys are constant length
	int fdesc = open(KEY_FILE, O_RDONLY);
	struct stat stats;
	fstat(fdesc, &stats);
	*numKeys = stats.st_size / ENTRY_LEN;
	char* keys = malloc(*numKeys); //we don't need '\0'

	//read each key
	char buffer[ENTRY_LEN + 1];
	for (size_t i = 0; i < *numKeys; i++) { //can escape loop in 2 ways
		if (read(fdesc, buffer, ENTRY_LEN) != ENTRY_LEN)
			break;
		keys[i] = buffer[6]; //grab the key
	}


	close(fdesc);
	return keys;
}

bool verifyName(ClientInfo* client) {
	//get encrypted message and seed
	ssize_t bytesRead = recv(client->cdesc, client->buffer, BUFFER_SIZE, 0);
	if (bytesRead <= 0)
		return false;
	client->buffer[bytesRead] = 0;

	//get the seed (theoretically the xored seed could be 0)
	client->seed = atoi(client->buffer + (MSG_LEN * 2));
	if (client->seed == 0)
		return false;

	//get and iterate through all the keys the server has -- if the number of
	//keys in the keyspace is small, this makes it take longer for someone who
	//doesn't know which keys are used to decrypt.
	size_t numKeys = 0;
	char* keys = getKeys(&numKeys);
	for (size_t i = 0; i < numKeys; i++) {
		//I could just make it byte once, but i still have to copy it to a fresh
		//buffer each time to not mess it up
		char* buffer = hexToByte(client->buffer, MSG_LEN * 2);

		//decrypt the things according to the key
		uint32_t seed = client->seed ^ ((keys[i] << 24) |
			(keys[i] << 16) | (keys[i] << 8) | (keys[i]));
		seedByteXor(buffer, MSG_LEN, keys[i], &seed);

		//check if the right key
		if (strncmp(buffer, MSG, MSG_LEN) == 0) {
			//copy the key and seed
			client->key  = keys[i];
			client->seed = seed;

			//we found the key, but miraculously it is now gone
			if (!getName(client->key, client->name))
				return false;

			//everything worked :)
			dprintf(client->cdesc, ":)");
			free(keys);
			return true;
		}

	}

	//it iterated through all the keys and did not find one that worked
	free(keys);
	return false;
}