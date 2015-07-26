#pragma once
#include "MathUtils.h"
class Collision {
public:
	Collision();
	~Collision();
	static Vec2 RectToRectCollision( const Vec2& r1Pos, float r1W, float r1H, const Vec2& r2Pos, float r2W, float r2H );
};

