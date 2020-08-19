#include<stdio.h>
#include<stdlib.h>
#include<unistd.h> //write

#include<sys/socket.h>

#include"Util.h"


void sendMessage(int clientDesc, char* from) {
    //get message info
	char buffer[BUFFER_SIZE];
	parse_t headerInfo = createMessage(buffer, from);

    //marshall message
	char header[HEADER_SIZE + 1];
	if (!marshall(headerInfo, header))
		return;

    //send message
	send(clientDesc, header, HEADER_SIZE, 0);
	recv(clientDesc, header, HEADER_SIZE, 0); //ack, however this gets stuck
		//if it never gets a response TODO
	send(clientDesc, buffer, headerInfo.length, 0);
	close(clientDesc);
}


void getMessages(int clientDesc, char* from) {
    //send request
	char buffer[BUFFER_SIZE];
	dprintf(clientDesc, "To: NULL\nFrom: %s\nVersion: %04X\nLength: 0000\n",
        from, VERSION);

    //FIXME: this could also receive messages because messages are just sent
    //over the buffer without an ack (and this could get descheduled) so it
    //doesn't call recv quickly enough
	ssize_t bytesRead = recv(clientDesc, buffer, BUFFER_SIZE, 0);
    dprintf(clientDesc, "Recieved");

    //parse response
	size_t lengths[1];
	sscanf(buffer, "%lu", lengths);
	if (*lengths == 0) {
		printf("You don't have any new messages\n");
		return;
	}

    //this is if there are new messages
	size_t received = 0;
	while (received < *lengths) {
		bytesRead = recv(clientDesc, buffer, BUFFER_SIZE, 0);
		if (bytesRead < 1) { //FIXME: why does it do this? - SEE FIXME above!!
			perror("Message::getMessages::recv");
			exit(1);
		}
		write(STDIN_FILENO, buffer, bytesRead);
		received += bytesRead;
	}

	close(clientDesc);
}
