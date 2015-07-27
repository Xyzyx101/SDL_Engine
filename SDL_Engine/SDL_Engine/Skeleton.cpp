#include "Skeleton.h"
#include "Sprite.h"
#include "Player.h"

Skeleton::Skeleton( Sprite* sprite, Player* player ) : GameObject( sprite ), player_(player), speed_( 100.f ), thinkDelay_( 1000 ) {
	lastThink_ = SDL_GetTicks();
}

Skeleton::~Skeleton() {
	delete sprite_;
}

void Skeleton::update( Uint32 dt ) {
	Uint32 thisTick = SDL_GetTicks();
	if( thisTick-lastThink_>thinkDelay_ ) {
		lastThink_ = thisTick;
		think();
	}
	vel_ = VectorMath::Normalize( &vel_ );
	if( vel_.x==0&&vel_.y==0 ) {
		sprite_->changeAnim( "stand" );
	} else if( abs( vel_.y )>abs( vel_.x ) ) {
		if( vel_.y>0 ) {
			sprite_->changeAnim( "down" );
		} else {
			sprite_->changeAnim( "up" );
		}
	} else {
		if( vel_.x>0 ) {
			sprite_->changeAnim( "right" );
		} else {
			sprite_->changeAnim( "left" );
		}
	}
	pos_ += vel_ * speed_ * (float)dt * 0.001f;
	sprite_->update( dt );
}

void Skeleton::think( ) {
	vel_ = player_->getPos()-pos_;
	vel_ = VectorMath::Normalize( &vel_ );
	fprintf( stdout, "think : x: %f y: %f", vel_.x, vel_.y );
}

void Skeleton::draw( Vec2 cameraOffset ) {
	sprite_->setPos( pos_ );
	sprite_->draw( cameraOffset );
}

void Skeleton::respondLevelCollision( Vec2 collision ) {
	pos_ += collision;
	vel_ = vel_ * -1.f;
}

void Skeleton::setPlayer( Player* player ) {
	player_ = player;
}