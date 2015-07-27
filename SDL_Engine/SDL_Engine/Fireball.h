#pragma once
#include "GameObject.h"
class Fireball :
	public GameObject {
public:
	Fireball( Sprite* sprite);
	~Fireball();
	void update( Uint32 dt );
	void draw( Vec2 cameraOffset );
	void setVel( Vec2 vel );
private:
	enum STATE {
		SHOOT,
		FLY,
		EXPLODE
	};
	void respondLevelCollision( Vec2 collision );
	void respondEnemyCollision();
	void explode();
	float			speed_;
	float			angle_;
	STATE			state_;
	Sint32			shootTimer_;
	Sint32			explodeTimer_;
};

