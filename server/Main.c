//#include<stdio.h>

#include"Util.h"

int main(int argc, char** argv) {
	int sockDesc = createServerSock(8080); //change port later?
	runner(sockDesc);

	return 0;
}
