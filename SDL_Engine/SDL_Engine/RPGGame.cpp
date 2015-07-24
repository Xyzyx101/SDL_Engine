#include "RPGGame.h"
#include<stdio.h>
#include "ObjectFactory.h"

RPGGame::RPGGame() : Game() {}


RPGGame::~RPGGame() {}

void RPGGame::loadAssets() {
	ObjectFactory::Init( renderer_ );
	//pTestSprite_ = new Sprite( renderer_, "party.png", false, getScreenSize() * 0.5f );
	pPlayer_ = ObjectFactory::Instantiate( GameObject::PLAYER, getScreenSize() * 0.5f );
}

void RPGGame::update( Uint32 dt ) {
	pPlayer_->update( dt );
}

void RPGGame::draw() {
	pPlayer_->draw();
	Game::draw();
}
