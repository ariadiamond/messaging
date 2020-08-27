#ifndef _CRYPTO_H_
#define _CRYPTO_H_


#include<stdint.h>

/*
 * base64
 */

//Get lengths from conversions
size_t SFToByteLen(size_t numChars);
size_t byteToSFLen(size_t numChars);

//Convert strings
char* byteToSF(char* bytes, size_t numChars);
char* SFToByte(char* SxFr, size_t numChars);

/*
 * Hex
 */

char* hexToByte(char* bytes, size_t numChars);
char* byteToHex(char* hex, size_t numChars);

#endif
