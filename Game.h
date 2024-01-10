#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Button.h"
#include "TextureManager.h"
#include "Component.h"
#include "Map.h"
#include "Level.h"
#include "Player.h"
#include "Enemy.h"

class Game {
	SDL_Window* gWindow;
	SDL_Renderer* gRenderer;
	bool isRunning;
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int windowWidth, int windowHeight, bool fullscreen);
	void loadMedia(int windowWidth, int windowHeight);
	void handleEvents();
	void render(int windowWidth, int windowHeight);
	void clean();
	bool running() { return isRunning; }
};