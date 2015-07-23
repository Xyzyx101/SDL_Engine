#pragma once
#include "Game.h"
#include "Sprite.h"
class RPGGame :
	public Game {
public:
	RPGGame();
	virtual ~RPGGame();
private:
	void loadAssets();
	void update( Uint32 dt );
	void draw();

	Sprite*		pTestSprite_;
};

