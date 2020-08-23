#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include"../shared/Convert.h"
#include"Test.h"



void SFtoIntTest(void) {
	nameTest("Six Four to Int");
	testResults(0  == SFtoInt('A'), "SFtoInt", "A");
	testResults(63 == SFtoInt('/'), "SFtoInt", "/");
	testResults(29 == SFtoInt('d'), "SFtoInt", "d");
	testResults(58 == SFtoInt('6'), "SFtoInt", "6");
}

void inttoSFTest(void) {
	nameTest("Int to Six Four");
	testResults('/' == inttoSF(63), "inttoSF", "63");
	testResults('J' == inttoSF( 9), "inttoSF",  "9");
	testResults('n' == inttoSF(39), "inttoSF", "39");
	testResults('4' == inttoSF(56), "inttoSF", "56");
}

void btSFHelper(char* bytes, char* compare) {
	char* result = byteToSF(bytes, strlen(bytes));
	if (!testResults(strcmp(result, compare) == 0, "bytetoSF", bytes)) {
		printf("%s%s\n", RED, result);
		printf("%s%s%s\n", GREEN, compare, UNSET);
	}
	free(result);
}

void bytetoSFTest(void) {
	nameTest("String: Byte to Six Four");
	btSFHelper("hello", "aGVsbG8=");
	btSFHelper("temporary nothing", "dGVtcG9yYXJ5IG5vdGhpbmc=");
	btSFHelper("#?0", "Iz8w");
	btSFHelper("ar j", "YXIgag==");
	btSFHelper("any carnal pleasure.", "YW55IGNhcm5hbCBwbGVhc3VyZS4=");
}

void SFtbHelper(char* SxFr, char* compare) {
	char* result = SFToByte(SxFr, strlen(SxFr));
	if (!testResults(strcmp(result, compare) == 0, "SFtoByte", SxFr)) {
		printf("%s%s\n", RED, result);
		printf("%s%s%s\n", GREEN, compare, UNSET);
	}

	free(result);
}

void SFtoByteTest(void) {
	nameTest("String: Six Four to Byte");
	SFtbHelper("aGVsbG8=", "hello");
	SFtbHelper("dGVtcG9yYXJ5IG5vdGhpbmc=", "temporary nothing");
	SFtbHelper("Iz8w", "#?0");
	SFtbHelper("YXIgag==", "ar j");
	SFtbHelper("YW55IGNhcm5hbCBwbGVhc3VyZS4=", "any carnal pleasure.");
}


int main(void) {
	//Converting characters
	SFtoIntTest();
	inttoSFTest();

	//Converting strings
	bytetoSFTest();
	SFtoByteTest();

	printResults();
	return 0;
}
