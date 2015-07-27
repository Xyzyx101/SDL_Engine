#pragma once
#include "Game.h"
#include "SDL.h"
#include <string>
#include <vector>
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
	void updateCamera();
	Player*						pPlayer_;
	std::vector<GameObject*>	enemies_;
	Level*						level_;
	Vec2						cameraOffset_;
};

