#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#ifndef CHIP8_H
#define CHIP8_H
#include "display.h"
#include "register.h"
#include "memory.h"
#endif

struct OPCode {
	uint8_t firstOrder: 4;
	uint8_t secondOrder: 4;
	uint8_t thirdOrder: 4;
	uint8_t fourthOrder: 4;
	char *description;
	void (*func)(struct OPCode);
} opcodes[34];


void _clearScreen(struct OPCode _) {
	clearScreen();
	next();
}

void jump(struct OPCode opc) {
	reg.programCounter = (opc.secondOrder << 8) | (opc.thirdOrder << 4) | opc.fourthOrder;
}

void setVx(struct OPCode opc) {
	reg.V[opc.secondOrder] = (opc.thirdOrder << 4) | opc.fourthOrder;
	next();
}

void addVx(struct OPCode opc) {
	reg.V[opc.secondOrder] += (opc.thirdOrder << 4) | opc.fourthOrder;
	next();
}

void setI(struct OPCode opc) {
	reg.I =  (opc.secondOrder << 8) | (opc.thirdOrder << 4) | opc.fourthOrder;
	next();
}

void draw(struct OPCode opc) {
	uint8_t x = reg.V[opc.secondOrder];
	uint8_t y = reg.V[opc.thirdOrder];
	// n is row
	uint8_t n = opc.fourthOrder;
	reg.V[0xF] = 0;
	for (int i = 0; i < n; ++i)
	{
		uint8_t sprites = mem.base[(reg.I + i)];

		for(int j = 0; j < 8; j++) {
			uint8_t sprite = (sprites >> (8 - (j + 1))) & 0x01;
			uint8_t axisX = (x + j) % SCREEN_WIDTH;
			uint8_t axisY = (y + i) % SCREEN_HEIGHT; 
			uint8_t oldValue = screen[axisY][axisX];
			uint8_t newValue = screen[axisY][axisX] ^ sprite;
			if (oldValue == 1 && newValue == 0) {
				reg.V[0xF] = 1;
			}
			screen[axisY][axisX] = newValue;
		}
	}
	next();
}

void returnFunc(struct OPCode _) {
	if(!reg.stack[reg.stackPointer]) {
		printf("Invalid return address\n");
		exit(1);
	}
	reg.programCounter = reg.stack[reg.stackPointer];
	reg.stack[reg.stackPointer] = 0;
	if(reg.stackPointer) {
		reg.stackPointer--;
	}
	next();
}

void call(struct OPCode opc) {
	reg.stack[++reg.stackPointer] = reg.programCounter;
	reg.programCounter = (opc.secondOrder << 8) | (opc.thirdOrder << 4) | opc.fourthOrder;
}

void skip(struct OPCode opc) {
	uint8_t x = opc.secondOrder;
	uint8_t kk = (opc.thirdOrder << 4) | opc.fourthOrder;
	if(reg.V[x] == kk) {
		next();
	}
	next();
}

void skipIfNotEqual(struct OPCode opc) {
	uint8_t x = opc.secondOrder;
	uint8_t kk = (opc.thirdOrder << 4) | opc.fourthOrder;
	if(reg.V[x] != kk) {
		next();
	}
	next();
}

void skipIfRegEqual(struct OPCode opc) {
	uint8_t x = opc.secondOrder;
	uint8_t y = opc.thirdOrder;
	if(reg.V[x] == reg.V[y]) {
		next();
	}
	next();
}

void setVxWithVy(struct OPCode opc) {
	reg.V[opc.secondOrder] = reg.V[opc.thirdOrder];
	next();
}

void or(struct OPCode opc) {
	reg.V[opc.secondOrder]= reg.V[opc.secondOrder] | reg.V[opc.thirdOrder];
	next();
}

void and(struct OPCode opc) {
	reg.V[opc.secondOrder] = reg.V[opc.secondOrder] & reg.V[opc.thirdOrder];
	next();
}

void xor(struct OPCode opc) {
	reg.V[opc.secondOrder] = reg.V[opc.secondOrder] ^ reg.V[opc.thirdOrder];
	next();
}

void addVxVy(struct OPCode opc) {
	int result = reg.V[opc.secondOrder] + reg.V[opc.thirdOrder];
	reg.V[0xF] = result > 0xFF;
	reg.V[opc.secondOrder] = result;

	next();
}

void subtractVxVy(struct OPCode opc) {
	reg.V[0xF] = reg.V[opc.secondOrder] > reg.V[opc.thirdOrder];
	reg.V[opc.secondOrder] -= reg.V[opc.thirdOrder];
	next();
}

void shiftRight(struct OPCode opc) {
	uint8_t lsb = reg.V[opc.secondOrder] & 0x01;
	reg.V[0xF] = lsb;
	reg.V[opc.secondOrder] /= 2;
	next();
}

void subtractVyVx(struct OPCode opc) {
	reg.V[0xF] = reg.V[opc.thirdOrder] > reg.V[opc.secondOrder];
	reg.V[opc.secondOrder] = reg.V[opc.thirdOrder] - reg.V[opc.secondOrder];
	next();
}

void shiftLeft(struct OPCode opc) {
	uint8_t rsb = reg.V[opc.secondOrder] & 0x80;
	reg.V[0xF] = rsb;
	reg.V[opc.secondOrder] *= 2;
	next();
}

void skipIfVxNotVy(struct OPCode opc) {
	if(reg.V[opc.secondOrder] != reg.V[opc.thirdOrder]) {
		next();
	}
	next();
}

void jumpWithV0(struct OPCode opc) {
	reg.programCounter = ((opc.secondOrder << 8) | (opc.thirdOrder << 4) | opc.fourthOrder) + reg.V[0x0];
	next();
}

void randomizeReg(struct OPCode opc) {
	uint8_t kk = (opc.thirdOrder << 4) | opc.fourthOrder;
	srand(time(NULL));
	uint8_t randomNum = rand() % 256;
	reg.V[opc.secondOrder] = randomNum & kk;
	next();
}

void addIWithVx(struct OPCode opc) {
	reg.I += reg.V[opc.secondOrder];
	next();
}

void setFont(struct OPCode opc) {
	uint8_t askedFont = reg.V[opc.secondOrder] & 0x01;
	// each font takes 5 column
	reg.I = askedFont * 5;
	next();
}

void storeBCD(struct OPCode opc) {
	uint8_t val = reg.V[opc.secondOrder];
	for(int i = 2; i >= 0; --i) {
		uint8_t digit = val % 10;
		mem.base[reg.I + i] = digit;
		val = (val - digit) / 10;
	}
	next();
}

void copyToMem(struct OPCode opc) {
	uint8_t x = opc.secondOrder;
	for(int i = 0; i <= x; i++) {
		mem.base[reg.I + i] = reg.V[i];
	}
	next();
}

void copyToReg(struct OPCode opc) {
	uint8_t x = opc.secondOrder;
	for(int i = 0; i <= x; i++) {
		reg.V[i] = mem.base[reg.I + 1];
	}
	next();
}

void skipIfPressed(struct OPCode opc) {
	if (keyboardIsDown && pressedKeyboard == reg.V[opc.secondOrder]) {
		next();
	}
	next();
}

void skipIfNotPressed(struct OPCode opc) {
	if (!keyboardIsDown || pressedKeyboard != reg.V[opc.secondOrder]) {
		next();
	}
	next();
}

void waitForKey(struct OPCode opc) {
	if (!keyboardIsDown) {
		return;
	}
	reg.V[opc.secondOrder] = pressedKeyboard;
	next();
}

void setVxWithDT(struct OPCode opc) {
	reg.V[opc.secondOrder] = reg.delay;
	next();
}

void setDT(struct OPCode opc) {
	reg.delay = reg.V[opc.secondOrder];
	next();
}

void setST(struct OPCode opc) {
	reg.sound = reg.V[opc.secondOrder];
	next();
}

void generateOPCodes() {
	opcodes[0].firstOrder = 0x0;
	opcodes[0].secondOrder = 0x0;
	opcodes[0].thirdOrder = 0xE;
	opcodes[0].fourthOrder = 0x0;
	opcodes[0].func = &_clearScreen;
	opcodes[0].description = "Clear Screen";

	opcodes[1].firstOrder = 0x0;
	opcodes[1].secondOrder = 0x0;
	opcodes[1].thirdOrder = 0xE;
	opcodes[1].fourthOrder = 0xE;
	opcodes[1].func = &returnFunc;
	opcodes[1].description = "Return from function";

	opcodes[2].firstOrder = 0x1;
	opcodes[2].func = &jump;
	opcodes[2].description = "Jump";

	opcodes[3].firstOrder = 0x2;
	opcodes[3].func = &call;
	opcodes[3].description = "Call subroutine";

	opcodes[4].firstOrder = 0x3;
	opcodes[4].func = &skip;
	opcodes[4].description = "Skip";

	opcodes[5].firstOrder = 0x4;
	opcodes[5].func = &skipIfNotEqual;
	opcodes[5].description = "Skip if not equal";

	opcodes[6].firstOrder = 0x5;
	opcodes[6].fourthOrder = 0x0;
	opcodes[6].func = &skipIfRegEqual;
	opcodes[6].description = "Skip if register is equal";
	
	opcodes[7].firstOrder = 0x6;
	opcodes[7].func = &setVx;
	opcodes[7].description = "Set Vx";

	opcodes[8].firstOrder = 0x7;
	opcodes[8].func = &addVx;
	opcodes[8].description = "Add Vx";

	opcodes[9].firstOrder = 0x8;
	opcodes[9].fourthOrder = 0x0;
	opcodes[9].func = &setVxWithVy;
	opcodes[9].description = "Set Vx with Wy";

	opcodes[10].firstOrder = 0x8;
	opcodes[10].fourthOrder = 0x1;
	opcodes[10].func = &or;
	opcodes[10].description = "Perform OR operation";

	opcodes[11].firstOrder = 0x8;
	opcodes[11].fourthOrder = 0x2;
	opcodes[11].func = &and;
	opcodes[11].description = "Perform AND operation";

	opcodes[12].firstOrder = 0x8;
	opcodes[12].fourthOrder = 0x3;
	opcodes[12].func = &xor;
	opcodes[12].description = "Perform XOR Operation";

	opcodes[13].firstOrder = 0x8;
	opcodes[13].fourthOrder = 0x4;
	opcodes[13].func = &addVxVy;
	opcodes[13].description = "Add Vx and Vy";

	opcodes[14].firstOrder = 0x8;
	opcodes[14].fourthOrder = 0x5;
	opcodes[14].func = &subtractVxVy;
	opcodes[14].description = "Substract Vx and Vy";

	opcodes[15].firstOrder = 0x8;
	opcodes[15].fourthOrder = 0x6;
	opcodes[15].func = &shiftRight;
	opcodes[15].description = "Perform shift right operation";

	opcodes[16].firstOrder = 0x8;
	opcodes[16].fourthOrder = 0x7;
	opcodes[16].func = &subtractVyVx;
	opcodes[16].description = "Substract Vy and Vx";

	opcodes[17].firstOrder = 0x8;
	opcodes[17].fourthOrder = 0xE;
	opcodes[17].func = &shiftLeft;
	opcodes[17].description = "Perform shift left operation";

	opcodes[18].firstOrder = 0x9;
	opcodes[18].fourthOrder = 0x0;
	opcodes[18].func = &skipIfVxNotVy;
	opcodes[18].description = "Skip if Vx != Vy";

	opcodes[19].firstOrder = 0xA;
	opcodes[19].func = &setI;
	opcodes[19].description = "Set I";

	opcodes[20].firstOrder = 0xB;
	opcodes[20].func = &jumpWithV0;
	opcodes[20].description = "Jump with V0";

	opcodes[21].firstOrder = 0xC;
	opcodes[21].func = &randomizeReg;
	opcodes[21].description = "Randomize register";

	opcodes[22].firstOrder = 0xD;
	opcodes[22].func = &draw;
	opcodes[22].description = "Draw";

	opcodes[23].firstOrder = 0xE;
	opcodes[23].thirdOrder = 0x9;
	opcodes[23].fourthOrder = 0xE;
	opcodes[23].func = &skipIfPressed;
	opcodes[23].description = "Skip next instruction if key with the value of Vx is pressed";

	opcodes[24].firstOrder = 0xE;
	opcodes[24].thirdOrder = 0xA;
	opcodes[24].fourthOrder = 0x1;
	opcodes[24].func = &skipIfNotPressed;
	opcodes[24].description = "Skip next instruction if key with the value of Vx is not pressed";

	opcodes[25].firstOrder = 0xF;
	opcodes[25].thirdOrder = 0x0;
	opcodes[25].fourthOrder = 0x7;
	opcodes[25].func = &setVxWithDT;
	opcodes[25].description = "Set Vx = delay timer value.";

	opcodes[26].firstOrder = 0xF;
	opcodes[26].thirdOrder = 0x0;
	opcodes[26].fourthOrder = 0xA;
	opcodes[26].func = &waitForKey;
	opcodes[26].description = "Wait for a key press, store the value of the key in Vx.";
	
	opcodes[27].firstOrder = 0xF;
	opcodes[27].thirdOrder = 0x1;
	opcodes[27].fourthOrder = 0x5;
	opcodes[27].func = &setDT;
	opcodes[27].description = "Set delay timer = Vx.";

	opcodes[28].firstOrder = 0xF;
	opcodes[28].thirdOrder = 0x1;
	opcodes[28].fourthOrder = 0x8;
	opcodes[28].func = &setST;
	opcodes[28].description = "Set sound timer = Vx";

	opcodes[29].firstOrder = 0xF;
	opcodes[29].thirdOrder = 0x1;
	opcodes[29].fourthOrder = 0xE;
	opcodes[29].func = &addIWithVx;
	opcodes[29].description = "Add I with Vx";

	opcodes[30].firstOrder = 0xF;
	opcodes[30].thirdOrder = 0x2;
	opcodes[30].fourthOrder = 0x9;
	opcodes[30].func = &setFont;
	opcodes[30].description = "Set font";

	opcodes[31].firstOrder = 0xF;
	opcodes[31].thirdOrder = 0x3;
	opcodes[31].fourthOrder = 0x3;
	opcodes[31].func = &storeBCD;
	opcodes[31].description = "Store BCD";

	opcodes[32].firstOrder = 0xF;
	opcodes[32].thirdOrder = 0x5;
	opcodes[32].fourthOrder = 0x5;
	opcodes[32].func = &copyToMem;
	opcodes[32].description = "Copy to memory";

	opcodes[33].firstOrder = 0xF;
	opcodes[33].thirdOrder = 0x6;
	opcodes[33].fourthOrder = 0x5;
	opcodes[33].func = &copyToReg;
	opcodes[33].description = "Copy to register";
}

void decodeOPCode(uint8_t opcode1, uint8_t opcode2) {
	struct OPCode opc;
	opc.firstOrder = opcode1  >> 4;
	opc.secondOrder = opcode1 & 0x0f;
	opc.thirdOrder = opcode2 >> 4;
	opc.fourthOrder = opcode2 & 0x0f;

	struct OPCode found;
	int maxHit = 0;
	for(int i = 0; i < sizeof(opcodes) / sizeof(opcodes[0]); i++) {
		struct OPCode _opc = opcodes[i];
		int hit = 0;
		if (opc.firstOrder == _opc.firstOrder) {
			hit++;
		}
		if (opc.secondOrder == _opc.secondOrder) {
			hit++;
		}
		if (opc.thirdOrder == _opc.thirdOrder) {
			hit++;
		}
		if (opc.fourthOrder == _opc.fourthOrder) {
			hit++;
		}
		if (hit > maxHit && opc.firstOrder == _opc.firstOrder) {
			maxHit = hit;
			found = opcodes[i];
		}
	}
	if(maxHit) {
		// TODO: add debugging feature
		// if (found.description) {
		// 	printf("Executing %02x%02x: %s, addr: %02x(%02x)\n", opcode1, opcode2, found.description,
		// 	   	   reg.programCounter, reg.programCounter - INTERPRETER_SIZE);
		// }
		if(!found.func) {
			printf("Function is not yet implemented for %02x%02x\n", opcode1, opcode2);
			exit(1);
		}
		found.func(opc);
	}
	else {
		printf("OPCode not found: %02x%02x\n", opcode1, opcode2);
		exit(1);
	}
}
