#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

#define SIZE 256
#define WINDOW_SIZE 512

typedef unsigned char byte;

byte pattern_rule(byte rule, byte pattern)
{
    byte d = 0;
    switch (pattern) {
	case 0b111:
	    d = ((rule >> 7) & 1);
	    break;
	case 0b110:
	    d = ((rule >> 6) & 1);
	    break;
	case 0b101:
	    d = ((rule >> 5) & 1);
	    break;
	case 0b100:
	    d = ((rule >> 4) & 1);
	    break;
	case 0b011:
	    d = ((rule >> 3) & 1);
	    break;
	case 0b010:
	    d = ((rule >> 2) & 1);
	    break;
	case 0b001:
	    d = ((rule >> 1) & 1);
            break;
        case 0b000:
            d = (rule & 1);
            break;
	}
	return d;
}

void map_rule(byte* world, byte rule)
{
    int i;
    byte next[SIZE];
    for (i = 0; i < SIZE; i++) {
	byte pat = (world[(i - 1) % SIZE] << 2) |
    	           (world[i]              << 1) |
                   (world[(i + 1) % SIZE]);
        next[i] = pattern_rule(rule, pat);
    }
    memcpy(world, next, SIZE * sizeof(byte));
}

int main(int argc, char** argv)
{
    int rule;
    sscanf(argv[1], "%d", &rule);

    byte world[SIZE];

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("Elementary automaton",
        SDL_WINDOWPOS_UNDEFINED,
	SDL_WINDOWPOS_UNDEFINED,
	WINDOW_SIZE, WINDOW_SIZE,
	SDL_WINDOW_OPENGL);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
	SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTexture(renderer,
	SDL_PIXELFORMAT_RGBA8888,
	SDL_TEXTUREACCESS_STREAMING,
	SIZE, SIZE);
    SDL_Event ev;

    memset(world, 0, SIZE * sizeof(byte));
    world[SIZE / 2] = 1;
    
    int i, j = 0, delta = SDL_GetTicks();
    int quit = 0, pause = 0;

    Uint32 pixels[SIZE * SIZE];
    memset(pixels, 0xFFFFFFFF, SIZE * SIZE * sizeof(Uint32));

    while (!quit) {
        if (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) {
            	quit = 1;
            }
        }
        if (SDL_GetTicks() - delta > 10 && !pause) {
            if (j == SIZE) pause = 1;
            if (world[0] == 1) pause = 1;
            if (world[SIZE - 1] == 1) pause = 1;
	    for(i = 0; i < SIZE; i++) {
		pixels[j*SIZE + i] = world[i] ? 0x0 : 0xFFFFFFFF;
            }
	    map_rule(world, (byte) rule);
            j++;	    
            
            SDL_UpdateTexture(texture, NULL, pixels, SIZE * sizeof(Uint32));
            SDL_RenderClear(renderer);
	    SDL_RenderCopy(renderer, texture, NULL, NULL);
	    SDL_RenderPresent(renderer);
            delta = SDL_GetTicks();
        }
    }
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}