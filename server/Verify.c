#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<stdio.h>

#include"Util.h"

extern CLArgs args;

char getKey(char* name) {
	int fdesc = open(KEY_FILE, O_RDONLY);

	const int ENTRY_LEN = 8;
	char buffer[ENTRY_LEN + 1];
	while (true) {
		if (read(fdesc, buffer, ENTRY_LEN) != ENTRY_LEN) {
			close(fdesc);
			return 0;
		}
		buffer[ENTRY_LEN] = 0;
		if (strncmp(buffer, name, ID_SIZE) == 0)
			break;
	}
	close(fdesc);

	return buffer[6];
}



bool verifyName(ClientInfo* client) {
	xorShift(&args.seed);
	client->seed = args.seed;
	args.seed += 193;
	dprintf(client->cdesc, "%u", client->seed);
	xorShift(&client->seed);
	char buffer[36]; //too long
	if (recv(client->cdesc, buffer, 35, 0) <= 0)
		return false;

	char name[37];
	if (sscanf(buffer, "%s", name) != 1)
		return false;

	strncpy(client->name, name, ID_SIZE);
	client->name[ID_SIZE + 1] = 0;

	client->key = getKey(client->name);
	if (client->key == 0)
		return false;

	char compare[32];
	strncpy(compare, "Hello friendo!", 15);
	seedByteXor(compare, 15, client->key, &client->seed);
	char* hexed = byteToHex(compare, 15);
	if (strncmp(hexed, (buffer + ID_SIZE), 30) != 0)
		return false;

	strcpy(buffer, ":)");
	send(client->cdesc, buffer, 2, 0);


	return true;
}