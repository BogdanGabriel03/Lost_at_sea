#include "Button.h"

extern SDL_Rect SpriteClips[BUTTON_SPRITE_TOTAL];

Button::Button(int btnId)
{
	mPosition.x = 0;
	mPosition.y = 0;
	pressedButtonAction = 0;
	ButtonId = btnId;
	switch (ButtonId) {
	case 0:
		mCurrentSprite = BUTTON_SPRITE_UNTOUCHED;
		break;
	case 1:
		mCurrentSprite = BUTTON_SPRITE_UNTOUCHED_BIG;
		break;
	case 2:
		mCurrentSprite = BUTTON_SPRITE_UNTOUCHED_TILE;
		break;
	case 3:
		mCurrentSprite = BUTTON_SPRITE_UNTOUCHED;
		break;
	}
}

void Button::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

void Button::handleEvent(SDL_Event* e, int BUTTON_WIDTH, int BUTTON_HEIGHT)
{
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if (x < mPosition.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > mPosition.x + BUTTON_WIDTH )
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < mPosition.y)
		{
			inside = false;
		}
		//Mouse below the button
		else if (y > mPosition.y + BUTTON_HEIGHT )
		{
			inside = false;
		}

		//Mouse is outside button
		if (!inside)
		{
			switch (ButtonId) {
			case 0:
				mCurrentSprite = BUTTON_SPRITE_UNTOUCHED;
				break;
			case 1:
				mCurrentSprite = BUTTON_SPRITE_UNTOUCHED_BIG;
				break;
			case 2:
				mCurrentSprite = BUTTON_SPRITE_UNTOUCHED_TILE;
				break;
			case 3:
				mCurrentSprite = BUTTON_SPRITE_UNTOUCHED;
				break;
			}
		}
		//Mouse is inside button
		else
		{
			switch (e->type) 
			{
				case SDL_MOUSEBUTTONDOWN:
					switch (getButtonId()) {
					case 0:
						if (y >= 300 && y < 345) {
							pressedButtonAction = 1;
						}
						else if (y >= 345 && y < 390) {
							pressedButtonAction = 2;
						}
						else if (y >= 390 && y < 435) {
							pressedButtonAction = 3;
						}
						else if (y >= 435 && y <= 480) {
							pressedButtonAction = 0;
						}
						break;
					case 1:
						if (y < 155 && x < 320) {
							pressedButtonAction = 5; //open shop menu
						}
						else if (y < 155 && x >= 320) {
							pressedButtonAction = 7; //level 1
						}
						else if (y >= 220 && x < 320) {
							pressedButtonAction = 9;//Level 2
						}
						else if ( y>=220 && x >=320 && y<375) {
							pressedButtonAction = 11;//Level 3
						}
						else {
							pressedButtonAction = 1;
						}
						break;
					case 2:
						pressedButtonAction = 1;//Tile that is being pressed - indicates the game to do the action of the tile
						break;
					case 3:
						if (y < 265) {
							pressedButtonAction = 1;
						}
						else if (x < 320) {
							pressedButtonAction = 1;
						}
						else {
							pressedButtonAction = 1;
						}
						break;
					}
				break;
				
				case SDL_MOUSEMOTION:
					switch (getButtonId()) {
					case 0:
						mCurrentSprite = BUTTON_SPRITE_ACTIVE;
						break;
					case 1:
						mCurrentSprite = BUTTON_SPRITE_ACTIVE_BIG;
						break;
					case 2:
						mCurrentSprite = BUTTON_SPRITE_ACTIVE_TILE;
						break;
					case 3:
						mCurrentSprite = BUTTON_SPRITE_ACTIVE;
						break;
					}
				break;
				
			}
		}
	}
}

void Button::render(TextureManager* gButtonSpriteSheetTexture, SDL_Renderer* gRenderer,SDL_Rect *clip)
{
	//Show current button sprite
	if (clip != NULL) {
		SpriteClips[mCurrentSprite].w = clip->w;
		SpriteClips[mCurrentSprite].h = clip->h;
	}
	gButtonSpriteSheetTexture->render(gRenderer, mPosition.x, mPosition.y,&SpriteClips[mCurrentSprite]);
}