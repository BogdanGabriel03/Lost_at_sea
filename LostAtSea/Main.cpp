#include "Game.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[])
{
	const int fps = 60;
	// max time between frames
	const int frameDelay = 1000 / fps;
	
	Game* game = new Game();

	game->init("Lost at sea", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);
	while (game->running()) {
		Uint32 frameStart = SDL_GetTicks();
		game->handleEvents();
		//game->update();
		game->render(SCREEN_WIDTH,SCREEN_HEIGHT);
		const int frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}
	game->clean();
	delete game;
	return 0;
}