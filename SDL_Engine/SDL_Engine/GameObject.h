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
	virtual void draw( ) = 0;
	virtual void setPos(Vec2 pos) = 0;
protected:
	Vec2	vel_;
	Vec2	pos_;
};
