#include<stdlib.h> //free malloc
#include<unistd.h> //close

#include<pthread.h>

#include<sys/socket.h>

#include"Util.h"

void runner(int sockDesc) {
	struct sockaddr clientAddr;
	socklen_t addrLen;

	//forever loop
	while (true) {
        //get a client
		//I'm a little worried about having cdesc be a stack space. TODO?
		int cdesc = accept(sockDesc, &clientAddr, &addrLen);

		//is it a valid request?
		if (cdesc < 0) {
			continue;
		}

		//start thread to handle request
		pthread_t thread;
		pthread_create(&thread, NULL, threadRunner, (void*) &cdesc);
	}
}

void* threadRunner(void* parg) {
    //get client descriptor
	int cdesc = *((int*) parg);

    //do the things
	recvMessage(cdesc);

    //close the descriptor
	close(cdesc);

    //end the thread
	return NULL;
}
