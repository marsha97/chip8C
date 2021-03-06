#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifndef CHIP8_H
#define CHIP8_H
#include "constants.h"
#include "display.h"
#include "memory.h"
#include "register.h"
#include "opcodes.h"
#endif

bool done = false;

void initMemAndReg() {
	reg.programCounter = INTERPRETER_SIZE;
	for (int i = 0; i < sizeof(reg.stack) / sizeof(short); i++) {
		reg.stack[i] = 0;
	}
	loadFonts();
	createSoundAndDelayThread();
}

void loop() {
	done = !loopGraphic();
	if(done) {
		return;
	}
	if (reg.programCounter < (INTERPRETER_SIZE + filesize) - 1) {
		decodeOPCode(mem.base[reg.programCounter], mem.base[reg.programCounter + 1]);
	}
	SDL_Delay(1.5);
}

int main(int argc, char const *argv[]) {
	if (argc != 2) {
		printf("Error: usage is %s FILEPATH\n", argv[0]);
		exit(1);
	}

	srand(time(NULL));
	loadMem((char*)argv[1]);
	generateOPCodes();
	initMemAndReg();
	initDisplay();
	while(!done) {
		loop();
	}

	endDisplay();
	return 0;
}
