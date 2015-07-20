#pragma once
#include "SDL.h"

class Game {
public:
	Game();
	~Game();
	int InitSDL();
	int InitTestImage();
	void Run();
	void Draw();
	void Clean();

private:
	SDL_Window*		window_;
	SDL_Renderer*	renderer_;
	SDL_Window*		testImage_;
	SDL_Texture*	texture_;
	SDL_Surface*	surface_;
};

