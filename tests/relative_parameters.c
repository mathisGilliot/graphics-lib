#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"


/*
 * ei_main --
 *
 *	Main function of the application.
 */
int ei_main(int argc, char** argv)
{
	ei_size_t	screen_size		= {600, 600};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	ei_widget_t*	frame;
	int		frame_x			= -15;
	int		frame_y			= -15;
	ei_color_t	frame_color		= {0xff, 0x00, 0x00, 0xff};
	ei_anchor_t     frame_anchor            = ei_anc_southeast;
	float           relative_frame_width    = 0.5;
	float           relative_frame_height   = 0.5;
	float           relative_frame_x        = 0.95;
	float           relative_frame_y        = 0.95;

	/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	/* Create, configure and place the frame on screen. */
	frame = ei_widget_create("frame", ei_app_root_widget());
	ei_frame_configure(frame, NULL, &frame_color,
			   NULL, NULL, NULL, NULL, NULL, NULL,
			   NULL, NULL, NULL);
	ei_place(frame, &frame_anchor, &frame_x, &frame_y, NULL, NULL, 
		 &relative_frame_x, &relative_frame_y, &relative_frame_width, 
		 &relative_frame_height);


        /* Create, configure and place a son for 'frame' on screen. */
	ei_widget_t*	son;
	ei_color_t	son_color	      = {0x00, 0xff, 0x00, 0xff};
	ei_anchor_t     son_anchor            = ei_anc_center;
	float           relative_son_width    = 0.5;
	float           relative_son_height   = 0.5;
	float           relative_son_x        = 0.5;
	float           relative_son_y        = 0.5;
	son = ei_widget_create("frame", frame);
	ei_frame_configure(son, NULL, &son_color,
			   NULL, NULL, NULL, NULL, NULL, NULL,
			   NULL, NULL, NULL);
	ei_place(son, &son_anchor, NULL, NULL, NULL, NULL, 
		 &relative_son_x, &relative_son_y, &relative_son_width, 
		 &relative_son_height);


        /* Create, configure and place a button on the screen. */
	ei_widget_t*	button;
	ei_size_t	button_size		= {150,50};
	int		button_x		= 0;
	ei_color_t	button_color		= {0x88, 0x88, 0x88, 0xff};
	char*		button_title		= "Plus court";
	ei_color_t	button_text_color	= {0x00, 0x00, 0x00, 0xff};
	int		button_corner_radius	= 10;
	ei_relief_t	button_relief		= ei_relief_raised;
	int		button_border_width	= 6;
	float           relative_button_x        = 1.0;
	float           relative_button_y        = 0.05;
	float           relative_button_width    = 0.8;
	ei_anchor_t     button_anchor            = ei_anc_northeast;

	button = ei_widget_create("button", ei_app_root_widget());
	ei_button_configure(button, &button_size, &button_color, &button_border_width, 
			    &button_corner_radius, &button_relief, &button_title, 
			    NULL, &button_text_color, NULL, NULL, NULL, NULL, 
			    NULL, NULL);
	ei_place(button, &button_anchor, &button_x, NULL, NULL, NULL, &relative_button_x, &relative_button_y, &relative_button_width, NULL );
	

	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}
