#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ei_widgetclass.h"
#include "ei_widget.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widgetframe.h"
#include "ei_widgetbutton.h"
#include "ei_widgettoplevel.h"

static struct ei_widgetclass_t* biblio_widgetclass = NULL;

ei_widgetclass_t* ei_widgetclass_from_name (ei_widgetclass_name_t name){

	if (biblio_widgetclass == NULL) {
		printf("AUCUNE WIDGETCLASS ENREGISTRE \n");
		return NULL;
	} else {
		ei_widgetclass_t* current = biblio_widgetclass;
		while (current != NULL) {
			if (strncmp(current->name, name, 20) == 0) {
				return current;
			}
			current = current->next;
		}
		printf("AUCUN WIDGETCLASS ASSOCIE AU" 
		       " NOM %s\n", name);
		return NULL;
	}
}		

void ei_widgetclass_register (ei_widgetclass_t* widgetclass){
	widgetclass->next = biblio_widgetclass;
	biblio_widgetclass = widgetclass;
}


void ei_frame_register_class () {
	ei_widgetclass_t *class_frame = malloc(sizeof(ei_widgetclass_t));
	char* nom = "frame";
	strncpy(class_frame->name, nom, 20);
	class_frame->allocfunc= frame_allocfunc;
	class_frame->releasefunc = frame_releasefunc;
	class_frame->drawfunc= frame_drawfunc;
	class_frame->setdefaultsfunc = frame_setdefaultsfunc;
	class_frame->geomnotifyfunc = frame_geomnotifyfunc;
	ei_widgetclass_register(class_frame);
}


void ei_button_register_class () {
	ei_widgetclass_t *class_button = malloc(sizeof(ei_widgetclass_t));
	char* nom = "button";
	strncpy(class_button->name, nom, 20);
	class_button->allocfunc= button_allocfunc;
	class_button->releasefunc = button_releasefunc;
	class_button->drawfunc= button_drawfunc;
	class_button->setdefaultsfunc = button_setdefaultsfunc;
	class_button->geomnotifyfunc = button_geomnotifyfunc;
	ei_widgetclass_register(class_button);

	/* enregistrement de traitant interne */
	ei_bind(ei_ev_mouse_buttondown, NULL, "button", &callback_press_button, NULL);
}

void ei_toplevel_register_class () {
	ei_widgetclass_t *class_toplevel = malloc(sizeof(ei_widgetclass_t));
	char* nom = "toplevel";
	strncpy(class_toplevel->name, nom, 20);
	class_toplevel->allocfunc= toplevel_allocfunc;
	class_toplevel->releasefunc = toplevel_releasefunc;
	class_toplevel->drawfunc= toplevel_drawfunc;
	class_toplevel->setdefaultsfunc = toplevel_setdefaultsfunc;
	class_toplevel->geomnotifyfunc = toplevel_geomnotifyfunc;
	ei_widgetclass_register(class_toplevel);

        /* enregistrement de traitant interne */
	ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel", &callback_move_toplevel, NULL);
	ei_bind(ei_ev_mouse_buttondown, NULL, "frame", &callback_resizing, NULL);
}
