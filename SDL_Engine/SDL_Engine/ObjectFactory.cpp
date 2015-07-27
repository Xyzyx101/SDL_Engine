#include "ObjectFactory.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <assert.h>
#include "Player.h"
#include "Skeleton.h"
#include "Fireball.h"
#include "Treasure.h"
#include "Sprite.h"

ObjectFactory::ObjectFactory() {}
ObjectFactory::~ObjectFactory() {}

ObjectFactory* ObjectFactory::instance_ = new ObjectFactory();

void ObjectFactory::Init( SDL_Renderer* renderer ) {
	ObjectFactory::instance_->renderer_ = renderer;
}

void ObjectFactory::setPlayer( Player* player) {
	ObjectFactory::instance_->player_ = player;
}

GameObject* ObjectFactory::Instantiate( GameObject::TYPE type, Vec2 pos ) {
	if( ObjectFactory::instance_->dataCache_.count( type )==0 ) {
		ObjectFactory::instance_->loadData( type );
	}
	GameObject* obj = ObjectFactory::instance_->createObject( type );
	obj->setPos( pos );
	return obj;
}

void ObjectFactory::loadData( GameObject::TYPE type ) {
	switch( type ) {
	case GameObject::TYPE::PLAYER:
		loadFile( type, "Player.dat" );
		break;
	case GameObject::TYPE::SKELETON:
		loadFile( type, "Skeleton.dat" );
		break;
	case GameObject::TYPE::FIREBALL:
		loadFile( type, "Fireball.dat" );
		break;
	case GameObject::TYPE::TREASURE:
		loadFile( type, "Treasure.dat" );
		break;
	default:
		fprintf( stderr, "Unknown GameObject::Type %d", type );
	}
}

void ObjectFactory::loadFile( GameObject::TYPE type, std::string filename ) {
#ifdef _WIN32
	const std::string pathSeparator = "\\";
#else
	const std::string pathSeparator = "/";
#endif
	std::string path = "data"+pathSeparator+filename;
	std::ifstream is( path, std::ifstream::in );
	if( !is.good() ) {
		fprintf( stderr, "Error opening file: %s\n", path );
		is.close();
		return;
	}
	std::vector<std::string> tokens;
	std::string token;
	int commentVal = '#';
	std::string trash;
	while( is.good() ) {
		if( is.peek()==commentVal ) {
			getline( is, trash );
			continue;
		}
		is>>token;
		tokens.push_back( token );
	}
	is.close();
	ObjectFactory::instance_->dataCache_[type] = tokens;
}

GameObject* ObjectFactory::createObject( GameObject::TYPE type ) {
	Sprite* sprite;
	std::string	imageFile;
	bool animated;
	std::map<std::string, Sprite::Anim> anims;

	auto tokens = dataCache_[type];
	auto it = tokens.begin();
	int err = 0;
	while( it<tokens.end() ) {
		if( *it=="image" ) {
			imageFile = *++it;
		} else if( *it=="animated" ) {
			animated = *++it=="true";
		} else if( *it=="anim" ) {
			if( expect( *++it, "{" ) ) {
				++it;
			} else {
				err = 1;
			}
			std::string animName;
			Uint16 numCells;
			std::vector<Uint16> sequence;
			Uint32 frameTime;
			bool loop;
			std::map<Uint16, Sprite::Anim::Cell> cells;
			while( it<tokens.end() ) {
				if( *it=="animName" ) {
					animName = *++it;
				} else 	if( *it=="numCells" ) {
					numCells = (Uint16)atoi( (++it)->c_str() );
				} else 	if( *it=="frameTime" ) {
					frameTime = (Uint16)atoi( (++it)->c_str() );
				} else if( *it=="sequence" ) {
					if( expect( *++it, "{" ) ) {
						++it;
					} else {
						err = 2;
					}
					while( it<tokens.end() ) {
						if( isNumber( *it ) ) {
							sequence.push_back( (Uint16)atoi( it->c_str() ) );
						} else if( *it=="}" ) {
							//sequence done
							break;
						} else {
							fprintf( stderr, "Unexpected token %s", *it );
							err = 3;
						}
						++it;
					}
				} else if( *it=="loop" ) {
					loop = *(++it)=="true";
				} else if( *it=="cell" ) {
					if( expect( *++it, "{" ) ) {
						++it;
					} else {
						err = 2;
					}
					CellData cellData;
					while( it<tokens.end() ) {
						if( *it=="index" ) {
							cellData.index = (Uint16)atoi( (++it)->c_str() );
						} else
							if( *it=="x" ) {
								cellData.x = (Uint16)atoi( (++it)->c_str() );
							} else if( *it=="y" ) {
								cellData.y = (Uint16)atoi( (++it)->c_str() );
							} else if( *it=="w" ) {
								cellData.w = (Uint16)atoi( (++it)->c_str() );
							} else if( *it=="h" ) {
								cellData.h = (Uint16)atoi( (++it)->c_str() );
							} else if( *it=="offsetX" ) {
								cellData.offsetX = (Uint16)atoi( (++it)->c_str() );
							} else if( *it=="offsetY" ) {
								cellData.offsetY = (Uint16)atoi( (++it)->c_str() );
							} else if( *it=="}" ) {
								cells[cellData.index] = Sprite::Anim::Cell( cellData.x, cellData.y, cellData.w, cellData.h, cellData.offsetX, cellData.offsetY );
								//cell done
								break;
							} else {
								fprintf( stderr, "Unexpected token %s", *it );
								err = 3;
							}
							++it;
					}
				} else if( *it=="}" ) {
					Sprite::Anim anim = Sprite::Anim( numCells, sequence, frameTime, loop );
					for( auto it = cells.begin(); it!=cells.end(); ++it ) {
						anim.addCell( it->first, it->second );
					}
					anims[animName] = anim;

					// anim done
					break;
				} else {
					fprintf( stderr, "Unexpected token %s", *it );
					err = 4;
				}
				++it;
			}
		}
		++it;
	}
	sprite = new Sprite( renderer_, imageFile, animated );
	for( auto it = anims.begin(); it!=anims.end(); ++it ) {
		sprite->addAnim( it->first, it->second );
	}
	GameObject* obj;
	switch( type ) {
	case GameObject::TYPE::PLAYER:
		obj = new Player( sprite );
		break;
	case GameObject::TYPE::SKELETON:
		obj = new Skeleton( sprite, player_ );
		break;
	case GameObject::TYPE::FIREBALL:
		obj = new Fireball( sprite );
		break;
	case GameObject::TYPE::TREASURE:
		obj = new Treasure( sprite );
		break;
	default:
		obj = nullptr;
		assert( 0 );
	}
	return obj;
}

bool ObjectFactory::expect( std::string tokenA, std::string tokenB ) {
	if( tokenA.compare( tokenB )==0 ) {
		return true;
	} else {
		fprintf( stderr, "Unexpected token %s  I expected %s", tokenA, tokenB );
		return false;
	}
}

bool ObjectFactory::isNumber( const std::string& s ) {
	return !s.empty()&&std::find_if( s.begin(),
		s.end(), []( char c ) { return !std::isdigit( c ); } )==s.end();
}