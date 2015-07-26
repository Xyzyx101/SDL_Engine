#pragma once
#include <string>
#include <vector>
#include "SDL.h"
#include "Level.h"
#include <set>
#include <map>

class json;

class LevelLoader {
public:
	LevelLoader(SDL_Renderer* renderer, Uint16 screenWidth, Uint16 screenHeight);
	~LevelLoader();
	Level* loadLevel( Level::LEVEL level );
private:
	SDL_Texture* loadTexture( std::string path );
	SDL_Texture* createLayerTexture( SDL_Texture* tileTex, std::vector<int> data, int levelWidth, int levelHeight, int tileWidth, int tileHeight, int imageWidth, int imageHeight );
	void createCollisionLayer( std::vector<int> data, int levelWidth, int levelHeight, int tileWidth, int tileHeight );
	SDL_Renderer*						renderer_;
	Uint16								screenWidth_;
	Uint16								screenHeight_;
	std::map<Uint16, std::set<Uint16>>	collisionLayer_;
};

