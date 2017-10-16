#include "ei_widgetbutton.h"


static ei_color_t button_pick_color = {0, 1, 0, 255};

void* button_allocfunc() {
	void* espace = calloc(1, sizeof(struct ei_button_t));
	return espace;
}

void button_releasefunc(struct ei_widget_t* widget){
	ei_button_t* widget_button = (ei_button_t*)widget;

	/* Gestion de la mémoire très superficielle... */ 
	if (widget == NULL) { 
		return; 
	} 
	
	free(widget_button);
} 
  

void button_drawfunc(struct ei_widget_t* widget, ei_surface_t surface,
		     ei_surface_t pick_surface, ei_rect_t* clipper) {

	ei_button_t* widget_button = (ei_button_t*)widget;

	ei_linked_point_t* first_point_ext = malloc(sizeof(ei_linked_point_t));

	ei_linked_point_t* second_point_ext = malloc(sizeof(ei_linked_point_t));

	/* On crée des couleurs plus claire et plus foncée pour le relief */
	
	ei_color_t dark = widget_button->color;
	dark.red = (0.9 * dark.red - 20 < 0) ? 0 :  0.9 * dark.red - 20;
	dark.blue = (0.9 * dark.blue - 20 < 0) ? 0 :  0.9 * dark.blue - 20;
	dark.green = (0.9 * dark.green - 20 < 0) ? 0 :  0.9 * dark.green - 20 ;
		
	ei_color_t bright = widget_button->color;
	bright.red = (1.1 * bright.red + 20 > 255) ? 255 : 1.1 * bright.red + 20;
	bright.blue = (1.1 * bright.blue + 20 > 255) ? 255 : 1.1 * bright.blue + 20;
	bright.green = (1.1 * bright.green + 20 > 255) ? 255 : 1.1 * bright.green + 20;

	/* On calcule l'intersection du clipper et du content_rect du parent*/

	if (widget != ei_app_root_widget()) {
		if (strcmp(widget->parent->wclass->name, "toplevel") == 0) {
			ei_toplevel_t *father = (ei_toplevel_t *)widget->parent;
			if (father->closing_button != widget)  {
				intersection_rect(widget, &clipper);
			}
		} else {
			intersection_rect(widget, &clipper);
		}
	}


	/*Dessin rectangle interne*/

	ei_rect_t rectangle_interne = widget->screen_location;
	rectangle_interne.top_left.x += widget_button->border_width;
	rectangle_interne.top_left.y += widget_button->border_width;
	rectangle_interne.size.width -= 2*widget_button->border_width;
	rectangle_interne.size.height -= 2*widget_button->border_width;
	
	ei_linked_point_t* first_point_int = malloc(sizeof(ei_linked_point_t));
	
	ei_linked_point_t* second_point_int = malloc(sizeof(ei_linked_point_t));

	ei_linked_point_t* centre_hg = malloc(sizeof(ei_point_t));
	ei_linked_point_t* centre_hd = malloc(sizeof(ei_point_t));
	ei_linked_point_t* centre_bg = malloc(sizeof(ei_point_t));;

	switch (widget_button->relief) {
	case ei_relief_none:
		first_point_int = rounded_frame(widget->screen_location, widget_button->corner_radius, EI_TRUE, centre_hg, centre_hd, centre_bg);
		second_point_int = rounded_frame(widget->screen_location, widget_button->corner_radius, EI_FALSE, centre_hg, centre_hd, centre_bg);
		ei_draw_polygon(surface, first_point_int, widget_button->color, clipper);
		ei_draw_polygon(surface, second_point_int, widget_button->color, clipper);	
		break;
	/* relief relevé ie foncé en bas */
	case ei_relief_raised:
		first_point_ext = rounded_frame(widget->screen_location, widget_button->corner_radius, EI_TRUE, centre_hg, centre_hd, centre_bg);
		second_point_ext = rounded_frame(widget->screen_location, widget_button->corner_radius, EI_FALSE, centre_hg, centre_hd, centre_bg);
		ei_draw_polygon(surface, first_point_ext, bright, clipper);
		ei_draw_polygon(surface, second_point_ext, dark, clipper);
		first_point_int = rounded_frame(rectangle_interne, widget_button->corner_radius - widget_button->border_width, EI_TRUE, centre_hg, centre_hd, centre_bg);
		second_point_int = rounded_frame(rectangle_interne, widget_button->corner_radius - widget_button->border_width , EI_FALSE, centre_hg, centre_hd, centre_bg);

		ei_draw_polygon(surface, first_point_int, widget_button->color, clipper);
		ei_draw_polygon(surface, second_point_int, widget_button->color, clipper);
		break;
	/* relief enfoncé ie foncé en haut */
	case ei_relief_sunken:
		first_point_ext = rounded_frame(widget->screen_location, widget_button->corner_radius, EI_TRUE, centre_hg, centre_hd, centre_bg);
		second_point_ext = rounded_frame(widget->screen_location, widget_button->corner_radius, EI_FALSE, centre_hg, centre_hd, centre_bg);
		ei_draw_polygon(surface, first_point_ext, dark, clipper);
		ei_draw_polygon(surface, second_point_ext, bright, clipper);
		first_point_int = rounded_frame(rectangle_interne, widget_button->corner_radius - widget_button->border_width, EI_TRUE, centre_hg, centre_hd, centre_bg);
		second_point_int = rounded_frame(rectangle_interne, widget_button->corner_radius - widget_button->border_width , EI_FALSE, centre_hg, centre_hd, centre_bg);

		ei_draw_polygon(surface, first_point_int, widget_button->color, clipper);
		ei_draw_polygon(surface, second_point_int, widget_button->color, clipper);
		break;
	   }
 
	/* Écriture du texte */
	ajout_text_button(surface, widget, centre_hg, centre_hd, centre_bg, clipper);
	/* Dessin de l'image */
	ajout_img_button(surface, widget, centre_hg, centre_hd, centre_bg, EI_FALSE, clipper);

	/* dessin de la pick surface */
	ei_draw_polygon(pick_surface, first_point_int, *(widget->pick_color), clipper);
	ei_draw_polygon(pick_surface, second_point_int, *(widget->pick_color), clipper);

	free(centre_hg);
	free(centre_hd);
	free(centre_bg);
	
}
	
void button_setdefaultsfunc(struct ei_widget_t* widget ) {
  	
	/* Picking Surface */
	button_pick_color.green += 1;
	widget->pick_id = ei_map_rgba(ei_app_root_surface(), &button_pick_color);
	widget->pick_color = malloc(sizeof(ei_color_t));
	*(widget->pick_color) = button_pick_color;

	/* Widget Hierachy Management */
	widget->children_head = NULL;
	widget->children_tail = NULL;
	widget->next_sibling = NULL;

	/* Geometry Management */
	widget->geom_params = NULL;
	(widget->requested_size).width = 100;
	(widget->requested_size).height = 50;
	(widget->screen_location).top_left.x = 0;
	(widget->screen_location).top_left.y = 0;
	(widget->screen_location).size.width = 300;
	(widget->screen_location).size.height = 200;
	widget->content_rect = &(widget->screen_location);
	ei_button_t* widget_button = (ei_button_t*)widget;
	widget_button->color = ei_default_background_color;
	widget_button->border_width = k_default_button_border_width;
	widget_button->relief = ei_relief_raised;
	widget_button->text = NULL;
	widget_button->text_font = ei_default_font;	
	widget_button->text_color = ei_font_default_color;	
	widget_button->text_anchor = ei_anc_center;	
	widget_button->img = NULL;
	widget_button->img_rect = NULL;
	widget_button->img_anchor = ei_anc_center;
	widget_button->callback = malloc(sizeof(ei_callback_t));
	*(widget_button->callback) = NULL;
	widget_button->user_param = NULL;
	widget_button->corner_radius = k_default_button_corner_radius;
}

void button_geomnotifyfunc (struct ei_widget_t* widget, ei_rect_t rect) {
	if (widget != NULL) {
		widget->screen_location = rect;
	}
}

void ajout_text_button(ei_surface_t surface, ei_widget_t* widget, ei_linked_point_t* point_hg, ei_linked_point_t* point_hd, ei_linked_point_t* point_bg, ei_rect_t* clipper) {

	/*création des points*/
	ei_button_t* widget_button = (ei_button_t*)widget;

	point_hg->point = widget->screen_location.top_left;

	point_hd->point.x = widget->screen_location.top_left.x + widget->screen_location.size.width;
	point_hd->point.y = widget->screen_location.top_left.y;

	point_bg->point.x = widget->screen_location.top_left.x + widget->screen_location.size.width;
	point_bg->point.y = widget->screen_location.top_left.y + widget->screen_location.size.height;

        /* Écriture du texte */
	int width_text;
	int height_text;
	ei_point_t point;

	if (widget_button->text != NULL) {
		hw_text_compute_size(widget_button->text, widget_button->text_font, &width_text, &height_text);
		anchor_manager_button(widget_button->text_anchor, &point, point_hg, point_hd, point_bg, width_text, height_text);
		ei_draw_text(surface, &point, widget_button->text, widget_button->text_font, &(widget_button->text_color), clipper);
	}
}

static ei_relief_t button_down = ei_relief_sunken;
static ei_relief_t button_up = ei_relief_raised;

ei_bool_t callback_press_button(ei_widget_t*   widget, 
				ei_event_t*    event, 
				void*          user_param){
	
	ei_button_t* button = (ei_button_t*)widget;
	if (event->type == ei_ev_mouse_buttondown) {
		if (button->relief != 0){
			ei_button_configure(widget, NULL, NULL, NULL, NULL, 
					    &button_down, NULL, NULL, NULL, NULL, NULL, 
					    NULL, NULL, NULL, NULL);
			ei_app_invalidate_rect(&(widget->screen_location));
		}
		ei_app_invalidate_rect(&(widget->screen_location));
		ei_bind(ei_ev_mouse_move, widget, "all", click_in_progress, NULL);
		ei_bind(ei_ev_mouse_buttonup, widget, "all", unbind_button, NULL);
	}
	return EI_FALSE;		
}

ei_bool_t click_in_progress(ei_widget_t*   widget, 
			    ei_event_t*    event, 
			    void*          user_param){

	ei_widget_t *widget_where = ei_widget_pick(&event->param.mouse.where);
	ei_button_t *button = (ei_button_t*)widget;
	
	if (widget_where != widget && button->relief == button_down) {
		ei_button_configure(widget, NULL, NULL, NULL, NULL, &button_up,
				    NULL, NULL, NULL, NULL, NULL, NULL, NULL,
				    NULL, NULL);
		ei_app_invalidate_rect(&(widget->screen_location));
	} else if (widget_where == widget && button->relief == button_up) {
		ei_button_configure(widget, NULL, NULL, NULL, NULL, &button_down,
				    NULL, NULL, NULL, NULL, NULL, NULL, NULL,
				    NULL, NULL);
		ei_app_invalidate_rect(&(widget->screen_location));
	}
	return EI_FALSE;
}


extern ei_bool_t unbind_button(ei_widget_t* widget, ei_event_t* event, void* user_param){

	ei_bool_t callback_ret;
	ei_widget_t *widget_where = ei_widget_pick(&event->param.mouse.where);
	ei_button_t* button = (ei_button_t*)widget;
	
	if (event->type == ei_ev_mouse_buttonup) {
		if (widget_where == widget) {
			if (button->relief != 0){
				ei_button_configure(widget, NULL, NULL, NULL, NULL, &button_up, NULL,
						    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
				ei_app_invalidate_rect(&(widget->screen_location));
			}
			if (button->callback != NULL) {
				callback_ret = (*(button->callback))(widget, event, user_param);
			}
		}
		ei_unbind(ei_ev_mouse_move, widget, "all", click_in_progress, NULL);
		ei_unbind(ei_ev_mouse_buttonup, widget, "all", unbind_button, NULL);
	}
	return EI_FALSE;	
}


void ajout_img_button(ei_surface_t surface, ei_widget_t* widget, ei_linked_point_t* point_hg, ei_linked_point_t* point_hd,
		      ei_linked_point_t* point_bg, ei_bool_t alpha, ei_rect_t* clipper) {
	ei_button_t* widget_button = (ei_button_t*)widget;
	int failure;
	ei_rect_t* dest_rect = malloc(sizeof(ei_rect_t));
	ei_rect_t* src_rect = malloc(sizeof(ei_rect_t));
	ei_rect_t* cont_rect = widget->parent->content_rect;

	if (widget_button->img != NULL) {
		/* Lock avant dessin */
		hw_surface_lock(widget_button->img);
		
		/* Initialisation de dest_rect et src_rect
		   pour qu'ils aient les mêmes dimensions que l'image */
		*dest_rect = *(widget_button->img_rect);
		*src_rect = *(widget_button->img_rect);

		/* Pour que l'image soit bien située, on fait une translation */
		dest_rect->top_left.x += cont_rect->top_left.x;
		dest_rect->top_left.y += cont_rect->top_left.y;

		/* Gestion d'une éventuelle sortie du parent */
		intersection_rect(widget, &dest_rect);
		/* Redimension du rectangle source en cas de changement */
		src_rect->size = dest_rect->size;

		/* Gestion de l'ancrage */
		anchor_manager(widget_button->img_anchor, &(dest_rect->top_left), point_hg, point_hd, point_bg, widget_button->img_rect->size.width, widget_button->img_rect->size.height);

		failure = ei_copy_surface (surface,
					   dest_rect,
					   widget_button->img,
					   src_rect,
					   alpha);
		/* En cas d'erreur, le signaler */
		if (failure) {
			printf("\nCopie impossible\nVérifiez les dimensions du bouton et de l'image\n");
		}
		/* Dessin terminé -> libération */
		hw_surface_unlock(widget_button->img);
	}
}

void anchor_manager_button (ei_anchor_t anchor, ei_point_t* point_ancr, ei_linked_point_t* point_hg, ei_linked_point_t* point_hd, 
		     ei_linked_point_t* point_bg, int width, int height) {
	
	ei_point_t milieu;

	milieu.x = (point_hd->point.x + point_hg->point.x)/2;
	milieu.y = (point_bg->point.y + point_hg->point.y)/2;
	
	/* Selon l'ancre, on modifie le point d'ancrage */	
	switch (anchor) {
	case ei_anc_none:
		point_ancr->x = milieu.x - width/2;
		point_ancr->y = milieu.y - height/2;
		break;
	case ei_anc_center:
		point_ancr->x = milieu.x - width/2;
		point_ancr->y = milieu.y - height/2;
		break;
	case ei_anc_north:
		point_ancr->x = milieu.x - width/2;
		point_ancr->y = point_hg->point.y;
		break;
	case ei_anc_northeast:
		point_ancr->x = point_hd->point.x - width;
		point_ancr->y = point_hg->point.y;
		break;
	case ei_anc_east:
		point_ancr->x = point_hd->point.x - width;
		point_ancr->y = milieu.y - height/2;
		break;
	case ei_anc_southeast:
		point_ancr->x = point_hd->point.x - width;
		point_ancr->y = point_bg->point.y - height;
		break;
	case ei_anc_south:
		point_ancr->x = milieu.x - width/2;
		point_ancr->y = point_bg->point.y - height;
		break;
	case ei_anc_southwest:
		point_ancr->x = point_hg->point.x;
		point_ancr->y = point_bg->point.y - height;
		break;
	case ei_anc_west:
		point_ancr->x = point_hg->point.x;
		point_ancr->y = milieu.y - height/2;
		break;
	case ei_anc_northwest:
		point_ancr->x = point_hg->point.x;
		point_ancr->y = point_hg->point.y;
		break;
	}
}
