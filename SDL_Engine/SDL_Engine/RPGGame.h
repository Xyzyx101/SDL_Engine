#pragma once
#include "Game.h"
#include "SDL.h"
#include <string>
#include <vector>
#include "Player.h"
#include "Level.h"
#include "Spawner.h"
#include "SDL_ttf.h"

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
	void onMouseDown( Vec2 coords );
	void checkPlayerBounds();
	void updateCamera();
	void removeDeadObjects();
	void drawHUD();
	void updateHud();
	void restartLevel();
	Player*						pPlayer_;
	std::vector<GameObject*>	enemies_;
	std::vector<GameObject*>	spells_;
	std::vector<Spawner*>		spawners_;
	Level*						level_;
	Vec2						cameraOffset_;
	Uint16						gold_;
	Sint16						playerHP_;
	TTF_Font*					pHudFont_;
	SDL_Texture*				pHudTexture_;
	SDL_Texture*				pHudDeadTexture_;
	Sint32						deadTimer_;
	bool						playerDead_;
};

