#include "Level.h"

void Level::init(TextureManager* Tiletexture, Player* newPlayer, Enemy* newEnemy,TextureManager* newPlayerTexture, TextureManager* newEnemyTexture,int W, int H) {
	TileWidth = W;
	TileHeight = H;
	TileSprite = Tiletexture;
	player = newPlayer;
	player->setPosition(23, 3);
	player->setAction(0);
	player->init(newPlayerTexture);

	monster = newEnemy;
	monster->init(newEnemyTexture);

	Tiles = new Button * [(300 / TileHeight) * (600 / TileWidth)];
	for (int i = 0;i < (300 / TileHeight) * (600 / TileWidth);++i) {
		Tiles[i] = new Button(2);
	}
	for (int i = 0;i < 300 / TileHeight;++i) {
		for (int j = 0;j < 600 / TileWidth;++j) {
			Tiles[i * (600 / TileWidth) + j]->setPosition(j * TileWidth + 20 + (j + 1) * 3, i * TileHeight + (i + 1) * 3);
		}
	}

}

void Level::draw() {
	if (LvlAction == 2) {
		player->draw();
		monster->draw();
	}
	else {
		SDL_Rect* clip = new SDL_Rect();
		clip->w = TileWidth;
		clip->h = TileHeight;

		for (int i = 0;i < 300 / TileHeight;++i) {
			for (int j = 0;j < 600 / TileWidth;++j) {
				Tiles[i * (600 / TileWidth) + j]->render(TileSprite, gRenderer, clip);// j * TileWidth + 20 + (j + 1) * 3, i * TileHeight + (i + 1) * 3, clip);
			}
		}
		player->draw();
		for (int i = 0;i < 300 / TileHeight;++i) {
			for (int j = 0;j < 600 / TileWidth;++j) {
				Tiles[i * (600 / TileWidth) + j]->setButtonAction(0);
			}
		}
		delete clip;
	}
}

Level::~Level() {

	for (int i = 0;i < 300 / TileHeight;++i) {
		if (Tiles[i] != nullptr) {
			delete[]Tiles[i];
		}
	}
	if (Tiles != nullptr) {
		delete[]Tiles;
	}
}

void Level::update(SDL_Event* event) {
	if (LvlAction == 0) {
		int opt = 0;
		for (int i = 0;i < 300 / TileHeight;++i) {
			for (int j = 0;j < 600 / TileWidth;++j) {
				Tiles[i * (600 / TileWidth) + j]->handleEvent(event, TileWidth, TileHeight);
				opt = opt | Tiles[i * (600 / TileWidth) + j]->getButtonAction();
				if (opt != 0) {
					player->setPosition(j * TileWidth + 20 + (j + 1) * 3, i * TileHeight + (i + 1) * 3);
					player->setAction(LvlLayout[i * (600 / TileWidth) + j]);
					player->update();
					if (LvlLayout[i * (600 / TileWidth) + j] == 1) { LvlAction = 2; /*found a monster*/ }
					else if (LvlLayout[i * (600 / TileWidth) + j] == 5) { LvlAction = 1; /*level finished*/ }
					LvlLayout[i * (600 / TileWidth) + j] = 0;
				}
				opt = 0;
			}
		}
	}
	else if (LvlAction == 2) {
		monster->update(player->getAttack());
		if (monster->getMonsterHealth() <= 0) {
			LvlAction = 0;
			player->setAction(0);
			monster->setMonsterHealth();
		}
		else{
			player->setHealth(player->getHealth() - monster->getMonsterAttack());
		}
	}
}