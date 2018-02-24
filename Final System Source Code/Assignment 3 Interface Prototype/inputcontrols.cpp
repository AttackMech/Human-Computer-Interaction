#include "inputcontrols.h"
//#include <allegro5\allegro5.h>
//#include <allegro5\allegro_image.h>
//#include <allegro5\allegro_primitives.h>
//#include <string>
//#include <array>

#define OFFSETX 1280
#define OFFSETY 720
#define BORDER 3.0
#define DISCOFFSET 12
#define CLEAR al_map_rgba(0, 0, 0, 0)
#define YELLOW al_map_rgb(255, 255, 0)
#define GREY al_map_rgb(54, 54, 54)

ButtonPos::ButtonPos()
{
	minX = 0;
	minY = 0;
	maxX = 0;
	minY = 0;
}

// constructor
InputControls::InputControls()
{
	trayOpen = false;
	powerStatus = false;
	discInsert = false;
	discType = 0;

	// create button position structures for hilighting
	for (int i = 0; i < controlNames.size(); ++i)
	{
		controls[i] = makeButton(i);
	}

	// load bitmaps
	controller = al_load_bitmap("images/controller.png");
	consoleOpen = al_load_bitmap("images/console_open.png");
	consoleClosed = al_load_bitmap("images/console_closed.png");
	consoleDisc = al_load_bitmap("images/console_disc.png");
	discBoth = al_load_bitmap("images/disc_both.png");
	discGameOnly = al_load_bitmap("images/disc_movie_in.png");
	discMovieOnly = al_load_bitmap("images/disc_game_in.png");
	controlMap = al_create_bitmap(1920 - (OFFSETX + BORDER), 1080);
	controlHilites = al_create_bitmap(1920 - (OFFSETX + BORDER), 1080);

} // end of constructor

// destructor
InputControls::~InputControls()
{
	al_destroy_bitmap(controlMap);
	al_destroy_bitmap(controlHilites);
	al_destroy_bitmap(controller);
	al_destroy_bitmap(consoleOpen);
	al_destroy_bitmap(consoleClosed);
	al_destroy_bitmap(discBoth);
	al_destroy_bitmap(discGameOnly);
	al_destroy_bitmap(discMovieOnly);
} // end of destructor

// get hilighted button based on mouse position
ALLEGRO_BITMAP* InputControls::getBitmap(int mx, int my)
{
	al_set_target_bitmap(controlHilites);
	al_clear_to_color(CLEAR);

	int mousePosition = getInput(mx, my);
	if (mousePosition >= 0)
	{
		if (mousePosition <= 1)
		{
			al_draw_rectangle(controls[mousePosition]->minX, controls[mousePosition]->minY + al_get_bitmap_height(discBoth) + BORDER,
				controls[mousePosition]->maxX, controls[mousePosition]->maxY + al_get_bitmap_height(discBoth) + BORDER, YELLOW, BORDER);
		}
		else if (mousePosition <= 11)
		{
			al_draw_rectangle(controls[mousePosition]->minX, controls[mousePosition]->minY + al_get_bitmap_height(discBoth) * 2 + BORDER * 2,
				controls[mousePosition]->maxX, controls[mousePosition]->maxY + al_get_bitmap_height(discBoth) * 2 + BORDER * 2, YELLOW, BORDER);
		}
		else
		{
			al_draw_rectangle(controls[mousePosition]->minX, controls[mousePosition]->minY, controls[mousePosition]->maxX, controls[mousePosition]->maxY, YELLOW, BORDER);
		}
	}
	// check if position of mouse is on a button
	//for (int i = 0; i < controlNames.size(); i++)
	//{
	//	if (mx >= controls[i]->minX && mx <= controls[i]->maxX && my >= controls[i]->minY && my <= controls[i]->maxY)
	//	{
	//		// draw hilights
	//		al_draw_rectangle(controls[i]->minX - (OFFSETX + BORDER), controls[i]->minY - (OFFSETY + BORDER), controls[i]->maxX - (OFFSETX + BORDER), controls[i]->maxY - (OFFSETY + BORDER), YELLOW, 2.0);
	//		
	//		break;			
	//	} // end of if
	//} // end of for

	// draw complete control bitmap - NEEED TO GET NUMBERS!!!!
	al_set_target_bitmap(controlMap);
	al_clear_to_color(GREY);
	switch (discType)
	{
	case 1:
		al_draw_bitmap(discMovieOnly, 0, 0, 0);
		break;
	case 2:
		al_draw_bitmap(discGameOnly, 0, 0, 0);
		break;
	default:
		al_draw_bitmap(discBoth, 0, 0, 0);
		break;
	}

	if (trayOpen && discInsert)
		al_draw_bitmap(consoleDisc, 0, al_get_bitmap_height(discBoth) + BORDER, 0);
	else if (trayOpen)
		al_draw_bitmap(consoleOpen, 0, (al_get_bitmap_height(discBoth) + BORDER), 0);
	else
		al_draw_bitmap(consoleClosed, 0, (al_get_bitmap_height(discBoth) + BORDER), 0);
	
	al_draw_bitmap(controller, 0, (float) (al_get_bitmap_height(discBoth) * 2 + BORDER * 2), 0);
	
	al_draw_bitmap(controlHilites, 0, 0, 0);

	return controlMap;
}

  /* uses the mouse position to determine what control input is selected (if any)
  and returns an int value based on the selection */
int InputControls::getInput(int mx, int my)
{
	int bmpHeight = al_get_bitmap_height(discBoth);

	// check mouse position against button positions
	for (int i = 0; i < controlNames.size(); ++i)
	{
		if (i <= 1)
		{
			if ((mx >= controls[i]->minX + OFFSETX + BORDER && mx <= controls[i]->maxX + OFFSETX + BORDER) &&
				(my >= controls[i]->minY + bmpHeight + BORDER && my <= controls[i]->maxY + bmpHeight + BORDER))
				return i;
		}
		else if (i <= 11)
		{
			if ((mx >= controls[i]->minX + OFFSETX + BORDER && mx <= controls[i]->maxX + OFFSETX + BORDER) &&
				(my >= controls[i]->minY + bmpHeight * 2 + BORDER * 2 && my <= controls[i]->maxY + bmpHeight * 2 + BORDER * 2))
				return i;
		}
		else
		{
			if ((mx >= controls[i]->minX + OFFSETX + BORDER && mx <= controls[i]->maxX + OFFSETX + BORDER) && (my >= controls[i]->minY && my <= controls[i]->maxY))
				return i;
		} // end else
	} // end of for

	return -1;
} // end of method getInput(int, int)

// makes ButtonPos structs to match the input control locations on the screen
ButtonPos* InputControls::makeButton(int bValue)
{
	ButtonPos *tempBP = new ButtonPos();
	switch (bValue) {

	case 0: // EJECT
		tempBP->minX = 367;
		tempBP->minY = 133;
		tempBP->maxX = 454;
		tempBP->maxY = 179;
		break;
	case 1: // POWER
		tempBP->minX = 464;
		tempBP->minY = 133;
		tempBP->maxX = 551;
		tempBP->maxY = 179;
		break;
	case 2: // UP
		tempBP->minX = 86;
		tempBP->minY = 91;
		tempBP->maxX = 122;
		tempBP->maxY = 137;
		break;
	case 3: // DOWN
		tempBP->minX = 86;
		tempBP->minY = 171;
		tempBP->maxX = 122;
		tempBP->maxY = 219;
		break;
	case 4: // LEFT
		tempBP->minX = 39;
		tempBP->minY = 137;
		tempBP->maxX = 86;
		tempBP->maxY = 171;
		break;
	case 5: // RIGHT
		tempBP->minX = 122;
		tempBP->minY = 137;
		tempBP->maxX = 170;
		tempBP->maxY = 171;
		break;
	case 6: // START
		tempBP->minX = 215;
		tempBP->minY = 92;
		tempBP->maxX = 255;
		tempBP->maxY = 130;
		break;
	case 7: // SELECT
		tempBP->minX = 376;
		tempBP->minY = 92;
		tempBP->maxX = 416;
		tempBP->maxY = 130;
		break;
	case 8: // A
		tempBP->minX = 495;
		tempBP->minY = 176;
		tempBP->maxX = 549;
		tempBP->maxY = 224;
		break;
	case 9: // B
		tempBP->minX = 544;
		tempBP->minY = 133;
		tempBP->maxX = 598;
		tempBP->maxY = 181;
		break;
	case 10: // X
		tempBP->minX = 448;
		tempBP->minY = 122;
		tempBP->maxX = 502;
		tempBP->maxY = 170;
		break;
	case 11: // Y
		tempBP->minX = 495;
		tempBP->minY = 80;
		tempBP->maxX = 549;
		tempBP->maxY = 128;
		break;
	case 12: // GAME
		tempBP->minX = 60;
		tempBP->minY = 35;
		tempBP->maxX = 271;
		tempBP->maxY = 315;
		break;
	case 13: // MOVIE
		tempBP->minX = 358;
		tempBP->minY = 35;
		tempBP->maxX = 569;
		tempBP->maxY = 319;
		break;
	default:
		break;
	}

	return tempBP;
} // end of method makeButton(int)

// inserts or removes disc from console
void InputControls::insertDisc(int discType)
{
	if (discType <= 0)
		return;

	if (powerStatus && trayOpen)
	{
		/*if (discInsert)
		{*/
			if (this->discType == discType)
			{
				this->discType = 0;
				discInsert = false;
			} // end if
			else
			{
				this->discType = discType;
				discInsert = true;
			}
		//} // end if
		//else
		//{
		//	this->discType = discType;
		//	discInsert = true;
		//} // end else
	} // end if
} // end of method insertDisc(int)


// reverses the status of console power
void InputControls::powerControl()
{
	powerStatus = !powerStatus;
	trayOpen = false;
}

// reverses the status of the console tray (if powered on)
void InputControls::trayControl()
{
	powerStatus ? trayOpen = !trayOpen : trayOpen = false;
} 

bool InputControls::getTrayOpen() { return trayOpen; } // returns true if the console disc tray is open

bool InputControls::getPowerStatus() { return powerStatus; }  // returns true if system powered on

bool InputControls::getDiscInsert() { return discInsert; }  // returns true if there is a disc loaded on the console

int InputControls::getDiscType() { return discType; } // returns the type of disc loaded into the console

std::string InputControls::getLastCtrl() { return controlNames[lastCtrl]; } // returns the string representation of the last input control used


int InputControls::getY(int mx, int my)
{
	int bmpHeight = al_get_bitmap_height(discBoth);
	// check mouse position against button positions
	for (int i = 0; i < controlNames.size(); ++i)
	{
		if (i <= 1)
		{
			if ((mx >= controls[i]->minX + OFFSETX + BORDER && mx <= controls[i]->maxX + OFFSETX + BORDER) &&
				(my >= controls[i]->minY + bmpHeight + BORDER && my <= controls[i]->maxY + bmpHeight + BORDER))
				return controls[i]->minY + bmpHeight + BORDER;
		}
		else if (i <= 11)
		{
			if ((mx >= controls[i]->minX + OFFSETX + BORDER && mx <= controls[i]->maxX + OFFSETX + BORDER) &&
				(my >= controls[i]->minY + bmpHeight * 2 + BORDER * 2 && my <= controls[i]->maxY + bmpHeight * 2 + BORDER * 2))
				return controls[i]->minY + bmpHeight * 2 + BORDER * 2;
		}
		else
		{
			if ((mx >= controls[i]->minX + OFFSETX + BORDER && mx <= controls[i]->maxX + OFFSETX + BORDER) && (my >= controls[i]->minY && my <= controls[i]->maxY))
				return controls[i]->minY;
		}

	} // end of for

	return -1;
}

int InputControls::getH()
{
	return al_get_bitmap_height(discBoth);
}