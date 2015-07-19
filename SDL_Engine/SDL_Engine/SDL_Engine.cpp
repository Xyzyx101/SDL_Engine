#include "SDL.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
	SDL_Window*	sdlWindow;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(0);
	}

	if ((sdlWindow = SDL_CreateWindow(
		"Bacon",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		1920,
		1080,
		SDL_WINDOW_FULLSCREEN)
		) == NULL) {
		return false;
	}
	SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
	SDL_SetRenderDrawColor(sdlRenderer, 255, 0, 0, 255);
	SDL_RenderClear(sdlRenderer);
	SDL_RenderPresent(sdlRenderer);

	int i = 0; while (++i < 10000) {
		SDL_SetRenderDrawColor(sdlRenderer, 255, 0, 0, 255);
		SDL_RenderClear(sdlRenderer);
		SDL_RenderPresent(sdlRenderer);
		SDL_SetRenderDrawColor(sdlRenderer, 0, 255, 0, 255);
		SDL_RenderClear(sdlRenderer);
		SDL_RenderPresent(sdlRenderer);
		SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 255, 255);
		SDL_RenderClear(sdlRenderer);
		SDL_RenderPresent(sdlRenderer);
	}
	SDL_Quit();
	return 0;
}