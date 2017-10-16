#include <stdio.h>
#include <stdlib.h>

#include "ei_widgetframe.h"
#include "ei_application.h"

static ei_color_t frame_pick_color = {1, 0, 0, 255};

void* frame_allocfunc() {
	void* espace = calloc(1, sizeof(struct ei_frame_t)); 
	return espace;
}


void frame_releasefunc(struct ei_widget_t* widget) {
	ei_frame_t* widget_frame = (ei_frame_t*)widget;

	/* Gestion de la méoire très superficielle... */

	if (widget == NULL) { 
		return; 
	} 

	free(widget_frame);
}

void frame_drawfunc(struct ei_widget_t* widget, ei_surface_t surface,
		    ei_surface_t pick_surface, ei_rect_t* clipper) {

	/* Dessin du relief */

	/* On utilise 6 points pour former 2 polygones */
	ei_linked_point_t* point_hg = malloc(sizeof(ei_linked_point_t));
	ei_linked_point_t* point_hd = malloc(sizeof(ei_linked_point_t));
	ei_linked_point_t* milieu2 = malloc(sizeof(ei_linked_point_t));
	ei_linked_point_t* milieu1 = malloc(sizeof(ei_linked_point_t));
	ei_linked_point_t* point_bg = malloc(sizeof(ei_linked_point_t));
	ei_linked_point_t* point_bd = malloc(sizeof(ei_linked_point_t));

	
	/* On calcule l'intersection du clipper et du content_rect du parent*/
	if (widget != ei_app_root_widget()) {
	        if (strcmp(widget->parent->wclass->name, "toplevel") == 0) {
			ei_toplevel_t *father = (ei_toplevel_t *)widget->parent;
			if (father->resizing_frame != widget)  {
				intersection_rect(widget, &clipper);
			}
		} else {
			intersection_rect(widget, &clipper);
		}
	}

	/* Dessin 1er polygone */
	dessiner_polygone(surface, widget, point_hg, point_hd, milieu2, 
			  milieu1, point_bg, point_bd, clipper);
	
	/* Dessin 2nd polygone */
	dessiner_polygone(surface, widget, point_hg, point_hd, milieu2, 
			  milieu1, point_bg, point_bd, clipper);
	
	/* Dessin du rectangle interne */
	dessiner_rectangle(surface, widget, point_hg, point_hd, point_bg, point_bd, clipper);

	/* Écriture du texte */
	ajout_text_frame(surface, widget, point_hg, point_hd, point_bg, clipper);

	/* Dessin de l'image */
	ajout_img_frame(surface, widget, point_hg, point_hd, point_bg, EI_FALSE, clipper);

	/* Dessin de la pick surface */
	ei_draw_polygon(pick_surface, point_hg, *(widget->pick_color), clipper);

	/* Libération des points */
	free(point_hg);
	free(point_hd);
	free(milieu2);
	free(milieu1);
	free(point_bg);
	free(point_bd);
}
	
void frame_setdefaultsfunc(struct ei_widget_t* widget) {
	
  	frame_pick_color.red += 1;
	widget->pick_id = ei_map_rgba(ei_app_root_surface(), &frame_pick_color);
	widget->pick_color = malloc(sizeof(ei_color_t));
	*(widget->pick_color) = frame_pick_color;

	/* Widget Hierachy Management */
	widget->children_head = NULL;
	widget->children_tail = NULL;
	widget->next_sibling = NULL;	

	/* Geometry Management */
	widget->geom_params = NULL;
	(widget->requested_size).width = 0;
	(widget->requested_size).height = 0;
	(widget->screen_location).top_left.x = 0;
	(widget->screen_location).top_left.y = 0;
	(widget->screen_location).size.width = 300;
	(widget->screen_location).size.height = 200;
	widget->content_rect = &(widget->screen_location);
	ei_frame_t* widget_frame = (ei_frame_t*)widget;
	widget_frame->color = ei_default_background_color;
	widget_frame->border_width = 0;
	widget_frame->relief = ei_relief_none;
	widget_frame->text= NULL;	
	widget_frame->text_font= ei_default_font;	
	widget_frame->text_color= ei_font_default_color;	
	widget_frame->text_anchor= ei_anc_center;	
	widget_frame->img= NULL;
	widget_frame->img_rect= NULL;
	widget_frame->img_anchor= ei_anc_center;
}

void frame_geomnotifyfunc (struct ei_widget_t* widget, ei_rect_t rect) {
	if (widget != NULL) {
		widget->screen_location = rect;
	}
}

void dessiner_polygone (ei_surface_t surface, ei_widget_t* widget, 
			ei_linked_point_t* point_hg, ei_linked_point_t* point_hd, 
			ei_linked_point_t* milieu2, ei_linked_point_t* milieu1, 
			ei_linked_point_t* point_bg, ei_linked_point_t* point_bd, 
			ei_rect_t* clipper) {

	int width = (widget->screen_location).size.width;
	int height = (widget->screen_location).size.height;
	int h = (width > height) ? (height/2) : (width/2);
	
	/* 1er point */
	point_hg->point = widget->screen_location.top_left;
	
	/* 2eme point */
	point_hg->next = point_hd;
	point_hd->point.x = point_hg->point.x + (widget->screen_location).size.width;
	point_hd->point.y = point_hg->point.y;
	
	/* 3eme point */
	point_hd->next = milieu2;
	milieu2->point.x = point_hd->point.x - h;
	milieu2->point.y = point_hd->point.y + h;
	
	/* 4eme point */
	milieu2->next = milieu1;
	milieu1->point.x = point_hg->point.x + h;
	milieu1->point.y = point_hg->point.y + h;

	/* 5eme point */
	milieu1->next = point_bg;
	point_bg->point.x = point_hg->point.x;
	point_bg->point.y = point_hg->point.y + (widget->screen_location).size.height;

	point_bg->next = NULL;
	
	point_bd->point.x = point_hg->point.x + (widget->screen_location).size.width;
	point_bd->point.y = point_hg->point.y + (widget->screen_location).size.height;
	point_bd->next = NULL;
	
	dessiner_relief(surface, widget, point_hg, point_hd, point_bg, point_bd, clipper);
}

void dessiner_relief (ei_surface_t surface, ei_widget_t* widget, ei_linked_point_t* point_hg, ei_linked_point_t* point_hd, 
		      ei_linked_point_t* point_bg, ei_linked_point_t* point_bd, 
		      ei_rect_t* clipper) {
	
	ei_frame_t* widget_frame = (ei_frame_t*)widget;

	/* Définition d'une couleur plus foncée */
	ei_color_t dark = widget_frame->color;
	dark.red = (0.8 * dark.red - 40 < 0) ? 0 :  0.8 * dark.red - 40;
	dark.blue = (0.8 * dark.blue - 40 < 0) ? 0 :  0.8 * dark.blue - 40;
	dark.green = (0.8 * dark.green - 40 < 0) ? 0 :  0.8 * dark.green - 40 ;
	
	/* Définition d'une couleur plus claire */
	ei_color_t bright = widget_frame->color;
	bright.red = (1.2 * bright.red + 40 > 255) ? 255 : 1.2 * bright.red + 40;
	bright.blue = (1.2 * bright.blue + 40 > 255) ? 255 : 1.2 * bright.blue + 40;
	bright.green = (1.2 * bright.green + 40 > 255) ? 255 : 1.2 * bright.green + 40;

	/* Dessin effectif avec rechaînage selon le relief */
	switch (widget_frame->relief) {
	case ei_relief_none:
		point_hd->next = point_bd;
		point_bd->next = point_bg;
		ei_draw_polygon(surface, point_hg, widget_frame->color, clipper);
		break;
		/* relief relevé ie foncé en bas */
	case ei_relief_raised:
		ei_draw_polygon(surface, point_hg, bright, clipper);
		point_bg->next = point_bd;
		ei_draw_polygon(surface, point_hd, dark, clipper);
		break;
		/* relief enfoncé ie foncé en haut */
	case ei_relief_sunken:
		ei_draw_polygon(surface, point_hg, dark, clipper);
		point_bg->next = point_bd;
		ei_draw_polygon(surface, point_hd, bright, clipper);
		break;
	}
}


void dessiner_rectangle (ei_surface_t surface, ei_widget_t* widget,
			 ei_linked_point_t* point_hg, ei_linked_point_t* point_hd, 
			 ei_linked_point_t* point_bg, ei_linked_point_t* point_bd, 
			 ei_rect_t* clipper) {

	ei_frame_t* widget_frame = (ei_frame_t*)widget;
	
	/* 1er point */
	point_hg->point.x += widget_frame->border_width;
	point_hg->point.y += widget_frame->border_width;
	
	/* 2eme point */
	point_hd->point.x -= widget_frame->border_width;
	point_hd->point.y += widget_frame->border_width;

	/* 6eme point */
	point_hd->next = point_bd;
	point_bd->point.x -= widget_frame->border_width;
	point_bd->point.y -= widget_frame->border_width;
	
	/* 5eme point */
	point_bd->next = point_bg;
	point_bg->point.x += widget_frame->border_width;
	point_bg->point.y -= widget_frame->border_width;
	
	/* NE PAS TOUCHER */
	point_bg->next = NULL;

	ei_draw_polygon(surface, point_hg, widget_frame->color, clipper);
	
}

void ajout_text_frame(ei_surface_t surface, ei_widget_t* widget,
ei_linked_point_t* point_hg, ei_linked_point_t* point_hd, 
ei_linked_point_t* point_bg, ei_rect_t* clipper) {

	ei_frame_t* widget_frame = (ei_frame_t*)widget;

	int width_text;
	int height_text;
	ei_point_t point;

	if (widget_frame->text != NULL) {
		/* Récupération de la taille du texte */
		hw_text_compute_size(widget_frame->text, widget_frame->text_font, &width_text, &height_text);
		/* Gestion de l'ancrage */
		anchor_manager(widget_frame->text_anchor, &point, point_hg, point_hd, point_bg, width_text, height_text);
		/* Écriture effectif du texte */
		ei_draw_text(surface, &point, widget_frame->text, widget_frame->text_font, &(widget_frame->text_color), clipper);
	}
}

void ajout_img_frame(ei_surface_t surface, ei_widget_t* widget, ei_linked_point_t* point_hg, ei_linked_point_t* point_hd, 
		     ei_linked_point_t* point_bg, ei_bool_t alpha, ei_rect_t* clipper) {
	ei_frame_t* widget_frame = (ei_frame_t*)widget;
	int failure;
	ei_rect_t* dest_rect = malloc(sizeof(ei_rect_t));
	ei_rect_t* src_rect = malloc(sizeof(ei_rect_t));
	ei_rect_t* cont_rect;
	/* Attention : la racine est un frame */
	if (widget->parent != NULL) {
		cont_rect = widget->parent->content_rect;
	}

	if (widget_frame->img != NULL) {
		/* Lock avant dessin */
		hw_surface_lock(widget_frame->img);
		
		/* Initialisation de dest_rect et src_rect
		   pour qu'ils aient les mêmes dimensions que l'image */
		*dest_rect = *(widget_frame->img_rect);
		*src_rect = *(widget_frame->img_rect);

		/* Pour que l'image soit bien située, on fait une translation */
		dest_rect->top_left.x += cont_rect->top_left.x;
		dest_rect->top_left.y += cont_rect->top_left.y;

		/* Gestion d'une éventuelle sortie du parent */
		intersection_rect(widget, &dest_rect);
		/* Redimension du rectangle source en cas de changement */
		src_rect->size = dest_rect->size;

		/* Gestion de l'ancrage */
		anchor_manager(widget_frame->img_anchor, &(dest_rect->top_left), point_hg, point_hd, point_bg, widget_frame->img_rect->size.width, widget_frame->img_rect->size.height);

		failure = ei_copy_surface (surface,
					   dest_rect,
					   widget_frame->img,
					   src_rect,
					   alpha);
		/* En cas d'erreur, le signaler */
		if (failure) {
			printf("\nCopie impossible\nVérifiez les dimensions du frame et de l'image\n");
		}
		/* Dessin terminé -> libération */
		hw_surface_unlock(widget_frame->img);
	}
}

void anchor_manager (ei_anchor_t anchor, ei_point_t* point_ancr, ei_linked_point_t* point_hg, ei_linked_point_t* point_hd, 
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
