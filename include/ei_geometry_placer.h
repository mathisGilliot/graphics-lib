#ifndef EI_GEOMETRY_PLACER_H
#define EI_GEOMETRY_PLACER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ei_types.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_widgettoplevel.h"
#include "ei_widgetbutton.h"

typedef struct widget_place_parameters {
	ei_geometry_param_t     w_param;
	ei_widget_t*            w_widget;
	ei_anchor_t		w_anchor;
	int			w_x;
	int			w_y;
	int			w_width;
	int			w_height;
	float			w_rel_x;
	float			w_rel_y;
	float			w_rel_width;
	float			w_rel_height;
	ei_bool_t               width_is_relative;
	ei_bool_t               height_is_relative;
	ei_bool_t               x_is_relative;
	ei_bool_t               y_is_relative;
} widget_place_parameters;

extern void placer_runfunc(ei_widget_t* widget);

extern void placer_releasefunc(ei_widget_t* widget);


/* alloue un espace pour le champ widget_place_parameters
 * d'un widget.
*/
extern void create_parameters(ei_widget_t* widget);


/* initialise tous les champs de widget_place_parameters
 * selon les valeurs entrées en paramètres et les valeurs
 * par défaut.
 */
extern void init_parameters(ei_widget_t*                widget,
			    ei_anchor_t*	        anchor,
			    int*		        x,
			    int*			y,
			    int*			width,
			    int*			height,
			    float*			rel_x,
			    float*			rel_y,
			    float*			rel_width,
			    float*			rel_height);


#endif
