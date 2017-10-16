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
	ei_color_t	root_bgcol		= {0x0, 0x0, 0x0, 0xff};

	ei_widget_t*	bleu;
	ei_widget_t*	blanc;
	ei_widget_t*	rouge;
	ei_size_t	frame_size		= {100,150};
	int		bleu_x			= 150;
	int		bleu_y			= 200;
	int		blanc_x			= 250;
	int		blanc_y			= 200;
	int		rouge_x			= 350;
	int		rouge_y			= 200;
	ei_color_t	bleu_color		= {0x00, 0x00, 0xff, 0xff};
	ei_color_t	blanc_color		= {0xff, 0xff, 0xff, 0xff};
	ei_color_t	rouge_color		= {0xff, 0x00, 0x00, 0xff};
	int		frame_border_width	= 6;

	/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	/* Create, configure and place the frame on screen. */
	bleu = ei_widget_create("frame", ei_app_root_widget());
	blanc = ei_widget_create("frame", ei_app_root_widget());
	rouge = ei_widget_create("frame", ei_app_root_widget());
	
	ei_frame_configure(bleu, &frame_size, &bleu_color,
			    &frame_border_width, NULL, NULL, NULL, NULL, NULL,
			   NULL, NULL, NULL);
	ei_frame_configure(blanc, &frame_size, &blanc_color,
			    &frame_border_width, NULL, NULL, NULL, NULL, NULL,
			   NULL, NULL, NULL);
	ei_frame_configure(rouge, &frame_size, &rouge_color,
			    &frame_border_width, NULL, NULL, NULL, NULL, NULL,
			   NULL, NULL, NULL);
	
	ei_place(bleu, NULL, &bleu_x, &bleu_y, NULL, NULL, NULL, NULL, NULL, NULL );
	ei_place(blanc, NULL, &blanc_x, &blanc_y, NULL, NULL, NULL, NULL, NULL, NULL );
	ei_place(rouge, NULL, &rouge_x, &rouge_y, NULL, NULL, NULL, NULL, NULL, NULL );

	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}
