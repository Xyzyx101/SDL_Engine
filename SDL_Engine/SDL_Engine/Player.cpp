#include "Player.h"
#include "Sprite.h"

Player::Player( Sprite* sprite ) : GameObject( sprite, GameObject::TYPE::PLAYER ), speed_( 135.f ), hp_( 5 ) {}

Player::~Player() {}

void Player::update( Uint32 dt ) {
	shootTimer_ -= (Sint32)dt;
	hurtTimer_ -= (Sint32)dt;
	vel_ = Vec2::Zero;
	if( left ) {
		vel_.x -= 1;
	}
	if( right ) {
		vel_.x += 1;
	}
	if( up ) {
		vel_.y -= 1;
	}
	if( down ) {
		vel_.y += 1;
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

void Player::draw( Vec2 cameraOffset ) {
	sprite_->setPos( pos_ );
	sprite_->draw( cameraOffset );
}

void Player::onKeyDown( Uint32 key ) {
	switch( key ) {
	case SDLK_a:
		left = true;
		break;
	case SDLK_d:
		right = true;
		break;
	case SDLK_w:
		up = true;
		break;
	case SDLK_s:
		down = true;
		break;
	}
}

void Player::onKeyUp( Uint32 key ) {
	switch( key ) {
	case SDLK_a:
		left = false;
		break;
	case SDLK_d:
		right = false;
		break;
	case SDLK_w:
		up = false;
		break;
	case SDLK_s:
		down = false;
		break;
	}
}

void Player::respondLevelCollision( Vec2 collision ) {
	pos_ += collision;
	vel_ = Vec2::Zero;
}

void Player::respondEnemyCollision() {
	if( hurtTimer_<0 ) {
		hurt();
	}
}

bool Player::canShoot() {
	return shootTimer_<0;
}

void Player::shoot() {
	shootTimer_ = 425;
}

Uint16 Player::getHealth() {
	return hp_;
}

void Player::hurt() {
	hp_--;
	hurtTimer_ = 350;
}