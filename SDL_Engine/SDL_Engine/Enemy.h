#pragma once
#include "GameObject.h"
#include "MathUtils.h"

class Sprite;
class Player;

class Enemy :
	public GameObject {
public:
	Enemy( Sprite* sprite, Player* player, GameObject::TYPE type, float speed, Uint16 hp );
	~Enemy();
	void update( Uint32 dt );
	void draw( Vec2 cameraOffset );
	void respondLevelCollision( Vec2 collision );
	void think();
	void setPlayer( Player* player );
	void hurt();
private:
	float			speed_;
	Player*			player_;
	Uint32			lastThink_;
	Uint32			thinkDelay_;
	Uint16			hp_;
};