#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include "Component.h"
#include "TextureManager.h"

class Enemy : public Component {
	int type;
	/*
		1 - weak - 10HP - 2ATK
		2 - medium - 25HP - 4ATK
		3 - difficult - 40HP - 8ATK
	*/
	int health, attack,maxHP;
	TextureManager* EnemyTexture;
	SDL_Renderer* gRenderer;
	TTF_Font* font;
public:
	Enemy(SDL_Renderer*,int type, TTF_Font*);
	Enemy(const Enemy& newEnemy) {
		type = newEnemy.type;
		health = newEnemy.health;
		attack = newEnemy.attack;
		maxHP = newEnemy.maxHP;
		EnemyTexture->free();
		EnemyTexture = newEnemy.EnemyTexture;
		gRenderer = newEnemy.gRenderer;
		font = newEnemy.font;
	}
	~Enemy() {}
	Enemy& operator= (const Enemy& newEnemy) {
		type = newEnemy.type;
		health = newEnemy.health;
		attack = newEnemy.attack;
		maxHP = newEnemy.maxHP;
		EnemyTexture->free();
		EnemyTexture = newEnemy.EnemyTexture;
		gRenderer = newEnemy.gRenderer;
		font = newEnemy.font;
		return *this;
	}
	void init(TextureManager*);
	void draw();
	void update(int dmg) { health -= dmg; }
	void setMonsterHealth() { health = maxHP; }
	int getMonsterHealth() { return health; }
};