#include "ei_draw_utilities.h"

ei_linked_point_t* arc(ei_point_t centre, float rayon, float angle_debut, float angle_fin){
	int compteur = 2;
	ei_linked_point_t* premier_point = malloc(sizeof(ei_linked_point_t));
	premier_point->point.x = centre.x + rayon*cos(angle_debut);
	premier_point->point.y = centre.y + rayon*sin(angle_debut);

	ei_linked_point_t* dernier_point = malloc(sizeof(ei_linked_point_t));
	dernier_point->point.x = centre.x + rayon*cos(angle_fin);
	dernier_point->point.y = centre.y + rayon*sin(angle_fin);
	dernier_point->next = NULL;
	
	ei_linked_point_t* point_courant = premier_point;
	
	for (float compteur_angle = 1.0; compteur_angle < 100.0; compteur_angle ++){
		ei_linked_point_t* new_point =  malloc(sizeof(ei_linked_point_t));
		new_point->point.x = centre.x + rayon*cos(angle_debut + compteur_angle*((angle_fin - angle_debut) / 100.0));
		new_point->point.y = centre.y + rayon*sin(angle_debut + compteur_angle*((angle_fin - angle_debut) / 100.0));
		point_courant->next = new_point;
		point_courant = point_courant->next;
		compteur += 1;		
	}

	point_courant->next = dernier_point;
	return premier_point;
}

ei_linked_point_t* rounded_frame(ei_rect_t rectangle, float rayon, ei_bool_t haut, ei_linked_point_t* centre_hg, ei_linked_point_t* centre_hd, ei_linked_point_t* centre_bg) {
		
	int width = rectangle.size.width;
	int height = rectangle.size.height;
	int h = (width > height) ? (height/2) : (width/2);
	
	ei_linked_point_t* first_middle_point =  malloc(sizeof(ei_linked_point_t)); 
	first_middle_point->point.x = rectangle.top_left.x + rectangle.size.width - h; 
	first_middle_point->point.y = rectangle.top_left.y + h; 
	
	ei_linked_point_t* second_middle_point =  malloc(sizeof(ei_linked_point_t));
	second_middle_point->point.x = rectangle.top_left.x + h;
	second_middle_point->point.y = rectangle.top_left.y + h;
	
	if (haut == EI_TRUE) {

		ei_linked_point_t* liste_hg = malloc(sizeof(ei_linked_point_t));
		centre_hg->point.x = rectangle.top_left.x + (int)rayon;
		centre_hg->point.y = rectangle.top_left.y + (int)rayon;
		liste_hg = arc(centre_hg->point, rayon, PI, 1.5*PI);

		ei_linked_point_t* point_courant = liste_hg;
	
		while (point_courant->next != NULL) {
			point_courant = point_courant->next;
		}

		ei_linked_point_t* liste_hd = malloc(sizeof(ei_linked_point_t));
		centre_hd->point.x = rectangle.top_left.x + rectangle.size.width - (int)rayon;
		centre_hd->point.y = rectangle.top_left.y + (int)rayon;
		liste_hd = arc(centre_hd->point, rayon, 1.5*PI, 1.75*PI);
		point_courant->next = liste_hd;

		while (point_courant->next != NULL) {
			point_courant = point_courant->next;
		}
	
		point_courant->next = first_middle_point;
	
		first_middle_point->next = second_middle_point;

		ei_linked_point_t* liste_bg = malloc(sizeof(ei_linked_point_t));

		centre_bg->point.x = rectangle.top_left.x + (int)rayon;
		centre_bg->point.y = rectangle.top_left.y + rectangle.size.height - (int)rayon;
		liste_bg = arc(centre_bg->point, rayon, 0.75*PI, PI);
		second_middle_point->next = liste_bg;

		return liste_hg;

	} else {
		
		ei_linked_point_t* liste_hd = malloc(sizeof(ei_linked_point_t));
		centre_hd->point.x = rectangle.top_left.x + rectangle.size.width - (int)rayon;
		centre_hd->point.y = rectangle.top_left.y + (int)rayon;
		liste_hd = arc(centre_hd->point, rayon, 2.0*PI, 1.75*PI);

		ei_linked_point_t* point_courant = liste_hd;
		
		while (point_courant->next != NULL) {
			point_courant = point_courant->next;
		}

		point_courant->next = first_middle_point;
    
		first_middle_point->next = second_middle_point;

		ei_linked_point_t* liste_bg = malloc(sizeof(ei_linked_point_t));
		centre_bg->point.x = rectangle.top_left.x + (int)rayon;
		centre_bg->point.y = rectangle.top_left.y + rectangle.size.height - (int)rayon;
		liste_bg = arc(centre_bg->point, rayon, 0.75*PI, 0.5*PI);
		second_middle_point->next = liste_bg;

		while (point_courant->next != NULL) {
			point_courant = point_courant->next;
		}

		
		ei_linked_point_t* liste_bd = malloc(sizeof(ei_linked_point_t));
		ei_point_t centre_bd;
		centre_bd.x = rectangle.top_left.x + rectangle.size.width - (int)rayon;
		centre_bd.y = rectangle.top_left.y + rectangle.size.height - (int)rayon;
		liste_bd = arc(centre_bd, rayon, 0.5*PI, 0.0);
		point_courant->next = liste_bd;

		return liste_hd;


	}
}

void intersection_rect(ei_widget_t* widget, ei_rect_t** clipper) {
	ei_rect_t* cont_rect = widget->parent->content_rect;

	if ((*clipper)->top_left.x < cont_rect->top_left.x) {
		(*clipper)->size.width -= (cont_rect->top_left.x) - ((*clipper)->top_left.x);
		(*clipper)->top_left.x = cont_rect->top_left.x;
	}
	if ((*clipper)->top_left.y < cont_rect->top_left.y) {
		(*clipper)->size.height -= (cont_rect->top_left.y) - ((*clipper)->top_left.y);
		(*clipper)->top_left.y = cont_rect->top_left.y;
	}
	if ((*clipper)->top_left.x + (*clipper)->size.width > cont_rect->top_left.x + cont_rect->size.width) {
		(*clipper)->size.width -= ((*clipper)->top_left.x + (*clipper)->size.width) - (cont_rect->top_left.x + cont_rect->size.width);
	}
	if ((*clipper)->top_left.y + (*clipper)->size.height > cont_rect->top_left.y + cont_rect->size.height) {
		(*clipper)->size.height -= ((*clipper)->top_left.y + (*clipper)->size.height) - (cont_rect->top_left.y + cont_rect->size.height);
	}
}


