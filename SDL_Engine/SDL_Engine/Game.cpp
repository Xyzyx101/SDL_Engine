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
screenHeight_( DEFAULT_SCREEN_HEIGHT ) {}

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
	loadAssets();
	return 0;
}

void Game::Run() {
	Uint32 lastTime = SDL_GetTicks(), currentTime = 0, dt;
	SDL_Event newEvent;
	while( running_ ) {
		while( SDL_PollEvent( &newEvent ) ) {
			handleEvent( newEvent );
		}

		currentTime = SDL_GetTicks();
		dt = currentTime-lastTime;
		lastTime = currentTime;

		//clear renderer
		SDL_SetRenderDrawColor( renderer_, CLEAR_COLOR_R, CLEAR_COLOR_G, CLEAR_COLOR_B, 255 );
		SDL_RenderClear( renderer_ );
		update( dt );
		draw();
	}
}

void Game::draw() {

	// rendering should happen in inherited class before calling this

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

void Game::onKeyDown( Uint32 key ) {}

void Game::onKeyUp( Uint32 key ) {}


Vec2 Game::getScreenSize() {
	return Vec2( screenWidth_, screenHeight_ );
}

void Game::Clean() {
	SDL_Quit();
}