#include "LevelLoader.h"
#include "SDL.h"
#include <iostream>
#include <fstream>
#include "lib/json/json.h"
#include "Level.h"

LevelLoader::LevelLoader() {}

LevelLoader::~LevelLoader() {}

Level* LevelLoader::loadLevel( const std::string& filename ) {
#ifdef _WIN32
	const std::string pathSeparator = "\\";
#else
	const std::string pathSeparator = "/";
#endif
	std::string path = "levels"+pathSeparator+filename;
	/*
	std::ifstream is( path, std::ifstream::in );
	if( !is.good() ) {
		fprintf( stderr, "Error opening file: %s\n", path );
		is.close();
		return;
	}
	std::string jsonString;
	is.seekg( 0, std::ios::end );
	jsonString.reserve( is.tellg() );
	is.seekg( 0, std::ios::beg );
	jsonString.assign( std::istreambuf_iterator<char>( is ), std::istreambuf_iterator<char>() );
	is.close();
	json levelData = json::parse( jsonString );
	Uint16 height = levelData["height"];
	Uint16 width = levelData["width"];
	Uint16 tileHeight = levelData["tileHeight"];
	Uint16 tileWidth = levelData["tileWidth"];
	*/
	std::ifstream levelFile( path, std::ifstream::binary );
	Json::Value levelData;
	levelFile>>levelData;
	levelFile.close();
	Level* level = new Level();
	return level;
}

SDL_Texture* LevelLoader::createLayerTexture( SDL_Texture* tileset, std::vector<Uint16> data ) {
	SDL_Texture* FIXME = nullptr;
	return FIXME;
}