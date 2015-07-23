#include "RPGGame.h"
#include<stdio.h>

RPGGame::RPGGame() : Game() {}


RPGGame::~RPGGame() {}

void RPGGame::loadAssets() {
	pTestSprite_ = new Sprite( renderer_, "party.png", false, getScreenSize() * 0.5f );
}

void RPGGame::update( Uint32 dt ) {

}

void RPGGame::draw() {
	pTestSprite_->draw();
	Game::draw();
}
