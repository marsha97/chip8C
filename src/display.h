#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define PIXEL_SIZE 10

uint8_t screen[SCREEN_HEIGHT][SCREEN_WIDTH];
SDL_Window *window = NULL;
SDL_Surface *surface = NULL;
SDL_Renderer *renderer = NULL;
SDL_Rect rect;
bool keyboardIsDown = false;
uint8_t pressedKeyboard;

void clearScreen() {
	for (int i = 0; i < SCREEN_HEIGHT; ++i)
	{
		for (int j = 0; j < SCREEN_WIDTH; ++j)
		{
			screen[i][j] = 0;
		}
	}
}

void render() {
	for (int i = 0; i < SCREEN_HEIGHT; ++i)
	{
		for (int j = 0; j < SCREEN_WIDTH; j++) {
			if (!screen[i][j]) {
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			}
			rect.x = j * PIXEL_SIZE;
			rect.y = i * PIXEL_SIZE;
			rect.w = PIXEL_SIZE;
			rect.h = PIXEL_SIZE;
			if (SDL_RenderFillRect(renderer, &rect) != 0) {
				printf("Can't render pixel: %s\n", SDL_GetError());
				exit(1);
			}
		}
	}
	if (SDL_UpdateWindowSurface(window) != 0) {
		printf("Can't update surface: %s\n", SDL_GetError());
		exit(1);
	}
}

void initDisplay() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Can't init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	window = SDL_CreateWindow(
		"Chip8C",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH * 10,
		SCREEN_HEIGHT * 10,
		0
	);
	if (!window) {
		printf("Failed to make window: %s\n", SDL_GetError());
		exit(1);
	}
	surface = SDL_GetWindowSurface(window);
	if (!surface) {
		printf("Failed to create surface: %s\n", SDL_GetError());
		exit(1);
	}
	renderer = SDL_CreateSoftwareRenderer(surface);

	if (!renderer) {
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_RenderClear(renderer);
	clearScreen();
}

void endDisplay() {
	SDL_Quit();
}

uint8_t getKeyboardValue(SDL_Scancode key) {
	switch (key)
	{
	case SDL_SCANCODE_1:
		return 0x1;
	case SDL_SCANCODE_2:
		return 0x2;
	case SDL_SCANCODE_3:
		return 0x3;
	case SDL_SCANCODE_4:
		return 0xC;
	case SDL_SCANCODE_Q:
		return 0x4;
	case SDL_SCANCODE_W:
		return 0x5;
	case SDL_SCANCODE_E:
		return 0x6;
	case SDL_SCANCODE_R:
		return 0xD;
	case SDL_SCANCODE_A:
		return 0x7;
	case SDL_SCANCODE_S:
		return 0x8;
	case SDL_SCANCODE_D:
		return 0x9;
	case SDL_SCANCODE_F:
		return 0xE;
	case SDL_SCANCODE_Z:
		return 0xA;
	case SDL_SCANCODE_X:
		return 0x0;
	case SDL_SCANCODE_C:
		return 0xB;
	case SDL_SCANCODE_V:
		return 0xF;
	default:
		return 0xFF;
	}
}

void pressKey(uint8_t key) {
	keyboardIsDown = true;
	pressedKeyboard = key;
}

void releaseKey() {
	keyboardIsDown = false;
}

void setKey(SDL_Scancode key) {
	uint8_t val = getKeyboardValue(key);
	if (val >= 0xFF) {
		releaseKey();
	}
	else {
		pressKey(val);
	}
}

bool loopGraphic() {
	SDL_Event e;
	while(SDL_PollEvent(&e)) {
		switch (e.type)
		{
			case SDL_QUIT:
				return false;
			case SDL_KEYDOWN:
				setKey(e.key.keysym.scancode);
				break;	
			case SDL_KEYUP:
				releaseKey();
				break;
			default:
				break;
		}
	}
	render();
	return true;
}
