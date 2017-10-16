#ifndef _EI_WIDGETTOPLEVEL_H_
#define _EI_WIDGETTOPLEVEL_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_widgetclass.h"
#include "ei_widget.h"
#include "ei_application.h"
#include "ei_event.h"
#include "ei_geometrymanager.h"
#include "ei_widgetframe.h"


void* toplevel_allocfunc();

void toplevel_releasefunc(struct ei_widget_t* widget);

void toplevel_drawfunc(struct ei_widget_t* widget, ei_surface_t surface,
		       ei_surface_t pick_surface, ei_rect_t* clipper);

void toplevel_setdefaultsfunc(struct ei_widget_t* widget);

void toplevel_geomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect);

typedef struct ei_toplevel_t {
	/* Attributs communs */
	ei_widget_t           widget;
	ei_color_t    	      color;
	int    		      border_width;
	/* Attributs spécifiques */
	char*                 title;
	ei_bool_t     	      closable;
	ei_axis_set_t         resizable;
	ei_size_t             min_size;
	ei_widget_t*          closing_button;
	ei_widget_t*          resizing_frame;
} ei_toplevel_t;

void dessin_rect(struct ei_widget_t* widget, ei_surface_t surface, 
		 ei_linked_point_t* point1, ei_linked_point_t* point2, 
		 ei_linked_point_t* point3, ei_linked_point_t* point4, 
		 ei_bool_t interieur, ei_rect_t* clipper, ei_surface_t pick_surface,
		 ei_linked_point_t* point_c1, ei_linked_point_t* point_c2,
		 ei_linked_point_t* point_c3);

void ecriture_contour(struct ei_widget_t* widget, ei_surface_t surface, 
		      ei_linked_point_t* point1, ei_rect_t* clipper);

ei_bool_t callback_move_toplevel(ei_widget_t* widget, ei_event_t* event, void* user_param);

ei_bool_t move_in_progress(ei_widget_t* widget, ei_event_t* event, void* user_param);

ei_bool_t end_move(ei_widget_t* widget, ei_event_t* event, void* user_param);

ei_bool_t callback_resizing(ei_widget_t* widget, ei_event_t* event, void* user_param);

ei_bool_t resize_in_progress(ei_widget_t* widget, ei_event_t* event, void* user_param);

ei_bool_t end_resize(ei_widget_t* widget, ei_event_t* event, void* user_param);

ei_bool_t closing_button_press(ei_widget_t* widget, ei_event_t* event, void* user_param);

typedef struct point_depart{
	ei_point_t souris;
	ei_point_t fenetre;
} point_depart;

#endif
