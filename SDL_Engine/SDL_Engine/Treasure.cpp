#include "Treasure.h"
#include "Sprite.h"
#include <stdlib.h>

Treasure::Treasure( Sprite* sprite ) : GameObject( sprite , GameObject::TYPE::TREASURE)  {
	disappearTimer_ = rand()%(4000)+4000;
}

Treasure::~Treasure() {}

void Treasure::update( Uint32 dt ) {
	disappearTimer_ -= (Sint32)dt;
	if( disappearTimer_<0 ) {
		dead_ = true;
	}
}

void Treasure::draw( Vec2 cameraOffset ) {
	sprite_->setPos( pos_ );
	sprite_->draw( cameraOffset );
}

void Treasure::respondLevelCollision( Vec2 collision ) {
	pos_ += collision;
}