#ifndef _CRYPTO_H_
#define _CRYPTO_H_


#include<stdint.h>
#include<stdbool.h>


uint8_t SFtoInt(char sxFr);
char inttoSF(uint8_t val);

char* byteToSF(char* bytes, size_t numChars);
char* SFToByte(char* SxFr, size_t numChars);

#endif
