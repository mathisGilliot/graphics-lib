#ifndef _EI_ARRONDI_H_
#define _EI_ARRONDI_H_

#include <stdlib.h>
#include <math.h> 
#include <stdio.h>

#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_widgetclass.h"
#include "ei_widget.h"
#ifndef PI
#define PI 3.14159265358979323846
#endif 

ei_linked_point_t* arc(ei_point_t centre, 
		       float rayon, 
		       float angle_debut, 
		       float angle_fin);

ei_linked_point_t* rounded_frame(ei_rect_t rectangle, float rayon, 
				 ei_bool_t haut, ei_linked_point_t* centre_hg, 
				 ei_linked_point_t* centre_hd, 
				 ei_linked_point_t* centre_bg);

void intersection_rect(ei_widget_t* widget, ei_rect_t** clipper);

#endif
