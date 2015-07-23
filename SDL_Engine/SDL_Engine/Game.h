#pragma once
#include "SDL.h"
#include "MathUtils.h"

class Game {
public:
	Game( void );
	~Game( void );
	int InitSDL();
	void Run();
	void Clean();
protected:
	void clearBackBuffer();
	void handleEvent( const SDL_Event& newEvent );
	virtual void draw();
	virtual void onKeyDown( Uint32 key );
	virtual void onKeyUp( Uint32 key );
	virtual void loadAssets() = 0;
	virtual void update( Uint32 dt ) = 0;
	Vec2 getScreenSize();

	SDL_Window*		window_;
	SDL_Renderer*	renderer_;
	bool			running_;
	Uint32			clearColor_;
	Uint16			screenWidth_;
	Uint16			screenHeight_;
};

