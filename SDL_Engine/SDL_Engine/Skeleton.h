#pragma once
#include "GameObject.h"
#include "MathUtils.h"

class Sprite;
class Player;

class Skeleton :
	public GameObject {
public:
	Skeleton( Sprite* sprite, Player* player );
	~Skeleton();
	void update( Uint32 dt );
	void draw( Vec2 cameraOffset );
	void respondLevelCollision( Vec2 collision );
	void think();
	void setPlayer( Player* player );
private:
	float			speed_;
	Player*			player_;
	Uint32			lastThink_;
	Uint32			thinkDelay_;
};