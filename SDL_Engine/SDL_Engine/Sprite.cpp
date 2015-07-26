#include "Sprite.h"
#include <assert.h>
#include "SDL.h"
#include "SDL_image.h"
#include "MathUtils.h"

Sprite::Sprite() {}

Sprite::Sprite( SDL_Renderer* renderer, SDL_Texture* image, bool animated ) :
Sprite( renderer, image, animated, Vec2() ) {}

Sprite::Sprite( SDL_Renderer* renderer, SDL_Texture* image, bool animated, Vec2 pos ) :
Sprite( renderer, image, animated, pos, 255 ) {}

Sprite::Sprite( SDL_Renderer* renderer, SDL_Texture* image, bool animated, Vec2 pos, Uint8 alpha ) :
pRenderer_( renderer ),
pImage_( image ),
animated_( animated ),
pos_( pos ) {
	assert( pImage_!=nullptr );
	setAlpha( alpha );
	if( !animated_ ) {
		int w, h;
		SDL_QueryTexture( pImage_, NULL, NULL, &w, &h );
		drawData_.frame_.w_ = (Sint16)w;
		drawData_.frame_.h_ = (Sint16)h;
		drawData_.frame_.halfW_ = w/2;
		drawData_.frame_.halfH_ = h/2;
	}
}

Sprite::Sprite( SDL_Renderer* renderer, std::string filename, bool animated = false ) :
Sprite( renderer, filename, animated, Vec2() ) {}

Sprite::Sprite( SDL_Renderer* renderer, std::string filename, bool animated, Vec2 pos ) :
Sprite( renderer, filename, animated, pos, 255 ) {}

Sprite::Sprite( SDL_Renderer* renderer, std::string filename, bool animated, Vec2 pos, Uint8 alpha ) :
pRenderer_( renderer ),
animated_( animated ),
pos_( pos ) {
	int err = 0;
#ifdef _WIN32
	const std::string pathSeparator = "\\";
#else
	const std::string pathSeparator = "/";
#endif
	std::string path = "sprites"+pathSeparator+filename;
	SDL_Surface* surface = IMG_Load( path.c_str() );
	if( surface==NULL ) {
		fprintf( stderr, "Failed to load '%s': %s\n", filename, IMG_GetError() );
		err = 1;
	}
	pImage_ = SDL_CreateTextureFromSurface( pRenderer_, surface );
	SDL_FreeSurface( surface );
	if( pImage_==nullptr ) {
		fprintf( stderr, "Failed to create texture: %s\n", SDL_GetError() );
		err = 2;
	}
	assert( pImage_!=nullptr );
	if( !animated ) {
		setAlpha( alpha );
		int w, h;
		SDL_QueryTexture( pImage_, NULL, NULL, &w, &h );
		drawData_.frame_.w_ = (Sint16)w;
		drawData_.frame_.h_ = (Sint16)h;
		drawData_.frame_.halfW_ = w/2;
		drawData_.frame_.halfH_ = h/2;
	}
}

Sprite::~Sprite() {
	if( pImage_ ) {
		SDL_DestroyTexture( pImage_ );
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

void Sprite::update( Uint32 dt ) {
	if( animated_ ) {
		drawData_.currentAnim_->update( dt );
	}
}

// draw will draw the image to the backbuffer but SDL_RenderPresent must be called somewhere else to actually be visible
void Sprite::draw(Vec2 cameraOffset) {
	SDL_Rect dest;
	if( animated_ ) {
		Sprite::Anim::Cell cell = drawData_.currentAnim_->getCurrentCell();
		SDL_Rect src;
		src.x = (Sint16)cell.x_+cell.offsetX_;
		src.y = (Sint16)cell.y_+cell.offsetY_;
		src.w = (Sint16)cell.w_;
		src.h = (Sint16)cell.h_;
		dest.x = (Sint16)pos_.x-cell.halfW_ - cameraOffset.x;
		dest.y = (Sint16)pos_.y-cell.halfH_ - cameraOffset.y;
		dest.w = (Sint16)cell.w_;
		dest.h = (Sint16)cell.h_;
		SDL_RenderCopy( pRenderer_, pImage_, &src, &dest );
	} else {
		dest.x = (Sint16)pos_.x-drawData_.frame_.halfW_ - cameraOffset.x;
		dest.y = (Sint16)pos_.y-drawData_.frame_.halfH_ - cameraOffset.y;
		dest.w = drawData_.frame_.w_;
		dest.h = drawData_.frame_.h_;
		SDL_RenderCopy( pRenderer_, pImage_, NULL, &dest );
	}
}

void Sprite::changeAnim( std::string newAnimName ) {
	Sprite::Anim* newAnim = &anims_[newAnimName];
	assert( newAnim!=nullptr );
	if( drawData_.currentAnim_!=newAnim ) {
		drawData_.currentAnim_ = &anims_[newAnimName];
		drawData_.currentAnim_->reset();
	}
}

void Sprite::addAnim( std::string animName, Sprite::Anim anim ) {
	anims_[animName] = anim;
}

Sint16 Sprite::getHalfWidth() {
	if( animated_ ) {
		return drawData_.currentAnim_->getCurrentCell().halfW_;
	} else {
		return drawData_.frame_.halfW_;
	}
}

Sint16 Sprite::getHalfHeight() {
	if( animated_ ) {
		return drawData_.currentAnim_->getCurrentCell().halfH_;
	} else {
		return drawData_.frame_.halfH_;
	}
}

Sprite::Anim::Anim() {}
Sprite::Anim::Anim( Uint16 numCells, std::vector<Uint16> sequence, Uint32 frameTime, bool loop ) :
sequence_( sequence ),
frameTime_( frameTime ),
loop_( loop ),
currentFrame_( 0 ),
currentTime_( 0 ) {
	cells_ = std::vector<Cell>( numCells );
}

void Sprite::Anim::addCell( Uint16 cellIdx, Cell cell ) {
	cells_[cellIdx] = cell;
}

void Sprite::Anim::update( Uint32 dt ) {
	currentTime_ += dt;
	while( currentTime_>frameTime_ ) {
		if( loop_ ) {
			currentFrame_ = (currentFrame_+1)%(sequence_.size()-1);
		} else {
			currentFrame_ = SDL_min( currentFrame_+1, (Uint16)(sequence_.size()-1) );
		}
		currentTime_ -= frameTime_;
	}
}

void Sprite::Anim::reset() {
	currentFrame_ = 0;
	currentTime_ = 0;
}

const Sprite::Anim::Cell& Sprite::Anim::getCurrentCell() {
	return cells_[currentFrame_];
}

Sprite::Anim::Cell::Cell() {}
Sprite::Anim::Cell::Cell( Uint16 x, Uint16 y, Uint16 w, Uint16 h, Uint16 offsetX, Uint16 offsetY ) : x_( x ), y_( y ), w_( w ), h_( h ), offsetX_( offsetX ), offsetY_( offsetY ) {
	halfW_ = w_/2;
	halfH_ = h_/2;
}
