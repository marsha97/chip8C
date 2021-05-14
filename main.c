#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "display.h"
#include "memory.h"
#include "register.h"

bool done = false;

void initMemAndReg() {
	reg.programCounter = INTERPRETER_SIZE;
	for (int i = 0; i < sizeof(reg.stack) / sizeof(short); i++) {
		reg.stack[i] = 0;
	}
	loadFonts();
}

void loop() {
	done = true;
}

int main(int argc, char const *argv[]) {
	if (argc != 2) {
		printf("Error: usage is %s FILEPATH\n", argv[0]);
		exit(1);
	}

	loadMem((char*)argv[1]);
	initMemAndReg();

	if (initDisplay() != 0) {
		return 1;
	}

	while(!done) {
		loop();
	}

	endDisplay();
	return 0;
}
