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
		if (read(fdesc, buffer, ENTRY_LEN) <= 0) {
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



uint32_t verifyName(int cdesc, char* name) {
	uint32_t seed = xorShift(args.seed);
	args.seed = seed;
	dprintf(cdesc, "%u", seed);
	seed = xorShift(seed);
	char buffer[36]; //too long
	if (recv(cdesc, buffer, 35, 0) <= 0)
		return 0;

	if (sscanf(buffer, "%s", name) != 1)
		return 0;

	name[ID_SIZE + 1] = 0;

	char key = getKey(name);
	if (key == 0)
		return 0;

	name[ID_SIZE] = key;

	char compare[32];
	strncpy(compare, "Hello friendo!", 15);
	seedByteXor(compare, 15, key, &seed);
	char* hexed = byteToHex(compare, 15);
	if (strncmp(hexed, (buffer + ID_SIZE), 30) != 0)
		return 0;

	strcpy(buffer, ":)");
	send(cdesc, buffer, 2, 0);


	return seed;
}