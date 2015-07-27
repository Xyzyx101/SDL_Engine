#pragma once
#include "SDL.h"
#include "MathUtils.h"
#include <map>
#include <set>

class Level {
	friend class LevelLoader;
public:
	enum LEVEL {
		CAVE
	};
	~Level();
	void drawLayer0( Vec2 cameraOffset );
	void drawLayer1( Vec2 cameraOffset );
	Uint16 getWidth();
	Uint16 getHeight();
	Uint16 getTileWidth();
	Uint16 getTileHeight();
	Vec2 checkCollision( Vec2 pos, Sint32 halfWidth, Sint32 halfHeight );
private:
	Level( SDL_Renderer* renderer, Uint16 width, Uint16 height, Uint16 screenWidth, Uint16 screenHeight, Uint16 tileWidth, Uint16 tileHeight );
	SDL_Renderer*						renderer_;
	SDL_Texture*						layer0Texture_;
	SDL_Texture*						layer1Texture_;
	std::map<Sint32, std::set<Sint32>>	collisionLayer_;
	int	fuckYouIdiot;
	Uint16								width_;
	Uint16								height_;
	Uint16								screenWidth_;
	Uint16								screenHeight_;
	Uint16								tileWidth_;
	Uint16								tileHeight_;
};

