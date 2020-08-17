#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<unistd.h>
#include<fcntl.h>

#include<sys/socket.h>

#include"../client/Util.h"
#include"../shared/Parse.h"

void sendMsg(int cdesc, char** argv) {
	char buffer[BUFFER_SIZE];
	int fdesc = open(argv[3], O_RDONLY);
	ssize_t bytesRead = read(fdesc, buffer, BUFFER_SIZE);
	close(fdesc);

	char header[HEADER_SIZE + 1];
	parse_t items = {
					.version = VERSION,
					.length = bytesRead
					};
	strncpy(items.from, argv[1], ID_SIZE);
	strncpy(items.to, argv[2], ID_SIZE);
	marshall(items, header);
	send(cdesc, header, HEADER_SIZE, 0);
	recv(cdesc, header, HEADER_SIZE, 0);
	send(cdesc, buffer, bytesRead, 0);

}

int main(int argc, char** argv) {
	int cdesc = createClientSock(8080);
	if (argc == 2)
		getMessages(cdesc, argv[1]);
	else if (argc == 4)
		sendMsg(cdesc, argv);
	else
		printf("Usage: %s from to messageFile", argv[0]);

	close(cdesc);
	exit(1);


}
