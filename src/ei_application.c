#include <stdio.h>

#include "hw_interface.h"
#include "ei_application.h"
#include "ei_widgetclass.h"
#include "ei_geometry_placer.h"
#include "ei_event.h"
#include "ei_event_manager.h"
#include "ei_widgetframe.h"



static ei_widget_t* widget_racine = NULL;
ei_surface_t main_window;
ei_surface_t pick_surface;
ei_linked_rect_t* list_rects = NULL;
static ei_bool_t no_quit_request = EI_TRUE;

void dessin(ei_widget_t* widget, ei_rect_t* rect);
void free_list(ei_linked_rect_t** list);


void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen) {

	/* Initialisation du matériel */
	hw_init();

	/* Enregistrement des classes */
	ei_frame_register_class();
	ei_button_register_class();
	ei_toplevel_register_class();

	/* Enregistrement des geometry_manager */
	ei_register_placer_manager();

	/* Création de la fenêtre principale */
	main_window = hw_create_window(main_window_size, fullscreen);

	/* Création de la pick_surface */
	hw_surface_lock(main_window);
	pick_surface = hw_surface_create(main_window, main_window_size, fullscreen);
	hw_surface_unlock(main_window);


	/* Définition de la racine comme un frame */
	widget_racine = ei_widget_create("frame", NULL);
	
	/* On met a jour le champ size pour que le fils ait accès
	 * au screen location, dans le cas du relatif
	 */
	widget_racine->screen_location.size = *main_window_size;
}

void ei_app_free() {
	ei_widget_destroy(widget_racine);
	hw_quit();
}

/* Dessin récursif de la hiérarchie des widgets si dans le rectangle de clipping */
void dessin(ei_widget_t* widget, ei_rect_t* rect) {

	if (widget == NULL) {
		return;
	}

	/* Dessin du widget */
	widget->wclass->drawfunc(widget, main_window, pick_surface, rect);

	/* Dessin des fils */
	dessin(widget->children_head,rect);

	/* Dessin des 2 boutons si le widget est une toplevel */
	if (strcmp(widget->wclass->name, "toplevel") == 0 ){
		ei_toplevel_t* top = (ei_toplevel_t*)widget;
		if (top->closable) {
			ei_widget_t* fils_bouton = top->closing_button;
			fils_bouton->wclass->drawfunc(fils_bouton, main_window, pick_surface, rect);
		}
		
		ei_widget_t* fils_frame = top->resizing_frame;
		fils_frame->wclass->drawfunc(fils_frame, main_window, pick_surface, rect);
	}

        /* Dessin des frères */
	dessin(widget->next_sibling, rect);

}


void free_list(ei_linked_rect_t** list) {
	ei_linked_rect_t* suiv = *list;
	while (suiv != NULL) {
		suiv = (*list)->next;
		free(*list);
		*list = suiv;
	}		
}

void ei_app_run() {
	ei_linked_rect_t* current = list_rects;
	ei_rect_t rect = hw_surface_get_rect(main_window);

	struct ei_event_t* event = malloc(sizeof(ei_event_t));

	/* Initialisation: parcours de la hiérachie récursivement */
	hw_surface_lock(main_window);
	hw_surface_lock(pick_surface);
	widget_racine->wclass->drawfunc(widget_racine, main_window, pick_surface, &rect);

	if (widget_racine->children_head != NULL) {
		dessin(widget_racine->children_head, &rect);
	}
	hw_surface_unlock(main_window);
	hw_surface_unlock(pick_surface);
	hw_surface_update_rects(pick_surface, NULL);
	hw_surface_update_rects(main_window, NULL);

	/* Fin initialisation */

	/*tant qu'on ne quitte pas*/
	while (no_quit_request == EI_TRUE){
		/* Attente d'un event */
		hw_event_wait_next(event);
		/* Analyse d'un event */
		event_analysis(event);
		/* Redessin */
		hw_surface_lock(main_window);
		hw_surface_lock(pick_surface);
		current = list_rects;
		while (current != NULL) {
			dessin(widget_racine, &(current->rect));
			current = current->next;
		}
		/* Libération surfaces dessin */
		hw_surface_unlock(main_window);
		hw_surface_unlock(pick_surface);
		hw_surface_update_rects(pick_surface, NULL);
		hw_surface_update_rects(main_window, NULL);

		/* Libération de la liste des rectangles à dessiner */
		free_list(&list_rects);
	}
}


void ei_app_invalidate_rect(ei_rect_t* rect) {
	ei_linked_rect_t* courant = list_rects;
	if (list_rects == NULL){
		list_rects = malloc(sizeof(ei_linked_rect_t));
		list_rects->rect = *rect;
		list_rects->next = NULL;
	} else {
		while(courant->next != NULL) {
			courant = courant->next;
		}
		courant->next = malloc(sizeof(ei_linked_rect_t));
		courant->next->rect = *rect;
		courant->next->next = NULL;
	}
}


void ei_app_quit_request() {
	no_quit_request = EI_FALSE;
}


ei_widget_t* ei_app_root_widget() {
	return widget_racine;
}


ei_surface_t ei_app_root_surface() {
	return pick_surface;
}

