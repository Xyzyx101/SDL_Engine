#include "RPGGame.h"
#include <stdio.h>
#include <string>
#include "ObjectFactory.h"
#include "LevelLoader.h"

RPGGame::RPGGame() : Game(), pPlayer_( nullptr ), level_( nullptr ) {}


RPGGame::~RPGGame() {}

void RPGGame::loadAssets() {
	ObjectFactory::Init( renderer_ );
	GameObject* playerObject = ObjectFactory::Instantiate( GameObject::PLAYER, getScreenSize() * 0.5f );
	pPlayer_ = static_cast<Player*>(playerObject);
	startLevel( Level::CAVE );
}

void RPGGame::update( Uint32 dt ) {
	pPlayer_->update( dt );
	checkPlayerBounds();
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
		pPlayer_->onKeyDown( key );
	}
}

void RPGGame::onKeyUp( Uint32 key ) {
	if( key==SDLK_UP
		||key==SDLK_DOWN
		||key==SDLK_LEFT
		||key==SDLK_RIGHT ) {
		pPlayer_->onKeyUp( key );
	}
}

void RPGGame::startLevel( Level::LEVEL level ) {
	LevelLoader levelLoader( renderer_, screenWidth_, screenHeight_ );
	level_ = levelLoader.loadLevel( level );
}

void RPGGame::checkPlayerBounds() {
	if( pPlayer_->getLeft()<0 ) {
		Vec2 newPos = Vec2( pPlayer_->getHalfWidth(), pPlayer_->getPos().y );
		pPlayer_->setPos( newPos );
	} else if( pPlayer_->getRight()>level_->getWidth() ) {
		Vec2 newPos = Vec2( level_->getWidth()-pPlayer_->getHalfWidth(), pPlayer_->getPos().y );
		pPlayer_->setPos( newPos );
	}
	if( pPlayer_->getTop()<0 ) {
		Vec2 newPos = Vec2( pPlayer_->getPos().x, pPlayer_->getHalfHeight() );
		pPlayer_->setPos( newPos );
	} else if( pPlayer_->getBottom()>level_->getHeight() ) {
		Vec2 newPos = Vec2( pPlayer_->getPos().x, level_->getHeight()-pPlayer_->getHalfHeight() );
		pPlayer_->setPos( newPos );
	}
}
