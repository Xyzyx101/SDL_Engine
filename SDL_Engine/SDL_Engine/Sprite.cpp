#include "Sprite.h"
#include "SDL.h"
#include "SDL_image.h"
#include "MathUtils.h"

Sprite::Sprite( SDL_Renderer* renderer, SDL_Texture* image ) :
Sprite( renderer, image, Vec2() ) {}

Sprite::Sprite( SDL_Renderer* renderer, SDL_Texture* image, Vec2 pos ) :
Sprite( renderer, image, pos, 255 ) {}

Sprite::Sprite( SDL_Renderer* renderer, SDL_Texture* image, Vec2 pos, Uint8 alpha ) :
pRenderer_( renderer ),
pImage_( image ),
pos_( pos ) {
	setAlpha( alpha );
	int w, h;
	SDL_QueryTexture( pImage_, NULL, NULL, &w, &h );
	w_ = (Sint16)w;
	h_ = (Sint16)h;
	halfW_ = w_/2;
	halfH_ = h_/2;
}

Sprite::Sprite( SDL_Renderer* renderer, char* filename ) :
Sprite( renderer, filename, Vec2() ) {}

Sprite::Sprite( SDL_Renderer* renderer, char* filename, Vec2 pos ) :
Sprite( renderer, filename, pos, 255 ) {}

Sprite::Sprite( SDL_Renderer* renderer, char* filename, Vec2 pos, Uint8 alpha ) :
pRenderer_( renderer ) {
	int err = 0;
	SDL_Surface* surface = IMG_Load( filename );
	if( surface==NULL ) {
		fprintf( stderr, "Image failed to load: %s\n", IMG_GetError() );
		err = 1;
	}
	pImage_ = SDL_CreateTextureFromSurface( pRenderer_, surface );
	SDL_FreeSurface( surface );
	if( pImage_==nullptr ) {
		fprintf( stderr, "Failed to create texture: %s\n", SDL_GetError() );
		err = 2;
	}
	if( err==0 ) {
		setAlpha( alpha );
		int w, h;
		SDL_QueryTexture( pImage_, NULL, NULL, &w, &h );
		w_ = (Sint16)w;
		h_ = (Sint16)h;
		halfW_ = w_/2;
		halfH_ = h_/2;
	} else {
		w_ = 0;
		h_ = 0;
		halfW_ = 0;
		halfH_ = 0;
	}
}

Uint8 Sprite::getAlpha() {
	Uint8 myAlpha;
	int err = SDL_GetTextureAlphaMod( pImage_, &myAlpha );
	if( err!=0 ) {
		fprintf( stderr, "Error getting alpha: %s\n", SDL_GetError() );
		return 0;
	} else {
		return myAlpha;
	}
}

void Sprite::setAlpha( const Uint8 alpha ) {
	int err = SDL_SetTextureAlphaMod( pImage_, alpha );
	if( err!=0 ) {
		fprintf( stderr, "Error setting alpha: %s\n", SDL_GetError() );
	}
}

int Sprite::loadImage( char* filename ) {
	SDL_Surface* surface = IMG_Load( filename );
	if( surface==NULL ) {
		fprintf( stderr, "Image failed to load: %s\n", IMG_GetError() );
		return 1;
	}
	pImage_ = SDL_CreateTextureFromSurface( pRenderer_, surface );
	SDL_FreeSurface( surface );
	if( pImage_==nullptr ) {
		fprintf( stderr, "Failed to create texture: %s\n", SDL_GetError() );
		return 2;
	}
	return 0;
}

void Sprite::Draw() {
	SDL_Rect dest;
	dest.x = (Sint16)pos_.x-halfW_;
	dest.y = (Sint16)pos_.y-halfH_;
	dest.w = w_;
	dest.h = h_;

	//This will draw the image to the backbuffer but SDL_RenderPresent must be called somewhere else to actually be visible
	SDL_RenderCopy( pRenderer_, pImage_, NULL, &dest );
}

