#pragma once
#include "MathUtils.h"
#include "Sprite.h"

class GameObject {
public:
	enum TYPE {
		PLAYER,
		ENEMY
	};
	GameObject();
	virtual ~GameObject();
	virtual void update( Uint32 dt ) = 0;
	virtual void draw( Vec2 cameraOffset ) = 0;
	virtual void setPos( Vec2 pos ) = 0;
	virtual Vec2 getPos() = 0;
protected:
	Vec2	vel_;
	Vec2	pos_;
};

