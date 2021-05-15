#include <stdint.h>
#include <SDL2/SDL.h>

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

int decrementSound(void *_) {
	while(1)
	{
		if (!reg.sound) {
			continue;
		}
		--reg.sound;
		SDL_Delay(16.7);
	}	
	return 0;
}

int decrementDelay(void *_) {
	while(1)
	{
		if (!reg.delay)
		{
			continue;
		}
		--reg.delay;
		SDL_Delay(16.7);
	}	
	return 0;
}

void createSoundAndDelayThread() {
	SDL_Thread *soundThread;
	SDL_Thread *delayThread;

	soundThread = SDL_CreateThread(decrementSound, "SoundThread", (void *)NULL);
	if (!soundThread) {
		printf("Error creating sound thread %s\n", SDL_GetError());
		exit(1);
	}
	else {
		SDL_DetachThread(soundThread); // remove when done
	}

	delayThread = SDL_CreateThread(decrementDelay, "DelayThread", (void *)NULL);
	if (!delayThread)
	{
		printf("Error creating delay thread %s\n", SDL_GetError());
		exit(1);
	}
	else {
		SDL_DetachThread(delayThread); // remove when done
	}
}
