#pragma once

#include <stdio.h>
#include <map>
#include <string>
#include <vector>
#include "MathUtils.h"
#include "SDL.h"

class Sprite {
public:
	Sprite();
	Sprite( SDL_Renderer* renderer, SDL_Texture* image, bool animated );
	Sprite( SDL_Renderer* renderer, SDL_Texture* image, bool animated, Vec2 pos );
	Sprite( SDL_Renderer* renderer, SDL_Texture* image, bool animated, Vec2 pos, Uint8 alpha );
	Sprite( SDL_Renderer* renderer, std::string fileName, bool animated );
	Sprite( SDL_Renderer* renderer, std::string fileName, bool animated, Vec2 pos );
	Sprite( SDL_Renderer* renderer, std::string fileName, bool animated, Vec2 pos, Uint8 alpha );
	~Sprite();

	class Anim {
	public:
		struct Cell {
			Uint16	x_;
			Uint16	y_;
			Uint16	w_;
			Uint16	h_;
			Uint16	offsetX_;
			Uint16	offsetY_;
			Uint16	halfW_;
			Uint16	halfH_;
			Cell();
			Cell( Uint16 x, Uint16 y, Uint16 w, Uint16 h, Uint16 offsetX, Uint16 offsetY );
		};
		Anim();
		Anim( Uint16 numCells, std::vector<Uint16> sequence, Uint32 frameTime, bool loop );
		const Cell& getCurrentCell();
		void addCell( Uint16 cellIdx, Cell cell );
		void update( Uint32 dt );
		void reset();
	private:
		Uint32					frameTime_;
		Uint32					currentTime_;
		Uint16					currentFrame_;
		bool					loop_;
		std::vector<Cell>		cells_;
		std::vector<Uint16>		sequence_;
	};

	struct Frame {
		Sint16							w_;
		Sint16							h_;
		Sint16							halfW_;
		Sint16							halfH_;
	};

	union DrawData {
		Frame	frame_;
		Anim*	currentAnim_;
	};

	inline Vec2 getPos() {
		return pos_;
	}

	inline void setPos( const Vec2 pos ) {
		pos_ = pos;
	}

	Uint8 getAlpha();
	void setAlpha( const Uint8 alpha );
	void draw(Vec2 cameraOffset);
	void update( Uint32 dt );
	void addAnim( std::string animName, Anim anim );
	void changeAnim( std::string newAnim );
	Sint16 getHalfWidth();
	Sint16 getHalfHeight();
private:
	int loadImage( char* filename );
	SDL_Renderer*					pRenderer_;
	SDL_Texture*					pImage_;
	Vec2							pos_;
	bool							animated_;
	std::map<std::string, Anim>		anims_;
	DrawData						drawData_;
};

