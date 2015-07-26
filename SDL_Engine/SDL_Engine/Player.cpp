#include "Player.h"
#include "Sprite.h"

Player::Player( Sprite* sprite ) : sprite_( sprite ), speed_( 450.f ) {}

Player::~Player() {
	delete sprite_;
}

void Player::update( Uint32 dt ) {
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

void Player::draw(Vec2 cameraOffset) {
	sprite_->setPos( pos_ );
	sprite_->draw(cameraOffset);
}

void Player::setPos( Vec2 pos ) {
	pos_ = pos;
}

Vec2 Player::getPos() {
	return pos_;
}

void Player::onKeyDown( Uint32 key ) {
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

void Player::onKeyUp( Uint32 key ) {
	if( key==SDLK_UP||key==SDLK_DOWN ) {
		vel_.y = 0;
	} else if( key==SDLK_LEFT||key==SDLK_RIGHT ) {
		vel_.x = 0;
	}
}

Sint16 Player::getRight() {
	return pos_.x+sprite_->getHalfWidth();
}

Sint16 Player::getLeft() {
	return pos_.x-sprite_->getHalfWidth();
}

Sint16 Player::getTop() {
	return pos_.y-getHalfHeight();
}

Sint16 Player::getBottom() {
	return pos_.y+getHalfHeight();
}

Sint16 Player::getHalfWidth() {
	return sprite_->getHalfWidth();
}

Sint16 Player::getHalfHeight() {
	return sprite_->getHalfHeight();
}