#include "Level.h"

void Level::init(TextureManager* Tiletexture, Player* newPlayer, TextureManager* newPlayerTexture,int W, int H) {
	TileWidth = W;
	TileHeight = H;
	TileSprite = Tiletexture;
	player = newPlayer;
	player->setPosition(23, 3);
	player->setAction(0);
	player->init(newPlayerTexture);
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
}

Level::~Level() {

	for (int i = 0;i < 300 / TileHeight;++i) {
		delete[]Tiles[i];
	}
	delete[]Tiles;
}

void Level::update(SDL_Event* event) {
	int opt = 0;
	for (int i = 0;i < 300 / TileHeight;++i) {
		for (int j = 0;j < 600 / TileWidth;++j) {
			Tiles[i * (600 / TileWidth) + j]->handleEvent(event, TileWidth, TileHeight);
			opt = opt | Tiles[i * (600 / TileWidth) + j]->getButtonAction();
			if (opt != 0) {
				player->setPosition(j * TileWidth + 20 + (j + 1) * 3, i * TileHeight + (i + 1) * 3);
				player->setAction(LvlLayout[i * (600 / TileWidth) + j]);
				player->update();
				if (LvlLayout[i * (600 / TileWidth) + j] == 5) { LvlEnded = true; }
				LvlLayout[i * (600 / TileWidth) + j] = 0;
			}
			opt = 0;
		}
	}
}