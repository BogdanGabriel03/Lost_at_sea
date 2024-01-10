#pragma once
#include "Component.h"
#include "TextureManager.h"

class Enemy : public Component {
	int type;
	/*
		1 - weak - 10HP - 2ATK
		2 - medium - 25HP - 4ATK
		3 - difficult - 40HP - 8ATK
	*/
	int health, attack;
	TextureManager* EnemyTexture;
	SDL_Renderer* gRenderer;
public:
	Enemy(SDL_Renderer*,int type,TextureManager*);
	~Enemy() {}
	void draw();
};