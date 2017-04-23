#include <stdio.h>

int main() {
#ifndef __arm__
	printf("ERROR: probably not compiled on Raspberry PI\n");
	return 0;
#endif

	return 0;
}
