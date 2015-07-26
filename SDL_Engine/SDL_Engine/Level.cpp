#include "Level.h"
#include "Collision.h"

Level::Level( SDL_Renderer* renderer, Uint16 width, Uint16 height, Uint16 screenWidth, Uint16 screenHeight, Uint16 tileWidth, Uint16 tileHeight ) :
renderer_( renderer ),
width_( width ),
height_( height ),
screenWidth_( screenWidth ),
screenHeight_( screenHeight ),
tileWidth_( tileHeight ),
tileHeight_( tileHeight ) {}

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

Vec2 Level::checkCollision( Vec2 pos, Sint32 halfWidth, Sint32 halfHeight ) {
	Vec2 collision = Vec2();
	Sint16 firstX = ((Sint16)pos.x-halfWidth)/tileWidth_*tileWidth_; //multiply and divide quantizes the pos to the tile size
	Sint16 lastX = ((Sint16)pos.x+halfWidth)/tileWidth_*tileWidth_;
	Sint16 firstY = ((Sint16)pos.y-halfHeight)/tileHeight_*tileHeight_;
	Sint16 lastY = ((Sint16)pos.y+halfHeight)/tileHeight_*tileHeight_;
	for( auto rowIt = collisionLayer_.lower_bound( firstY ); rowIt!=collisionLayer_.upper_bound( lastY ); ++rowIt ) {
		for( auto colIt = rowIt->second.lower_bound( firstX ); colIt!=rowIt->second.upper_bound( lastX ); ++colIt ) {
			collision = Collision::RectToRectCollision( pos, halfWidth*2, halfHeight*2, Vec2( (*colIt)+tileHeight_/2, rowIt->first+tileWidth_/2 ), tileWidth_, tileHeight_ );
			if( collision!=Vec2::Zero ) {
				return collision;
			}
		}
	}
	return collision;
}
