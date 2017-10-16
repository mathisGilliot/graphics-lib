#ifndef _EI_WIDGETBUTTON_H_
#define _EI_WIDGETBUTTON_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_widgetclass.h"
#include "ei_widget.h"
#include "ei_draw_utilities.h"
#include "ei_event.h"
#include "ei_application.h"
#include "ei_widgettoplevel.h"


extern void* button_allocfunc();

extern void button_releasefunc(struct ei_widget_t* widget);

extern void button_drawfunc(struct ei_widget_t* widget, ei_surface_t surface,
		     ei_surface_t pick_surface, ei_rect_t* clipper);

extern void button_setdefaultsfunc(struct ei_widget_t* widget);

extern void button_geomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect);

typedef struct ei_button_t {
	ei_widget_t           widget;
	ei_color_t     	      color;
	int    		      border_width;
	ei_relief_t           relief;
	char*                 text;
	ei_font_t     	      text_font;
	ei_color_t	      text_color;
	ei_anchor_t	      text_anchor;
	ei_surface_t	      img;
	ei_rect_t*	      img_rect;
	ei_anchor_t	      img_anchor;
	ei_callback_t*	      callback;
	void*	              user_param;
	int		      corner_radius;	
} ei_button_t;

extern void ajout_text_button(ei_surface_t surface, ei_widget_t* widget, ei_linked_point_t* point_hg, ei_linked_point_t* point_hd, ei_linked_point_t* point_bg, ei_rect_t* clipper);

extern void ajout_img_button(ei_surface_t surface, ei_widget_t* widget, ei_linked_point_t* point_hg, ei_linked_point_t* point_hd,
			     ei_linked_point_t* point_bg, ei_bool_t alpha, ei_rect_t* clipper);

extern ei_bool_t callback_press_button(ei_widget_t* widget, ei_event_t* event, void* user_param);

extern ei_bool_t click_in_progress(ei_widget_t* widget, ei_event_t* event, void* user_param);

extern ei_bool_t unbind_button(ei_widget_t* widget, ei_event_t* event, void* user_param);

void anchor_manager_button (ei_anchor_t anchor, ei_point_t* point_ancr, ei_linked_point_t* point_hg, ei_linked_point_t* point_hd, 
			    ei_linked_point_t* point_bg, int width, int height);
#endif
