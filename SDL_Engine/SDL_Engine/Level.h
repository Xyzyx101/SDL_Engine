#pragma once
#include <vector>
#include "SDL.h"

class Level {
public:
	Level();
	Level(SDL_Texture* texture, std::string filename, const SDL_PixelFormat* format, Uint16 screenWidth, Uint16 screenHeight);
	~Level();
	void draw( SDL_Surface* dest );
	Uint16 getWidth();
	Uint16 getHeight();
private:
	SDL_Texture*		pTexture_;
	Uint16**			ppTileMapLayer0_;
	Uint16**			ppTileMapLayer1_;
	std::vector<SDL_Rect> rectMap_;
	Uint16	tileMapHeight_;
	Uint16	tileMapWidth_;

	std::string levelInfoFilename_;
	std::string tileRectsFilename_;
	std::string levelTilesFilename_;

	void loadLevelData();
	void blitTiles();

	void loadLevelInfo();
	void loadTileRects();
	void loadLevelTiles();
};

