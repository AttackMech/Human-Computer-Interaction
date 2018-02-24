#include "consoleui.h"
//#include "inputcontrols.h"
//#include "screens.h"
//#include <allegro5\allegro5.h>
//#include <allegro5\allegro_image.h>
//#include <vector>

#define SCREENX 1920
#define SCREENY 1080
#define UIX 1280
#define UIY 720
#define BORDER 3
#define GREY al_map_rgb(54, 54, 54)
#define RED al_map_rgb(255, 0, 0)
#define BLACK al_map_rgb(0, 0, 0)

// constructor
ConsoleUI::ConsoleUI(ALLEGRO_DISPLAY *display) {
	currScreen = new MainScreen();
	currType = 0;
	mouseX = 1284;
	mouseY = 784;
	userInput = -1;

	ui = al_create_bitmap(UIX, UIY);
	ctrl = al_create_bitmap(SCREENX - (UIX + BORDER), SCREENY);
	this->display = display;

	inputCtrl = new InputControls();
	currScreen = new MainScreen();
} // end of constructor

// destructor
ConsoleUI::~ConsoleUI()
{
	//al_destroy_bitmap(ui);
	//al_destroy_bitmap(ctrl);
	//al_destroy_display(display);

	while (prevScreen.size() > 0)
	{
		currScreen = prevScreen.back();
		delete currScreen;
		prevScreen.pop_back();
	}

	delete inputCtrl;
} // end of destructor

// starts the UI experience
int ConsoleUI::start()
{
	ALLEGRO_MOUSE_STATE mState;
	ALLEGRO_KEYBOARD_STATE kState;
	ALLEGRO_FONT *font = al_load_ttf_font("images/Tahoma.ttf", 24, 0);
	
	//ALLEGRO_EVENT_QUEUE *eventQ = al_create_event_queue();
	ALLEGRO_EVENT ev;
	
	//if (!eventQ)
	//{
	//	abort();
	//	return -1;
	//}

	//al_register_event_source(eventQ, al_get_keyboard_event_source());
	//al_register_event_source(eventQ, al_get_mouse_event_source());

	// main loop
	while (true)
	{
		// keep mouse in control range
		al_get_mouse_state(&mState);

		if (mState.x <= (UIX + BORDER))
		{
			//if (mState.y <= (UIY + BORDER))
				al_set_mouse_xy(display, mouseX, mState.y);
		}
		else if (mState.y <= (UIY + BORDER))
		{
			if (mState.x <= (UIX + BORDER))
				al_set_mouse_xy(display, mState.y, mouseY);
		}

		// check for input if user pushes button
		if (al_mouse_button_down(&mState, 1))
		{
			userInput = inputCtrl->getInput(mState.x, mState.y);
			while (true)
			{
				al_get_mouse_state(&mState);
				if (!al_mouse_button_down(&mState, 1))
					break;
			} // end while
		} // end if
		else
			userInput = -1;
		
		// process the user input if valid
		screenInfoProcessing = false;
		if (userInput >= 0)
		{
			switch (userInput)
			{
				case 0: // Eject button
					inputCtrl->trayControl();
					if ((currScreen->getScreen() == "GAME" || currScreen->getScreen() == "PLAYER") && prevScreen.back()->getScreen() == "MAIN")
					{
						delete currScreen;
						currScreen = prevScreen.back();
						prevScreen.pop_back();
					}
					break;
				case 1: // Power button
					inputCtrl->powerControl();
					if (!inputCtrl->getPowerStatus())
					{
						while (prevScreen.size() > 0)
						{
							currScreen = prevScreen.back();
							prevScreen.pop_back();
							delete currScreen;
							currScreen = new MainScreen();
						}
						break;
					}
					break;
				case 9: // B button
					if (prevScreen.size() > 0)
					{
						delete currScreen;
						currScreen = prevScreen.back();
						prevScreen.pop_back();
					}
					break;
				case 12: // Game disc
					inputCtrl->insertDisc(1);
					break;
				case 13: // Movie disc
					inputCtrl->insertDisc(2);
					break;
				case 2: case 3: case 4: case 5: case 8: case 10: case 11: // other buttons
					if (inputCtrl->getPowerStatus())
						screenInfoProcessing = currScreen->processInput(userInput);
					break;
				default:
					break;
			} // end switch
		} // end if

		// process any screen changes
		if (screenInfoProcessing)
		{
			// get change from screen class and convert to int
			std::string scrnChg = currScreen->getChanges();
			int change = 0;
			while (actions[change] != scrnChg)
			{
				change++;
			}

			// use change int to determine actions
			switch (change)
			{
				case 0: // new media select screen
					prevScreen.push_back(currScreen);
					currScreen = new MediaSelectScreen(currScreen->getNextTypeA(), currScreen->getNextTypeB());
					break;
				case 1: // new listings screen
					prevScreen.push_back(currScreen);
					currScreen = new ListingsScreen(currScreen->getNextTypeA(), currScreen->getNextTypeB());
					break;
				case 2: // new game play screen
					prevScreen.push_back(currScreen);
					currScreen = new GamePlayScreen(currScreen->getNextTypeA(), "GAME");
					break;
				case 3: // new media play screen
					prevScreen.push_back(currScreen);
					currScreen = new MediaPlayScreen(currScreen->getNextTypeA(), currScreen->getNextTypeB());
					break;
				case 4: // new purchase information screen
					prevScreen.push_back(currScreen);
					currScreen = new PurchaseInfoScreen(currScreen->getNextTypeA(), currScreen->getNextTypeB());
					break;
				case 5: // new confirm purchase details screen
					prevScreen.push_back(currScreen);
					currScreen = new ConfirmDetailsScreen(currScreen->getNextTypeA(), currScreen->getNextTypeB());
					break;
				case 6: // move back one screen
					if (prevScreen.size() > 0)
					{
						delete currScreen;
						currScreen = prevScreen.back();
						prevScreen.pop_back();
					}
					break;
				case 11: // change tray status on console
					inputCtrl->trayControl();
					break;
				case 12: case 13:// power off/on console
					inputCtrl->powerControl();
					break;
				case 14: // new keyboard input screen
					prevScreen.push_back(currScreen);
					currScreen = new KeyboardInput();
					break;
				case 16: // new details missing screen
					prevScreen.push_back(currScreen);
					currScreen = new DetailsMissingScreen();
					break;
				case 17: // new final confirm purchase screen
					prevScreen.push_back(currScreen);
					currScreen = new FinalConfirmScreen();
					break;
				case 18: // new success screen
					prevScreen.push_back(currScreen);
					currScreen = new SuccessScreen();
					break;
				case 19: // return to main screen
					while (prevScreen.size() > 0)
					{
						currScreen = prevScreen.back();
						prevScreen.pop_back();
						if (prevScreen.size() != 0)
							delete currScreen;
					}
					break;
				case 20: // deal with text entered from keyboard
					if (KeyboardInput *k = dynamic_cast<KeyboardInput*>(currScreen))
					{
						std::string result = k->getResult();
						if (ListingsScreen *ls = dynamic_cast<ListingsScreen*>(prevScreen.back()))
						{
							if (result == "")
								result = "Search";
							else
								ls->setText(result);
						}
						else if (ConfirmDetailsScreen *cds = dynamic_cast<ConfirmDetailsScreen*>(prevScreen.back()))
						{
							cds->setText(result);
						}
						else
						{
							break;
							throw "screen cast error";
						}
					}
					else
					{
						break;
						throw "keyboard cast error";
					}
					// prevscreen.back() typeof ListingsScreen ? below w/list : below w/conf_deets
					//prevScreen.back().setText((KeyboardInput)currScreen->getResult()); // fix!!get subclass method from parent pointer
					delete currScreen;
					currScreen = prevScreen.back();
					prevScreen.pop_back();
					break;
				case 21: // play the current disc in the console
					if (inputCtrl->getDiscType() == 1)
					{
						prevScreen.push_back(currScreen);
						currScreen = new GamePlayScreen("DEVICE", "GAME");
					}
					else if (inputCtrl->getDiscType() == 2)
					{
						prevScreen.push_back(currScreen);
						currScreen = new MediaPlayScreen("DEVICE", "MOVIE");
					}
					if (inputCtrl->getTrayOpen())
						inputCtrl->trayControl();
					break;
				case 22: // new Netflix Screen
					prevScreen.push_back(currScreen);
					currScreen = new NetflixAppScreen();
					break;
				default:
					break;
			} // end switch
		} // end if

		// draw complete ui to screen
		if (inputCtrl->getPowerStatus())
			ui = currScreen->getBitmap();
		else
		{
			al_set_target_bitmap(ui);
			al_clear_to_color(BLACK);
		}

		ctrl = inputCtrl->getBitmap(mState.x, mState.y);

		al_set_target_bitmap(al_get_backbuffer(display));

		al_draw_bitmap(ui, 0, 0, 0);
		al_draw_bitmap(ctrl, UIX + BORDER, 0, 0);

		al_draw_text(font, RED, 10, 800, 0, "Use controls on right to simulate manual input.");
		al_draw_text(font, RED, 10, 850, 0, "Press ESC to quit.");
		//al_draw_textf(font, al_map_rgb(255, 0, 0), 5, 730, 0, "POWER = %s, TRAY = %s", inputCtrl->getPowerStatus() ? "true": "false", inputCtrl->getTrayOpen() ? "true": "false");
		//al_draw_textf(font, al_map_rgb(255, 0, 0), 0, 770, 0, "User Input = %i", userInput);
		//al_draw_textf(font, al_map_rgb(255, 0, 0), 0, 800, 0, "minX: %i, minY: %i", currScreen->getMinX(), currScreen->getMinY());
		//al_draw_textf(font, al_map_rgb(255, 0, 0), 0, 820, 0, "maxX: %i, maxY: %i", currScreen->getMaxX(), currScreen->getMaxY());
		//al_draw_textf(font, al_map_rgb(0, 0, 255), 500, 820, 0, "size of prevScreen vector = %i", prevScreen.size());

		// swap screen and backbuffer
		al_flip_display();
		al_clear_to_color(GREY);


		//if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		//{
		//	if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
		//	{
		//		//al_destroy_bitmap(ui);
		//		al_destroy_event_queue(eventQ);
		//		break;
		//	}
		//}
		// check for exit key (esc)
		al_get_keyboard_state(&kState);
		if (al_key_down(&kState, ALLEGRO_KEY_ESCAPE))
		{
			//al_destroy_bitmap(ui);
			al_destroy_font(font);
			break;
		} // end if
	} // end while

	return 0;
} // end of method start()