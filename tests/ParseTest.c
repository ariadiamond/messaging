#include<string.h>
#include<stdio.h>
#include<stdbool.h>

#include"Test.h"
#include"../shared/Parse.h"

bool inval(char* to, char* from) {
	if (strlen(to) != ID_SIZE || strlen(from) != ID_SIZE)
		return true;
	return false;
}

void marshallRunner(char* to, char* from, uint16_t num, char* compare) {
	parse_t obj;
	strncpy(obj.to, to, ID_SIZE);
	obj.to[ID_SIZE] = 0;
	strncpy(obj.from, from, ID_SIZE);
	obj.from[ID_SIZE] = 0;
	obj.length = num;
	char header[HEADER_SIZE + 1];
	if(!marshall(obj, header) && inval(to, from)) {
		testResults(true, "marshall", to);
		return;
	}
	int8_t cmpVal = strcmp(header, compare);
	if (!testResults(cmpVal == 0, "marshall", to)) {
		printf("%s%s\n", GREEN, compare);
		printf("%s%s%s\n", RED, header, UNSET);
	}
}

void demarshallRunner(char* to, char* from, uint16_t num, char* compare) {
	parse_t test;
	if (!demarshall(compare, &test) && inval(to, from)) {
		testResults(true, "demarshall", to);
		return;
	}
	int cmpVal = strcmp(test.from, from);
	cmpVal |= strcmp(test.to, to);
	cmpVal |= (num - test.length);
	if(!testResults(cmpVal == 0, "demarshall", to)) {
		printf("To: %s%s %s%s%s\n", GREEN, to, RED, test.to, UNSET);
		printf("From: %s%s %s%s%s\n", GREEN, from, RED, test.from, UNSET);
		printf("Length: %s%04X %s%04X%s\n", GREEN, num, RED, test.length, UNSET);
	}
}

void marshallTests() {
	nameTest("Marshall");
	marshallRunner("Maia", "aria", 1, "To: Maia\nFrom: aria\nLength: 0001\n");
	marshallRunner("aaaa", "bbbb", 16, "To: aaaa\nFrom: bbbb\nLength: 0010\n");
	marshallRunner("", "too", 33, "To: \nFrom: too\nLength: 0021\n");
}

void demarshallTests() {
	nameTest("Demarshall");
	demarshallRunner("aaaa", "bbbb", 16, "To: aaaa\nFrom: bbbb\nLength: 0010\n");
	demarshallRunner("Maia", "aria", 1, "To: Maia\nFrom: aria\nLength: 0001\n");
	demarshallRunner("invalid", "too long", 33, "This doesn't matter");
	demarshallRunner("ai", "0xxa", 64, "To: ai\nFrom: 0xxa\nLength: 0040\n");
}

int main(void) {
	marshallTests();
	demarshallTests();

	printResults();
	return 0;
}
