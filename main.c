#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"


int main(int argc, char const *argv[])
{
	if (argc != 2) {
		printf("Error: usage is %s FILEPATH\n", argv[0]);
		exit(1);
	}

	load_mem((char*)argv[1]);
	return 0;
}
