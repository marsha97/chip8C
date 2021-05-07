#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "memory.h"
#include "register.h"


void init() {
	reg.programCounter = INTERPRETER_SIZE;
	for (int i = 0; i < sizeof(reg.stack) / sizeof(short); i++) {
		reg.stack[i] = 0;
	}
}

int main(int argc, char const *argv[])
{
	if (argc != 2) {
		printf("Error: usage is %s FILEPATH\n", argv[0]);
		exit(1);
	}

	load_mem((char*)argv[1]);
	return 0;
}
