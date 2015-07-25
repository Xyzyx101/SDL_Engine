#pragma once
#include <string>
#include <vector>
#include "SDL.h"

class Level;
class json;

class LevelLoader {
public:
	LevelLoader();
	~LevelLoader();
	Level* loadLevel( const std::string& filename );
private:
	SDL_Texture* createLayerTexture(SDL_Texture* tileset, std::vector<Uint16> data);
};

