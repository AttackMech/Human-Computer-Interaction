#ifndef _INPUTCONTROLS_H
#define _INPUTCONTROLS_H

#include <allegro5\allegro5.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <string>
#include <array>
//#include <allegro5\allegro5.h>
//#include <string>
//#include <array>


struct ButtonPos
{
	int minX, minY, maxX, maxY;
	ButtonPos();
};


class InputControls {

private:
	int discType, lastCtrl;
	bool trayOpen, powerStatus, discInsert;
	std::array<ButtonPos*, 14> controls;
	std::array<std::string, 14> controlNames = { "EJECT", "POWER", "UP", "DOWN", "LEFT", "RIGHT", "START", "SELECT", "A", "B", "X", "Y", "GAME", "MOVIE" };
	std::array<std::string, 3> discNames = { "NONE", "GAME", "MOVIE" };
	ALLEGRO_BITMAP *controlMap, *controlHilites, *controller, *consoleOpen, *consoleClosed, *consoleDisc, *discBoth, *discGameOnly, *discMovieOnly;
	ButtonPos* makeButton(int bValue);

	const static int controllerX = 1, controllerY = 2, consoleX = 3, consoleY = 4, discX = 5, discY = 6;

public:
	InputControls();
	~InputControls();
	ALLEGRO_BITMAP *getBitmap(int mx, int my);
	int getInput(int mx, int my);
	void powerControl();
	void trayControl();
	bool getTrayOpen();
	bool getPowerStatus();
	bool getDiscInsert();
	int getDiscType();
	void insertDisc(int discType);
	std::string getLastCtrl();

	int getY(int mx, int my);
	int getH();
};

#endif



