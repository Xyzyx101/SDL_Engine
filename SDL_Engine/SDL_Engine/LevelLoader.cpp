#include "LevelLoader.h"
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <fstream>
#include "include/json/json.h"
#include "Level.h"
#include <assert.h>
#include "Game.h"

LevelLoader::LevelLoader( SDL_Renderer* renderer, Uint16 screenWidth, Uint16 screenHeight ) : renderer_( renderer ), screenWidth_( screenWidth ), screenHeight_( screenHeight ) {}

LevelLoader::~LevelLoader() {}

Level* LevelLoader::loadLevel( Level::LEVEL level ) {
	std::string filename, tilemapName;
#ifdef _WIN32
	const std::string pathSeparator = "\\";
#else
	const std::string pathSeparator = "/";
#endif
	switch( level ) {
	case Level::LEVEL::CAVE:
		filename = "Cave.lvl";
		tilemapName = "cave.png";
		break;
	default:
		assert( true );
	}
	filename = "levels"+pathSeparator+filename;
	tilemapName = "tiles"+pathSeparator+tilemapName;

	std::ifstream levelFile( filename, std::ifstream::binary );
	Json::Value levelData;
	levelFile>>levelData;
	levelFile.close();

	int width = levelData.get( "width", 0 ).asInt();
	int height = levelData.get( "height", 0 ).asInt();
	int tileWidth = levelData.get( "tilewidth", 0 ).asInt();
	int tileHeight = levelData.get( "tileheight", 0 ).asInt();

	Json::Value tileSet = levelData["tilesets"][0];
	SDL_Texture* tileTexture = loadTexture( tilemapName );
	int imageWidth = tileSet["imagewidth"].asInt();
	int imageHeight = tileSet["imageheight"].asInt();

	std::map<std::string, SDL_Texture*> layerTextures;
	LayerMap layer0;
	LayerMap layer1;
	Json::Value layers = levelData["layers"];
	for( size_t idx = 0; idx<layers.size(); ++idx ) {
		Json::Value layerData = layers[idx]["data"];
		std::vector<int> data;
		data.reserve( layerData.size() );
		int dataIdx = 0;
		auto it = layerData.begin();
		while( it!=layerData.end() ) {
			data.push_back( (*it++).asInt() );
		}
		std::string layerName = layers[idx]["name"].asString();
		if( layerName=="collision" ) {
			createCollisionLayer( data, width, height, tileWidth, tileHeight );
		} else if( layerName=="layer0" ) {
			createLayerMap( layer0, data, width, height, tileWidth, tileHeight );
		} else if( layerName=="layer1" ) {
			createLayerMap( layer1, data, width, height, tileWidth, tileHeight );
		} else {
			assert( false );
		}
		SDL_Texture* layerTexture = createLayerTexture( tileTexture, data, width, height, tileWidth, tileHeight, imageWidth, imageHeight );
		layerTextures[layerName] = layerTexture;
	}
	Level* newLevel = new Level( renderer_, width * tileWidth, height * tileHeight, screenWidth_, screenHeight_, tileWidth, tileHeight );
	newLevel->layer0Texture_ = layerTextures["layer0"];
	newLevel->layer1Texture_ = layerTextures["layer1"];
	newLevel->collisionLayer_ = collisionLayer_;
	newLevel->layer0_ = layer0;
	newLevel->layer1_ = layer1;
	newLevel->tileTexture_ = tileTexture;
	return newLevel;
}

SDL_Texture* LevelLoader::loadTexture( std::string path ) {
	SDL_Surface* surface = IMG_Load( path.c_str() );
	if( surface==nullptr ) {
		fprintf( stderr, "Failed to load '%s': %s\n", path, IMG_GetError() );
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer_, surface );
	SDL_FreeSurface( surface );
	if( texture==nullptr ) {
		fprintf( stderr, "Failed to create texture: %s\n", SDL_GetError() );
	}
	assert( texture!=nullptr );
	return texture;
}

SDL_Texture* LevelLoader::createLayerTexture( SDL_Texture* tileTex, std::vector<int> data, int levelWidth, int levelHeight, int tileWidth, int tileHeight, int imageWidth, int imageHeight ) {
	
	SDL_Texture* layerTexture = SDL_CreateTexture( renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, levelWidth * tileWidth, levelHeight * tileHeight );

	SDL_SetTextureBlendMode( layerTexture, SDL_BLENDMODE_BLEND );
	SDL_SetRenderTarget( renderer_, layerTexture );
	SDL_SetRenderDrawColor( renderer_, 0, 0, 0, 0 );
	SDL_RenderClear( renderer_ );
	SDL_Rect src, dest;
	src.w = tileWidth;
	src.h = tileHeight;
	dest.w = tileWidth;
	dest.h = tileHeight;
	int destIdx = 0;
	for( auto it = data.begin(); it!=data.end(); ++it ) {
		int tileIdx = *it-1; //minus 1 because tile 0 is nothing
		if( tileIdx==-1 ) { destIdx++; continue; }
		src.x = (Sint16)((tileIdx * tileWidth)%imageWidth);
		src.y = (Sint16)((tileIdx * tileWidth)/imageWidth * tileHeight);
		dest.x = (Sint16)((destIdx % levelWidth) * tileWidth);
		dest.y = (Sint16)((destIdx/levelWidth) * tileHeight);
		if( SDL_RenderCopy( renderer_, tileTex, &src, &dest )!=0 ) {
			fprintf( stderr, "createLayerTexture error : %s\n", SDL_GetError() );
		}

		destIdx++;
	}

	SDL_SetRenderTarget( renderer_, NULL ); //reset renderer
	return layerTexture;
}

void LevelLoader::createLayerMap( LayerMap& layerMap, std::vector<int> data, int levelWidth, int levelHeight, int tileWidth, int tileHeight ) {
	int destIdx = 0;
	for( auto it = data.begin(); it!=data.end(); ++it ) {
		if( *it==0 ) { ++destIdx; continue; }
		Sint32 x = destIdx%levelWidth * tileWidth;
		Sint32 y = destIdx/levelWidth * tileHeight;
		auto rowMapIter = layerMap.find( y );
		if( rowMapIter==layerMap.end() ) {
			layerMap.emplace( y, std::map<Sint16, Uint16>() );
		}
		layerMap[y][x] = *it;
		++destIdx;
	}
}

void LevelLoader::createCollisionLayer( std::vector<int> data, int levelWidth, int levelHeight, int tileWidth, int tileHeight ) {
	int destIdx = 0;
	for( auto it = data.begin(); it!=data.end(); ++it ) {
		if( *it==0 ) { ++destIdx; continue; }
		Sint32 x = destIdx%levelWidth * tileWidth;
		Sint32 y = destIdx/levelWidth * tileHeight;
		auto rowMapIter = collisionLayer_.find( y );
		if( rowMapIter==collisionLayer_.end() ) {
			collisionLayer_.emplace( y, std::set<Sint32>() );
		}
		collisionLayer_[y].insert( x );
		++destIdx;
	}
}