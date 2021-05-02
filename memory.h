#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "utils.h"


struct Memory
{
	unsigned char base[MAX_MEMORY];
} mem;


void load_mem(char *filepath) {
	FILE *fileptr;
	int filesize = 0;

	if ((fileptr = fopen(filepath, "rb")) == NULL) {
		printf("Load file error, invalid file\n");
		exit(1);
	}

	fseek(fileptr, 0L, SEEK_END);
	filesize = ftell(fileptr);
	rewind(fileptr);

	if (filesize > MAX_MEMORY) {
		printf("Binary file exceeds max value\n");
		exit(1);
	}
	fread(mem.base + INTERPRETER_SIZE, filesize, 1, fileptr);
	fclose(fileptr);
}
