#include "Game.h"
#include "SDL_image.h"
#include <stdio.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

Game::Game() :
window_(0),
renderer_(0),
testImage_(0),
texture_(0),
surface_(0)
{
}

Game::~Game()
{
}

int Game::InitSDL() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		return -1;
	}

	if ((window_ = SDL_CreateWindow(
		"Bacon",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_FULLSCREEN_DESKTOP)
		) == NULL) {
		return -2;
	}
	renderer_ = SDL_CreateRenderer(window_, -1, 0);

	texture_ = SDL_CreateTexture(
		renderer_,
		SDL_PIXELFORMAT_ABGR8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);
	return 0;
}

int Game::InitTestImage()
{
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
	SDL_RenderSetLogicalSize(renderer_, SCREEN_WIDTH, SCREEN_HEIGHT);
	surface_ = IMG_Load("party.png");
	if (surface_ == NULL) {
		printf("Image failed to load");
		return -1;
	}
	printf("surface: %p\n", surface_);
 	printf("pixels: %p\n", surface_->pixels);
	return 0;
}

void Game::Run()
{
	int i = 0; while (++i < 500) {
		Draw();
	}
}

void Game::Draw()
{
	int err = SDL_UpdateTexture(texture_, NULL, surface_->pixels, surface_->pitch);
	if (err != 0) {
		fprintf(stdin, "Error in SDL_UpdateTexture");
	}
	SDL_RenderClear(renderer_);
	SDL_Rect srcRect = SDL_Rect();
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = surface_->w / 2;
	srcRect.h = surface_->h / 2;

	SDL_Rect dstRect = SDL_Rect();
	dstRect.x = 0;


	dstRect.y = 0;
	dstRect.w = SCREEN_WIDTH / 2;
	dstRect.h = SCREEN_HEIGHT / 2;

	SDL_RenderCopy(renderer_, texture_, &srcRect, &dstRect);
	if (err != 0) {
		fprintf(stdin, "Error in SDL_RenderCopy");
	}
	SDL_RenderPresent(renderer_);
}

void Game::Clean()
{
	SDL_Quit();
}