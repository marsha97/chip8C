#include <stdint.h>
#include <SDL2/SDL.h>
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

uint8_t screen[SCREEN_HEIGHT][SCREEN_WIDTH];
SDL_Window *window = NULL;
SDL_Surface *surface = NULL;
SDL_Renderer *renderer = NULL;

void clearScreen() {
	for (int i = 0; i < SCREEN_HEIGHT; ++i)
	{
		for (int j = 0; j < SCREEN_WIDTH; ++j)
		{
			screen[i][j] = 0;
		}
	}
	SDL_RenderClear(renderer);
}

void render() {

}

int initDisplay() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Can't init SDL: %s\n", SDL_GetError());
		return 1;
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
		return 1;
	}
	surface = SDL_GetWindowSurface(window);
	renderer = SDL_CreateSoftwareRenderer(surface);

	if (!renderer) {
		printf("Failed to create renderer: %s\n", SDL_GetError());
		return 1;
	}
	clearScreen();
	return 0;
}

void endDisplay() {
	SDL_Quit();
}
