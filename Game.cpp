#include "Game.h"

Button* MainMenuButtons[TOTAL_BUTTONS - 1];
Button* SettingsBackButton = nullptr;
Button* LevelSelectionButton[4];
Button* YesButton = nullptr, * NoButton = nullptr;
TextureManager* MainMenuTexture = nullptr;
TextureManager* gButtonSpriteSheetTexture = nullptr;
TextureManager* gButtonTextSpriteSheetTexture = nullptr;
TextureManager* LevelIconsSheetTexture = nullptr;
TextureManager* SettingsMenuTexture = nullptr, * LevelSelectionTexture = nullptr;
TextureManager* MapBaseTexture = nullptr;
TextureManager* TileTexture = nullptr;
TextureManager* PlayerSprite[3];
TextureManager* EndLvlTexture = nullptr;
TextureManager* MonsterTexture;
SDL_Rect LevelIconsSprites[4];
SDL_Rect gTextClips[TOTAL_BUTTONS + 2];
SDL_Rect SpriteClips[BUTTON_SPRITE_TOTAL];
Component *firstMonster;

/*lvl layout represents what every tile does when stepped on:
	0 - nothing ( empty tile )
	1 - enemy
	2 - gold reward / raid
	3 - heal 20 HP
	4 - ATK buff ( ATK++ )
	5 - Found next piece of map / end of level
*/

int Lvl1Layout[15] = { 0, 0, 1, 0, 1, 0, 1, 1, 3, 5, 1, 4, 0, 1, 2 };
int Lvl2Layout[25] = { 0,0,0,0,0,1,0,1,0,0,0,1,3,1,0,2,1,0,1,1,0,5,2,1,4 };
int Lvl3Layout[36] = { 0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
int LvlAccesible[4] = { 15,15,0,0 };
int contor=0;

Map* map;
Level* Levels[3];
Player* NewPlayer;
TTF_Font* Font;			//globally used font
int opt;

Game::Game() : isRunning(false), gWindow(nullptr), gRenderer(nullptr) {}

Game::~Game() {
	//future purposes
}

void Game::init(const char* title, int xpos, int ypos, int windowWidth, int windowHeight, bool fullscreen) {
	isRunning = true;
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		isRunning = false;
	}
	else
	{
		std::cout << "Subsystems initialized" << std::endl;

		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		auto window = SDL_CreateWindow(title, xpos, ypos, windowWidth, windowHeight, flags);
		if (window == nullptr)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			isRunning = false;
		}
		else {
			std::cout << "Window created" << std::endl;
			gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == nullptr)
			{
				printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
				isRunning = false;
			}
			else {
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
				std::cout << "Renderer created" << std::endl;

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					isRunning = false;
				}

				//Initialize SDL_ttf
				if (TTF_Init() < 0)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					isRunning = false;
				}
			}
		}
	}

	for (int i = 0;i < TOTAL_BUTTONS - 1;++i) {
		MainMenuButtons[i] = new Button(0);
	}
	SettingsBackButton = new Button(0);
	for (int i = 0;i < 4;++i) {
		LevelSelectionButton[i] = new Button(1);
	}
	YesButton = new Button(3);
	NoButton = new Button(3);

	MainMenuTexture = new TextureManager();
	gButtonSpriteSheetTexture = new TextureManager();
	gButtonTextSpriteSheetTexture = new TextureManager();
	SettingsMenuTexture = new TextureManager();
	LevelSelectionTexture = new TextureManager();
	LevelIconsSheetTexture = new TextureManager();
	MapBaseTexture = new TextureManager();
	TileTexture = new TextureManager();
	EndLvlTexture = new TextureManager();
	MonsterTexture = new TextureManager();
	firstMonster = new Enemy(gRenderer, 1,MonsterTexture);

	map = new Map(gRenderer);
	map->init(MapBaseTexture);

	for (int i = 0;i < 3;++i) {
		PlayerSprite[i] = new TextureManager();
	}
	
	loadMedia(windowWidth, windowHeight);
	NewPlayer = new Player(gRenderer, Font);
}

void Game::loadMedia(int windowWidth, int windowHeight) {
	if (!gButtonSpriteSheetTexture->loadFromFile("./Assets/HoverButtons.png", gRenderer))
	{
		printf("Failed to load button sprite texture!\n");
		isRunning = false;
	}
	else
	{
		//Set sprites
		for (int i = 0; i < 4; ++i)
		{
			SpriteClips[i].x = i % 2 * 320;
			SpriteClips[i].y = 0;
			SpriteClips[i].w = i < 2 ? BUTTON_WIDTH : 320;
			SpriteClips[i].h = i < 2 ? BUTTON_HEIGHT : 155;
		}
		for (int i = 4;i < 6;++i) {
			SpriteClips[i].x = i / 5 * 120;
			SpriteClips[i].y = 155;
			SpriteClips[i].w = 120;
			SpriteClips[i].h = 100;
		}

		//Set buttons in position
		MainMenuButtons[0]->setPosition(240, 300);
		MainMenuButtons[1]->setPosition(240, 345);
		MainMenuButtons[2]->setPosition(240, 390);
		SettingsBackButton->setPosition(windowWidth - BUTTON_WIDTH, windowHeight - BUTTON_HEIGHT);
		LevelSelectionButton[0]->setPosition(0, 0);
		LevelSelectionButton[1]->setPosition(320, 0);
		LevelSelectionButton[2]->setPosition(0, 220);
		LevelSelectionButton[3]->setPosition(320, 220);
		YesButton->setPosition(140, 320);
		NoButton->setPosition(350, 320);
	}

	if (!gButtonTextSpriteSheetTexture->loadFromFile("./Assets/TextButtons.png", gRenderer)) {
		printf("Failed to load buttons' texts texture!\n");
		isRunning = false;
	}
	else {
		for (int i = 0; i < TOTAL_BUTTONS + 2; ++i)
		{
			gTextClips[i].x = 0;
			gTextClips[i].y = i * 45;
			gTextClips[i].w = BUTTON_WIDTH;
			gTextClips[i].h = BUTTON_HEIGHT;
		}
	}

	if (!MainMenuTexture->loadFromFile("./Assets/MainMenu.png", gRenderer))
	{
		printf("Failed to load main menu texture!\n");
		isRunning = false;
	}
	if (!SettingsMenuTexture->loadFromFile("./Assets/SettingsMenu.png", gRenderer))
	{
		printf("Failed to load main menu texture!\n");
		isRunning = false;
	}
	if (!LevelSelectionTexture->loadFromFile("./Assets/LevelSelection.png", gRenderer))
	{
		printf("Failed to load main menu texture!\n");
		isRunning = false;
	}
	if (!MapBaseTexture->loadFromFile("./Assets/MapLayout.png", gRenderer))
	{
		printf("Failed to load map texture!\n");
		isRunning = false;
	}
	if (!TileTexture->loadFromFile("./Assets/TileSprite.png", gRenderer))
	{
		printf("Failed to load tile texture!\n");
		isRunning = false;
	}
	if (!LevelIconsSheetTexture->loadFromFile("./Assets/LevelIcons.png", gRenderer))
	{
		printf("Failed to load main menu texture!\n");
		isRunning = false;
	}
	if (!PlayerSprite[0]->loadFromFile("./Assets/Lvl1Player.png", gRenderer))
	{
		printf("Failed to load main menu texture!\n");
		isRunning = false;
	}
	if (!PlayerSprite[1]->loadFromFile("./Assets/Lvl2Player.png", gRenderer))
	{
		printf("Failed to load main menu texture!\n");
		isRunning = false;
	}
	if (!PlayerSprite[2]->loadFromFile("./Assets/Lvl3Player.png", gRenderer))
	{
		printf("Failed to load main menu texture!\n");
		isRunning = false;
	}
	if (!EndLvlTexture->loadFromFile("./Assets/EndLvlScreen.png", gRenderer))
	{
		printf("Failed to load end level screen texture!\n");
		isRunning = false;
	}
	if (!MonsterTexture->loadFromFile("./Assets/OctopusMonster.png", gRenderer))
	{
		printf("Failed to load monster texture!\n");
		isRunning = false;
	}
	else {
		for (int i = 0; i < 4; ++i)
		{
			LevelIconsSprites[i].x = i % 2 * 260;
			LevelIconsSprites[i].y = i / 2 * 140;
			LevelIconsSprites[i].w = 260;
			LevelIconsSprites[i].h = 140;
		}
	}

	Font = TTF_OpenFont("./Assets/P22 Posada Regular Regular.ttf", 28);
	if (Font == nullptr)
	{
		printf("Failed to load P22 Poasada Regular Regular font! SDL_ttf Error: %s\n", TTF_GetError());
		isRunning = false;
	}

}

void Game::render(int windowWidth, int windowHeight) {
	//std::cout << "Rendering option number " << opt << std::endl;
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
	switch (opt) {
	case 0:
		MainMenuTexture->render(gRenderer, 0, 0);
		//Render buttons
		for (int i = 0; i < TOTAL_BUTTONS - 1; ++i)
		{
			MainMenuButtons[i]->render(gButtonSpriteSheetTexture, gRenderer);
		}
		for (int i = 0;i < TOTAL_BUTTONS - 1;++i) {
			gButtonTextSpriteSheetTexture->render(gRenderer, 240, 300 + 45 * i, &gTextClips[i]);
		}

		break;
	case 1:
		LevelSelectionTexture->render(gRenderer, 0, 0);
		SettingsBackButton->render(gButtonSpriteSheetTexture, gRenderer);
		gButtonTextSpriteSheetTexture->render(gRenderer, windowWidth - BUTTON_WIDTH, windowHeight - BUTTON_HEIGHT, &gTextClips[3]);
		for (int i = 0;i < 4;++i) {
			LevelSelectionButton[i]->render(gButtonSpriteSheetTexture, gRenderer);
		}
		for (int i = 0;i < 4;++i) {
			LevelIconsSheetTexture->render(gRenderer, i % 2 * 260 + 20 + i % 2 * 45, i / 2 * 140 + 15 + i / 2 * 80, &LevelIconsSprites[i]);
		}
		break;
	case 2:
		SettingsMenuTexture->render(gRenderer, 0, 0);
		SettingsBackButton->render(gButtonSpriteSheetTexture, gRenderer);
		gButtonTextSpriteSheetTexture->render(gRenderer, windowWidth - BUTTON_WIDTH, windowHeight - BUTTON_HEIGHT, &gTextClips[3]);
		break;
	case 3:
		LevelSelectionTexture->render(gRenderer, 0, 0);
		SettingsBackButton->render(gButtonSpriteSheetTexture, gRenderer);
		gButtonTextSpriteSheetTexture->render(gRenderer, windowWidth - BUTTON_WIDTH, windowHeight - BUTTON_HEIGHT, &gTextClips[3]);
		break;
	case 6:
		EndLvlTexture->render(gRenderer, 0, 0);
		YesButton->render(gButtonSpriteSheetTexture, gRenderer);
		NoButton->render(gButtonSpriteSheetTexture, gRenderer);
		gButtonTextSpriteSheetTexture->render(gRenderer, 140, 320, &gTextClips[4]);
		gButtonTextSpriteSheetTexture->render(gRenderer, 350, 320, &gTextClips[5]);
		break;
	case 7:
		map->draw();
		Levels[0]->draw();
		SettingsBackButton->render(gButtonSpriteSheetTexture, gRenderer);
		gButtonTextSpriteSheetTexture->render(gRenderer, windowWidth - BUTTON_WIDTH, windowHeight - BUTTON_HEIGHT, &gTextClips[3]);
		break;
	case 8:
		EndLvlTexture->render(gRenderer, 0, 0);
		YesButton->render(gButtonSpriteSheetTexture, gRenderer);
		NoButton->render(gButtonSpriteSheetTexture, gRenderer);
		gButtonTextSpriteSheetTexture->render(gRenderer, 140, 320, &gTextClips[4]);
		gButtonTextSpriteSheetTexture->render(gRenderer, 350, 320, &gTextClips[5]);
		break;
	case 9:
		map->draw();
		Levels[1]->draw();
		SettingsBackButton->render(gButtonSpriteSheetTexture, gRenderer);
		gButtonTextSpriteSheetTexture->render(gRenderer, windowWidth - BUTTON_WIDTH, windowHeight - BUTTON_HEIGHT, &gTextClips[3]);
		break;
	case 10:
		EndLvlTexture->render(gRenderer, 0, 0);
		YesButton->render(gButtonSpriteSheetTexture, gRenderer);
		NoButton->render(gButtonSpriteSheetTexture, gRenderer);
		gButtonTextSpriteSheetTexture->render(gRenderer, 140, 320, &gTextClips[4]);
		gButtonTextSpriteSheetTexture->render(gRenderer, 350, 320, &gTextClips[5]);
		break;
	case 11:
		map->draw();
		Levels[2]->draw();
		SettingsBackButton->render(gButtonSpriteSheetTexture, gRenderer);
		gButtonTextSpriteSheetTexture->render(gRenderer, windowWidth - BUTTON_WIDTH, windowHeight - BUTTON_HEIGHT, &gTextClips[3]);
		break;
	case 12:
		map->draw();
		NewPlayer->draw();
		firstMonster->draw();
		//Monster->render(gRenderer, (windowWidth - Monster->getWidth()) / 2, 30);
	}
	SDL_RenderPresent(gRenderer);
}

void Game::handleEvents() {
	SDL_Event event;
	int LvlAction;
	while (SDL_PollEvent(&event) != 0) {
		switch (event.type)
		{
		case SDL_QUIT: {
			isRunning = false;
			break;
		}
		default:
			break;
		}

		switch (opt) {
		case 0:
			//Main Menu 
			for (int i = 0; i < TOTAL_BUTTONS - 1; ++i)
			{
				MainMenuButtons[i]->handleEvent(&event, BUTTON_WIDTH, BUTTON_HEIGHT);
				opt = opt | MainMenuButtons[i]->getButtonAction();
			}
			SettingsBackButton->setButtonAction(opt);
			break;
		case 1:
			//Level Selection menu
			YesButton->setButtonAction(0);
			NoButton->setButtonAction(0);
			for (int i = 0; i < TOTAL_BUTTONS - 1; ++i)
			{
				MainMenuButtons[i]->setButtonAction(0);
			}
			for (int i = 0; i < 4; ++i)
			{
				LevelSelectionButton[i]->handleEvent(&event, 320, 155);
				if (LvlAccesible[i] != 0) { opt = opt | LevelSelectionButton[i]->getButtonAction(); }
			}
			SettingsBackButton->setButtonId(0);
			SettingsBackButton->handleEvent(&event, BUTTON_WIDTH, BUTTON_HEIGHT);

			if (opt == 1) {
				opt = SettingsBackButton->getButtonAction();
			}
			else {
				SettingsBackButton->setButtonAction(opt);
			}
			if (LvlAccesible[1] == 15 && contor == 0) {
				contor++;
				Levels[0] = new Level(gRenderer);
				Levels[0]->init(gButtonSpriteSheetTexture, NewPlayer, PlayerSprite[0], 116, 96);
				Levels[0]->setLvlLayout(Lvl1Layout);
			}
			if (LvlAccesible[1] == 0 && contor == 1) {
				contor++;
				LvlAccesible[2] = 15;
				NewPlayer = Levels[0]->getPlayerState();
				delete Levels[0];
				Levels[1] = new Level(gRenderer);
				Levels[1]->init(gButtonSpriteSheetTexture, NewPlayer,PlayerSprite[1], 116, 56);
				Levels[1]->setLvlLayout(Lvl2Layout);
			}
			else if (LvlAccesible[2] == 0 && contor == 2) {
				contor++;
				LvlAccesible[3] = 15;
				NewPlayer = Levels[1]->getPlayerState();
				delete Levels[1];
				Levels[2] = new Level(gRenderer);
				Levels[2]->init(gButtonSpriteSheetTexture, NewPlayer, PlayerSprite[2], 96, 46);
				Levels[2]->setLvlLayout(Lvl3Layout);
			}
			else if (LvlAccesible[3] == 0 && contor ==3) {
				contor++;
				delete Levels[2];
			}
			break;
		case 2:
			//Settings menu
			for (int i = 0; i < TOTAL_BUTTONS - 1; ++i)
			{
				MainMenuButtons[i]->setButtonAction(0);
			}
			SettingsBackButton->handleEvent(&event, BUTTON_WIDTH, BUTTON_HEIGHT);
			opt = SettingsBackButton->getButtonAction();
			break;
		case 3:
			//Extras menu
			for (int i = 0; i < TOTAL_BUTTONS - 1; ++i)
			{
				MainMenuButtons[i]->setButtonAction(0);
			}
			SettingsBackButton->handleEvent(&event, BUTTON_WIDTH, BUTTON_HEIGHT);
			opt = SettingsBackButton->getButtonAction();
			break;
		case 5:
			//Shop menu
			break;
		case 6:
			//Level 1 ended - show level ending screen
			NoButton->handleEvent(&event, BUTTON_WIDTH, BUTTON_HEIGHT);
			if (NoButton->getButtonAction() != 0) { opt = 1; SettingsBackButton->setButtonAction(opt); }
			YesButton->handleEvent(&event, BUTTON_WIDTH, BUTTON_HEIGHT);
			opt = opt | YesButton->getButtonAction();
			Levels[0]->setLvlState(0);
			break;
		case 7:
			//Level 1 layout
			for (int i = 0; i < 4; ++i)
			{
				LevelSelectionButton[i]->setButtonAction(0);
			}
			SettingsBackButton->setButtonId(1);
			Levels[0]->update(&event);
			SettingsBackButton->handleEvent(&event, BUTTON_WIDTH, BUTTON_HEIGHT);
			LvlAction = Levels[0]->getLvlState();
			if (LvlAction == 1) {
				LvlAccesible[1] = 0;
				opt = 6;
			}
			else if (LvlAction == 2) {
				opt = 12;
			}
			else {
				opt = SettingsBackButton->getButtonAction();
			}
			break;
		case 8:
			//Level 2 ended - show level ending screen
			NoButton->handleEvent(&event, BUTTON_WIDTH, BUTTON_HEIGHT);
			if (NoButton->getButtonAction() != 0) { opt = 1; SettingsBackButton->setButtonAction(opt); }
			YesButton->handleEvent(&event, BUTTON_WIDTH, BUTTON_HEIGHT);
			opt = opt | YesButton->getButtonAction();
			Levels[1]->setLvlState(0);
			break;
		case 9:
			//Level 2 layout
			for (int i = 0; i < 4; ++i)
			{
				LevelSelectionButton[i]->setButtonAction(0);
			}
			SettingsBackButton->setButtonId(1);
			Levels[1]->update(&event);
			SettingsBackButton->handleEvent(&event, BUTTON_WIDTH, BUTTON_HEIGHT);
			LvlAction = Levels[1]->getLvlState();
			if (LvlAction == 1) {
				LvlAccesible[2] = 0;
				opt = 8;
			}
			else {
				opt = SettingsBackButton->getButtonAction();
			}
			break;
		case 10:
			//Level 3 ended - finally
			NoButton->handleEvent(&event, BUTTON_WIDTH, BUTTON_HEIGHT);
			if (NoButton->getButtonAction() != 0) { opt = 1; SettingsBackButton->setButtonAction(opt); }
			YesButton->handleEvent(&event, BUTTON_WIDTH, BUTTON_HEIGHT);
			opt = opt | YesButton->getButtonAction();
			Levels[2]->setLvlState(0);
			break;
		case 11:
			//Level 3 Layout
			for (int i = 0; i < 4; ++i)
			{
				LevelSelectionButton[i]->setButtonAction(0);
			}
			SettingsBackButton->setButtonId(1);
			Levels[2]->update(&event);
			SettingsBackButton->handleEvent(&event, BUTTON_WIDTH, BUTTON_HEIGHT);
			LvlAction = Levels[2]->getLvlState();
			if (LvlAction == 1) {
				LvlAccesible[3] = 0;
				opt = 10;
			}
			else {
				opt = SettingsBackButton->getButtonAction();
			}
			break;
		}
	}
}

void Game::clean() {
	MainMenuTexture->free();
	gButtonSpriteSheetTexture->free();
	gButtonTextSpriteSheetTexture->free();
	SettingsMenuTexture->free();
	LevelSelectionTexture->free();
	LevelIconsSheetTexture->free();
	delete SettingsBackButton;
	SettingsBackButton = nullptr;
	for (int i = 0;i < TOTAL_BUTTONS - 1;++i) {
		delete MainMenuButtons[i];
		MainMenuButtons[i] = nullptr;
	}
	for (int i = 0;i < 4;++i) {
		delete LevelSelectionButton[i];
		LevelSelectionButton[i] = nullptr;
	}
	if (NewPlayer != nullptr) {
		delete NewPlayer;
	}
	TTF_CloseFont(Font);
	Font = NULL;
	TileTexture->free();
	MapBaseTexture->free();
	delete map;
	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);
	gWindow = nullptr;
	gRenderer = nullptr;
	std::cout << "Game cleaned" << std::endl;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}