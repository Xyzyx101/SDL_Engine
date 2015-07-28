#pragma once
#include "GameObject.h"
#include "MathUtils.h"

class Sprite;

class Player :
	public GameObject {
public:
	Player( Sprite* sprite );
	~Player();
	void update( Uint32 dt );
	void draw( Vec2 cameraOffset );
	void onKeyDown( Uint32 key );
	void onKeyUp( Uint32 key );
	void respondLevelCollision( Vec2 collision );
	void respondEnemyCollision();
	bool canShoot();
	void shoot();
	Uint16 getHealth();
	void hurt();
private:
	float		speed_;
	Sint16		hp_;
	Sint32		shootTimer_;
	Sint32		hurtTimer_;
};