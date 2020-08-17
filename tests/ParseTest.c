#include<string.h>
#include<stdio.h>
#include<stdbool.h>

#include"Test.h"
#include"../shared/Parse.h"

bool verbose = false;

bool inval(char* to, char* from) {
	if (strlen(to) != ID_SIZE || strlen(from) != ID_SIZE)
		return true;
	return false;
}

void marshallRunner(char* to, char* from, uint16_t ver, uint16_t len, char* compare) {
	parse_t obj;
	strncpy(obj.to, to, ID_SIZE);
	obj.to[ID_SIZE] = 0;
	strncpy(obj.from, from, ID_SIZE);
	obj.from[ID_SIZE] = 0;
	obj.length = len;
	obj.version = ver;
	char header[HEADER_SIZE + 1];
	if(!marshall(obj, header) && inval(to, from)) {
		testResults(true, "marshall", to);
		return;
	}
	int8_t cmpVal = strcmp(header, compare);
	testResults(cmpVal == 0, "marshall", to);
	if (verbose) {
		printf("%s%s\n", GREEN, compare);
		printf("%s%s%s\n", RED, header, UNSET);
	}
}

void demarshallRunner(char* to, char* from, uint16_t ver, uint16_t len, char* compare) {
	parse_t test;
	if (!demarshall(compare, &test) && inval(to, from)) {
		testResults(true, "demarshall", to);
		return;
	}
	int cmpVal = strcmp(test.from, from);
	cmpVal |= strcmp(test.to, to);
	cmpVal |= (len - test.length);
	cmpVal |= (ver - test.version);
	testResults(cmpVal == 0, "demarshall", to);
	if (verbose) {
		printf("To: %s%s %s%s%s\n", GREEN, to, RED, test.to, UNSET);
		printf("From: %s%s %s%s%s\n", GREEN, from, RED, test.from, UNSET);
		printf("Length: %s%04X %s%04X%s\n", GREEN, len, RED, test.length, UNSET);
		printf("Version: %s%04X %s%04X%s\n", GREEN, ver, RED, test.version, UNSET);
	}
}

void marshallTests() {
	nameTest("Marshall");
	marshallRunner("Maia", "aria", 1, 1, "To: Maia\nFrom: aria\nVersion: 0001\nLength: 0001\n");
	marshallRunner("aaaa", "bbbb", 1, 16, "To: aaaa\nFrom: bbbb\nVersion: 0001\nLength: 0010\n");
	marshallRunner("", "too", 2, 33, "To: \nFrom: too\nVersion: 0002\nLength: 0021\n");
}

void demarshallTests() {
	nameTest("Demarshall");
	demarshallRunner("aaaa", "bbbb", 16, 16, "To: aaaa\nFrom: bbbb\nVersion: 0010\nLength: 0010\n");
	demarshallRunner("Maia", "aria", 26, 1, "To: Maia\nFrom: aria\nVersion: 0001A\nLength: 0001\n");
	demarshallRunner("invalid", "too long", 33, 593, "This doesn't matter"); //invalid input
	demarshallRunner("ai", "0xxa", 11, 64, "To: ai\nFrom: 0xxa\nVersion: 000B\nLength: 0040\n"); //invalid input
}

int main(int argc, char** argv) {
	
	if (argc == 2 && argv[1][0] == '-' && argv[1][1] == 'v' && argv[1][2] == 0)
		verbose = true;
	marshallTests();
	demarshallTests();

	printResults();
	return 0;
}
