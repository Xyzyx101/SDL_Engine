#include "Game.h"
#include "SDL_image.h"
#include <stdio.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

Game::Game() :
window_( 0 ),
renderer_( 0 ),
testImage_( 0 ),
texture_( 0 ),
surface_( 0 ) {}

Game::~Game() {}

int Game::InitSDL() {
	if( SDL_Init( SDL_INIT_EVERYTHING )<0 ) {
		fprintf( stderr, "Unable to init SDL: %s\n", SDL_GetError() );
		SDL_Quit();
		return 1;
	}

	window_ = SDL_CreateWindow(
		"SDL Test Window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_FULLSCREEN_DESKTOP );
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
	SDL_RenderSetLogicalSize( renderer_, SCREEN_WIDTH, SCREEN_HEIGHT );
	return 0;
}

int Game::InitTestImage() {
	surface_ = IMG_Load( "party.png" );
	if( surface_==NULL ) {
		fprintf( stderr, "Image failed to load: %s\n", IMG_GetError() );
		SDL_DestroyWindow( window_ );
		SDL_Quit();
		return 4;
	}
	texture_ = SDL_CreateTextureFromSurface( renderer_, surface_ );
	SDL_FreeSurface( surface_ );
	if( texture_==nullptr ) {
		fprintf( stderr, "Failed to create texture: %s\n", SDL_GetError() );
		SDL_DestroyRenderer( renderer_ );
		SDL_DestroyWindow( window_ );
		SDL_Quit();
		return 5;
	}
	return 0;
}

void Game::Run() {
	int i = 0; while( ++i<5 ) {
		Draw();
		SDL_Delay( 1000 );
	}
}

void Game::Draw() {
	//draws 1/4 of the image on 1/4 of the screen

	SDL_Rect srcRect = SDL_Rect();
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = surface_->w/2;
	srcRect.h = surface_->h/2;

	SDL_Rect dstRect = SDL_Rect();
	dstRect.x = 0;
	dstRect.y = 0;
	dstRect.w = SCREEN_WIDTH/2;
	dstRect.h = SCREEN_HEIGHT/2;

	//clear renderer
	SDL_RenderClear( renderer_ );

	//draw texture
	SDL_RenderCopy( renderer_, texture_, &srcRect, &dstRect );

	//update screen
	SDL_RenderPresent( renderer_ );
}

void Game::Clean() {
	SDL_Quit();
}