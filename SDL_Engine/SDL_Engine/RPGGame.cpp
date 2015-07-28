#include "RPGGame.h"
#include <stdio.h>
#include <string>
#include "ObjectFactory.h"
#include "LevelLoader.h"
#include "Collision.h"
#include "Fireball.h"
#include <string>

RPGGame::RPGGame() : Game(), pPlayer_( nullptr ), level_( nullptr ), pHudFont_( nullptr ), pHudTexture_( nullptr ), cameraOffset_( Vec2( 0, 0 ) ) {}

RPGGame::~RPGGame() {}

void RPGGame::loadAssets() {
	ObjectFactory::Init( renderer_ );
	GameObject* playerObject = ObjectFactory::Instantiate( GameObject::PLAYER, getScreenSize() * 0.35f );
	pPlayer_ = static_cast<Player*>(playerObject);
	ObjectFactory::setPlayer( pPlayer_ );
	
	pHudFont_ = TTF_OpenFont( "MysteryQuest-Regular.ttf", 108 );
	if( pHudFont_==NULL ) {
		fprintf( stderr, "Unable to load font" );
	}
	updateHud();
	startLevel( Level::CAVE );
	spawners_.push_back( new Spawner( GameObject::SKELETON, getScreenSize() * 0.5f, 3000, 7000,  true, level_->getWidth(), level_->getHeight() ) );
	spawners_.push_back( new Spawner( GameObject::SKELETONTOUGH, getScreenSize() * 0.5f, 6000, 12000, true, level_->getWidth(), level_->getHeight() ) );
	spawners_.push_back( new Spawner( GameObject::ZOMBIE, getScreenSize() * 0.5f, 5000, 9000, true, level_->getWidth(), level_->getHeight() ) );
	spawners_.push_back( new Spawner( GameObject::ZOMBIETOUGH, getScreenSize() * 0.5f, 6000, 12000, true, level_->getWidth(), level_->getHeight() ) );
	spawners_.push_back( new Spawner( GameObject::TREASURE, Vec2::Zero, 3000, 5000, true, level_->getWidth(), level_->getHeight() ) );
}

void RPGGame::update( Uint32 dt ) {
	pPlayer_->update( dt );
	checkPlayerBounds();
	Vec2 playerCollision = level_->checkCollision( pPlayer_->getPos(), pPlayer_->getHalfWidth(), pPlayer_->getHalfHeight() );
	if( playerCollision!=Vec2::Zero ) {
		pPlayer_->respondLevelCollision( playerCollision );
	}
	for( auto spawner:spawners_ ) {
		auto enemy = spawner->spawn( dt );
		if( enemy!=nullptr ) {
			enemies_.push_back( enemy );
		}
	}
	Vec2 enemyCollision;
	for( auto enemy:enemies_ ) {
		enemy->update( dt );
		enemyCollision = level_->checkCollision( enemy->getPos(), enemy->getHalfWidth(), enemy->getHalfHeight() );
		if( enemyCollision!=Vec2::Zero ) {
			enemy->respondLevelCollision( enemyCollision );
		}
		enemyCollision = Collision::RectToRectCollision( enemy->getPos(), enemy->getHalfWidth(), enemy->getHalfHeight(), pPlayer_->getPos(), pPlayer_->getHalfWidth(), pPlayer_->getHalfHeight() );
		if( enemyCollision!=Vec2::Zero ) {
			if( enemy->getType()==GameObject::TYPE::TREASURE ) {
				gold_ += 1000;
				updateHud();
				enemy->dead_ = true;
			} else {
				pPlayer_->respondEnemyCollision();
			}
		}
	}
	Vec2 spellCollision;
	for( auto spell:spells_ ) {
		spell->update( dt );
		//spellCollision = level_->checkCollision( spell->getPos(), spell->getHalfWidth(), spell->getHalfHeight() );
		if( spellCollision!=Vec2::Zero ) {
			spell->respondLevelCollision( spellCollision );
		}
		for( auto enemy:enemies_ ) {
			enemyCollision = Collision::RectToRectCollision( spell->getPos(), spell->getHalfWidth(), spell->getHalfHeight(), enemy->getPos(), enemy->getHalfWidth(), enemy->getHalfHeight() );
			if( enemyCollision!=Vec2::Zero ) {
				if( spell->getType()==GameObject::TYPE::FIREBALL ) {
					Fireball* fireball = (Fireball*)spell;
					if( fireball->canHurt() ) {
						enemy->hurt();
					}
				}
				spell->respondLevelCollision( Vec2::Zero );
			}
		}
	}
	// Pixel Collision will never work on sdl 2.0 because you cannot read from textures reliably
	//auto obj1 = ( GameObject* )pPlayer_;
	//auto obj2 = enemies_[0];
	//auto q = Collision::PixelCollision( renderer_, obj1, obj2 );

	updateCamera();
	//fprintf( stdout, "x: %f, y: %f\n", pPlayer_->getPos().x, pPlayer_->getPos().y );
	removeDeadObjects();
}

void RPGGame::draw() {
	level_->drawLayer0( cameraOffset_ );
	pPlayer_->draw( cameraOffset_ );
	for( auto enemy:enemies_ ) {
		enemy->draw( cameraOffset_ );
	}
	for( auto spell:spells_ ) {
		spell->draw( cameraOffset_ );
	}
	level_->drawLayer1( cameraOffset_ );
	drawHUD();
	Game::draw();
}

void RPGGame::onKeyDown( Uint32 key ) {
	if( key==SDLK_ESCAPE ) {
		running_ = false;
	}
	if( key==SDLK_w
		||key==SDLK_s
		||key==SDLK_a
		||key==SDLK_d ) {
		pPlayer_->onKeyDown( key );
	}
}

void RPGGame::onKeyUp( Uint32 key ) {
	if( key==SDLK_w
		||key==SDLK_s
		||key==SDLK_a
		||key==SDLK_d ) {
		pPlayer_->onKeyUp( key );
	}
}

void RPGGame::onMouseDown( Vec2 coords ) {
	if( !pPlayer_->canShoot() ) {
		return;
	}
	pPlayer_->shoot();
	coords = coords+cameraOffset_;
	Fireball* fireball = (Fireball*)ObjectFactory::Instantiate( GameObject::FIREBALL, pPlayer_->getPos() );
	Vec2 direction = coords-pPlayer_->getPos();
	fireball->setVel( direction );
	spells_.push_back( fireball );
}

void RPGGame::startLevel( Level::LEVEL level ) {
	LevelLoader levelLoader( renderer_, screenWidth_, screenHeight_ );
	level_ = levelLoader.loadLevel( level );
}

void RPGGame::checkPlayerBounds() {
	if( pPlayer_->getLeft()<0 ) {
		Vec2 newPos = Vec2( pPlayer_->getHalfWidth(), pPlayer_->getPos().y );
		pPlayer_->setPos( newPos );
	} else if( pPlayer_->getRight()>level_->getWidth() ) {
		Vec2 newPos = Vec2( level_->getWidth()-pPlayer_->getHalfWidth(), pPlayer_->getPos().y );
		pPlayer_->setPos( newPos );
	}
	if( pPlayer_->getTop()<0 ) {
		Vec2 newPos = Vec2( pPlayer_->getPos().x, pPlayer_->getHalfHeight() );
		pPlayer_->setPos( newPos );
	} else if( pPlayer_->getBottom()>level_->getHeight() ) {
		Vec2 newPos = Vec2( pPlayer_->getPos().x, level_->getHeight()-pPlayer_->getHalfHeight() );
		pPlayer_->setPos( newPos );
	}
}

void RPGGame::updateCamera() {
	Sint16 cameraBoundx = screenWidth_/4;
	Sint16 cameraBoundY = screenHeight_/4;
	Vec2 playerPos = pPlayer_->getPos();
	if( playerPos.x-cameraOffset_.x<cameraBoundx ) {
		cameraOffset_.x = playerPos.x-cameraBoundx;
	} else if( playerPos.x > cameraOffset_.x+screenWidth_-cameraBoundx ) {
		cameraOffset_.x = playerPos.x-screenWidth_+cameraBoundx;
	}
	if( playerPos.y-cameraOffset_.y<cameraBoundY ) {
		cameraOffset_.y = playerPos.y-cameraBoundY;
	} else if( playerPos.y > cameraOffset_.y+screenHeight_-cameraBoundY ) {
		cameraOffset_.y = playerPos.y-screenHeight_+cameraBoundY;
	}
	if( cameraOffset_.x<0 ) {
		cameraOffset_.x = 0;
	} else if( cameraOffset_.x+screenWidth_>level_->getWidth() ) {
		cameraOffset_.x = level_->getWidth()-screenWidth_;
	}
	if( cameraOffset_.y<0 ) {
		cameraOffset_.y = 0;
	} else if( cameraOffset_.y+screenHeight_>level_->getHeight() ) {
		cameraOffset_.y = level_->getHeight()-screenHeight_;
	}
}

void RPGGame::removeDeadObjects() {
	if( pPlayer_->dead_ ) {
		running_ = false;
	}
	for( auto it = enemies_.begin(); it!=enemies_.end(); ) {
		if( (*it)->dead_ ) {
			delete * it;
			it = enemies_.erase( it );
		} else {
			++it;
		}
	}
	for( auto it = spells_.begin(); it!=spells_.end(); ) {
		if( (*it)->dead_ ) {
			delete * it;
			it = spells_.erase( it );
		} else {
			++it;
		}
	}
}

void RPGGame::updateHud() {
	std::string hud = "Gold : "+std::to_string( gold_ );
	SDL_Color textColor = { 193, 6, 6 };
	SDL_Surface* textSurface = TTF_RenderText_Solid( pHudFont_, hud.c_str(), textColor );
	if( textSurface==nullptr ) {
		fprintf( stderr, "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	if( pHudTexture_!=nullptr ) {
		SDL_DestroyTexture( pHudTexture_ );
	}
	pHudTexture_ = SDL_CreateTextureFromSurface( renderer_, textSurface );
	if( pHudTexture_==nullptr ) {
		fprintf( stderr, "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
	}
	SDL_FreeSurface( textSurface );
}

void RPGGame::drawHUD() {
	SDL_Rect dest;
	dest.x = screenWidth_/5;
	dest.y = 0;
	dest.w = screenWidth_*1/5;
	dest.h = screenHeight_/20;
	SDL_RenderCopy( renderer_, pHudTexture_, NULL, &dest );
}
