#ifndef _TEST_H_
#define _TEST_H_

 
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define MAGENTA "\x1b[35m"
#define BLUE "\x1b[34m"
#define UNSET "\x1b[0m"

#include<stdint.h>
#include<stdbool.h>

void nameTest(char* testType);

bool testResults(bool pass, char* func, char* arg);

void printResults();


#endif
