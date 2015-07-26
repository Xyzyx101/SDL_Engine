#include "Level.h"

Level::Level( SDL_Renderer* renderer, Uint16 width, Uint16 height, Uint16 screenWidth, Uint16 screenHeight, SDL_Texture* layer0Texture, SDL_Texture* layer1Texture ) :
renderer_( renderer ),
width_( width ),
height_( height ),
screenWidth_( screenWidth ),
screenHeight_( screenHeight ),
layer0Texture_( layer0Texture ),
layer1Texture_( layer1Texture ) {}

Level::~Level() {
	SDL_DestroyTexture( layer0Texture_ );
	SDL_DestroyTexture( layer1Texture_ );
}

void Level::drawLayer0( Vec2 cameraOffset ) {
	SDL_Rect src, dest;
	src.x = 0;
	src.y = 0;
	src.w = screenWidth_;
	src.h = screenHeight_;
	dest.x = -cameraOffset.x;
	dest.y = -cameraOffset.y;
	dest.w = screenWidth_;
	dest.h = screenHeight_;
	SDL_RenderCopy( renderer_, layer0Texture_, &src, &dest );
}

void Level::drawLayer1( Vec2 cameraOffset ) {
	SDL_Rect src, dest;
	src.x = 0;
	src.y = 0;
	src.w = screenWidth_;
	src.h = screenHeight_;
	dest.x = -cameraOffset.x;
	dest.y = -cameraOffset.y;
	dest.w = screenWidth_;
	dest.h = screenHeight_;
	SDL_RenderCopy( renderer_, layer1Texture_, &src, &dest );
}

Uint16 Level::getWidth() {
	return width_;
}

Uint16 Level::getHeight() {
	return height_;
}