#include<stdio.h>

#include"Shared.h"


/*
 * Character Conversions
 */

uint8_t hexToInt(char hex) {
	if (hex >= '0' && hex <= '9')
		return (hex - '0');
	if (hex >= 'A' && hex <= 'F' )
		return (10 + (hex - 'A'));
	if (hex >= 'a' && hex <= 'f')
		return (10 + (hex - 'a'));
	printf("This: %c (%d) is not a valid hex character.\n", hex, hex);
	exit(1);
}

char intToHex(uint8_t val) {
	if (val > 15) {
		printf("This: %d is not a valid hex value.\n", val);
		exit(1);
	}
	if (val < 10)
		return val + '0';
	return (val - 10) + 'A';
}

/*
 * String Conversion
 */

char* hexToByte(char* hex, size_t numChars) {
	if (numChars % 2 != 0) {
		printf("That's not a valid length!\n");
		exit(1);
	}

	char* ret = malloc(sizeof(char) * ((numChars / 2) + 1));
	for (size_t i = 0, j = 0; j < numChars; i++, j += 2) {
		ret[i] = (hexToInt(hex[j]) << 4) | hexToInt(hex[j + 1]);
	}
	return ret;
}


 char* byteToHex(char* bytes, size_t numChars) {
	 char* ret = malloc(sizeof(char) * ((numChars * 2) + 1));
	 for (size_t i = 0, j = 0; i < numChars; i++, j += 2) {
		 ret[j] = intToHex((bytes[i] & 0xF0) >> 4); //I don't need mask do I?
		 ret[j + 1] = intToHex((bytes[i] & 0x0F));
	 }
	 return ret;
 }
