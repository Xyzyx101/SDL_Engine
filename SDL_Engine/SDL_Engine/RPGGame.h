#pragma once
#include "Game.h"
#include <string>
#include "Player.h"
#include "Level.h"

class RPGGame :
	public Game {
public:
	RPGGame();
	virtual ~RPGGame();
	void startLevel( Level::LEVEL );
private:
	void loadAssets();
	void update( Uint32 dt );
	void draw();
	void onKeyDown( Uint32 key );
	void onKeyUp( Uint32 key );
	void checkPlayerBounds();
	Player*			pPlayer_;
	Level*			level_;
};

