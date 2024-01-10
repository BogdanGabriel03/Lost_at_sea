#pragma once
#include "TextureManager.h"
class Component {
public:
	virtual ~Component() {};
	virtual void draw() = 0;
};