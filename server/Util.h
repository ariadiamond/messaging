#ifndef _UTIL_H_
#define _UTIL_H_

/*
 * Includes
 */
// Error handling
#include<errno.h>
#include<stdio.h>

#include"../shared/Parse.h"

/*
 * Threads Functions
 */
void* threadRunner(void* parg);

/*
 * Ports Functions
 */
 int createServerSock(size_t port);
 void runner(int sockDesc);



#endif