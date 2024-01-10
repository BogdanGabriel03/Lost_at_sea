#include "Enemy.h"

Enemy::Enemy(SDL_Renderer*gRenderer,int type,TextureManager* mnstr) {
	this->type = type;
	switch (type) {
	case 1:
		health = 10;
		attack = 2;
		break;
	case 2:
		health = 25;
		attack = 4;
		break;
	case 3:
		health = 40;
		attack = 8;
		break;
	}
	EnemyTexture = mnstr;
	this->gRenderer = gRenderer;
}

void Enemy::draw() {
	EnemyTexture->render(gRenderer, (640 - EnemyTexture->getWidth()) / 2, 30);
}