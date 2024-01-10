#include "Player.h"

//TextureManager *Stats;

Player::Player(SDL_Renderer* gRenderer, TTF_Font* gFont) {
	maxHP = 100;
	action = 0;
	health = 100;
	attack = 3;
	experience = 0;
	gold = 500;
	LVLUPEXP = 100;
	PlayerLevel = 1;
	font = gFont;
	PlayerSprite = nullptr;
	this->gRenderer = gRenderer;
	pozX = 0;
	pozY = 0;
}

void Player::init(TextureManager* ptr) {
	PlayerSprite = ptr;
}

Player::~Player() {
	std::cout << "Player destructor\n";
}

void Player::setPosition(int x, int y) {
	pozX = x;
	pozY = y;
}

void Player::update() {
	switch (action) {
	case 2:
		gold += 15;
		break;
	case 3:
		health += maxHP / 2;
		if (health > maxHP) {
			health = maxHP;
		}
		break;
	case 4:
		attack++;
		break;
	}
}

void Player::draw() {
	TextureManager TextTexture[5];
	SDL_Color textColor = { 255,255,255 };
	std::stringstream sstext[5];

	sstext[4].str("");
	switch (action) {
	case 0:
		//just a plain tile - nothing to do
		sstext[4] << "...";
		break;
	case 1:
		//fight a monster
		sstext[4] << "Monster appeared!";
		break;
	case 2:
		//found a treasure
		sstext[4] << "Found a treasure! +15 gold";
		break;
	case 3:
		//heal 50% health
		sstext[4] << "Heal 50% maxHealth";
		break;
	case 4:
		//ATK increases ( ATK++ )
		sstext[4] << "ATK + 1";
		break;
	case 5:
		//you found the nest piece of map - level finished
		sstext[4] << "Map piece found!\n";
		break;
	}

	for (int i = 0;i < 4;++i) {
		sstext[i].str("");
	}
	sstext[0] << "HP: " << health << " / " << maxHP;
	sstext[1] << "ATK: " << attack;
	sstext[2] << "Lvl: " << PlayerLevel << "       EXP: " << experience << " / " << LVLUPEXP;
	sstext[3] << "Gold: " << gold;
	for (int i = 0;i < 5;++i) {
		if (!TextTexture[i].loadFromRenderedText(sstext[i].str().c_str(), textColor, gRenderer, font)) {
			printf("Error! Could not initialize shit! %s", TTF_GetError());
		}
	}
	PlayerSprite->render(gRenderer, pozX, pozY);
	for (int i = 0;i < 4;++i) {
		TextTexture[i].render(gRenderer, 15, 305 + 45 * i);
	}
	TextTexture[4].render(gRenderer, (640 - TextTexture[4].getWidth()) / 2, 440);
}