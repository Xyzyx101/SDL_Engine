#pragma once
#include "SDL.h"
#include "MathUtils.h"

class Sprite;

class GameObject {
public:
	enum TYPE {
		PLAYER,
		SKELETON
	};
	GameObject( Sprite* sprite );
	virtual ~GameObject();
	void setPos( Vec2 pos );
	Vec2 getPos();
	Sint16 getRight();
	Sint16 getLeft();
	Sint16 getTop();
	Sint16 getBottom();
	Sint16 getHalfWidth();
	Sint16 getHalfHeight();
	SDL_Texture* getTextureOverlap( Sint16  left, Sint16  right, Sint16  top, Sint16  bottom );
	virtual void update( Uint32 dt ) = 0;
	virtual void draw( Vec2 cameraOffset ) = 0;
	virtual void respondLevelCollision( Vec2 collision ) = 0;
protected:
	Vec2		vel_;
	Vec2		pos_;
	Sprite*		sprite_;
};

