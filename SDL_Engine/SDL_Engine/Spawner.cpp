#include "Spawner.h"
#include <stdlib.h>
#include "ObjectFactory.h"

Spawner::Spawner( GameObject::TYPE type, Vec2 pos, Uint16 minTime, Uint16 maxTime, bool random, Uint16 screenWidth, Uint16 screenHeight ) : type_( type ), pos_( pos ), minTime_( minTime ), maxTime_( maxTime ), random_( random ), screenWidth_( screenWidth ), screenHeight_( screenHeight ) {
	srand( SDL_GetTicks() );
	setNextTime();
}

Spawner::~Spawner() {}

GameObject* Spawner::spawn( Uint32 dt ) {
	nextTime_ -= (Sint32)dt;
	if( nextTime_<0 ) {
		setNextTime();
		if( random_ ) {
			Uint16 x, y;
			x = rand()%screenWidth_;
			y = rand()%screenHeight_;
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