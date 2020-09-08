#include<stdio.h>
#include"Test.h"

uint8_t passed[2] = {0, 0};

void nameTest(char* testType) {
	printf("%s====================\n", MAGENTA);
	printf("%s tests\n", testType);
	printf("====================%s\n", UNSET);
}

bool testResults(bool correct, char* func, char* arg) {
	printf("Test %d: %s(%s) [", passed[1] + 1, func, arg);
	if (correct) {
		printf("\x1b[1;92m%s%s]\n", "passed", UNSET);
		passed[0]++;
	} else
		printf("\x1b[1;91m%s%s]\n", "failed", UNSET);

	passed[1]++;
	return correct;
}

void printResults() {
	printf("\n%sYou passed", MAGENTA);
	if (passed[0] == passed[1])
		printf("%s %d %s", GREEN, passed[0], MAGENTA);
	else
		printf("%s %d %s", RED, passed[0], MAGENTA);

	printf("out of %s%d%s Tests.%s\n\n", GREEN, passed[1], MAGENTA, UNSET);
}
