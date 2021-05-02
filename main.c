#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"


int main(int argc, char const *argv[])
{
	#define MAX_FILEPATH 1000
	char filepath[MAX_FILEPATH];

	init_mem();
	printf("File to load: ");

	fgets(filepath, MAX_FILEPATH, stdin);
	// remove new line
	filepath[strcspn(filepath, "\n")] = 0;

	if (ferror(stdin)) {
		printf("Read failed\n");
		exit(1);
	}
	const int allocatedSize = load_mem(filepath);
	printf("Fetched %d bytes\n", allocatedSize);

	for (int i = 0; i < allocatedSize; ++i)
	{
		printf("%x ", mem.base[i]);
	}
	deinit_mem();
	return 0;
}
