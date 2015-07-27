#include "Collision.h"
#include <math.h>
#include <algorithm>
#include "SDL.h"
#include "GameObject.h"

#define ZERO 0.000001f

Collision::Collision() {}

Collision::~Collision() {}

Vec2 Collision::RectToRectCollision( const Vec2& r1Pos, float r1W, float r1H, const Vec2& r2Pos, float r2W, float r2H ) {
	Vec2 correctionValue( 0, 0 );
	float r1HalfWidth = r1W * 0.5f;
	float r1HalfHeight = r1H * 0.5f;
	float r2HalfWidth = r2W * 0.5f;
	float r2HalfHeight = r2H * 0.5;

	Vec2 r1Center( r1Pos.x+r1HalfWidth, r1Pos.y+r1HalfHeight );
	Vec2 r2Center( r2Pos.x+r2HalfWidth, r2Pos.y+r2HalfHeight );

	float overlapX = (r1HalfWidth+r2HalfWidth)-abs( r1Center.x-r2Center.x );
	float overlapY = (r1HalfHeight+r2HalfHeight)-abs( r1Center.y-r2Center.y );
	if( overlapX>ZERO && overlapY>ZERO ) {

		if( overlapX<overlapY ) {
			if( r1Center.x<r2Center.x ) {
				overlapX *= -1.0f;
			}
			correctionValue.x = overlapX;
			return correctionValue;
		} else {
			if( r1Center.y<r2Center.y ) {
				overlapY *= -1.0f;
			}
			correctionValue.y = overlapY;
			return correctionValue;
		}
	} else {
		// no collision
		return correctionValue;
	}
}

bool Collision::PixelCollision( SDL_Renderer* DELETEME, GameObject* obj1, GameObject* obj2 ) {
	Vec2 rectCollision = Collision::RectToRectCollision( obj1->getPos(), obj1->getHalfWidth()* 2.0f, obj1->getHalfHeight()*2.0f, obj2->getPos(), obj2->getHalfWidth()*2.0f, obj2->getHalfHeight()*2.0f );
	if( rectCollision==Vec2::Zero ) {
		return false;
	}
	Sint16 left = std::max( obj1->getLeft(), obj2->getLeft() );
	Sint16 right = std::min( obj1->getRight(), obj2->getRight() );
	Sint16 top = std::max( obj1->getTop(), obj2->getTop() );
	Sint16 bottom = std::min( obj1->getBottom(), obj2->getBottom() );
	SDL_Texture* obj1Texture = obj1->getTextureOverlap( left, right, top, bottom );
	SDL_Texture* obj2Texture = obj2->getTextureOverlap( left, right, top, bottom );
	SDL_Rect dest;
	dest.x = 0;
	dest.y = 0;
	dest.w = right-left;
	dest.h = bottom-top;
	SDL_RenderCopy( DELETEME, obj1Texture, NULL, &dest );
	dest.x = 32;
	SDL_RenderCopy( DELETEME, obj2Texture, NULL, &dest );
	return false;
}