#ifndef _CRYPT_H_
#define _CRYPT_H_

#include<stdint.h>
#include<stdlib.h> //size_t

void xorShift(uint32_t* seed);

void seedByteXor(char* bytes, size_t numBytes, char key, uint32_t* seed);

void byteXor(char* bytes, size_t numBytes, char key);

#endif
