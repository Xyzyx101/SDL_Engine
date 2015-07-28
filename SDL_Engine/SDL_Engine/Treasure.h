#pragma once
#include "GameObject.h"
class Treasure :
	public GameObject {
public:
	Treasure( Sprite* sprite );
	~Treasure();
	void update( Uint32 dt );
	void draw( Vec2 cameraOffset );
	void respondLevelCollision( Vec2 collision );
	void hurt();
private:
	Sint32		disappearTimer_;
};

