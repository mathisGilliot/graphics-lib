#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ei_types.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_geometry_placer.h"

static struct ei_geometrymanager_t *biblio_geometrie = NULL;

/* Réalise une "insertion en tête" du gestionnaire dans la liste */
void ei_geometrymanager_register(ei_geometrymanager_t* geometrymanager){
	geometrymanager->next = biblio_geometrie;
	biblio_geometrie = geometrymanager;
}


ei_geometrymanager_t* ei_geometrymanager_from_name(ei_geometrymanager_name_t name){
	if (biblio_geometrie == NULL) {
		printf("AUCUN GESTIONNAIRE DE GEOMETRIE ENREGISTRE \n");
		return NULL;
	} else {
		ei_geometrymanager_t* current = biblio_geometrie;
		while (current != NULL) {
			if (strncmp(current->name, name, 20) == 0) {
				return current;
			}
			current = current->next;
		}
		printf("AUCUN GESTIONNAIRE DE GEOMETRIE ASSOCIE AU" 
		       " NOM %s\n", name);
		return NULL;
	}
}


void ei_geometrymanager_unmap (ei_widget_t* widget){
}

void ei_register_placer_manager (){
	struct ei_geometrymanager_t *placeur = malloc(sizeof(struct ei_geometrymanager_t));
	ei_geometrymanager_name_t nom = "placeur";
	strncpy(placeur->name, nom, 20);
	placeur->runfunc = placer_runfunc;
	placeur->releasefunc = placer_releasefunc;
	placeur->next = NULL;
	ei_geometrymanager_register(placeur);
}


void ei_place (ei_widget_t*             widget,
	       ei_anchor_t*		anchor,
	       int*			x,
	       int*			y,
	       int*			width,
	       int*			height,
	       float*			rel_x,
	       float*			rel_y,
	       float*			rel_width,
	       float*			rel_height){

	if (widget->geom_params != NULL
	    && strcmp(widget->geom_params->manager->name, "placeur") == 0) {
		free(widget->geom_params);
		create_parameters(widget);
	} else if (widget->geom_params == NULL) {
		create_parameters(widget);
	}

	init_parameters(widget, anchor, x, y, width, height, rel_x, rel_y, rel_width, rel_height);
	placer_runfunc(widget);
}


