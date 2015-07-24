#pragma once
#include "GameObject.h"

class Sprite;

class Player :
	public GameObject {
public:
	Player(Sprite* sprite);
	virtual ~Player();
	virtual void update( Uint32 dt );
	virtual void draw();
	virtual void setPos( Vec2 pos );
	void onKeyDown( Uint32 key );
	void onKeyUp( Uint32 key );
	Sprite* sprite_;
	float speed_;
};