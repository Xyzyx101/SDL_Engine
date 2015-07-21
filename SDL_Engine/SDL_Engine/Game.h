#pragma once
#include "SDL.h"
#include "MathUtils.h"

class Game {
public:
	Game(void);
	~Game(void);
	int InitSDL();
	int InitTestImage();
	void Run();
	void Clean();
protected:
	void clearBackBuffer();
	void handleEvent( const SDL_Event& newEvent );
	virtual void Draw();
	virtual void onKeyDown( Uint32 key );
	virtual void onKeyUp( Uint32 key );

	Vec2	velocity_, pos_;
	bool	running_;
	Uint32	clearColor_;
	Uint16	screenWidth_;
	Uint16	screenHeight_;

private:
	SDL_Window*		window_;
	SDL_Renderer*	renderer_;
	SDL_Texture*	testImage_;
};

