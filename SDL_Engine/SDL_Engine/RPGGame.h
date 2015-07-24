#pragma once
#include "Game.h"
#include "Player.h"
class RPGGame :
	public Game {
public:
	RPGGame();
	virtual ~RPGGame();
private:
	void loadAssets();
	void update( Uint32 dt );
	void draw();
	void onKeyDown( Uint32 key );
	void onKeyUp( Uint32 key );
	GameObject*		pPlayer_;
};

