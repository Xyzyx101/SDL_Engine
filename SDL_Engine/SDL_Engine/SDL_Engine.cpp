#define _CRT_SECURE_NO_WARNINGS
#ifdef _DEBUG
#undef NDDEBUG
#else
#define NDEBUG
#endif


#include "SDL.h"
#include "RPGGame.h"
#include <stdio.h>
#include <windows.h>

int main(int argc, char* argv[]) {
#ifdef _DEBUG
	// redirects sdterr and stdout to another window
	if (AttachConsole(ATTACH_PARENT_PROCESS) || AllocConsole()){
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
	}
#endif
	RPGGame g = RPGGame();
	int err = g.InitSDL();
	if (err != 0) {
		return err;
	}
	g.Run();
	g.Clean();
	return 0;
}