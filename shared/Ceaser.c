#include"Shared.h"

void ceaser(char* bytes, size_t numChars, int spin) {
	for (size_t i = 0; i < numChars; i++)
		bytes[i] += spin;
}
