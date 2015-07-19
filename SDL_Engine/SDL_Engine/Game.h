#pragma once
#include "SDL.h"

class Game
{
	SDL_Window* window_;
	SDL_Renderer* renderer_;
	SDL_Window* testImage_;
	SDL_Texture* texture_;
	SDL_Surface* surface_;

public:
	Game();
	~Game();

	int InitSDL();
	int InitTestImage();
	void Run();
	void Draw();
	void Clean();
};

