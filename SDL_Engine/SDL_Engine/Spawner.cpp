#include "Spawner.h"
#include <stdlib.h>
#include "ObjectFactory.h"
#include "Level.h"

Spawner::Spawner( GameObject::TYPE type, Level* level, Vec2 pos, Uint16 minTime, Uint16 maxTime, bool random ) : type_( type ), level_(level), pos_( pos ), minTime_( minTime ), maxTime_( maxTime ), random_( random ) {
	srand( SDL_GetTicks() );
	setNextTime();
	screenWidth_ = level_->getWidth();
	screenHeight_ = level_->getHeight();
}

Spawner::~Spawner() {}

GameObject* Spawner::spawn( Uint32 dt ) {
	nextTime_ -= (Sint32)dt;
	if( nextTime_<0 ) {
		setNextTime();
		if( random_ ) {
			Uint16 x, y;
			do {
				x = rand()%screenWidth_;
				y = rand()%screenHeight_;
			} while( level_->checkCollision(Vec2(x,y), 32, 32) != Vec2::Zero);
			return ObjectFactory::Instantiate( type_, Vec2( x, y ) );
		} else {
			return ObjectFactory::Instantiate( type_, pos_ );
		}
	} else {
		return nullptr;
	}
}

void Spawner::setNextTime() {
	nextTime_ = rand()%(maxTime_-minTime_)+minTime_;
}