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
	void OnKeyDown( Uint16 key );
	void OnKeyUp( Uint16 key );
	Sprite* sprite_;
	float speed_;
};