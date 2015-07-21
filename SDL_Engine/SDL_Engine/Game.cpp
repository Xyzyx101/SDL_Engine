#include "Game.h"
#include <stdio.h>
#include "SDL_image.h"
#include "MathUtils.h"

#define DEFAULT_SCREEN_WIDTH 1024
#define DEFAULT_SCREEN_HEIGHT 768

#define CLEAR_COLOR_R 0
#define CLEAR_COLOR_G 0
#define CLEAR_COLOR_B 0

Game::Game( void ) :
running_( true ),
window_( 0 ),
renderer_( 0 ),
screenWidth_( DEFAULT_SCREEN_WIDTH ),
screenHeight_( DEFAULT_SCREEN_HEIGHT ),
testImage_( 0 ) {}

Game::~Game( void ) {}

int Game::InitSDL() {
	if( SDL_Init( SDL_INIT_EVERYTHING )<0 ) {
		fprintf( stderr, "Unable to init SDL: %s\n", SDL_GetError() );
		SDL_Quit();
		return 1;
	}

	window_ = SDL_CreateWindow(
		"SDL Test Window",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		screenWidth_,
		screenHeight_,
		SDL_WINDOW_SHOWN );
	if( window_==nullptr ) {
		fprintf( stderr, "Create window failed: %s\n", SDL_GetError() );
		SDL_Quit();
		return 2;
	}
	renderer_ = SDL_CreateRenderer( window_, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC );
	if( renderer_==nullptr ) {
		fprintf( stderr, "Create Renderer fails: %s\n", SDL_GetError() );
		SDL_DestroyWindow( window_ );
		SDL_Quit();
		return 3;
	}
	SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "linear" );  // make the scaled rendering look smoother.
	SDL_RenderSetLogicalSize( renderer_, screenWidth_, screenHeight_ );
	return 0;
}

int Game::InitTestImage() {
	SDL_Surface* surface = IMG_Load( "party.png" );
	if( surface==NULL ) {
		fprintf( stderr, "Image failed to load: %s\n", IMG_GetError() );
		SDL_DestroyWindow( window_ );
		SDL_Quit();
		return 4;
	}
	testImage_ = SDL_CreateTextureFromSurface( renderer_, surface );
	SDL_FreeSurface( surface );
	if( testImage_==nullptr ) {
		fprintf( stderr, "Failed to create texture: %s\n", SDL_GetError() );
		SDL_DestroyRenderer( renderer_ );
		SDL_DestroyWindow( window_ );
		SDL_Quit();
		return 5;
	}
	return 0;
}

void Game::Run() {
	SDL_Event newEvent;
	while( running_ ) {
		while( SDL_PollEvent( &newEvent ) ) {
			handleEvent( newEvent );
		}
		pos_ += velocity_;
		Draw();
	}
}

void Game::Draw() {
	//clear renderer
	SDL_SetRenderDrawColor( renderer_, CLEAR_COLOR_R, CLEAR_COLOR_G, CLEAR_COLOR_B, 255 );
	SDL_RenderClear( renderer_ );

	SDL_Rect dstRect;
	dstRect.x = (Sint16)pos_.x;
	dstRect.y = (Sint16)pos_.y;
	dstRect.w = screenWidth_;
	dstRect.h = screenHeight_;

	//draw texture
	SDL_RenderCopy( renderer_, testImage_, NULL, &dstRect );

	//update screen
	SDL_RenderPresent( renderer_ );
}

void Game::handleEvent( const SDL_Event& e ) {
	switch( e.type ) {
	case SDL_QUIT:
		running_ = false;
		break;
	case SDL_KEYDOWN:
		onKeyDown( e.key.keysym.sym );
		break;
	case SDL_KEYUP:
		onKeyUp( e.key.keysym.sym );
		break;
	}
}

void Game::onKeyDown( Uint32 key ) {
	if( key==SDLK_UP ) {
		velocity_.y -= 1.0f;
	}
	if( key==SDLK_DOWN ) {
		velocity_.y += 1.0f;
	}
	if( key==SDLK_LEFT ) {
		velocity_.x -= 1.0f;
	}
	if( key==SDLK_RIGHT ) {
		velocity_.x += 1.0f;
	}
}

void Game::onKeyUp( Uint32 key ) {
	if( key==SDLK_UP ) {
		velocity_.y = 0.0f;
	}
	if( key==SDLK_DOWN ) {
		velocity_.y = 0.0f;
	}
	if( key==SDLK_LEFT ) {
		velocity_.x = 0.0f;
	}
	if( key==SDLK_RIGHT ) {
		velocity_.x = 0.0f;
	}
}

void Game::Clean() {
	SDL_Quit();
}