#include "ei_widget.h"
#include "ei_widgetframe.h"
#include "ei_widgetbutton.h"
#include "ei_application.h"
#include "ei_widgettoplevel.h"
#include <string.h>

void ei_children_destroy(ei_widget_t* widget);
/* Parcours récursif des widgets*/
void parcours_arbre(ei_widget_t* widget, uint32_t id_pixel, ei_widget_t** widget_click);

ei_widget_t* ei_widget_create (ei_widgetclass_name_t	class_name,
			       ei_widget_t*		parent){

	ei_widgetclass_t* class = ei_widgetclass_from_name(class_name);
	ei_widget_t* new_widget = class->allocfunc();
	
	if (parent != NULL) {
		if (parent->children_head == NULL) {
			parent->children_head = new_widget;
		} else {
			parent->children_tail->next_sibling = new_widget;
		}
		parent->children_tail = new_widget;
	}

	class->setdefaultsfunc(new_widget);
	new_widget->wclass = class;
	new_widget->parent = parent;

	return new_widget;	
}

void ei_children_destroy(ei_widget_t* widget){
	if (widget == NULL) {
		return;
	}
	if (widget->next_sibling != NULL) {
		widget->next_sibling->wclass->releasefunc(widget->next_sibling);
	}
	if (widget->children_head != NULL) {
	widget->children_head->wclass->releasefunc(widget->children_head);
	}
	widget->wclass->releasefunc(widget);
	
}

void ei_widget_destroy(ei_widget_t* widget){
	ei_widget_t* current_sibling = NULL;
	if (widget == NULL) {
		return;
	}
	if (widget != ei_app_root_widget()) {
		current_sibling = widget->parent->children_head;	
		while (current_sibling->next_sibling != widget && current_sibling->next_sibling != NULL) {
			current_sibling = current_sibling->next_sibling;
		}
		if (current_sibling->next_sibling == NULL) {
			widget->parent->children_head = widget->next_sibling;
		} else {
			current_sibling->next_sibling = widget->next_sibling;
		}
	}
	ei_children_destroy(widget->children_head);
	widget->wclass->releasefunc(widget);
}
	
void ei_frame_configure	(ei_widget_t*		widget,
			 ei_size_t*		requested_size,
			 const ei_color_t*	color,
			 int*			border_width,
			 ei_relief_t*		relief,
			 char**			text,
			 ei_font_t*		text_font,
			 ei_color_t*		text_color,
			 ei_anchor_t*		text_anchor,
			 ei_surface_t*		img,
			 ei_rect_t**		img_rect,
			 ei_anchor_t*		img_anchor){

 
	ei_frame_t* frame = (ei_frame_t*)widget;
	
       	if (requested_size != NULL) {
		widget->requested_size = *requested_size;
	}
	if (color != NULL) {
		frame->color = *color;
	} 
	if (border_width != NULL) {
		frame->border_width = *border_width;
	} 
	if (relief != NULL) {
		frame->relief = *relief;
	}
	/* Attention recopie obligatoire */
	if (text != NULL) {
		char* txt = *text;
		frame->text = malloc(30*sizeof(char));
		strncpy(frame->text, txt, 30);
	}
	if (text_font != NULL) {
		frame->text_font = *text_font;
	} 
	if (text_color != NULL) {
		frame->text_color = *text_color;
	} 
	if (text_anchor != NULL) {
		frame->text_anchor = *text_anchor;
	} 
	if (img != NULL) {
		frame->img = *img;
	}
	/* Attention recopie obligatoire */
	if (img_rect != NULL) {
		frame->img_rect = malloc(sizeof(ei_rect_t));
		*(frame->img_rect) = **img_rect;
	}
	if (img_anchor != NULL) {
		frame->img_anchor = *img_anchor;
	}
}

void ei_button_configure(ei_widget_t*           widget,
			 ei_size_t*		requested_size,
			 const ei_color_t*	color,
			 int*			border_width,
			 int*			corner_radius,
			 ei_relief_t*		relief,
			 char**			text,
			 ei_font_t*		text_font,
			 ei_color_t*		text_color,
			 ei_anchor_t*		text_anchor,
			 ei_surface_t*		img,
			 ei_rect_t**		img_rect,
			 ei_anchor_t*		img_anchor,
			 ei_callback_t*		callback,
			 void**			user_param){

	ei_button_t* button = (ei_button_t*)widget;
	
       	if (requested_size != NULL) {
		widget->requested_size = *requested_size;
	}
	if (color != NULL) {
		button->color = *color;
	} 
	if (border_width != NULL) {
		button->border_width = *border_width;
	} 
	if (relief != NULL) {
		button->relief = *relief;
	}
	/* Attention recopie obligatoire */
	if (text != NULL) {
		char* txt = *text;
		button->text = malloc(30*sizeof(char));
		strncpy(button->text, txt, 30);
	}
	if (text_font != NULL) {
		button->text_font = *text_font;
	} 
	if (text_color != NULL) {
		button->text_color = *text_color;
	} 
	if (text_anchor != NULL) {
		button->text_anchor = *text_anchor;
	} 
	if (img != NULL) {
		button->img = *img;
	}
	/* Attention recopie obligatoire */
	if (img_rect != NULL) {
		button->img_rect = malloc(sizeof(ei_rect_t));
		*(button->img_rect) = **img_rect;
	}
	if (img_anchor != NULL) {
		button->img_anchor = *img_anchor;
	}
	if (corner_radius != NULL) {
		button->corner_radius = *corner_radius;
	}
	if (callback != NULL) {
		button->callback = callback;
	}
	if (user_param != NULL) {
		button->user_param = *user_param;
	}
}

void ei_toplevel_configure(ei_widget_t*		widget,
			   ei_size_t*		requested_size,
			   ei_color_t*		color,
			   int*			border_width,
			   char**		title,
			   ei_bool_t*		closable,
			   ei_axis_set_t*	resizable,
			   ei_size_t**		min_size){


	ei_toplevel_t* toplevel = (ei_toplevel_t*)widget;
	
       	if (requested_size != NULL) {
		widget->requested_size = *requested_size;
		widget->requested_size.height += ei_font_default_size;
	}
	if (color != NULL) {
		toplevel->color = *color;
	} 
	if (border_width != NULL) {
		toplevel->border_width = *border_width;
	}
	/* Attention recopie obligatoire */
	if (title != NULL) {
		char* txt = *title;
		toplevel->title = malloc(30*sizeof(char));
		strncpy(toplevel->title, txt, 20);
	}
	if (closable != NULL) {
		toplevel->closable = *closable;
		if (toplevel->closable == EI_FALSE
		    && toplevel->closing_button != NULL) {
			ei_widget_destroy(toplevel->closing_button);
			toplevel->closing_button = NULL;
		}
	}
	if (resizable != NULL) {
		toplevel->resizable = *resizable;
	}
	if (min_size != NULL) {
		toplevel->min_size = **min_size;
	}
}

void parcours_arbre(ei_widget_t* widget, uint32_t id_pixel, ei_widget_t** widget_click) {
	if (widget == NULL) {
		return;
	}
	if (widget->pick_id == id_pixel) {
		*widget_click = widget;
		return;
	}
	parcours_arbre(widget->next_sibling, id_pixel, widget_click);
	parcours_arbre(widget->children_head, id_pixel, widget_click);
}

ei_widget_t* ei_widget_pick (ei_point_t* where) {
	ei_widget_t* widget_click = malloc(sizeof(ei_widget_t));
	ei_widget_t* racine = malloc(sizeof(ei_widget_t));
	widget_click = ei_app_root_widget();
	racine = ei_app_root_widget();
	
	ei_surface_t pick_surface = ei_app_root_surface();

	uint8_t* origine =  malloc(sizeof(uint8_t));

	origine = hw_surface_get_buffer(pick_surface);
	
        /* on caste en un pointeur vers un 32 bits */
	uint32_t* origine_32bits = (uint32_t*)origine;

	ei_size_t taille_pick = hw_surface_get_size(pick_surface);
	uint32_t*  adresse_pixel = malloc(sizeof(uint32_t));
	adresse_pixel = (origine_32bits + (taille_pick.width * where->y) + where->x);


	uint8_t *pixel_4c = malloc(4*sizeof(uint8_t));

	/* on recaste en tableau de uint8_t */
	pixel_4c = (uint8_t*)adresse_pixel;

	int ir = 0;
	int ig = 0;
	int ib = 0;
	int ia = 0;
	hw_surface_get_channel_indices(pick_surface, &ir, &ig, &ib, &ia);

	ei_color_t couleur_pixel = {pixel_4c[ir], pixel_4c[ig], pixel_4c[ib], 255};
	
	uint32_t id_pixel = ei_map_rgba(pick_surface, &couleur_pixel);

	parcours_arbre(racine, id_pixel, &widget_click);

	return widget_click;

}
