#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<unistd.h>
#include<fcntl.h>

#include<sys/socket.h>

#include"../client/Util.h"
#include"../shared/Parse.h"

void sendMsg(Info info, char* to, char* file) {
	int fdesc = open(file, O_RDONLY);
	ssize_t bytesRead = read(fdesc, info.buffer, BUFFER_SIZE / 2);
	close(fdesc);

	parse_t items = {
					.version = VERSION,
					.length = bytesRead
					};
	strncpy(items.from, info.name, ID_SIZE);
	strncpy(items.to, to, ID_SIZE);
	items.from[ID_SIZE] = 0;
	items.to[ID_SIZE] = 0;

	info.buffer[bytesRead] = 0;

	passMessage(&info, items);

}

int main(int argc, char** argv) {
	Info info = { .key = argv[2][0] };
	strncpy(info.name, argv[1], ID_SIZE);
	info.name[ID_SIZE] = 0;

	switch (argc) {
		case 3:
			info.cdesc = createClientSock("127.0.0.1", 8080);
			if (!verify(&info))
				exit(1);
			getMessages(&info);
			break;
		case 4:
			info.cdesc = createClientSock("127.0.0.1", atoi(argv[3]));
			if (!verify(&info))
				exit(1);
			getMessages(&info);
			break;
		case 5:
			info.cdesc = createClientSock("127.0.0.1", 8080);
			if (!verify(&info))
				exit(1);
			sendMsg(info, argv[3], argv[4]);
			break;
		case 6:
			info.cdesc = createClientSock("127.0.0.1", atoi(argv[5]));
			if (!verify(&info))
				exit(1);
			sendMsg(info, argv[3], argv[4]);
			break;
		default:
			printf("Usage: %s from key [to messageFile] [port]", argv[0]);
	}
	return 0;
}
