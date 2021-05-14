#include <stdint.h>
#include <SDL2/SDL.h>
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

uint8_t screen[SCREEN_HEIGHT][SCREEN_WIDTH];
SDL_Window *window = NULL;
SDL_Surface *surface = NULL;
SDL_Renderer *renderer = NULL;


void loadFonts(unsigned char *arr) {
	/** 1 sprites sprite has 5 bytes.
		But it is easier to declare it into 1D, because memory in this program only has 1D(u char[])
	**/
	unsigned char characters[90] = {
		0xf0, 0x90, 0x90, 0x90, 0xf0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xf0, 0x10, 0xf0, 0x80, 0xf0, // 2
		0xf0, 0x10, 0xf0, 0x10, 0xf0, // 3
		0x90, 0x90, 0xf0, 0x10, 0x10, // 4
		0xf0, 0x80, 0xf0, 0x10, 0xf0, // 5
		0xf0, 0x80, 0xf0, 0x90, 0xf0, // 6
		0xf0, 0x10, 0x20, 0x40, 0x40, // 7
		0xf0, 0x90, 0xf0, 0x90, 0xf0, // 8
		0xf0, 0x90, 0xf0, 0x10, 0xf0, // 9
		0xf0, 0x90, 0xf0, 0x90, 0x90, // A
		0xe0, 0x90, 0xe0, 0x90, 0xf0, // B
		0xf0, 0x80, 0x80, 0x80, 0xf0, // C
		0xe0, 0x90, 0x90, 0x90, 0xf0, // D
		0xf0, 0x80, 0xf0, 0x80, 0xf0, // E
		0xf0, 0x80, 0xf0, 0x80, 0x80, // F
	};
	for(int i = 0; i < sizeof(characters) / sizeof(characters[0]); i++ ){
		arr[0] = characters[i];
	}
}

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
