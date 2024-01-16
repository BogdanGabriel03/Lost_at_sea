#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include "Component.h"
#include "TextureManager.h"

class Player :public Component
{
	int action;
	int health, attack, experience, gold, maxHP;
	int LVLUPEXP;
	int pozX, pozY;
	TextureManager* PlayerSprite;
	SDL_Renderer* gRenderer;
	int PlayerLevel;
	TTF_Font* font;
public:
	Player(SDL_Renderer* gRenderer, TTF_Font*);
	~Player();
	void setPosition(int, int);
	void init(TextureManager* ptr);
	void draw();
	void setAction(int ACT) { action = ACT; }
	//int getHealth() { return health; }
	void update();
	void print() {
		std::cout << health << " " << attack << " " << experience << " " << gold << " " << maxHP << "\n";
		std::cout << PlayerSprite << " " << gRenderer << " " << font << "\n";
	}
	int getAttack() { return attack; }
};