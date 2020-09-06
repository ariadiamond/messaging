#include<stdlib.h> //free malloc
#include<unistd.h> //close

#include<pthread.h>

#include<sys/socket.h>

#include"Util.h"

void runner(int sockDesc) {
	struct sockaddr clientAddr;
	socklen_t addrLen = sizeof(clientAddr);

	//forever loop
	while (true) {
		//get a client
		int* cdesc = malloc(sizeof(int));
		*cdesc = accept(sockDesc, &clientAddr, &addrLen);

		//is it a valid request?
		if (*cdesc < 0) {
			free(cdesc);
			continue;
		}

		//start thread to handle request
		pthread_t thread;
		pthread_create(&thread, NULL, threadRunner, (void*) cdesc);
	}
}

void* threadRunner(void* parg) {
	//get client descriptor
	ClientInfo client = {
			.cdesc = *((int*) parg)};
	free(parg);

	//Check key and agree on seed
	if (!verifyName(&client)) {
		close(client.cdesc);
		return NULL;
	}
	//do the things
	//no longer closing the connection each time
	bool recv = true;
	while (recv)
		recv = recvMessage(&client);

	//cleanup
	close(client.cdesc);

	//end the thread
	return NULL;
}
