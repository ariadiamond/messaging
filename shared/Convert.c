#include<stdio.h>
#include<stdlib.h>

#include"Convert.h"

/*
 * Character conversions
 */

uint8_t SFtoInt(char sxFr) {
	if (sxFr > 64 && sxFr < 91) //A-Z
		return (sxFr - 65);
	if (sxFr > 96 && sxFr < 123) //a-z
	 	return (sxFr - 71);
	if (sxFr > 47 && sxFr < 58) //0-9
		return (sxFr + 4);
	if (sxFr == 43) //+
		return 62;
	if (sxFr == 47) //-
		return 63;
	if (sxFr == 61) //padding
		return 0;

	//not valid
	printf("This is not a valid base64 character %c\n", sxFr);
	exit(1);
}

char inttoSF(uint8_t val) {
	if (val < 26)
		return (val + 65);
	if (val < 52)
		return (val + 71);
	if (val < 62)
		return (val - 4);
	if (val == 62)
		return '+';
	if (val == 63)
		return '/';

	printf("This is not a valid base64 value: %d\n", val);
	exit(1);
}

/*
 * String conversions
 */

char* SFToByte(char* SxFr, size_t numChars) {
	char* byte = malloc(sizeof(char) * numChars); //over estimates
	size_t i = 0, j = 0;
	for (; i < numChars; i += 4, j += 3) {
		uint32_t val = (SFtoInt(SxFr[i]) & 0x3F) << 18;
		val |= (SFtoInt(SxFr[i + 1]) & 0x3F) << 12;
		val |= (SFtoInt(SxFr[i + 2]) & 0x3F) << 6;
		val |= (SFtoInt(SxFr[i + 3]) & 0x3F);

		byte[j]     = (val & 0x00FF0000) >> 16;
		byte[j + 1] = (val & 0x0000FF00) >> 8;
		byte[j + 2] = (val & 0x000000FF);
	}

	byte[j + 3] = 0;
	return byte;
}


char* byteToSF(char* byte, size_t numChars) {
	char* sxFr = malloc(sizeof(char) * (numChars * 2)); //over estimate
	size_t i = 0, j = 0;
	for (; i < numChars - 2; i += 3, j +=4) {
		uint32_t val = byte[i] << 16;
		val |= byte[i + 1] << 8;
		val |= byte[i + 2];

		sxFr[j]     = inttoSF((val & 0x00FC0000) >> 18);
		sxFr[j + 1] = inttoSF((val & 0x0003F000) >> 12);
		sxFr[j + 2] = inttoSF((val & 0x00000FC0) >> 6);
		sxFr[j + 3] = inttoSF( val & 0x0000003F);
	}
	if (numChars % 3 == 1) {
		sxFr[j]     = inttoSF((byte[i] & 0xFC) >> 2);
		sxFr[j + 1] = inttoSF((byte[i] & 0x03) << 4);
		sxFr[j + 2] = '=';
		sxFr[j + 3] = '=';
	} else if (numChars % 3 == 2) {
		uint16_t val = (byte[i] << 8) | (byte[i + 1]);
		sxFr[j]      = inttoSF((val & 0xFC00) >> 10);
		sxFr[j + 1]  = inttoSF((val & 0x03F0) >> 4);
		sxFr[j + 2]  = inttoSF((val & 0x000F) << 2);
		sxFr[j + 3]  = '=';
	}

	sxFr[j + 4] = 0; //null terminate
	return sxFr;
}