#pragma once

#include <stdio.h>
#include "MathUtils.h"
#include "SDL.h"

class Sprite {
public:
	Sprite( SDL_Renderer* renderer, SDL_Texture* image );
	Sprite( SDL_Renderer* renderer, SDL_Texture* image, Vec2 pos );
	Sprite( SDL_Renderer* renderer, SDL_Texture* image, Vec2 pos, Uint8 alpha );
	Sprite( SDL_Renderer* renderer, char* fileName );
	Sprite( SDL_Renderer* renderer, char* fileName, Vec2 pos );
	Sprite( SDL_Renderer* renderer, char* fileName, Vec2 pos, Uint8 alpha );

	virtual ~Sprite() {
		if( pImage_ ) {
			SDL_DestroyTexture( pImage_ );
		}
	}

	inline Vec2 getPos() {
		return pos_;
	}

	inline void setPos( const Vec2 pos ) {
		pos_ = pos;
	}

	Uint8 getAlpha();
	void setAlpha( const Uint8 alpha );
	virtual void Draw();

protected:
	int loadImage( char* filename );
	SDL_Renderer*	pRenderer_;
	SDL_Texture*	pImage_;
	Vec2			pos_;
	Sint16			w_;
	Sint16			h_;
	Sint16			halfW_;
	Sint16			halfH_;
};

