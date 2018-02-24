#include "screens.h"
//#include "consoleui.h"
//#include <allegro5\allegro.h>
//#include <allegro5\allegro_image.h>
//#include <allegro5\allegro_primitives.h>
//#include <allegro5\allegro_font.h>
//#include <allegro5\allegro_ttf.h>
//#include <iostream>
//#include <fstream>
//#include <string>

#define UI_W 1280
#define UI_H 720
#define CLEAR al_map_rgba(0, 0, 0, 0)
#define YELLOW al_map_rgb(255, 255, 0)
#define BLACK al_map_rgb(0, 0, 0)
#define RED al_map_rgb(255, 0, 0)

MenuPos::MenuPos()
{
	minX = -1;
	minY = -1;
	maxX = -1;
	maxY = -1;
}

// ABC constructor
ScreenUI::ScreenUI(const char* imageFilename, std::string menuType, int menuSize)
{
	uiImage = al_load_bitmap(imageFilename);
	fullUI = al_create_bitmap(UI_W, UI_H);
	currPos = 0;
	maxPos = menuSize;
	changeType = "NONE";
	nextTypeA = "DEFAULT";
	nextTypeB = "DEFAULT";
	setMenuNames();
	screen = menuType;
	makeHilitePositions(menuType);
} // end of ABC constructor

// ABC destructor
ScreenUI::~ScreenUI()
{
	int count = hilites.size() - 1;
	while (count >= 0)
	{
		delete hilites[count--];
	}
	al_destroy_bitmap(uiImage);
	al_destroy_bitmap(fullUI);
} // end of ABC destructor

// returns a bitmap of the UI with the appropriate item highlighted
ALLEGRO_BITMAP* ScreenUI::getBitmap()
{
	al_set_target_bitmap(fullUI);
	al_draw_bitmap(uiImage, 0, 0, 0);
	if (maxPos > 0)
		al_draw_rectangle(hilites[currPos]->minX, hilites[currPos]->minY, hilites[currPos]->maxX, hilites[currPos]->maxY, YELLOW, 3);
	return fullUI;
}

// creates a series of hilight positions for the menu
void ScreenUI::makeHilitePositions(std::string menuType)
{
	//open file
	std::ifstream posFile;
	posFile.open("images/screen_highlights.txt");
	
	// read to proper section
	if (posFile.is_open())
	{
		std::string line;
		while (std::getline(posFile, line))
		{
			if (line == menuType)
				break;
		}
	}

	// read ints to struct and store in vector
	for (int i = 0; i < maxPos; i++)
	{
		MenuPos *temp = new MenuPos();
		posFile >> temp->minX;
		posFile >> temp->minY;
		posFile >> temp->maxX;
		posFile >> temp->maxY;
		hilites.push_back(temp);
	}
	//close file
	posFile.close();
}

int ScreenUI::getMinX()
{
	return hilites[currPos]->minX;
}
int ScreenUI::getMaxX()
{
	return hilites[currPos]->maxX;
}
int ScreenUI::getMinY()
{
	return hilites[currPos]->minY;
}
int ScreenUI::getMaxY()
{
	return hilites[currPos]->maxY;
}

/* Definitions for MainScreen class */

// sets the names of the menu options for the main screen
void MainScreen::setMenuNames()
{
	menuNames.push_back("PLAY DISC");
	menuNames.push_back("ON DEVICE");
	menuNames.push_back("STORE");
	menuNames.push_back("APPS");
	menuNames.push_back("SETTINGS");
} // end of method setMenuNames()

// overridden method processes the input passed from the user
bool MainScreen::processInput(int inputValue)
{
	switch (inputValue)
	{
		case 2:  // Up
			if (currPos <= 1)
				currPos = 2;
			else if (currPos == 3)
				currPos = 1;
			else
				--currPos;
			changeType = "MOVE MENU SELECTION";
			return true;
		case 3:  // Down
			if (currPos <= 1)
				currPos = 2;
			else if (currPos == 2 || currPos == 4)
				currPos = 1;
			else
				++currPos;
			changeType = "MOVE MENU SELECTION";
			return true;
		case 4: case 5: // Left, Right
			if (currPos <= 1)
				currPos == 0 ? currPos = 1 : currPos = 0;
			else if (currPos >= 3)
				currPos = 2;
			else
				currPos = 3;
			changeType = "MOVE MENU SELECTION";
			return true;
		case 8: // A button - will progress to a new screen
			switch (currPos)
			{
				case 0:
					changeType = "PLAY DISC";
					break;
				case 1:
					changeType = "SELECT SCREEN";
					nextTypeA = "DEVICE";
					break;
				case 2:
					changeType = "SELECT SCREEN";
					nextTypeA = "STORE";
					break;
				case 3:
					changeType = "LISTING SCREEN";
					nextTypeA = "DEVICE";
					nextTypeB = "APPS";
					break;
				case 4:
					changeType = "NONE";
					break;
			}
			return true;
		case 11: // Y button
			changeType = "CHANGE TRAY";
			return true;
		default:
			return false;
	} // end of switch
} // end of method processInput(int)


/*Definitions for MediaSelectScreen class*/

// sets the names of the menu options for the console screen
void MediaSelectScreen::setMenuNames()
{
	menuNames.push_back("GAMES");
	menuNames.push_back("MOVIES");
	menuNames.push_back("MUSIC");
	menuNames.push_back("TELEVISION");
} // end of method setMenuNames()

// processes the input passed from the user
bool MediaSelectScreen::processInput(int inputValue)
{
	nextTypeA = screenTypeA;

	switch (inputValue)
	{
	case 2:  // Up
		if (currPos > 0)
		{
			currPos == 1 ? currPos = maxPos - 1 : --currPos;
			changeType = "MOVE MENU SELECTION";
			return true;
		}
		else
			return false;
	case 3:  // Down
		if (currPos > 0)
		{
			currPos == maxPos - 1 ? currPos = 1 : ++currPos;
			changeType = "MOVE MENU SELECTION";
			return true;
		}
		else
			return false;
	case 4: case 5:  // Left, Right
		currPos > 0 ? currPos = 0 : currPos = 1;
		changeType = "MOVE MENU SELECTION";
		return true;
	case 8: // A button - will progress to a new screen
		changeType = "LISTING SCREEN";
		switch (currPos)
		{
		case 0:
			nextTypeB = "GAME";
			break;
		case 1:
			nextTypeB = "MOVIE";
			break;
		case 2:
			nextTypeB = "MUSIC";
			break;
		case 3:
			nextTypeB = "TV";
			break;
		}
		return true;
	default:
		return false;
	} // end of switch
} // end of method processInput(int)


/* Definitions for ListingsScreen class */

// constructor
ListingsScreen::ListingsScreen(std::string typeA, std::string typeB) : ScreenUI("images/ListingsScreen.jpg", "LISTING", 10)
{ 
	screenTypeA = typeA;
	screenTypeB = typeB;

	if (screenTypeB == "GAME")
		icon = al_load_bitmap("images/game_icon.png");
	else if (screenTypeB == "MOVIE")
		icon = al_load_bitmap("images/movie_icon.png");
	else if (screenTypeB == "MUSIC")
		icon = al_load_bitmap("images/music_icon.png");
	else if (screenTypeB == "TV")
		icon = al_load_bitmap("images/tv_icon.png");
	else if (screenTypeB == "APPS")
		icon = al_load_bitmap("images/apps_icon.png");
	setMenuNames();
	setMenuItems();
} // end of constructor

// sets the names of the menu options for the console screen
void ListingsScreen::setMenuNames()
{
	menuNames.push_back("SEARCH");

	for (int i = 1; i < maxPos; i++)
	{
		menuNames.push_back(screenTypeB + " " + intNumToString(i));
	} // end for
} // end of method setMenuNames()

// processes the input passed from the user
bool ListingsScreen::processInput(int inputValue)
{
	changeType = "NONE";
	nextTypeA = screenTypeA;
	nextTypeB = screenTypeB;

	switch (inputValue)
	{
	case 2:  // Up
		if (currPos > 0)
			currPos >= rowSize ? currPos -= rowSize : currPos = 0;
		else
			currPos = maxPos - 1;
		changeType = "MOVE MENU SELECTION";
		return true;
	case 3:  // Down
		if (currPos > 0)
			currPos <= maxPos - 1 - rowSize ? currPos += rowSize : currPos = 0;
		else
			currPos++;
		changeType = "MOVE MENU SELECTION";
		return true;
	case 4:  // Left
		if (currPos > 0)
		{
			currPos % rowSize == 1 ? currPos += rowSize - 1 : --currPos;
			changeType = "MOVE MENU SELECTION";
			return true;
		}
		else
			return false;
	case 5:  // Right
		if (currPos > 0)
		{
			currPos % rowSize == 0 ? currPos -= rowSize - 1 : ++currPos;
			changeType = "MOVE MENU SELECTION";
			return true;
		}
		else
			return false;
	case 8: // A button
		switch (currPos)
		{
		case 0: // search box
			changeType = "KYBD";
			break;
		default: // progress to new screen
			if (screenTypeA == "CONSOLE" || screenTypeA == "DEVICE")
			{
				if (screenTypeB == "GAME")
					changeType = "GAME SCREEN";
				else if (screenTypeB == "APPS")
					changeType = "NETFLIX";
				else
					changeType = "MEDIA SCREEN";
			}
			else
				changeType = "PURCHASE SCREEN";
			break;
		}
		return true;
	case 10: // X Button - change listings page
		listPage > 0 ? --listPage : listPage = 5;
		searchTerm = "Search";
		setMenuItems();
		changeType = "SCROLL LISTINGS";
		return true;
	case 11: // Y Button - change listings page
		listPage < 5 ? ++listPage : listPage = 0;
		searchTerm = "Search";
		setMenuItems();
		changeType = "SCROLL LISTINGS";
		return true;
	default:
		return false;
	} // end of switch
} // end of method processInput(int)

// set the numbers for the listing items on screen
void ListingsScreen::setMenuItems()
{
	std::string itemNum;
	int menuNum = 1;

	for (int i = 1; i < menuNames.size(); i++)
	{
		itemNum = screenTypeB + " " + intNumToString((listPage * rowSize * colSize) + i);
		menuNames[i] = itemNum;
	}
	if (searchTerm != "Search")
	{
		itemNum = "";
		for (int i = 0; i < searchTerm.length(); i++)
		{
			if (i >= 7)
			{
				itemNum.append("...");
				i = searchTerm.length();
				break;
			}
			else
				itemNum.append(searchTerm.substr(i, 1));;
		} // end for
		menuNames[1] = itemNum;		
	} // end if
} // end of method setMenuItems();

// return a bitmap of the complete UI image
ALLEGRO_BITMAP* ListingsScreen::getBitmap()
{
	al_set_target_bitmap(fullUI);

	al_draw_bitmap(uiImage, 0, 0, 0);  // draw background
	al_draw_bitmap(icon, iconX, iconY, 0);  // draw associated icon
	al_draw_text(font, BLACK, searchX, searchY, ALLEGRO_ALIGN_LEFT, searchTerm.c_str());  // draw search term in box
	al_draw_text(font, BLACK, infoX, infoY, ALLEGRO_ALIGN_LEFT, menuNames[currPos].c_str());  // draw info title based on current selection
	
	// draw individual listing item titles
	setMenuItems();
	int menuNum = 1;
	for (int i = 0; i < colSize; i++)
	{
		for (int j = 0; j < rowSize; j++)
		{
			al_draw_text(font, BLACK, (textX + textXShift * j), (textY + textYShift *i), ALLEGRO_ALIGN_CENTER, menuNames[menuNum].c_str());
			menuNum++;
		} // end for
	} // end for

	// highlight current selection
	al_draw_rectangle(hilites[currPos]->minX, hilites[currPos]->minY, hilites[currPos]->maxX, hilites[currPos]->maxY, YELLOW, 3);
	
	return fullUI;
} // end of method getBitmap()


/* Definitions for MediaPlayScreen class */

// sets the names of the menu options for the media player screen
void MediaPlayScreen::setMenuNames()
{
	menuNames.push_back("PREV");
	menuNames.push_back("REW");
	menuNames.push_back("STOP");
	menuNames.push_back("PAUSE");
	menuNames.push_back("PLAY");
	menuNames.push_back("ADV");
	menuNames.push_back("NEXT");
} // end of method setMenuNames()

// processes the input passed from the user
bool MediaPlayScreen::processInput(int inputValue)
{
	changeType = "NONE";

	switch (inputValue)
	{
	case 4:  // Left
		currPos > 0 ? --currPos : currPos = maxPos - 1;
		changeType = "MOVE MENU SELECTION";
		return true;
	case 5:  // Right
		currPos < maxPos - 1 ? ++currPos : currPos = 0;
		changeType = "MOVE MENU SELECTION";
		return true;
	default:
		return false;
	} // end of switch
} // end of method processInput(int)


/* Definitions for GamePlaySCreen class */

// sets the names of the menu options for the console screen
void GamePlayScreen::setMenuNames()
{
	menuNames.push_back("PLAY");
	menuNames.push_back("LOAD");
	menuNames.push_back("OPTIONS");
	menuNames.push_back("QUIT");
	currPos = 4;
} // end of method setMenuNames()

  // processes the input passed from the user
bool GamePlayScreen::processInput(int inputValue)
{
	changeType = "NONE";

	switch (inputValue)
	{
	case 2:  // Up
		currPos > 0 ? --currPos : currPos = maxPos - 1;
		changeType = "MOVE MENU SELECTION";
		return true;
	case 3:  // Down
		currPos < maxPos - 1 ? ++currPos : currPos = 0;
		changeType = "MOVE MENU SELECTION";
		return true;
	case 8: // A button - only works to quit
		if (currPos == maxPos - 1)
		{
			changeType = "MAIN SCREEN";
			return true;
		}
		else
			return false;
	default:
		return false;
	} // end of switch
} // end of method processInput(int)


/* Definitions for PurchaseInfoScreen class */

// constructor
PurchaseInfoScreen::PurchaseInfoScreen(std::string typeA, std::string typeB) : ScreenUI("images/PrePurchaseInfoScreen.jpg", "PURCHASE", 1)
{
	screenTypeA = typeA;
	screenTypeB = typeB;
	shot1 = al_load_bitmap("images/screenshot1.jpg");
	shot2 = al_load_bitmap("images/screenshot2.jpg");
	shot3 = al_load_bitmap("images/screenshot3.jpg");

	if (screenTypeB == "GAME")
		icon = al_load_bitmap("images/game_icon.png");
	else if (screenTypeB == "MOVIE")
		icon = al_load_bitmap("images/movie_icon.png");
	else if (screenTypeB == "MUSIC")
		icon = al_load_bitmap("images/music_icon.png");
	else if (screenTypeB == "TV")
		icon = al_load_bitmap("images/tv_icon.png");
	else if (screenTypeB == "APPS")
		icon = al_load_bitmap("images/apps_icon.png");
	setMenuNames();
} // end of constructor

// sets the names of the menu options for the purchase info screen
void PurchaseInfoScreen::setMenuNames()
{
	menuNames.push_back("PURCHASE");

} // end of method setMenuNames()

// processes the input passed from the user
bool PurchaseInfoScreen::processInput(int inputValue)
{
	nextTypeA = screenTypeA;
	nextTypeB = screenTypeB;
	switch (inputValue)
	{
	case 2:  // Up
		// scroll listing up
		changeType = "SCROLL INFO";
		return true;
	case 3:  // Down
		// scroll listing down
		changeType = "SCROLL INFO";
		return true;
	case 4:  // Left - show prev screenshot
		currShot > 1 ? --currShot : currShot = 3;
		changeType = "SCROLL SHOTS";
		return true;
	case 5:  // Right - show next screenshot
		currShot < 3 ? ++currShot : currShot = 1;
		changeType = "SCROLL SHOTS";
		return true;
	case 8: // A button
		changeType = "DETAILS SCREEN";
		return true;
	default:
		return false;
	} // end of switch
} // end of method processInput(int)

ALLEGRO_BITMAP* PurchaseInfoScreen::getBitmap()
{
	al_set_target_bitmap(fullUI);
	al_draw_bitmap(uiImage, 0, 0, 0); // draw background
	
	// draw screenshot
	switch (currShot)
	{
		case 1:
			al_draw_bitmap(shot1, shotX, shotY, 0);
			break;
		case 2:
			al_draw_bitmap(shot2, shotX, shotY, 0);
			break;
		case 3:
			al_draw_bitmap(shot3, shotX, shotY, 0);
			break;
	}

	al_draw_bitmap(icon, iconX, iconY, 0); // draw icon
	al_draw_rectangle(hilites[currPos]->minX, hilites[currPos]->minY, hilites[currPos]->maxX, hilites[currPos]->maxY, YELLOW, 3); // draw menu highlight

	return fullUI;
}


/* Definitions for ConfirmDetailsScreen class */

// constructor
ConfirmDetailsScreen::ConfirmDetailsScreen(std::string typeA, std::string typeB) : ScreenUI("images/CustomerDetailsScreen.jpg", "DETAILS", 15)
{
	screenTypeA = typeA;
	screenTypeB = typeB;
	for (int i = 0; i < 11; i++)
	{
		detailItems[i] = "";
	}
	setMenuNames();
} // end of constructor

// sets the names of the menu options for the console screen
void ConfirmDetailsScreen::setMenuNames()
{
	menuNames.push_back("FIRST");
	menuNames.push_back("LAST");
	menuNames.push_back("ADDRESS");
	menuNames.push_back("ADDRESS 2");
	menuNames.push_back("CITY");
	menuNames.push_back("PROVINCE");
	menuNames.push_back("POSTCODE");
	menuNames.push_back("COUNTRY");
	menuNames.push_back("CARD #");
	menuNames.push_back("CARD 1");
	menuNames.push_back("CARD 2");
	menuNames.push_back("CARD 3");
	menuNames.push_back("EXP MO");
	menuNames.push_back("EXP YR");
	menuNames.push_back("PURCHASE");
} // end of method setMenuNames()

// processes the input passed from the user
bool ConfirmDetailsScreen::processInput(int inputValue)
{
	nextTypeA = screenTypeA;
	nextTypeB = screenTypeB;
	changeType = "NONE";

	switch (inputValue)
	{
	case 2:  // Up
		switch (currPos)
		{
			case 0: case 1:
				currPos = 9;
				break;
			case 2: case 5: case 6: case 7: case 8: case 10:
				currPos -= 2;
				break;
			case 3: case 4: case 9:
				--currPos;
				break;
			case 11: case 12: case 13:
				currPos = 7;			
				break;
			case 14:
				currPos = maxPos - 1;
				break;
		}
		return true;
	case 3:  // Down
		switch (currPos)
		{
		case 0: case 1:
			currPos = 2;
			break;
		case 2: case 3: case 7: case 8:
			++currPos;
			break;
		case 4: case 5: case 6:
			currPos += 2;
			break;
		case 9: case 10: case 11: case 12: case 13:
			currPos = 0;
			break;
		case 14:
			currPos = maxPos - 1;
			break;
		}
		return true;
	case 4:  // Left
		switch (currPos)
		{
		case 0: case 2: case 3: case 4: case 6: case 8: case 9:
			currPos = maxPos - 1;
			break;
		case 1: case 5: case 7: case 10: case 12: case 13:
			--currPos;
			break;
		case 11:
			currPos = 8;
			break;
		case 14:
			currPos = 0;
			break;
		}
		return true;
	case 5:  // Right
		switch (currPos)
		{
		case 0: case 4: case 6: case 9: case 10: case 11: case 12:
			++currPos;
			break;
		case 1: case 2: case 3: case 5: case 7: case 13:
			currPos = maxPos - 1;
			break;
		case 8:
			currPos = 11;
			break;
		case 14:
			currPos = 0;
			break;
		}
		return true;
	case 8: // A button
		
		switch (currPos)
		{
			case 11: case 12: case 13:  // credit card options
				cardSelect = currPos;
				changeType = "MOVE MENU SELECTION";
				return true;
			case 14: // purchase button
				changeType = "CONFRIM PURCHASE";
				for (int i = 0; i < 11; i++)
				{
					if (detailItems[i] == "" && i != 3)
					{
						changeType = "DETAILS MISSING";
						i = 11;
						break;
					}
				}
				//missing.clear();
				//// check fields for entries
				//for (int i = 0; i < 11; i++)
				//{
				//	if (i == 2 && detailItems[2].empty() && !detailItems[3].empty())
				//	{
				//		detailItems[i] = detailItems[i + 1];  // transfer details of address 2 to address 1
				//		detailItems[i + 1].clear();
				//	}
				//	if (i = 3)
				//		break;  // do not need to check address line 2
				//	if (detailItems[i].empty())
				//	{
				//		changeType = "DETAILS MISSING";
				//		return true;
				//		if (missing.empty())
				//			i < 9 ? missing.append(menuNames[i]) : missing.append("\n" + menuNames[i + 3]); // add missing items to present later
				//	} // end if
				//} // end for
				//// display confirm
				//if (missing.empty())
				//{
				//	changeType = "CONFIRM PURCHASE";
				//	return true;
				//}
				//changeType = "BACK";
				return true;
			default:
				changeType = "KYBD";
				return true;
		}
	default:
		return false;
	} // end of switch
} // end of method processInput(int)

ALLEGRO_BITMAP* ConfirmDetailsScreen::getBitmap()
{
	al_set_target_bitmap(fullUI);
	al_draw_bitmap(uiImage, 0, 0, 0);
	al_draw_rectangle(hilites[currPos]->minX, hilites[currPos]->minY, hilites[currPos]->maxX, hilites[currPos]->maxY, YELLOW, 3);
	// print text
	for (int i = 0; i < 11; i++)
	{
		al_draw_text(font, BLACK, hilites[i]->minX + textOffsetX, hilites[i]->minY + textOffsetY, ALLEGRO_ALIGN_LEFT, detailItems[i].c_str());
	}
	// show card option selected
	al_draw_filled_circle(circleX + (cardSelect - 11) * circleOffsetX, circleY, circleR, BLACK);

	return fullUI;
}


/* Definitions for KeyboardInput class */

// constructor
KeyboardInput::KeyboardInput() : ScreenUI ("images/Keyboard.jpg", "KYBD", 5)
{
	screenTypeA = "DEFAULT";
	screenTypeB = "DEFAULT";
	nextTypeA = "DEFAULT";
	nextTypeB = "DEFAULT";
	resultText.clear();
	setMenuNames();
} // end of constructor

// sets the first key positions for the keyboard rows
void KeyboardInput::setMenuNames()
{
	menuNames.push_back("ROW 1");
	menuNames.push_back("ROW 2");
	menuNames.push_back("ROW 3");
	menuNames.push_back("ROW 4");
	menuNames.push_back("SPACE");

	maxPos = 42; // set for number of keys, not number of rows
} // end of method setMenuNames()

// processes the input passed from the user
bool KeyboardInput::processInput(int inputValue)
{
	std::string keyValue = getKey();
	switch (inputValue)
	{
		case 2: // UP
			if (currPos < 10)
				currPos = maxPos - 1;
			else if (currPos == maxPos - 1)
				currPos = 35; // to B key
			else if (currPos >= 20 && currPos <= 29)
				currPos -= 11;
			/*else if (currPos >= 31 && currPos <= 40)
				currPos -= 10;*/
			else
				currPos -= 10;
			changeType = "MOVE MENU SELECTION";
			return true;
		case 3: // DOWN
			if (currPos == maxPos -1)
				currPos = 4; // to 5 key
			else if (currPos >= 31)
				currPos = maxPos - 1;
			else if (currPos >= 10 && currPos <= 19)
				currPos += 11;
			/*else if (currPos >= 10 && currPos <= 19)
				currPos += 12;*/
			else
				currPos += 10;
			changeType = "MOVE MENU SELECTION";
			return true;
		case 4: // LEFT
			if (currPos == maxPos - 1)
				currPos = maxPos - 1;
			else if (currPos == 0 || currPos == 21 || currPos == 31)
				currPos +=9;
			else if (currPos == 10)
				currPos += 10;
			else
				currPos--;
			changeType = "MOVE MENU SELECTION";
			return true;
		case 5: // RIGHT
			if (currPos == maxPos - 1)
				currPos = maxPos - 1;
			else if (currPos == 9 || currPos == 30 || currPos == 40)
				currPos -= 9;
			else if (currPos == 20)
				currPos -= 10;
			else
				currPos++;
			changeType = "MOVE MENU SELECTION";
			return true;
		case 8: // A button
			// get key value entered
			if (keyValue == "bkspc")
			{
				if (resultText.length() > 0)
					resultText.pop_back();
				changeType = "ADD TEXT";
			}
			else if (keyValue == "enter")
				changeType = "TEXT ENTER";
			else if (keyValue == "cxl")
				changeType = "BACK";
			else
			{
				if (resultText.length() <= 30)
					resultText.append(keyValue);
				changeType = "ADD TEXT";
			}
			//changeType = "MOVE MENU SELECTION";
			return true;
		case 9: // B button
			changeType = "BACK";
			return true;
		case 10: // X button
			if (resultText.length() > 0)
				resultText.pop_back();
			changeType = "ADD TEXT";
			return true;
		case 11: // Y button
			changeType = "TEXT ENTER";
			return true;
		default:
			return false;
	} // end switch
} // end of method processInput(int)

// return a full bitmap of all screen elements
ALLEGRO_BITMAP* KeyboardInput::getBitmap()
{
	al_set_target_bitmap(fullUI);
	al_draw_bitmap(uiImage, 0, 0, 0);
	al_draw_text(font, BLACK, inputX, inputY, ALLEGRO_ALIGN_LEFT, resultText.c_str());
	
	// calculate highlight positions
	if (currPos == 20)
		al_draw_rectangle((hilites[1]->minX + (currPos - 10) * keyShiftX), hilites[1]->minY, bkspcMaxX, hilites[1]->maxY, YELLOW, 3);
	else if (currPos == 30)
		al_draw_rectangle((hilites[2]->minX + (currPos - 21) * keyShiftX), hilites[2]->minY, enterMaxX, hilites[2]->maxY, YELLOW, 3);
	else if (currPos == 40)
		al_draw_rectangle((hilites[3]->minX + (currPos - 31) * keyShiftX), hilites[3]->minY, cxlMaxX, hilites[3]->maxY, YELLOW, 3);
	else if (currPos == maxPos - 1)
		al_draw_rectangle(hilites[4]->minX, hilites[4]->minY, hilites[4]->maxX, hilites[4]->maxY, YELLOW, 3);
	else if (currPos >=0 && currPos <= 9)
		al_draw_rectangle(hilites[0]->minX + (currPos * keyShiftX), hilites[0]->minY, hilites[0]->maxX + (currPos * keyShiftX), hilites[0]->maxY, YELLOW, 3);
	else if (currPos >= 10 && currPos <= 19)
		al_draw_rectangle(hilites[1]->minX + ((currPos - 10) * keyShiftX), hilites[1]->minY, hilites[1]->maxX + ((currPos - 10) * keyShiftX), hilites[1]->maxY, YELLOW, 3);
	else if (currPos >= 21 && currPos <= 29)
		al_draw_rectangle(hilites[2]->minX + ((currPos - 21) * keyShiftX), hilites[2]->minY, hilites[2]->maxX + ((currPos - 21) * keyShiftX), hilites[2]->maxY, YELLOW, 3);
	else
		al_draw_rectangle(hilites[3]->minX + ((currPos - 31) * keyShiftX), hilites[3]->minY, hilites[3]->maxX + ((currPos - 31) * keyShiftX), hilites[3]->maxY, YELLOW, 3);
	
	return fullUI;
} // end of method getBitmap()

// returns a string based on the current value of the hilighted keyboard selection
std::string KeyboardInput::getKey()
{
	switch (currPos)
	{
	case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
		return intNumToString(currPos + 1);
	case 9:
		return "0";
	case 10:
		return "Q";
	case 11:
		return "W";
	case 12:
		return "E";
	case 13:
		return "R";
	case 14:
		return "T";
	case 15:
		return "Y";
	case 16:
		return "U";
	case 17:
		return "I";
	case 18:
		return "O";
	case 19:
		return "P";
	case 20:
		return "bkspc";
	case 21:
		return "A";
	case 22:
		return "S";
	case 23:
		return "D";
	case 24:
		return "F";
	case 25:
		return "G";
	case 26:
		return "H";
	case 27:
		return "J";
	case 28:
		return "K";
	case 29:
		return "L";
	case 30:
		return "enter";
	case 31:
		return "Z";
	case 32:
		return "X";
	case 33:
		return "C";
	case 34:
		return "V";
	case 35:
		return "B";
	case 36:
		return "N";
	case 37:
		return "M";
	case 38:
		return ",";
	case 39:
		return ".";
	case 40:
		return "cxl";
	case 41:
		return " ";
	default:
		return "?";
	} // end switch
} // end of method getKey()


/* Definitions for FinalConfirmScreen class */

// processes the input passed from the user
bool FinalConfirmScreen::processInput(int inputValue)
{
	if (inputValue == 8)
	{
		changeType = "SUCCESS SCREEN";
		return true;
	}
	else
		return false;
}


/* Definitions for SuccessScreen class */

// processes the input passed from the user
bool SuccessScreen::processInput(int inputValue)
{
	if (inputValue == 8)
	{
		changeType = "MAIN SCREEN";
		return true;
	}
	else
		return false;
}


/* Definitions for DetailsMissingScreen class */

// processes the input passed from the user
bool DetailsMissingScreen::processInput(int inputValue)
{
	if (inputValue == 8)
	{
		changeType = "BACK";
		return true;
	}
	else
		return false;
}


//turns a positive int into the equivalent string
// *Note - only works for up to 2-digit numbers
std::string intNumToString(int num)
{
	std::string result;
	if (num < 0 || num >= 100)
		result = "0";
	if (num < 10)
	{
		switch (num)
		{
		case 0:
			result = "0";
			break;
		case 1:
			result = "1";
			break;
		case 2:
			result = "2";
			break;
		case 3:
			result = "3";
			break;
		case 4:
			result = "4";
			break;
		case 5:
			result = "5";
			break;
		case 6:
			result = "6";
			break;
		case 7:
			result = "7";
			break;
		case 8:
			result = "8";
			break;
		case 9:
			result = "9";
			break;
		default:
			result = "0";
			break;
		} // end switch
	}
	else
	{
		result = intNumToString(num / 10);
		result.append(intNumToString(num % 10));
	} // end else
	return result;
} // end function intDigitToString(int)