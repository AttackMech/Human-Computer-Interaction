#ifndef _SCREENS_H
#define _SCREENS_H

//#include "consoleui.h"
#include <allegro5\allegro5.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <iostream>
#include <fstream>
#include <string>
//#include <allegro5\allegro5.h>
//#include <allegro5\allegro_font.h>
//#include <allegro5\allegro_ttf.h>
#include <vector>

struct MenuPos
{
	int minX, minY, maxX, maxY;
	MenuPos();
};



class ScreenUI
{
protected:
	ALLEGRO_BITMAP *fullUI, *uiImage;
	int currPos, maxPos;
	bool firstRun = true;
	std::string screen, changeType, screenTypeA, screenTypeB, nextTypeA, nextTypeB;
	std::vector<std::string> menuNames;
	std::vector<MenuPos*> hilites;

	virtual void setMenuNames() { };
	virtual void makeHilitePositions(std::string menuType);

	MenuPos* getMenuPos() { return hilites[currPos]; }

public:
	ScreenUI(const char* imageFilename, std::string menuType, int menuSize);
	
	virtual ~ScreenUI();
	virtual bool processInput(int inputValue) = 0;
	virtual ALLEGRO_BITMAP* getBitmap();
	int getMinX(); // fix
	int getMaxX();
	int getMinY();
	int getMaxY();

	std::string getScreen() { return screen; }
	std::string getChanges() { return changeType; }
	std::string getTypeA() { return screenTypeA; }
	std::string getTypeB() { return screenTypeB; }
	std::string getNextTypeA() { return nextTypeA; }
	std::string getNextTypeB() { return nextTypeB; }
};

class MainScreen : public ScreenUI
{
	private:
		void setMenuNames();	
		
	public:
		MainScreen() : ScreenUI("images/MainScreen.jpg", "MAIN", 5) { screenTypeA = "DEFAULT"; screenTypeB = "DEFAULT"; }
		~MainScreen() { return; };
		bool processInput(int inputValue);
};

class MediaSelectScreen : public ScreenUI
{
private:
	void setMenuNames();

public:
	MediaSelectScreen(std::string typeA, std::string typeB) : ScreenUI("images/MediaSelectScreen.jpg", "SELECT", 4) { screenTypeA = typeA; screenTypeB = typeB; }
	~MediaSelectScreen() { return; };
	bool processInput(int inputValue);
};

class ListingsScreen : public ScreenUI
{
	private:
		static const int rowSize = 3, colSize = 3, searchX = 154, searchY = 105, infoX = 824, infoY = 134, iconX = 988, iconY = 481, textX = 194, textY = 183, textXShift = 229, textYShift = 155;
		
		int listPage = 0;
		bool searching = false;
		std::string searchTerm = "Search";
		ALLEGRO_BITMAP *icon;
		ALLEGRO_FONT *font = al_load_ttf_font("images/Tahoma.ttf", 30, 0);

		void setMenuNames();
		void setMenuItems();

	public:
		ListingsScreen(std::string typeA, std::string typeB);
		~ListingsScreen() { al_destroy_font(font); };
		
		std::string getSelection() { return menuNames[currPos]; }
		void ListingsScreen::setText(std::string term) { searchTerm = term; }
		
		bool processInput(int inputValue);
		ALLEGRO_BITMAP* getBitmap();
};

class MediaPlayScreen : public ScreenUI
{
	private:
		void setMenuNames();

	public:
		MediaPlayScreen(std::string typeA, std::string typeB) : ScreenUI("images/MediaPlayScreen.jpg", "PLAYER", 7) { screenTypeA = typeA; screenTypeB = typeB; };
		~MediaPlayScreen() { return; };
		bool processInput(int inputValue);
};

class GamePlayScreen : public ScreenUI
{
private:
	void setMenuNames();
public:
	GamePlayScreen(std::string typeA, std::string typeB) : ScreenUI("images/GamePlayScreen.jpg", "GAME", 4) { screenTypeA = typeA; screenTypeB = typeB; };
	~GamePlayScreen() { return; };
	bool processInput(int inputValue);
};

class PurchaseInfoScreen : public ScreenUI
{
	private:
		void setMenuNames();

		static const int shotX = 703, shotY = 88, iconX = 889, iconY = 481;

		std::string itemName;
		int currShot = 1;
		ALLEGRO_BITMAP *shot1, *shot2, *shot3, *icon;

	public:
		PurchaseInfoScreen(std::string typeA, std::string typeB);
		~PurchaseInfoScreen() { return; };
		bool processInput(int inputValue);
		ALLEGRO_BITMAP* getBitmap();
};

class ConfirmDetailsScreen : public ScreenUI
{
	private:
		const static int textOffsetX = 8, textOffsetY = 2, circleX = 427, circleY = 530, circleR = 4, circleOffsetX = 117;
		//std::array <std::string, 11> detailItems;
		std::string detailItems[11];
		std::string itemName;
		bool missing;
		int cardSelect = 11;
		bool purchased = false;
		ALLEGRO_FONT *font = al_load_ttf_font("images/Tahoma.ttf", 16, 0);

		void setMenuNames();

	public:
		ConfirmDetailsScreen(std::string typeA, std::string typeB);
		~ConfirmDetailsScreen() { al_destroy_font(font); };
		bool processInput(int inputValue);
		ALLEGRO_BITMAP* getBitmap();
		//std::string getMissing() { return missing; }
		void setText(std::string text) { currPos > 11 ? detailItems[currPos - 3] = text : detailItems[currPos] = text; }
};

class KeyboardInput : public ScreenUI
{
	private:
		static const int inputX = 156, inputY = 224, keyShiftX = 97, bkspcMaxX = 1176, enterMaxX = 1146, cxlMaxX = 1146;

		ALLEGRO_FONT *font = al_load_font("images/Tahoma.ttf", 24, 0);
		std::string resultText;

		void setMenuNames();
		std::string getKey();

	public:
		KeyboardInput();
		~KeyboardInput() { al_destroy_font(font); };
		bool processInput(int inputValue);
		ALLEGRO_BITMAP* getBitmap();
		std::string getResult() { return resultText; }
};

class FinalConfirmScreen : public ScreenUI
{
	private:
		void setMenuNames() { return; }

	public:
		FinalConfirmScreen() : ScreenUI("images/FinalConfScreen.png", "FINALCONF", 0) { screenTypeA = "DEFAULT"; screenTypeB = "DEFAULT"; };
		~FinalConfirmScreen() { return; };
		bool processInput(int inputValue);
};

class SuccessScreen : public ScreenUI
{
private:
	void setMenuNames() { return; }

public:
	SuccessScreen() : ScreenUI("images/SuccessScreen.png", "SUCCESS", 0) { screenTypeA = "DEFAULT"; screenTypeB = "DEFAULT"; };
	~SuccessScreen() { return; };
	bool processInput(int inputValue);
};

class DetailsMissingScreen : public ScreenUI
{
private:
	void setMenuNames() { return; }

public:
	DetailsMissingScreen() : ScreenUI("images/NeedDetailsScreen.jpg", "MISSING", 0) { screenTypeA = "DEFAULT"; screenTypeB = "DEFAULT"; };
	~DetailsMissingScreen() { return; }
	bool processInput(int inputValue);
};

class NetflixAppScreen : public ScreenUI
{
private:
	void setMenuNames() { return; }
public:
	NetflixAppScreen() : ScreenUI("images/App_netflix.jpg", "NETFLIX", 0) { screenTypeA = "CONSOLE"; screenTypeB = "APPS"; };
	~NetflixAppScreen() { return; };
	bool processInput(int inputValue) { return false; };
};


std::string intNumToString(int digit);

#endif

