#include "Enemy.h"

Enemy::Enemy(SDL_Renderer*gRenderer,int type,TTF_Font* gFont) {
	this->type = type;
	switch (type) {
	case 1:
		maxHP = 10;
		health = 10;
		attack = 2;
		break;
	case 2:
		maxHP = 25;
		health = 25;
		attack = 4;
		break;
	case 3:
		maxHP = 40;
		health = 40;
		attack = 8;
		break;
	}
	EnemyTexture = nullptr;
	this->gRenderer = gRenderer;
	font = gFont;
}

void Enemy::init(TextureManager* ptr) {
	EnemyTexture = ptr;
}

void Enemy::draw() {
	TextureManager TextTexture[2];
	SDL_Color textColor = { 255,255,255 };
	std::stringstream monsterStats[2];

	for (int i = 0;i < 2;++i) {
		monsterStats[i].str("");
	}

	monsterStats[0] << "Enemy HP: " << health << " / " << maxHP;
	monsterStats[1] << "Enemy ATK:  " << attack;

	for (int i = 0;i < 2;++i) {
		if (!(TextTexture[i].loadFromRenderedText(monsterStats[i].str().c_str(), textColor, gRenderer, font))) {
			printf("Error! Could not initialize shit! %s\n", TTF_GetError());
		}
	}

	for (int i = 0;i < 2;++i) {
		TextTexture[i].render(gRenderer, 15, 30+45*i);
	}
	
	EnemyTexture->render(gRenderer, (640 - EnemyTexture->getWidth()) / 2, 30);
}