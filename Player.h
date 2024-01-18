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
	Player(const Player& newPlayer) {
		action = newPlayer.action;
		health = newPlayer.health;
		attack = newPlayer.attack;
		experience = newPlayer.experience;
		gold = newPlayer.gold;
		maxHP = newPlayer.maxHP;
		LVLUPEXP = newPlayer.LVLUPEXP;
		pozX = newPlayer.pozX;
		pozY = newPlayer.pozY;
		PlayerSprite->free();
		PlayerSprite = newPlayer.PlayerSprite;
		gRenderer = newPlayer.gRenderer;
		PlayerLevel = newPlayer.PlayerLevel;
		font = newPlayer.font;
	}
	Player& operator= (const Player& newPlayer) {
		action = newPlayer.action;
		health = newPlayer.health;
		attack = newPlayer.attack;
		experience = newPlayer.experience;
		gold = newPlayer.gold;
		maxHP = newPlayer.maxHP;
		LVLUPEXP = newPlayer.LVLUPEXP;
		pozX = newPlayer.pozX;
		pozY = newPlayer.pozY;
		PlayerSprite->free();
		PlayerSprite = newPlayer.PlayerSprite;
		gRenderer = newPlayer.gRenderer;
		PlayerLevel = newPlayer.PlayerLevel;
		font = newPlayer.font;
		return *this;
	}
	~Player();
	void setPosition(int, int);
	void init(TextureManager* ptr);
	void draw();
	void setAction(int ACT) { action = ACT; }
	void update();
	void print() {
		std::cout << health << " " << attack << " " << experience << " " << gold << " " << maxHP << "\n";
		std::cout << PlayerSprite << " " << gRenderer << " " << font << "\n";
	}
	int getAttack() { return attack; }
	int getHealth() { return health; }
	int getLevel() { return PlayerLevel; }
	int getExperience() { return experience; }
	int getGold() { return gold; }
	void setHealth(int HP) { health = HP; }
	void setAttack(int ATK) { attack = ATK; }
	void setLevel(int lvl) { PlayerLevel = lvl; }
	void setExperience(int XP) { experience = XP; }
	void setGold(int g) { gold = g; }
};