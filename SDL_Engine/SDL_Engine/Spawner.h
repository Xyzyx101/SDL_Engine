#pragma once
#include "SDL.h"
#include "MathUtils.h"
#include "GameObject.h"

class Level;

class Spawner {
public:
	Spawner(GameObject::TYPE type, Level* level, Vec2 pos, Uint16 minTime, Uint16 maxTime, bool random = false);
	~Spawner();
	GameObject* spawn( Uint32 dt );
private:
	void setNextTime();
	Level*				level_;
	GameObject::TYPE	type_;
	Vec2				pos_;
	Uint16				minTime_;
	Uint16				maxTime_;
	Sint32				nextTime_;
	bool				random_;
	Uint16				screenWidth_;
	Uint16				screenHeight_;
};

