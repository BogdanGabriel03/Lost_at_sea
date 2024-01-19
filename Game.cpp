#include "Game.h"

Button* MainMenuButtons[TOTAL_BUTTONS - 1];
Button* SettingsBackButton = nullptr;
Button* LevelSelectionButton[4];
Button* PurchaseSelectionButton[4];
Button* YesButton = nullptr, * NoButton = nullptr;
Button* AttackButton = nullptr;
TextureManager* MainMenuTexture = nullptr;
TextureManager* gButtonSpriteSheetTexture = nullptr;
TextureManager* gButtonTextSpriteSheetTexture = nullptr;
TextureManager* LevelIconsSheetTexture = nullptr;
TextureManager* SettingsMenuTexture = nullptr, * LevelSelectionTexture = nullptr, * shopTexture=nullptr;
TextureManager* MapBaseTexture = nullptr;
TextureManager* TileTexture = nullptr;
TextureManager* PlayerSprite[3];
TextureManager* EndLvlTexture = nullptr;
TextureManager* MonsterTexture;
SDL_Rect LevelIconsSprites[4];
SDL_Rect gTextClips[TOTAL_BUTTONS + 3];
SDL_Rect SpriteClips[BUTTON_SPRITE_TOTAL];

const int TOTAL_DATA = 10;
/*
	gData[0] = saved(boolean)
	gData[1] = contor;
	gData[2] = LvlAccesible[1];
	gData[3] = LvlAccesible[2];
	gData[4] = LvlAccesible[3];
	gData[5] = player health;
	gData[6] = player attack;
	gData[7] = player level;
	gData[8] = player experience;
	gData[9] = player gold;
*/
Sint32 gData[TOTAL_DATA];

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
int Lvl3Layout[36] = { 0,0,0,2,0,1,1,2,0,0,0,4,0,1,1,0,1,3,0,0,0,1,1,1,1,0,0,1,5,1,4,1,0,3,1,2 };
int LvlAccesible[4] = { 15,15,0,0 };
int contor=0;
bool SaveLoaded = false;

Map* map;
Level* Levels[3];
Player* NewPlayer;
Enemy* firstMonster;
TTF_Font* Font;			//globally used font
int opt,last_opt;

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
		PurchaseSelectionButton[i] = new Button(4);
	}
	YesButton = new Button(3);
	NoButton = new Button(3);
	AttackButton = new Button(3);

	MainMenuTexture = new TextureManager();
	gButtonSpriteSheetTexture = new TextureManager();
	gButtonTextSpriteSheetTexture = new TextureManager();
	SettingsMenuTexture = new TextureManager();
	shopTexture = new TextureManager();
	LevelSelectionTexture = new TextureManager();
	LevelIconsSheetTexture = new TextureManager();
	MapBaseTexture = new TextureManager();
	TileTexture = new TextureManager();
	EndLvlTexture = new TextureManager();
	MonsterTexture = new TextureManager();
	
	
	map = new Map(gRenderer);
	map->init(MapBaseTexture);

	for (int i = 0;i < 3;++i) {
		PlayerSprite[i] = new TextureManager();
	}
	
	loadMedia(windowWidth, windowHeight);
	NewPlayer = new Player(gRenderer, Font);
	if (SaveLoaded == true) {
		contor = gData[1];
		LvlAccesible[1] = gData[2];
		LvlAccesible[2] = gData[3];
		LvlAccesible[3] = gData[4];
		NewPlayer->setHealth(gData[5]);
		NewPlayer->setAttack(gData[6]);
		NewPlayer->setLevel(gData[7]);
		NewPlayer->setExperience(gData[8]);
		NewPlayer->setGold(gData[9]);
		gData[0] = 0;
	}
	firstMonster = new Enemy(gRenderer, 1,Font);
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
		AttackButton->setPosition((windowWidth - BUTTON_WIDTH) / 2, 220);
		PurchaseSelectionButton[0]->setPosition(0, 0);
		PurchaseSelectionButton[1]->setPosition(320, 0);
		PurchaseSelectionButton[2]->setPosition(0, 220);
		PurchaseSelectionButton[3]->setPosition(320, 220);
	}

	if (!gButtonTextSpriteSheetTexture->loadFromFile("./Assets/TextButtons.png", gRenderer)) {
		printf("Failed to load buttons' texts texture!\n");
		isRunning = false;
	}
	else {
		for (int i = 0; i < TOTAL_BUTTONS + 3; ++i)
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
	if (!shopTexture->loadFromFile("./Assets/shop.png", gRenderer))
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

	SDL_RWops* file = SDL_RWFromFile("./Assets/stats.bin", "r+b");

	//File does not exist
	if (file == NULL)
	{
		printf("Warning: Unable to open file! SDL Error: %s\n", SDL_GetError());

		//Create file for writing
		file = SDL_RWFromFile("./Assets/stats.bin", "w+b");
		if (file != NULL)
		{
			printf("New file created!\n");

			//Initialize data
			for (int i = 0; i < TOTAL_DATA; ++i)
			{
				gData[i] = 0;
				SDL_RWwrite(file, &gData[i], sizeof(Sint32), 1);
			}

			//Close file handler
			SDL_RWclose(file);
		}
		else
		{
			printf("Error: Unable to create file! SDL Error: %s\n", SDL_GetError());
			isRunning = false;
		}
	}
	else
	{
		//Load data
		printf("Reading file...!\n");
		for (int i = 0; i < TOTAL_DATA; ++i)
		{
			SDL_RWread(file, &gData[i], sizeof(Sint32), 1);
		}
		std::cout << "\n";
		if (gData[0] == 1) {
			SaveLoaded = 1;
		}

		//Close file handler
		SDL_RWclose(file);
	}

}

void Game::render(int windowWidth, int windowHeight) {
	//std::cout << "Rendering option number " << opt << std::endl;
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
	switch (opt) {
	case 0:
		//Main menu
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
		//Level selction menu
		LevelSelectionTexture->render(gRenderer, 0, 0);
		SettingsBackButton->render(gButtonSpriteSheetTexture, gRenderer);
		gButtonTextSpriteSheetTexture->render(gRenderer, windowWidth - BUTTON_WIDTH+25, windowHeight - BUTTON_HEIGHT, &gTextClips[3]);
		for (int i = 0;i < 4;++i) {
			LevelSelectionButton[i]->render(gButtonSpriteSheetTexture, gRenderer);
		}
		for (int i = 0;i < 4;++i) {
			LevelIconsSheetTexture->render(gRenderer, i % 2 * 260 + 20 + i % 2 * 45, i / 2 * 140 + 15 + i / 2 * 80, &LevelIconsSprites[i]);
		}
		break;
	case 2:
		//Settings menu
		SettingsMenuTexture->render(gRenderer, 0, 0);
		SettingsBackButton->render(gButtonSpriteSheetTexture, gRenderer);
		gButtonTextSpriteSheetTexture->render(gRenderer, windowWidth - BUTTON_WIDTH+25, windowHeight - BUTTON_HEIGHT, &gTextClips[3]);
		break;
	case 3:
		//Extras menu
		LevelSelectionTexture->render(gRenderer, 0, 0);
		SettingsBackButton->render(gButtonSpriteSheetTexture, gRenderer);
		gButtonTextSpriteSheetTexture->render(gRenderer, windowWidth - BUTTON_WIDTH+25, windowHeight - BUTTON_HEIGHT, &gTextClips[3]);
		break;
	case 5:
		//Shop
		shopTexture->render(gRenderer, 0, 0);
		for (int i = 0;i < 4;++i) {
			PurchaseSelectionButton[i]->render(gButtonSpriteSheetTexture, gRenderer);  //these are now used to explore the shop
		}
		SettingsBackButton->render(gButtonSpriteSheetTexture, gRenderer);
		gButtonTextSpriteSheetTexture->render(gRenderer, windowWidth - BUTTON_WIDTH + 25, windowHeight - BUTTON_HEIGHT, &gTextClips[3]);
		break;
	case 6:
		//End Level 1 Screen
		EndLvlTexture->render(gRenderer, 0, 0);
		YesButton->render(gButtonSpriteSheetTexture, gRenderer);
		NoButton->render(gButtonSpriteSheetTexture, gRenderer);
		gButtonTextSpriteSheetTexture->render(gRenderer, 140, 320, &gTextClips[4]);
		gButtonTextSpriteSheetTexture->render(gRenderer, 350, 320, &gTextClips[5]);
		break;
	case 7:
		//First level
		map->draw();
		Levels[0]->draw();
		SettingsBackButton->render(gButtonSpriteSheetTexture, gRenderer);
		gButtonTextSpriteSheetTexture->render(gRenderer, windowWidth - BUTTON_WIDTH+25, windowHeight - BUTTON_HEIGHT, &gTextClips[3]);
		break;
	case 8:
		//End Level Screen
		EndLvlTexture->render(gRenderer, 0, 0);
		YesButton->render(gButtonSpriteSheetTexture, gRenderer);
		NoButton->render(gButtonSpriteSheetTexture, gRenderer);
		gButtonTextSpriteSheetTexture->render(gRenderer, 140, 320, &gTextClips[4]);
		gButtonTextSpriteSheetTexture->render(gRenderer, 350, 320, &gTextClips[5]);
		break;
	case 9:
		//Second level
		map->draw();
		Levels[1]->draw();
		SettingsBackButton->render(gButtonSpriteSheetTexture, gRenderer);
		gButtonTextSpriteSheetTexture->render(gRenderer, windowWidth - BUTTON_WIDTH+25, windowHeight - BUTTON_HEIGHT, &gTextClips[3]);
		break;
	case 10:
		//End Level Screen
		EndLvlTexture->render(gRenderer, 0, 0);
		YesButton->render(gButtonSpriteSheetTexture, gRenderer);
		NoButton->render(gButtonSpriteSheetTexture, gRenderer);
		gButtonTextSpriteSheetTexture->render(gRenderer, 140, 320, &gTextClips[4]);
		gButtonTextSpriteSheetTexture->render(gRenderer, 350, 320, &gTextClips[5]);
		break;
	case 11:
		//Third level
		map->draw();
		Levels[2]->draw();
		SettingsBackButton->render(gButtonSpriteSheetTexture, gRenderer);
		gButtonTextSpriteSheetTexture->render(gRenderer, windowWidth - BUTTON_WIDTH+25, windowHeight - BUTTON_HEIGHT, &gTextClips[3]);
		break;
	case 12:
		//Monster Fight Screen
		map->draw();
		Levels[last_opt/8+last_opt/10]->draw();
		AttackButton->render(gButtonSpriteSheetTexture, gRenderer);
		gButtonTextSpriteSheetTexture->render(gRenderer, (windowWidth - BUTTON_WIDTH)/2, 220, &gTextClips[6]);
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
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_s) {
				std::cout << NewPlayer->getHealth() << " " << NewPlayer->getAttack() << " " << NewPlayer->getGold() << "\n";
				gData[0] = 1;
				gData[1] = contor-1;
				gData[2] = LvlAccesible[1];
				gData[3] = LvlAccesible[2];
				gData[4] = LvlAccesible[3];
				gData[5] = NewPlayer->getHealth();
				gData[6] = NewPlayer->getAttack();
				gData[7] = NewPlayer->getLevel();
				gData[8] = NewPlayer->getExperience();
				gData[9] = NewPlayer->getGold();
			}
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
				Levels[0]->init(gButtonSpriteSheetTexture, NewPlayer, firstMonster, PlayerSprite[0], MonsterTexture,116, 96);
				Levels[0]->setLvlLayout(Lvl1Layout);
			}
			else if (LvlAccesible[1] == 0 && contor == 1) {
				contor++;
				LvlAccesible[2] = 15;
				if (SaveLoaded == false) {
					NewPlayer = Levels[0]->getPlayerState();
					delete Levels[0];
				}
				Levels[1] = new Level(gRenderer);
				delete firstMonster;
				firstMonster = nullptr;
				firstMonster = new Enemy(gRenderer, 2, Font);
				Levels[1]->init(gButtonSpriteSheetTexture, NewPlayer, firstMonster,PlayerSprite[1], MonsterTexture,116, 56);
				Levels[1]->setLvlLayout(Lvl2Layout);
			}
			else if (LvlAccesible[2] == 0 && contor == 2) {
				contor++;
				LvlAccesible[3] = 15;
				if (SaveLoaded == false) {
					NewPlayer = Levels[1]->getPlayerState();
					delete Levels[1];
				}
				Levels[2] = new Level(gRenderer);
				firstMonster->setType(3);
				Levels[2]->init(gButtonSpriteSheetTexture, NewPlayer, firstMonster,PlayerSprite[2], MonsterTexture,96, 46);
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
			for (int i = 0; i < 4; ++i)
			{
				LevelSelectionButton[i]->setButtonAction(0);
			}
			for (int i = 0;i < 4;++i) {
				PurchaseSelectionButton[i]->handleEvent(&event, 320, 155);
			}
			SettingsBackButton->setButtonId(1);
			SettingsBackButton->handleEvent(&event, BUTTON_WIDTH, BUTTON_HEIGHT);
			opt = SettingsBackButton->getButtonAction();
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
				last_opt = 7;
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
			else if (LvlAction == 2) {
				opt = 12;
				last_opt = 9;
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
			else if (LvlAction == 2) {
				opt = 12;
				last_opt = 11;
			}
			else {
				opt = SettingsBackButton->getButtonAction();
			}
			break;
		case 12:
			//Monster fight layout
			AttackButton->handleEvent(&event, BUTTON_WIDTH, BUTTON_HEIGHT);
			if (AttackButton->getButtonAction() == 1) {
				Levels[last_opt/8+last_opt/10]->update(&event);
			}
			if (Levels[last_opt / 8 + last_opt / 10]->getLvlState() == 0) {
				opt = last_opt;
			}
			AttackButton->setButtonAction(0);
			break;
		}
	}
}

void Game::clean() {

	SDL_RWops* file = SDL_RWFromFile("./Assets/stats.bin", "w+b");
	if (file != NULL)
	{
		//Save data
		for (int i = 0; i < TOTAL_DATA; ++i)
		{
			SDL_RWwrite(file, &gData[i], sizeof(Sint32), 1);
		}
		//Close file handler
		SDL_RWclose(file);
	}
	else
	{
		printf("Error: Unable to save file! %s\n", SDL_GetError());
	}

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
	for (int i = 0;i < 3;++i) {
		if (LvlAccesible[i+1]!=0 && Levels[i] != nullptr) {
			delete Levels[i];
		}
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