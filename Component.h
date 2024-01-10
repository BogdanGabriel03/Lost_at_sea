#pragma once
#include "TextureManager.h"
class Component {
public:
	virtual ~Component() {};
	virtual void init(TextureManager* ptr) = 0;
	//virtual void update() = 0;
	virtual void draw() = 0;
};