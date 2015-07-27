#include "Spawner.h"
#include <stdlib.h>
#include "ObjectFactory.h"

Spawner::Spawner( GameObject::TYPE type, Vec2 pos, Uint16 minTime, Uint16 maxTime ) : type_( type ), pos_( pos ), minTime_( minTime ), maxTime_( maxTime ) {
	srand( SDL_GetTicks() );
	//setNextTime();
	nextTime_ = -1;
}

Spawner::~Spawner() {}

GameObject* Spawner::spawn( Uint32 dt ) {
	nextTime_ -= (Sint32)dt;
	if( nextTime_<0 ) {
		fprintf( stdout, "spawn\n" );
		setNextTime();
		return ObjectFactory::Instantiate( type_, pos_ );
	} else {
		return nullptr;
	}
}

void Spawner::setNextTime() {
	nextTime_ = rand()%(maxTime_-minTime_)+minTime_;
}