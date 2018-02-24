#include "consoleui.h"
#include <allegro5\allegro5.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_image.h>


int main(void)
{
	if (!al_init())
	{
		// couldn't start allegro
		al_show_native_message_box(NULL, "Error!", "Allegro has failed to initialize.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		abort();
		return -1;
	}

	if (!al_install_mouse())
	{
		//abort_example("Couldn't install mouse.\n");
		al_show_native_message_box(NULL, "Error!", "Unable to install mouse functions.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		abort();
		return -1;
	}

	if (!al_install_keyboard())
	{
		//abort_example("Couldn't install keyboard.\n");
		al_show_native_message_box(NULL, "Error!", "Unable to install keyboard functions.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		abort();
		return -1;
	}

	al_init_font_addon();
	al_init_image_addon();
	al_init_primitives_addon();

	//if (!al_init_font_addon() || !al_init_ttf_addon())
	//{
	//	//abort_example("Couldn't initialize fonts.\n");
	//	al_show_native_message_box(NULL, "Error!", "Unable to initialize fonts.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
	//	abort();
	//	return -1;
	//}
	
	if (!al_init_image_addon() || !al_init_ttf_addon())
	{
		//abort_example("Couldn't initialize images.\n");
		al_show_native_message_box(NULL, "Error!", "Unable to initalize images.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		abort();
		return -1;
	}

	//al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	ALLEGRO_DISPLAY *display = al_create_display(1920, 1080);

	ConsoleUI *cui = new ConsoleUI(display);
	if (cui->start() == -1)
	{
		abort();
		return -1;
	}
	
	delete cui;
	al_destroy_display(display);

	return 0;	
}