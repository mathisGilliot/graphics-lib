#include "ei_geometry_placer.h"

ei_color_t red_close = {245, 0, 0, 0xff};
ei_color_t transparent_resize = {0, 0, 0, 0};


/* Fonction générique: remplace la valeur :
 * par la valeur par défault si elle était nulle,
 * par la valeur entrée sinon.
 Garde aussi un booleen si il a remplacé. 
*/
static void int_default_if_null(int* to_test, int* to_replace, int default_value, ei_bool_t* bool);

/* Fonction générique: remplace la valeur :
 * par la valeur par défault si elle était nulle,
 * par la valeur entrée sinon.
 */
static void float_default_if_null(float* to_test, float* to_replace, float default_value);


static void int_default_if_null(int* to_test, int* to_replace, int default_value, ei_bool_t* bool){
	if (to_test == NULL) {
		*to_replace = default_value;
		*bool = EI_FALSE;
	} else {
		*to_replace = *to_test;
		*bool = EI_TRUE;
	}
}

static void float_default_if_null(float* to_test, float* to_replace, float default_value){
	if (to_test == NULL) {
		*to_replace = default_value;
	} else {
		*to_replace = *to_test;
	}
}

void placer_runfunc(ei_widget_t* widget) {

	widget_place_parameters *parameters = ((widget_place_parameters*)(widget->geom_params));

	ei_anchor_t anchor = (parameters->w_anchor);
	int width = 0;
	int height = 0;
	int width_parent = 0;
	int height_parent = 0;
	int x_parent = 0;
	int y_parent = 0;

	if (widget->parent != NULL) {
		if (strcmp(widget->parent->wclass->name, "toplevel") == 0) {
			ei_toplevel_t *father = (ei_toplevel_t *)widget->parent;
			if (father->resizing_frame == widget
			    || father->closing_button == widget) {
				width_parent = widget->parent->screen_location.size.width;
				height_parent = widget->parent->screen_location.size.height;
				x_parent = widget->parent->screen_location.top_left.x;
				y_parent = widget->parent->screen_location.top_left.y;
			} else {
				width_parent = widget->parent->content_rect->size.width;
				height_parent = widget->parent->content_rect->size.height;
				x_parent = widget->parent->content_rect->top_left.x;
				y_parent = widget->parent->content_rect->top_left.y;
			}
		} else {
			width_parent = widget->parent->content_rect->size.width;
			height_parent = widget->parent->content_rect->size.height;
			x_parent = widget->parent->content_rect->top_left.x;
			y_parent = widget->parent->content_rect->top_left.y;	
		}

	}

	int x = parameters->w_rel_x * width_parent + parameters->w_x;
	int y = parameters->w_rel_y * height_parent + parameters->w_y;

	/* EN RELATIF */
	if (parameters->width_is_relative != 0){
		width = parameters->w_width;
	} else {
		if (parameters->w_rel_width != 0.0){
			width = parameters->w_rel_width * width_parent;
		} else {
			width = (widget->requested_size).width;
		}
	}

	if (parameters->height_is_relative != 0){
		height = parameters->w_height;
	} else {
		if (parameters->w_rel_height != 0.0){
			height = parameters->w_rel_height * height_parent;
		} else {
			height = (widget->requested_size).height;
		}
	}
	

        /* Différents cas de ancres */
	switch (anchor){
		
	case ei_anc_center:
		x = x - width / 2;
		y = y - height / 2;
		break;
	case ei_anc_north:
		x = x - width / 2;
		break;
	case ei_anc_northeast:
		x = x - width;
		break;
	case ei_anc_east:
		x = x - width;
		y = y - height / 2;
		break;
	case ei_anc_southeast:	
		x = x - width;
		y = y - height;
		break;
	case ei_anc_south:
		x = x - width / 2;
		y = y - height;
		break;
	case ei_anc_southwest:	
		y = y - height;
		break;
	case ei_anc_west:
		y = y - height / 2;
		break;
	case ei_anc_northwest:
		break;
	case ei_anc_none:
		break;
	}

	
	ei_rect_t new_screen_location;
	new_screen_location.top_left.x = x + x_parent;
	new_screen_location.top_left.y = y + y_parent;
	new_screen_location.size.width = width;
	new_screen_location.size.height = height;
	
	/* Appel à la geomnotifyfunc du widget: màj de la screen_location */
	widget->wclass->geomnotifyfunc(widget, new_screen_location);

        /* Modif content_rect: cas du toplevel */
	if (strcmp(widget->wclass->name, "toplevel") == 0){
		ei_toplevel_t* top_level = (ei_toplevel_t*)(widget);
		ei_rect_t new_content_rect;
		new_content_rect.top_left.x = widget->screen_location.top_left.x
			+ top_level->border_width;
		new_content_rect.top_left.y = widget->screen_location.top_left.y
			+ ei_font_default_size
			+ top_level->border_width;
		new_content_rect.size.width = widget->screen_location.size.width
			- 2 * top_level->border_width;
		new_content_rect.size.height = widget->screen_location.size.height
			- 2 * top_level->border_width
			- ei_font_default_size;
		*(widget->content_rect) = new_content_rect;
	}

	/* Appel de la runfunc des fils */
	if (widget->children_head != NULL) {
		ei_widget_t* fils_courant = widget->children_head;
		while (fils_courant != NULL) {
			if (fils_courant->geom_params != NULL){
				fils_courant->geom_params->manager->runfunc(fils_courant);
			}
			fils_courant = fils_courant->next_sibling;
		}		
	}	
}

void placer_releasefunc(ei_widget_t* widget) {
}



void create_parameters(ei_widget_t* widget){
	widget_place_parameters* place_param = malloc(sizeof(widget_place_parameters));
	place_param->w_param.manager = ei_geometrymanager_from_name("placeur");
	widget->geom_params = (ei_geometry_param_t*)place_param;
}

void init_parameters(ei_widget_t*               widget,
		     ei_anchor_t*		anchor,
		     int*			x,
		     int*			y,
		     int*			width,
		     int*			height,
		     float*			rel_x,
		     float*			rel_y,
		     float*			rel_width,
		     float*			rel_height){

	widget_place_parameters *parameters = ((widget_place_parameters*)(widget->geom_params));
	
	parameters->w_widget = widget;

	if (anchor == NULL) {
		parameters->w_anchor = ei_anc_northwest; 
	} else {
		parameters->w_anchor = *anchor; 	
	}

	int_default_if_null(x, &parameters->w_x, 0, &parameters->x_is_relative);
	int_default_if_null(y, &parameters->w_y, 0, &parameters->y_is_relative);
	int_default_if_null(width, &parameters->w_width, 
			    widget->requested_size.width, &parameters->width_is_relative);
	int_default_if_null(height, &parameters->w_height, 
			    widget->requested_size.height, &parameters->height_is_relative);
	
	float_default_if_null(rel_x, &parameters->w_rel_x, 0.0);
	float_default_if_null(rel_y, &parameters->w_rel_y, 0.0);
	float_default_if_null(rel_width, &parameters->w_rel_width, 0.0);
	float_default_if_null(rel_height, &parameters->w_rel_height, 0.0);


	/* dans le cas du top level, content_rect */
	if (strcmp(widget->wclass->name, "toplevel") == 0){
		ei_rect_t* content = malloc(sizeof(ei_rect_t));
		*content = widget->screen_location;
		widget->content_rect = content;
	}
	
}
