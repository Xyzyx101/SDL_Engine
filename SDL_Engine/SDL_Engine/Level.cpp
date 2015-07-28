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

void Level::drawLayer( LAYER layerName, Vec2 cameraOffset ) {
	LayerMap layer;
	switch( layerName ) {
	case LAYER0:
		layer = layer0_;
		break;
	case LAYER1:
		layer = layer1_;
		break;
	}
	SDL_Rect src, dest;
	src.w = tileWidth_;
	src.h = tileHeight_;
	dest.w = tileWidth_;
	dest.h = tileHeight_;
	Sint32 left = cameraOffset.x-tileWidth_;
	Sint32 right = cameraOffset.x+screenWidth_+tileWidth_;
	Sint32 top = cameraOffset.y-tileHeight_;
	Sint32 bottom = cameraOffset.y+screenHeight_+tileHeight_;
	int texWidth, texHeight;
	SDL_QueryTexture( tileTexture_, NULL, NULL, &texWidth, &texHeight );
	for( auto rowIt = layer.lower_bound( top ); rowIt!=layer.upper_bound( bottom ); ++rowIt ) {
		for( auto colIt = rowIt->second.lower_bound( left ); colIt!=rowIt->second.upper_bound( right ); ++colIt ) {
			dest.x = colIt->first-cameraOffset.x;
			dest.y = rowIt->first-cameraOffset.y;
			Uint16 tileIdx = colIt->second-1; //minus 1 because 0 is no tile
			src.x = (Sint16)((tileIdx * tileWidth_)%texWidth);
			src.y = (Sint16)((tileIdx * tileWidth_)/texWidth * tileHeight_);
			SDL_RenderCopy( renderer_, tileTexture_, &src, &dest );
		}
	}
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

Uint16 Level::getTileWidth() {
	return tileWidth_;
}

Uint16 Level::getTileHeight() {
	return tileHeight_;
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
