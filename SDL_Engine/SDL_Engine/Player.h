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
private:
	float		speed_;
	Sint32		shootTimer_;
};