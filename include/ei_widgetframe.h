#ifndef _EI_WIDGETFRAME_H_
#define _EI_WIDGETFRAME_H_

#include <stdlib.h>
#include <string.h>

#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_widgetclass.h"
#include "ei_widget.h"
#include "ei_draw_utilities.h"
#include "ei_widgettoplevel.h"


void* frame_allocfunc();

void frame_releasefunc(struct ei_widget_t* widget);

void frame_drawfunc(struct ei_widget_t* widget, ei_surface_t surface,
		    ei_surface_t pick_surface, ei_rect_t* clipper);

void frame_setdefaultsfunc(struct ei_widget_t* widget);

void frame_geomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect);

typedef struct ei_frame_t {
	/* Attributs communs */
	ei_widget_t           widget;
	ei_color_t    	      color;
	int    		      border_width;
	/* Attributs spécifiques */
	ei_relief_t           relief;
	char*                 text;
	ei_font_t     	      text_font;
	ei_color_t	      text_color;
	ei_anchor_t	      text_anchor;
	ei_surface_t	      img;
	ei_rect_t*	      img_rect;
	ei_anchor_t	      img_anchor;
} ei_frame_t;

void dessiner_polygone (ei_surface_t surface, ei_widget_t* widget, 
ei_linked_point_t* point1, ei_linked_point_t* point2, 
ei_linked_point_t* point3, ei_linked_point_t* point4, 
ei_linked_point_t* point5, ei_linked_point_t* point6, ei_rect_t* clipper);

void dessiner_relief (ei_surface_t surface, ei_widget_t* widget, 
ei_linked_point_t* point_hg, ei_linked_point_t* point_hd, 
ei_linked_point_t* point_bg, ei_linked_point_t* point_bd, ei_rect_t* clipper);

void dessiner_rectangle (ei_surface_t surface, ei_widget_t* widget, 
ei_linked_point_t* point1, ei_linked_point_t* point2, ei_linked_point_t* point5, 
ei_linked_point_t* point6, ei_rect_t* clipper);

void ajout_text_frame(ei_surface_t surface, ei_widget_t* widget, 
ei_linked_point_t* point_hg, ei_linked_point_t* point_hd, ei_linked_point_t* point_bg, ei_rect_t* clipper);

void ajout_img_frame(ei_surface_t surface, ei_widget_t* widget, ei_linked_point_t* point_hg, ei_linked_point_t* point_hd, 
		     ei_linked_point_t* point_bg, ei_bool_t alpha, ei_rect_t* clipper);

void anchor_manager (ei_anchor_t anchor, ei_point_t* point_ancr, ei_linked_point_t* point_hg, ei_linked_point_t* point_hd, 
		     ei_linked_point_t* point_bg, int width, int height);
#endif
