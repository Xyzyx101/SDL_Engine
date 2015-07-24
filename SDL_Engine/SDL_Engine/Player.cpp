#include "Player.h"
#include "Sprite.h"

Player::Player( Sprite* sprite ) : sprite_( sprite ), speed_(150) {}

Player::~Player() {
	delete sprite_;
}

void Player::update( Sint32 dt ) {
	vel_ = VectorMath::Normalize( &vel_ );
	if( vel_.x==0&&vel_.y==0 ) {

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
	pos_ += vel_ * speed_ * dt;
	sprite_->update( dt );
}

void Player::draw( Sint32 dt ) {

}

void Player::setPos( Vec2 pos ) {

}

void Player::OnKeyDown( Uint16 key ) {
	switch( key ) {
	case SDLK_LEFT:
		vel_.x -= 1;
		break;
	case SDLK_RIGHT:
		vel_.x += 1;
		break;
	case SDLK_UP:
		vel_.y -= 1;
		break;
	case SDLK_DOWN:
		vel_.y += 1;
		break;
	}
}

void Player::OnKeyUp( Uint16 key ) {
	if( key==SDLK_UP||key==SDLK_DOWN ) {
		vel_.y = 0;
	} else if( key==SDLK_LEFT||key==SDLK_RIGHT ) {
		vel_.x = 0;
	}
}