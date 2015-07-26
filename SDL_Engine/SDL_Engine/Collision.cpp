#include "Collision.h"
#include <math.h>

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
		/*
		if( r1Center.x<r2Center.x ) {
			overlapX *= -1.0f;
		}
		correctionValue.x = overlapX;
		if( r1Center.y<r2Center.y ) {
			overlapY *= -1.0f;
		}
		correctionValue.y = overlapY;
		return correctionValue;*/
	} else {
		// no collision
		return correctionValue;
	}
}
