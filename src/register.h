#include <stdint.h>

struct Register
{
	uint8_t V[16];
	uint16_t I;
	uint8_t sound;
	uint8_t delay;
	uint16_t programCounter;
	uint8_t stackPointer;
	uint16_t stack[16];
} reg;

void next() {
	reg.programCounter += 2;
}