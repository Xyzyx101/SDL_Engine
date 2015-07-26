#pragma once
#include "GameObject.h"
#include "MathUtils.h"

class Sprite;

class Player :
	public GameObject {
public:
	Player( Sprite* sprite );
	virtual ~Player();
	virtual void update( Uint32 dt );
	virtual void draw(Vec2 cameraOffset);
	virtual void setPos( Vec2 pos );
	virtual Vec2 getPos();
	void onKeyDown( Uint32 key );
	void onKeyUp( Uint32 key );
	Sint16 getRight();
	Sint16 getLeft();
	Sint16 getTop();
	Sint16 getBottom();
	Sint16 getHalfWidth();
	Sint16 getHalfHeight();
	void respondLevelCollision( Vec2 collision );
private:
	Sprite*		sprite_;
	float		speed_;
};