#include "Player.h"
#include "Sprite.h"

Player::Player( Sprite* sprite ) : GameObject( sprite ), speed_( 150.f ) {}

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
	fprintf( stdout, "x: %f  y: %f\n", pos_.x, pos_.y );
	sprite_->update( dt );
}

void Player::draw(Vec2 cameraOffset) {
	sprite_->setPos( pos_ );
	sprite_->draw(cameraOffset);
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

void Player::respondLevelCollision( Vec2 collision ) {
	pos_ += collision;
	vel_ = Vec2::Zero;
}