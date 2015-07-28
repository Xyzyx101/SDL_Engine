#include "Enemy.h"
#include "Sprite.h"
#include "Player.h"

Enemy::Enemy( Sprite* sprite, Player* player, GameObject::TYPE type, float speed, Uint16 hp ) : GameObject( sprite,type ), player_(player), speed_( speed ), hp_(hp), thinkDelay_( 1000 ) {
	lastThink_ = SDL_GetTicks();
}

Enemy::~Enemy() {
}

void Enemy::update( Uint32 dt ) {
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

void Enemy::think( ) {
	vel_ = player_->getPos()-pos_;
	vel_ = VectorMath::Normalize( &vel_ );
}

void Enemy::draw( Vec2 cameraOffset ) {
	sprite_->setPos( pos_ );
	sprite_->draw( cameraOffset );
}

void Enemy::respondLevelCollision( Vec2 collision ) {
	pos_ += collision;
	vel_ = vel_ * -1.f;
}

void Enemy::setPlayer( Player* player ) {
	player_ = player;
}

void Enemy::hurt() {
	hp_--;
	if( hp_<=0 ) {
		dead_ = true;
	}
}