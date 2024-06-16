#include "Map.h"

void Map::init(TextureManager* NewMapTexture) {
	MapTexture = NewMapTexture;
}

void Map::draw() {
	MapTexture->render(gRenderer, 0, 0);
}