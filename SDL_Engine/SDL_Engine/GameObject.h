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
	virtual void update( Sint32 dt ) = 0;
	virtual void draw( Sint32 dt ) = 0;
	virtual void setPos(Vec2 pos) = 0;
protected:
	void LoadGameObject();
	void processDataFile( std::string filename );
	Vec2	vel_;
	Vec2	pos_;
};

