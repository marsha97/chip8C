#include <stdint.h>
#include <stdlib.h>
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
	void (*func)(struct OPCode);
} opcodes[6];


void _clearScreen(struct OPCode _) {
	clearScreen();
	reg.programCounter += 2;
}

void jump(struct OPCode opc) {
	reg.programCounter = (opc.secondOrder << 8) | (opc.thirdOrder << 4) | opc.fourthOrder;
}

void setVx(struct OPCode opc) {
	reg.V[opc.secondOrder] = (opc.thirdOrder << 4) | opc.fourthOrder;
	reg.programCounter += 2;
}

void addVx(struct OPCode opc) {
	reg.V[opc.secondOrder] += (opc.thirdOrder << 4) | opc.fourthOrder;
	reg.programCounter += 2;
}

void setI(struct OPCode opc) {
	reg.I =  (opc.secondOrder << 8) | (opc.thirdOrder << 4) | opc.fourthOrder;
	reg.programCounter += 2;
}

void draw(struct OPCode opc) {
	uint8_t x = reg.V[opc.secondOrder];
	uint8_t y = reg.V[opc.thirdOrder];
	// n is row
	uint8_t n = opc.fourthOrder;
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
	render();
	reg.programCounter += 2;
}

void generateOPCodes() {
	opcodes[0].firstOrder = 0x0;
	opcodes[0].secondOrder = 0x0;
	opcodes[0].thirdOrder = 0xE;
	opcodes[0].fourthOrder = 0x0;
	opcodes[0].func = &_clearScreen;

	opcodes[1].firstOrder = 0x1;
	opcodes[1].func = &jump;

	opcodes[2].firstOrder = 0x6;
	opcodes[2].func = &setVx;

	opcodes[3].firstOrder = 0x7;
	opcodes[3].func = &addVx;

	opcodes[4].firstOrder = 0xA;
	opcodes[4].func = &setI;

	opcodes[5].firstOrder = 0xD;
	opcodes[5].func = &draw;
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
		if (hit > maxHit) {
			maxHit = hit;
			found = opcodes[i];
		}
	}
	if(maxHit) {
		found.func(opc);
	}
	else {
		printf("OPCode not found: %x%x\n", opcode1, opcode2);
		exit(1);
	}
}
