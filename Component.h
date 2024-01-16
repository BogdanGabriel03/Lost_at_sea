#pragma once

class Component {
public:
	virtual ~Component() {};
	virtual void draw() = 0;
};