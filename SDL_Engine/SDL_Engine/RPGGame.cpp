#include "RPGGame.h"
#include <stdio.h>
#include <string>
#include "ObjectFactory.h"
#include "LevelLoader.h"

RPGGame::RPGGame() : Game(), pPlayer_(nullptr), level_(nullptr) {}


RPGGame::~RPGGame() {}

void RPGGame::loadAssets() {
	ObjectFactory::Init( renderer_ );
	pPlayer_ = ObjectFactory::Instantiate( GameObject::PLAYER, getScreenSize() * 0.5f );
	startLevel( Level::CAVE );
}

void RPGGame::update( Uint32 dt ) {
	pPlayer_->update( dt );
}

void RPGGame::draw() {
	level_->drawLayer0();
	pPlayer_->draw();
	level_->drawLayer1();
	Game::draw();
}

void RPGGame::onKeyDown( Uint32 key ) {
	if( key==SDLK_ESCAPE ) {
		running_ = false;
	}
	if( key==SDLK_UP
		||key==SDLK_DOWN
		||key==SDLK_LEFT
		||key==SDLK_RIGHT ) {
		static_cast<Player*>(pPlayer_)->onKeyDown( key );
	}
}

void RPGGame::onKeyUp( Uint32 key ) {
	if( key==SDLK_UP
		||key==SDLK_DOWN
		||key==SDLK_LEFT
		||key==SDLK_RIGHT ) {
		static_cast<Player*>(pPlayer_)->onKeyUp( key );
	}
}

void RPGGame::startLevel( Level::LEVEL level ) {
	LevelLoader levelLoader(renderer_, screenWidth_, screenHeight_);
	level_ = levelLoader.loadLevel( level );
}
