#include "RPGGame.h"
#include <stdio.h>
#include <string>
#include "ObjectFactory.h"
#include "LevelLoader.h"

RPGGame::RPGGame() : Game(), pPlayer_( nullptr ), level_( nullptr ), cameraOffset_( Vec2( 0, 0 ) ) {}

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
	updateCamera();
	//fprintf( stdout, "x: %f, y: %f\n", pPlayer_->getPos().x, pPlayer_->getPos().y );
}

void RPGGame::draw() {
	level_->drawLayer0( cameraOffset_ );
	pPlayer_->draw( cameraOffset_ );
	level_->drawLayer1( cameraOffset_ );
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

void RPGGame::updateCamera() {
	Sint16 cameraBoundx = screenWidth_/4;
	Sint16 cameraBoundY = screenHeight_/4;
	Vec2 playerPos = pPlayer_->getPos();
	if( playerPos.x-cameraOffset_.x<cameraBoundx ) {
		cameraOffset_.x = playerPos.x-cameraBoundx;
	} else if( playerPos.x > cameraOffset_.x+screenWidth_-cameraBoundx ) {
		cameraOffset_.x = playerPos.x-screenWidth_+cameraBoundx;
	}
	if( playerPos.y-cameraOffset_.y<cameraBoundY ) {
		cameraOffset_.y = playerPos.y-cameraBoundY;
	} else if( playerPos.y > cameraOffset_.y+screenHeight_-cameraBoundY ) {
		cameraOffset_.y = playerPos.y-screenHeight_+cameraBoundY;
	}
	if( cameraOffset_.x<0 ) {
		cameraOffset_.x = 0;
	} else if( cameraOffset_.x + screenWidth_>level_->getWidth() ) {
		cameraOffset_.x = level_->getWidth()-screenWidth_;
	}
	if( cameraOffset_.y<0 ) {
		cameraOffset_.y = 0;
	} else if( cameraOffset_.y+screenHeight_>level_->getHeight() ) {
		cameraOffset_.y = level_->getHeight()-screenHeight_;
	}
}
