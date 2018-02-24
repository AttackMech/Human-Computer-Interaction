#ifndef _CONSOLEUI_H
#define _CONSOLEUI_H

#include "inputcontrols.h"
#include "screens.h"
//#include <allegro5\allegro5.h>
#include <allegro5\allegro5.h>
#include <allegro5\allegro_image.h>
//#include <vector>
#include <string>
#include <vector>
#include <array>

class ConsoleUI {

private:
	int currType, mouseX, mouseY, userInput;
	bool screenInfoProcessing;

	std::array <std::string, 12> screens = { "MAIN", "SELECT", "LISTING", "GAME", "PLAYER", "PURCHASE", "DETAILS", "KYBD", "FINALCONF", "SUCCESS", "MISSING", "NETFLIX" };
	std::array <std::string, 8> screenType = { "DEFAULT", "GAME", "MOVIE", "MUSIC", "TV", "APPS", "DEVICE", "STORE" };
	std::array <std::string, 24> actions = { "SELECT SCREEN", "LISTING SCREEN", "GAME SCREEN", "MEDIA SCREEN", "PURCHASE SCREEN", "DETAILS SCREEN", "BACK",
		"MOVE MENU SELECTION", "SCROLL LISTINGS", "SCROLL INFO", "SCROLL SHOTS", "CHANGE TRAY", "POWER OFF", "POWER ON", "KYBD", "ADD TEXT", "DETAILS MISSING",
		"CONFRIM PURCHASE", "SUCCESS SCREEN", "MAIN SCREEN", "TEXT ENTER", "PLAY DISC", "NETFLIX", "NONE"};
	std::vector<ScreenUI*> prevScreen;

	ScreenUI *currScreen;
	InputControls *inputCtrl;

	ALLEGRO_BITMAP *ui, *ctrl;
	ALLEGRO_DISPLAY *display;
	
public:
	ConsoleUI(ALLEGRO_DISPLAY *display);
	~ConsoleUI();
	int start();
};

#endif

