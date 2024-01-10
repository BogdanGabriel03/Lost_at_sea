#pragma once
#include <SDL.h>
#include "TextureManager.h"

const int TOTAL_BUTTONS = 4;
const int BUTTON_WIDTH = 150;
const int BUTTON_HEIGHT = 45;

enum ButtonSprite
{
	BUTTON_SPRITE_UNTOUCHED = 0,
	BUTTON_SPRITE_ACTIVE = 1,
	BUTTON_SPRITE_UNTOUCHED_BIG = 2,
	BUTTON_SPRITE_ACTIVE_BIG = 3,
	BUTTON_SPRITE_UNTOUCHED_TILE = 4,
	BUTTON_SPRITE_ACTIVE_TILE = 5,
	BUTTON_SPRITE_TOTAL = 6
};

class Button
{
public:
	//Initializes internal variables
	Button(int btnId);
	//Sets top left position
	void setPosition(int x, int y);

	//Handles mouse event
	void handleEvent(SDL_Event* e,int BUTTON_WIDTH,int BUTTON_HEIGHT);

	int getButtonAction() {
		return pressedButtonAction;
	}
	void setButtonAction(int opt) {
		pressedButtonAction = opt;
	}
	int getButtonId() {
		return ButtonId;
	}
	void setButtonId(int opt) {
		ButtonId = opt;
	}
	//Shows button sprite
	void render(TextureManager* gButtonSpriteSheetTexture, SDL_Renderer* gRenderer,SDL_Rect* clip = NULL);

private:
	//Top left position
	SDL_Point mPosition;
	int pressedButtonAction;
	//Currently used global sprite
	ButtonSprite mCurrentSprite;
	int ButtonId;
};