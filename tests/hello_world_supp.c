#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"


ei_bool_t button_press(ei_widget_t* widget, ei_event_t* event, void* user_param);
ei_bool_t process_key(ei_widget_t* widget, ei_event_t* event, void* user_param);
ei_callback_t button_callback = button_press;


ei_bool_t button_press(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
    printf("Click !\n");
    return EI_TRUE;
}

ei_bool_t process_key(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if (event->param.key.key_sym == SDLK_ESCAPE) {
		ei_app_quit_request();
		return EI_TRUE;
	}
        
        return EI_FALSE;
}

int ei_main(int argc, char** argv)
{
	ei_size_t	screen_size	= {800, 800};
        ei_color_t      root_bgcol      = {0x52, 0x7f, 0xb4, 0xff};
	
        ei_size_t       window_size     = {500,500};
        char*           window_title    = "Resizable x";
        ei_color_t      window_color    = {0xA0,0xA0,0xA0, 0xff};
        int             window_border_width    = 2;
        ei_bool_t       closable        = EI_TRUE;
        ei_axis_set_t   window_resizable = ei_axis_x;

        ei_size_t       window_size_2     = {200,200};
        char*           window_title_2    = "Resizable y";
        ei_color_t      window_color_2    = {0xA0,0xA0,0xA0, 0xff};
        int             window_border_width_2    = 2;
        ei_bool_t       closable_2        = EI_FALSE;
        ei_axis_set_t   window_resizable_2 = ei_axis_y;


        ei_point_t	window_position	 = {500,500}; 
	ei_point_t	window_position_2 = {200, 200};
     
        ei_widget_t*    toplevel;
	ei_widget_t*    toplevel_2;

	ei_app_create(&screen_size, EI_FALSE); 
        ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);        

	/* top_level 1 */
        toplevel = ei_widget_create("toplevel", ei_app_root_widget());
        ei_toplevel_configure(toplevel, &window_size, &window_color, &window_border_width, &window_title, &closable, &window_resizable, NULL);
        ei_place(toplevel, NULL, &(window_position.x), &(window_position.y), NULL, NULL, NULL, NULL, NULL, NULL);

	/* top_level_2 */
	toplevel_2 = ei_widget_create("toplevel", ei_app_root_widget());
        ei_toplevel_configure(toplevel_2, &window_size_2, &window_color_2, &window_border_width_2, &window_title_2, &closable_2, &window_resizable_2, NULL);
        ei_place(toplevel_2, NULL, &(window_position_2.x), &(window_position_2.y), NULL, NULL, NULL, NULL, NULL, NULL);
  

	ei_bind(ei_ev_keydown, NULL, "all", process_key, NULL);

        ei_app_run();

	ei_unbind(ei_ev_keydown, NULL, "all", process_key, NULL);

	ei_app_free();

	return (EXIT_SUCCESS);
}
