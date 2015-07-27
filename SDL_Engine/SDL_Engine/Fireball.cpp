#include "Fireball.h"
#include "Sprite.h"

#define PI 3.14159265

Fireball::Fireball( Sprite* sprite) : GameObject(sprite, GameObject::TYPE::TREASURE), speed_(250.f), state_(STATE::SHOOT) {
	shootTimer_ = 300;
	sprite_->changeAnim( "shoot" );
}

Fireball::~Fireball() {}

void Fireball::update( Uint32 dt ) {
	switch( state_ ) {
	case STATE::SHOOT:
		shootTimer_ -= (Sint32)dt;
		if( shootTimer_<0 ) {
			sprite_->changeAnim( "fly" );
			state_ = STATE::FLY;
		}
		break;
	case STATE::FLY:
		pos_ += vel_ * speed_ * (float)dt * 0.001f;
		break;
	case STATE::EXPLODE:
		explodeTimer_ -= (Sint32)dt;
		if( explodeTimer_<0 ) {
			dead_ = true;
		}
		break;
	}
	sprite_->update( dt );
}

void Fireball::draw( Vec2 cameraOffset ) {
	sprite_->setPos( pos_ );
	sprite_->drawAngle( cameraOffset, angle_ );
}

void Fireball::setVel( Vec2 vel ) {
	vel_ = vel;
	vel_ = VectorMath::Normalize( &vel_ );
	angle_ = atan2( vel_.y, vel_.x )*180/PI;
}
	
void Fireball::respondLevelCollision( Vec2 collision ) {
	explode();
}

void Fireball::explode() {
	state_ = STATE::EXPLODE;
	vel_ = Vec2::Zero;
	explodeTimer_ = 825;
	sprite_->changeAnim( "explode" );
}