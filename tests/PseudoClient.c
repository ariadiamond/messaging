#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<unistd.h>
#include<fcntl.h>

#include<sys/socket.h>

#include"../client/Util.h"
#include"../shared/Parse.h"

void sendMsg(int cdesc, char** argv, uint32_t seed) {
	char buffer[BUFFER_SIZE];
	int fdesc = open(argv[4], O_RDONLY);
	ssize_t bytesRead = read(fdesc, buffer, BUFFER_SIZE / 2);
	close(fdesc);

	parse_t items = {
					.version = VERSION,
					.length = bytesRead
					};
	strncpy(items.from, argv[1], ID_SIZE);
	strncpy(items.to, argv[3], ID_SIZE);
	items.from[ID_SIZE] = 0;
	items.to[ID_SIZE] = 0;

	buffer[bytesRead] = 0;

	passMessage(cdesc, items, buffer, argv[2][0], &seed);

}

int main(int argc, char** argv) {
	int cdesc = createClientSock(8080);
	uint32_t seed = verify(cdesc, argv[2][0], argv[1]);
	if (argc == 3)
		getMessages(cdesc, argv[1], argv[2][0], &seed);
	else if (argc == 5)
		sendMsg(cdesc, argv, seed);
	else
		printf("Usage: %s from key to messageFile", argv[0]);

	close(cdesc);
	exit(1);


}
