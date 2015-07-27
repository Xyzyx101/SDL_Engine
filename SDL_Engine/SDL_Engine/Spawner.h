#pragma once
#include "SDL.h"
#include "MathUtils.h"
#include "GameObject.h"

class Spawner {
public:
	Spawner(GameObject::TYPE type, Vec2 pos, Uint16 minTime, Uint16 maxTime, bool random = false, Uint16 screenWidth = 0, Uint16 screenHeight = 0);
	~Spawner();
	GameObject* spawn( Uint32 dt );
private:
	void setNextTime();
	GameObject::TYPE	type_;
	Vec2				pos_;
	Uint16				minTime_;
	Uint16				maxTime_;
	Sint32				nextTime_;
	bool				random_;
	Uint16				screenWidth_;
	Uint16				screenHeight_;
};

