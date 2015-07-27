#include "GameObject.h"
#include "Sprite.h"

GameObject::GameObject(Sprite* sprite, TYPE type) : sprite_(sprite), type_(type), dead_(false) {}

GameObject::~GameObject() {}

void GameObject::setPos( Vec2 pos ) {
	pos_ = pos;
}

Vec2 GameObject::getPos() {
	return pos_;
}

Sint16 GameObject::getRight() {
	return pos_.x+sprite_->getHalfWidth();
}

Sint16 GameObject::getLeft() {
	return pos_.x-sprite_->getHalfWidth();
}

Sint16 GameObject::getTop() {
	return pos_.y-getHalfHeight();
}

Sint16 GameObject::getBottom() {
	return pos_.y+getHalfHeight();
}

Sint16 GameObject::getHalfWidth() {
	return sprite_->getHalfWidth();
}

Sint16 GameObject::getHalfHeight() {
	return sprite_->getHalfHeight();
}

GameObject::TYPE GameObject::getType() {
	return type_;
}
SDL_Texture* GameObject::getTextureOverlap( Sint16  left, Sint16  right, Sint16  top, Sint16  bottom ) {
	return sprite_->getTextureOverlap( left, right, top, bottom );
}