#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#include "Game.h"
#include <stdio.h>
#include <windows.h>

int main(int argc, char* argv[]) {
	if (AttachConsole(ATTACH_PARENT_PROCESS) || AllocConsole()){
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
	}
	Game g = Game();
	int err = g.InitSDL();
	if (err != 0) {
		return err;
	}
	err = g.InitTestImage();
	if (err != 0) {
		return err;
	}
	g.Run();
	g.Clean();
	return 0;
}