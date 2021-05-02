struct Register
{
	unsigned char generic[16];
	unsigned short *I;
	unsigned char *sound;
	unsigned char *delay;
	unsigned short *programCounter;
	unsigned char *stackPointer;
	unsigned short stack[16];
};