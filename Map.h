#pragma once
#include "Component.h"
#include "TextureManager.h"

class Map : public Component {
protected:
	TextureManager* MapTexture;
	SDL_Renderer* gRenderer;
public:
	Map(SDL_Renderer* gRenderer) : MapTexture(nullptr), gRenderer(gRenderer) {};
	~Map() {};
	void init(TextureManager* NewMapTexture);
	//void update() { std::cout << "Map Updated!\n"; };
	void draw();
};