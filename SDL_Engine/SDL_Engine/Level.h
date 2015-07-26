#pragma once
#include "SDL.h"

class Level {
	friend class LevelLoader;
public:
	enum LEVEL {
		CAVE
	};
	~Level();
	void drawLayer0();
	void drawLayer1();
private:
	Level( SDL_Renderer* renderer, Uint16 screenWidth, Uint16 screenHeight, SDL_Texture* layer0Texture, SDL_Texture* layer1Texture );
	SDL_Renderer*	renderer_;
	SDL_Texture*	layer0Texture_;
	SDL_Texture*	layer1Texture_;
	Uint16			screenWidth_;
	Uint16			screenHeight_;
};

