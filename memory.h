#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "utils.h"


struct Memory
{
	unsigned char *base;
} mem;


void init_mem() {
	mem.base = (unsigned char *)malloc(INTERPRETER_SIZE * sizeof(*mem.base));
	if (mem.base == NULL) {
		printf("Failed to allocate interpreter memory\n");
		exit(1);
	}
}


void deinit_mem() {
	safeFree((void **)&mem.base);
}


int load_mem(char *filepath) {
	FILE *fileptr;
	int allocatedSize = 0;

	if ((fileptr = fopen(filepath, "rb")) == NULL) {
		printf("Load file error, invalid file\n");
		exit(1);
	}

	fseek(fileptr, 0L, SEEK_END);
	allocatedSize = ftell(fileptr) + INTERPRETER_SIZE;
	rewind(fileptr);

	if (allocatedSize > MAX_MEMORY) {
		printf("Binary file exceeds max value\n");
		exit(1);
	}
	mem.base = (unsigned char *)realloc(mem.base, (allocatedSize) * sizeof(*mem.base));
	if (mem.base == NULL) {
		printf("Allocation failed\n");
		exit(1);
	}
	fread(mem.base + INTERPRETER_SIZE, allocatedSize, 1, fileptr);
	fclose(fileptr);
	return allocatedSize;
}
