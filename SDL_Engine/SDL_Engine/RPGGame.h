#pragma once
#include "Game.h"
#include <string>
#include "Player.h"
class RPGGame :
	public Game {
public:
	RPGGame();
	virtual ~RPGGame();
	void startLevel( std::string level );
private:
	void loadAssets();
	void update( Uint32 dt );
	void draw();
	void onKeyDown( Uint32 key );
	void onKeyUp( Uint32 key );
	GameObject*		pPlayer_;
};

