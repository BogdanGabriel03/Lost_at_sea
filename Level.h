#pragma once
#include "Map.h"
#include "Button.h"
#include "Player.h"

class Level :public Map {
	int TileWidth, TileHeight;
	bool LvlEnded;
	int* LvlLayout;
	Button** Tiles;
	TextureManager* TileSprite;
	Player* player;
public:
	Level(SDL_Renderer* gRenderer) : Map(gRenderer) {
		LvlEnded = false; TileWidth = 0; TileHeight = 0;LvlLayout = nullptr;TileSprite = nullptr;Tiles = nullptr;player = nullptr;
	}
	~Level();
	int getTileWidth() { return  TileWidth; }
	int getTileHeight() { return  TileHeight; }
	void init(TextureManager* TileTexture, Player* newPlayer, TextureManager* newPlayerTexture,int W, int H);
	void draw();
	void update(SDL_Event* event);
	void setLvlLayout(int* NewLvlLayout) { LvlLayout = NewLvlLayout; }
	bool getLvlState() { return LvlEnded; }
	void setLvlState(bool opt) { LvlEnded = opt; }
	Player* getPlayerState() {
		return player;
	}
};