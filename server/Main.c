//#include<unistd.h> //getopt
#include<stdlib.h> //atoi

#include"Util.h"

/*
 * CLI parsing
 */

typedef struct cliArgs_t {
	uint16_t port;
} cliArgs;

cliArgs parseArgs(int argc, char** argv) {
	cliArgs args = {
						.port = 8080
						};
	if (argc == 2) {
		int ret = atoi(argv[1]);
		if (ret > 0 || ret < UINT16_MAX)
			args.port = ret;
	}
	return args;
}




int main(int argc, char** argv) {
	cliArgs args = parseArgs(argc, argv);
	printf("\x1b[1;34mStarting server on port: %hu\x1b[0m\n", args.port);
	int sockDesc = createServerSock(args.port);
	runner(sockDesc);

	return 0;
}
