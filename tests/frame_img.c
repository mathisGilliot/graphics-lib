#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_utils.h"


/*
 * process_key --
 *
 *	Callback called when any key is pressed by the user.
 *	Simply looks for the "Escape" key to request the application to quit.
 */
ei_bool_t process_key(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if (event->param.key.key_sym == SDLK_ESCAPE) {
		ei_app_quit_request();
		return EI_TRUE;
	}
	return EI_FALSE;
}

/*
 * ei_main --
 *
 *	Main function of the application.
 */
int ei_main(int argc, char** argv)
{
	ei_size_t	screen_size		= {600, 600};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	static char* g_ball_filename = "misc/flag.png";

	static ei_widget_t*		g_ball				= NULL;
	static ei_surface_t		g_ball_image			= NULL;
	static ei_size_t		g_ball_size			= { -1, -1 };
	static ei_anchor_t g_ball_anchor = ei_anc_center;


	/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	g_ball_image = hw_image_load(g_ball_filename, ei_app_root_surface()); 

	g_ball_size = hw_surface_get_size(g_ball_image);
	g_ball_size.width += 50;
	g_ball_size.height += 50;

	ei_rect_t* rect_img = malloc(sizeof(ei_rect_t));
	*rect_img = hw_surface_get_rect(g_ball_image);

	g_ball = ei_widget_create("frame", ei_app_root_widget());
	ei_frame_configure(g_ball, &g_ball_size, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &g_ball_image, &rect_img, &g_ball_anchor);
	ei_place(g_ball, NULL, NULL, NULL, &g_ball_size.width, &g_ball_size.height, NULL, NULL, NULL, NULL);

	/* Hook the keypress callback to the event. */
	ei_bind(ei_ev_keydown, NULL, "all", process_key, NULL);

	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_unbind(ei_ev_keydown, NULL, "all", process_key, NULL);
	ei_app_free();

	return (EXIT_SUCCESS);
}
