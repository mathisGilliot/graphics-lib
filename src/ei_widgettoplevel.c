#include "ei_widgettoplevel.h"


/* Définition des couleurs */
ei_color_t white = {245, 245, 245, 0};
ei_color_t red = {245, 0, 0, 0xff};
static ei_color_t title_bar_pick_color = {0, 0, 1, 255};
static ei_color_t inside_pick_color = {1, 0, 1, 255};

/* Fonction de fermeture toplevel */
ei_bool_t closing_button_press(ei_widget_t* widget, 
			       ei_event_t* event, 
			       void* user_param){
	ei_rect_t* rect = malloc(sizeof(ei_rect_t));
	rect->top_left.x = widget->parent->screen_location.top_left.x - 5;
	rect->top_left.y = widget->parent->screen_location.top_left.y - 5;
	rect->size.width = widget->parent->screen_location.size.width + 10;
	rect->size.height = widget->parent->screen_location.size.height + 10;
	ei_widget_destroy(widget->parent); 
	ei_app_invalidate_rect(rect);
	return EI_TRUE;
}

ei_callback_t closing_button_callback = closing_button_press;

void* toplevel_allocfunc(){
	void* espace = calloc(1, sizeof(struct ei_toplevel_t)); 
	return espace;
}

void toplevel_releasefunc(struct ei_widget_t* widget){
	ei_toplevel_t* widget_toplevel = (ei_toplevel_t*)widget;

	/* Gestion de la mémoire très superficielle */ 
	if (widget == NULL) { 
		return; 
	}
	
	free(widget_toplevel);
	
}

void toplevel_drawfunc(struct ei_widget_t* widget, ei_surface_t surface,
		       ei_surface_t pick_surface, ei_rect_t* clipper){

	ei_linked_point_t* point_hg = malloc(sizeof(ei_linked_point_t));
	ei_linked_point_t* point_hd = malloc(sizeof(ei_linked_point_t));
	ei_linked_point_t* point_bd = malloc(sizeof(ei_linked_point_t));
	ei_linked_point_t* point_bg = malloc(sizeof(ei_linked_point_t));
	ei_linked_point_t* point_c1 = malloc(sizeof(ei_linked_point_t));
	ei_linked_point_t* point_c2 = malloc(sizeof(ei_linked_point_t));
	ei_linked_point_t* point_c3 = malloc(sizeof(ei_linked_point_t));
	
        /* Calcul de l'intersection du clipper et du content_rect du parent */
	if (widget != ei_app_root_widget()) {
	        intersection_rect(widget, &clipper);
	}
	
		
	/* Dessin du rectangle externe */
	dessin_rect(widget, surface, point_hg, point_hd, point_bd, point_bg, 
		    EI_FALSE, clipper, pick_surface, point_c1, point_c2, point_c3);
	
	/* Dessin du rectangle interne */
	dessin_rect(widget, surface, point_hg, point_hd, point_bd, point_bg, 
		    EI_TRUE, clipper, pick_surface, point_c1, point_c2, point_c3);

	/* Écriture du contour (bouton compris) */
	ecriture_contour(widget, surface, point_hg, clipper);
	
}

void toplevel_setdefaultsfunc(struct ei_widget_t* widget){
	
        /* Picking Surface */
	title_bar_pick_color.blue += 1;
	widget->pick_id = ei_map_rgba(ei_app_root_surface(), &title_bar_pick_color);
	widget->pick_color = malloc(sizeof(ei_color_t));
	*(widget->pick_color) = title_bar_pick_color;

	/* Widget Hierachy Management */
	widget->children_head = NULL;
	widget->children_tail = NULL;
	widget->next_sibling = NULL;

	/* Geometry Management */
	widget->geom_params = NULL;
	(widget->requested_size).width = 320;
	(widget->requested_size).height = 240;
	(widget->screen_location).top_left.x = 0;
	(widget->screen_location).top_left.y = 0;
	(widget->screen_location).size.width = 300;
	(widget->screen_location).size.height = 200;
	widget->content_rect = &(widget->screen_location);
	ei_toplevel_t* widget_toplevel = (ei_toplevel_t*)widget;
	widget_toplevel->color = ei_default_background_color;
	widget_toplevel->border_width = 4;
	char* nom = "toplevel";
	widget_toplevel->title = nom;
	widget_toplevel->closable = EI_TRUE;
	widget_toplevel->resizable = ei_axis_both;
	widget_toplevel->min_size.width = 160;
	widget_toplevel->min_size.height = 120;
	widget_toplevel->closing_button = ei_widget_create ("button", widget);
	widget_toplevel->resizing_frame = ei_widget_create("frame", widget);
	
	/* Configuration du bouton de fermeture */
	ei_size_t button_size = {10, 10};
	int button_corner_radius = 5;
	ei_relief_t button_relief = ei_relief_none;

	ei_button_configure(widget_toplevel->closing_button, &button_size, &red, NULL,
			    &button_corner_radius, &button_relief, NULL,
			    NULL, NULL, NULL, NULL, NULL, NULL,
			    &closing_button_callback, NULL);

	/* Configuration du frame de redimentionnement */
	ei_size_t resize_frame_size = {10, 10};

	ei_color_t b_color = widget_toplevel->color;
	b_color.red = (0.8 * b_color.red - 40 < 0) ? 0 :  0.8 * b_color.red - 40;
	b_color.blue = (0.8 * b_color.blue - 40 < 0) ? 0 :  0.8 * b_color.blue - 40;
	b_color.green = (0.8 * b_color.green - 40 < 0) ? 0 :  0.8 * b_color.green - 40 ;
	ei_frame_configure(widget_toplevel->resizing_frame, &resize_frame_size, 
			   &b_color, NULL, NULL, NULL, NULL, NULL, NULL, 
			   NULL, NULL, NULL);
	

}

void toplevel_geomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect){
	if (widget != NULL) {
		widget->screen_location = rect;
	}
}

void dessin_rect(struct ei_widget_t* widget, ei_surface_t surface, 
		 ei_linked_point_t* point_hg, ei_linked_point_t* point_hd, 
		 ei_linked_point_t* point_bd, ei_linked_point_t* point_bg, 
		 ei_bool_t interieur, ei_rect_t* clipper, ei_surface_t pick_surface,
		 ei_linked_point_t* point_c1, ei_linked_point_t* point_c2,
		 ei_linked_point_t* point_c3){

	ei_toplevel_t* widget_toplevel = (ei_toplevel_t*)widget;

	ei_color_t dark = widget_toplevel->color;
	dark.red = (0.8 * dark.red - 40 < 0) ? 0 :  0.8 * dark.red - 40;
	dark.blue = (0.8 * dark.blue - 40 < 0) ? 0 :  0.8 * dark.blue - 40;
	dark.green = (0.8 * dark.green - 40 < 0) ? 0 :  0.8 * dark.green - 40 ;

	/* 1er point */
	point_hg->point = widget->screen_location.top_left;
	
	/* 2eme point */
	point_hg->next = point_hd;
	point_hd->point.x = point_hg->point.x + (widget->screen_location).size.width;
	point_hd->point.y = point_hg->point.y;

	/* 3eme point */
	point_hd->next = point_bd;
	point_bd->point.x = point_hg->point.x + (widget->screen_location).size.width;
	point_bd->point.y = point_hg->point.y + (widget->screen_location).size.height;

	/* 4eme point */
	point_bd->next = point_bg;
	point_bg->point.x = point_hg->point.x;
	point_bg->point.y = point_hg->point.y + (widget->screen_location).size.height;
	point_bg->next = NULL;
	
	if(interieur) {
		int marge_interieure = 10;
		/* 1er point */
		point_hg->point.x += widget_toplevel->border_width;
		point_hg->point.y += widget_toplevel->border_width + ei_font_default_size;
	
		/* 2eme point */
		point_hd->point.x -= widget_toplevel->border_width;
		point_hd->point.y += widget_toplevel->border_width + ei_font_default_size;
		point_hd->next = point_c1;

                /* 3eme point */
		point_c1->point.x = point_bd->point.x - widget_toplevel->border_width;
		point_c1->point.y = point_bd->point.y - marge_interieure;
		point_c1->next = point_c2;

                /* 4eme point */
		point_c2->point.x = point_bd->point.x - marge_interieure;
		point_c2->point.y = point_bd->point.y - marge_interieure;
		point_c2->next = point_c3;

		/* 5eme point */
		point_c3->point.x = point_bd->point.x - marge_interieure;
		point_c3->point.y = point_bd->point.y - widget_toplevel->border_width;
		point_c3->next = point_bg;
	
		/* 6eme point */
		point_bg->point.x += widget_toplevel->border_width;
		point_bg->point.y -= widget_toplevel->border_width;

		ei_draw_polygon(surface, point_hg, widget_toplevel->color, clipper);
		ei_draw_polygon(pick_surface, point_hg, inside_pick_color, clipper);
	} else {
		ei_draw_polygon(surface, point_hg, dark, clipper);
		ei_draw_polygon(pick_surface, point_hg, *widget->pick_color, clipper);
	}
}

void ecriture_contour(struct ei_widget_t* widget, ei_surface_t surface, 
		    ei_linked_point_t* point_hg, ei_rect_t* clipper){

	ei_toplevel_t* widget_toplevel = (ei_toplevel_t*)widget;

	/* Récupération taille du texte */
	int title_width;
	int title_height;
	hw_text_compute_size(widget_toplevel->title, ei_default_font, &title_width, &title_height);
	
	/* Définition du point de localisation */
	ei_point_t point_titre;
	point_titre.x = widget->screen_location.top_left.x + widget_toplevel->border_width + 20;
	point_titre.y = (widget->screen_location.top_left.y + point_hg->point.y - title_height)/2;

	int marge = 7;
	ei_anchor_t sud_est = ei_anc_southeast;
	float cent = 1.0;

	/* Placement du bouton de fermeture */
	if (widget_toplevel->closing_button != NULL) {
		ei_place(widget_toplevel->closing_button, NULL, &marge, &marge, NULL, 
		 NULL, NULL, NULL, NULL, NULL);
	}

	/* Placement du bouton de redimentionnement */
	ei_place(widget_toplevel->resizing_frame, &sud_est, NULL, 
		 NULL, NULL, NULL, &cent, &cent, NULL, NULL);


	/* Écriture du titre */
	ei_draw_text(surface, &point_titre, widget_toplevel->title, NULL, &white, clipper);
}	


ei_bool_t callback_move_toplevel(ei_widget_t* widget, 
				 ei_event_t* event, 
				 void* user_param) {

	point_depart *depart = malloc(sizeof(point_depart));

        /* on reordonne les fils */
	if (widget != widget->parent->children_tail) {
		/* on le place en dernier fils */
		ei_widget_t* prec = widget->parent->children_head;
		if (prec == widget) {
			/* C'était le premier */
			widget->parent->children_head = widget->next_sibling;
			widget->parent->children_tail->next_sibling = widget;
			widget->parent->children_tail = widget;
			widget->next_sibling = NULL;
		} else {
			/* ce n'était pas le premier */
			while (prec->next_sibling != widget) {
				prec = prec->next_sibling;
			}  /* le prec est juste avant */
			prec->next_sibling = widget->next_sibling;
			widget->next_sibling = NULL;
			widget->parent->children_tail->next_sibling = widget;
			widget->parent->children_tail = widget;
		}
	
		/* Calcul du rectangle à redessiner */
		ei_rect_t invalide = widget->screen_location;
		/* On met un peu d'écart */
		invalide.top_left.y -= 2;
		invalide.top_left.x -= 2;
		invalide.size.width += 5;
		invalide.size.height += 5;
		/* on l'ajoute aux rectangles à modifier */
		ei_app_invalidate_rect(&invalide);
		/* on reordonne les fils */
	}



	/* on bind pour un move */
	depart->souris = event->param.mouse.where;
	depart->fenetre = widget->screen_location.top_left;

	/* On soustrait le content_rect du parent */

	if (widget->parent != NULL) {
		depart->fenetre.x -= widget->parent->content_rect->top_left.x;
		depart->fenetre.y -= widget->parent->content_rect->top_left.y;
	}
	
	ei_bind(ei_ev_mouse_move, widget, "all", move_in_progress, depart);
	ei_bind(ei_ev_mouse_buttonup, widget, "all", end_move, depart);
	
	return EI_FALSE;
}



ei_bool_t move_in_progress(ei_widget_t* widget, ei_event_t* event, void* user_param){
	ei_anchor_t ancre = ei_anc_northwest;
	point_depart* depart = (point_depart*)user_param;
	ei_point_t delta = {event->param.mouse.where.x - depart->souris.x,
			    event->param.mouse.where.y - depart->souris.y};


        /* Calcul du rectangle à redessiner */
	ei_rect_t invalide = widget->screen_location;
	invalide.size.width += abs(delta.x);
	invalide.size.height += abs(delta.y);
	if (delta.x < 0) {
		invalide.top_left.x += delta.x;
	}
	if (delta.y < 0) {
		invalide.top_left.y += delta.y;
	}

	/* On met un peu d'écart */
	invalide.top_left.y -= 2;
	invalide.top_left.x -= 2;
	invalide.size.width += 5;
	invalide.size.height += 5;

        /* on l'ajoute aux rectangles à modifier */
	ei_app_invalidate_rect(&invalide);


	ei_point_t new_fenetre = {depart->fenetre.x + delta.x,
				  depart->fenetre.y + delta.y};


	int new_width = widget->screen_location.size.width;
	int new_height = widget->screen_location.size.height;

	if (widget->parent != NULL){
		new_fenetre.x -= widget->parent->content_rect->top_left.x;
		new_fenetre.y -= widget->parent->content_rect->top_left.y;
	}
	
	ei_place(widget, &ancre, &(new_fenetre.x), &(new_fenetre.y),
		 &new_width, &new_height, NULL, NULL, NULL, NULL);

	/* Màj de "depart" */
	depart->souris = event->param.mouse.where;
	depart->fenetre = widget->screen_location.top_left;

	return EI_FALSE;
}

ei_bool_t end_move(ei_widget_t* widget, ei_event_t* event, void* user_param){

	if (event->type == ei_ev_mouse_buttonup) {
		ei_unbind(ei_ev_mouse_move, widget, "all", move_in_progress, NULL);
		ei_unbind(ei_ev_mouse_buttonup, widget, "all", end_move, NULL);
	}
	return EI_FALSE;

}


ei_bool_t callback_resizing(ei_widget_t* widget, 
			    ei_event_t* event, 
			    void* user_param){

	if (widget->parent != NULL) {
		ei_toplevel_t *father = (ei_toplevel_t *)widget->parent;
		if (father->resizing_frame == widget
		    && father->resizable != 0) {
			ei_point_t *depart = malloc(sizeof(ei_point_t));
			*depart = event->param.mouse.where;
	
			ei_bind(ei_ev_mouse_move, widget, "all", resize_in_progress, depart);
			ei_bind(ei_ev_mouse_buttonup, widget, "all", end_resize, depart);
		}
	}
	return EI_FALSE;
}

ei_bool_t resize_in_progress(ei_widget_t* widget, ei_event_t* event, void* user_param){
	ei_anchor_t ancre = ei_anc_northwest;
	ei_point_t* depart = (ei_point_t*)user_param;
	ei_point_t delta = {event->param.mouse.where.x - depart->x, 
			    event->param.mouse.where.y - depart->y};

        /* Gestion de l'axe de redimentionnement */
	ei_toplevel_t* top_level = (ei_toplevel_t*)widget->parent;
	if (top_level->resizable == ei_axis_x) {
		delta.y = 0;
	} else if (top_level->resizable == ei_axis_y) {
		delta.x = 0;
	}


	/* Gestion de la taille minimale */
	if (delta.x < 0 
	    && (widget->parent->screen_location.size.width - delta.x) 
	    <= top_level->min_size.width){
		delta.x = 0;
	}
	if (delta.y < 0 
	    && (widget->parent->screen_location.size.height - delta.y) 
	    <= top_level->min_size.height){
		delta.y = 0;
	}


        /* Calcul du rectangle à redessiner */
	ei_rect_t invalide = widget->parent->screen_location;
	
	if (delta.x > 0) {
		invalide.size.width += delta.x;
	}
	if (delta.y > 0) {
		invalide.size.height += delta.y;
	}

	/* On met un peu d'écart */
	invalide.top_left.y -= 2;
	invalide.top_left.x -= 2;
	invalide.size.width += 5;
	invalide.size.height += 5;

        /* On l'ajoute aux rectangles à modifier */
	ei_app_invalidate_rect(&invalide);

	int new_width = widget->parent->screen_location.size.width + delta.x;
	int new_height = widget->parent->screen_location.size.height + delta.y;
	int new_x = widget->parent->screen_location.top_left.x;
	int new_y = widget->parent->screen_location.top_left.y;

	ei_place(widget->parent, &ancre, &new_x, &new_y, 
		 &new_width, &new_height, NULL, NULL, NULL, NULL);

	/* Màj de "depart" */
	*depart = event->param.mouse.where;

	return EI_FALSE;
}

ei_bool_t end_resize(ei_widget_t* widget, ei_event_t* event, void* user_param){

	if (event->type == ei_ev_mouse_buttonup) {
		ei_unbind(ei_ev_mouse_move, widget, "all", resize_in_progress, NULL);
		ei_unbind(ei_ev_mouse_buttonup, widget, "all", end_resize, NULL);
	}
	return EI_FALSE;

}
