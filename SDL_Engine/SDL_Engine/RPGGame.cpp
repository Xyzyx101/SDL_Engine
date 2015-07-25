#include "RPGGame.h"
#include <stdio.h>
#include <string>
#include "ObjectFactory.h"
#include "LevelLoader.h"

RPGGame::RPGGame() : Game() {}


RPGGame::~RPGGame() {}

void RPGGame::loadAssets() {
	ObjectFactory::Init( renderer_ );
	pPlayer_ = ObjectFactory::Instantiate( GameObject::PLAYER, getScreenSize() * 0.5f );
}

void RPGGame::update( Uint32 dt ) {
	pPlayer_->update( dt );
}

void RPGGame::draw() {
	pPlayer_->draw();
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

void RPGGame::startLevel( std::string level ) {
	LevelLoader levelLoader;
	levelLoader.loadLevel( level );
}
