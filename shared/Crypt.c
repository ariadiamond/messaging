#include"Crypt.h"


void xorShift(uint32_t* seed) {
	*seed ^= *seed << 3;
	*seed ^= *seed >> 5;
	*seed ^= *seed << 8;
	*seed ^= *seed >> 17;
}

/*
 * This is hopefully better than just basic byte xor because the key is not
 * static and so statistical analysis is a lot harder.
 * Because xorShift is not cryptographically secure, this could be an issue.
 * Also there is more key passing, because you have to pass a seed for each
 * message.
 */

//excludes null terminating byte in numBytes
void seedByteXor(char* bytes, size_t numBytes, char key, uint32_t *seed) {
	for (size_t i = 0; i < numBytes; i++) {
		uint8_t iv = 0;
		switch(i % 4) {
			case 0:
				xorShift(seed);
				iv = *seed & (0xFF000000) >> 24;
				break;
			case 1:
				iv = *seed & (0x00FF0000) >> 16;
				break;
			case 2:
				iv = *seed & (0x0000FF00) >> 8;
				break;
			case 3:
				iv = *seed & (0x000000FF);
		}
		bytes[i] ^= (key ^ iv);
	}
	bytes[numBytes] = 0;

}

void byteXor(char* bytes, size_t numBytes, char key) {
	for (size_t i = 0; i < numBytes; i++)
		bytes[i] ^= key;
}