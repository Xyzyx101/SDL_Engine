#pragma once
#include "GameObject.h"
#include "MathUtils.h"
#include <map>

class ObjectFactory {
public:
	static void Init( SDL_Renderer* renderer );
	static GameObject* Instantiate( GameObject::TYPE type, Vec2 pos );
	~ObjectFactory();

private:
	struct CellData {
		Uint16	x;
		Uint16	y;
		Uint16	w;
		Uint16	h;
		Uint16	offsetX;
		Uint16	offsetY;
	};
	ObjectFactory();
	ObjectFactory( ObjectFactory const& ) = delete;
	void operator=(ObjectFactory const&) = delete;

	void loadData( GameObject::TYPE type );
	void loadFile( GameObject::TYPE type, std::string filename );
	GameObject* createObject( GameObject::TYPE type );
	bool expect( std::string tokenA, std::string tokenB );
	bool ObjectFactory::isNumber( const std::string& s );

	static ObjectFactory*									instance_;
	SDL_Renderer*											renderer_;
	std::map<GameObject::TYPE, std::vector<std::string>>	dataCache_;
};

