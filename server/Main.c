#include<unistd.h> //getopt
#include<fcntl.h> //open
#include<stdlib.h> //atoi exit
#include<stdio.h>

#include"Util.h"

/*
 * Command line arguments parsing
 */

CLArgs args = {
			  .logging = false,
			  .remove  = true };


void usage(char* argv) {
	dprintf(STDERR_FILENO, "Usage: %s [-p port] [-l] [-r]\n", argv);
	dprintf(STDERR_FILENO, "\t-p port \x1b[3mwhere port is the port number (default is 8080)\x1b[0m\n");
	dprintf(STDERR_FILENO, "\t-l \x1b[3mto enable logging\x1b[0m\n");
	dprintf(STDERR_FILENO, "\t-r \x1b[3mto disable removing files after requests (helpful for debugging)\x1b[0m\n");
	exit(1);
}

uint16_t parseArgs(int argc, char** argv) {
	uint16_t port = 8080; //default port - this is not needed in CLArgs

	//parsing args using getopt
	for (uint8_t i = 0; i < 3; i++) {
		int optChar = getopt(argc, argv, "p:lrs:");
		if (optChar < 0)
			return port;

		switch (optChar) {
			case 'l':
				args.logging = true;
				break;
			case 'p':
				port = atoi(optarg);
				if (port == 0)
					usage(argv[0]);
				break;
			case 'r':
				args.remove = false;
				break;
			case 's':
				args.seed = atoi(optarg);
				break;
			default:
				usage(argv[0]);
		}
	}
	return port;
}


int main(int argc, char** argv) {
	uint16_t port = parseArgs(argc, argv);

	//make sure files exist
	if (args.logging) {
		int fdesc = open(ERR_FILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
		close(fdesc);
	}
	int fdesc = open(KEY_FILE, O_WRONLY | O_CREAT | O_APPEND, 0600);
	close(fdesc);

	int sockDesc = createServerSock(port);
	printf("\x1b[1;36mStarted server on port: %hu\x1b[0m\n", port);

	runner(sockDesc);

	return 0;
}
