#include "ObjectFactory.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

ObjectFactory::ObjectFactory() {}
ObjectFactory::~ObjectFactory() {}

void ObjectFactory::Init( SDL_Renderer* renderer ) {
	ObjectFactory::instance_->renderer_ = renderer;
}

GameObject* ObjectFactory::Instantiate( GameObject::TYPE type, Vec2 pos ) {
	if( ObjectFactory::instance_->dataCache_.count( type )==0 ) {
		ObjectFactory::instance_->loadData( type );
	}
	GameObject* obj = ObjectFactory::instance_->createObject( type );
	obj.setPos( pos );
	return obj;
}

void ObjectFactory::loadData( GameObject::TYPE type ) {
	switch( type ) {
	case GameObject::TYPE::PLAYER:
		loadFile( type, "player.dat" );
		break;
	case GameObject::TYPE::ENEMY:
		loadFile( type, "enemy.dat" );
		break;
	default:
		fprintf( stderr, "Unknown GameObject::Type %d", type );
	}
}

void ObjectFactory::loadFile( GameObject::TYPE type, std::string filename ) {

#ifdef _WIN32
	const wchar_t pathSeparator = '\\';
#else
	const char pathSeparator =	'/';
#endif
	std::string path = "data"+pathSeparator+filename;
	std::ifstream is( "test.txt", std::ifstream::in );
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
	std::string	filename;
	bool animated;
	std::map<std::string, Sprite::Anim> anims;

	auto tokens = dataCache_[type];
	auto it = tokens.begin();
	int err = 0;
	while( it<tokens.end() ) {
		if( *it=="image" ) {
			filename = *it;
		} else if( *it=="animated" ) {
			animated = *it=="true";
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
			std::vector<Sprite::Anim::Cell> cells;
			while( it<tokens.end() ) {
				if( *it=="animName" ) {
					animName = *it;
				} else 	if( *it=="frameTime" ) {
					frameTime = (Uint16)atoi( it->c_str() );
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
					loop = *it=="true";
				} else if( *it=="cell" ) {
					if( expect( *++it, "{" ) ) {
						++it;
					} else {
						err = 2;
					}
					CellData cellData;
					while( it<tokens.end() ) {
						if( *it=="x" ) {
							cellData.x = (Uint16)atoi( it->c_str() );
						} else if( *it=="y" ) {
							cellData.y = (Uint16)atoi( it->c_str() );
						} else if( *it=="w" ) {
							cellData.w = (Uint16)atoi( it->c_str() );
						} else if( *it=="h" ) {
							cellData.h = (Uint16)atoi( it->c_str() );
						} else if( *it=="offsetX" ) {
							cellData.offsetX = (Uint16)atoi( it->c_str() );
						} else if( *it=="offsetY" ) {
							cellData.offsetY = (Uint16)atoi( it->c_str() );
						} else if( *it=="}" ) {
							cells.push_back( Sprite::Anim::Cell( cellData.x, cellData.y, cellData.w, cellData.h, cellData.offsetX, cellData.offsetY ) );
							//cell done
							break;
						} else {
							fprintf( stderr, "Unexpected token %s", *it );
							err = 3;
						}
						++it;
					}
				} else if( *it=="}" ) {
					anims[animName] = Sprite::Anim( numCells, sequence, frameTime, loop );
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
	switch( type ) {
	case GameObject::TYPE::PLAYER:
		GameObject* obj = new Player();
		break;
	case GameObject::TYPE::ENEMY:
		GameObject* obj = new Enemy();
		break;
	}
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