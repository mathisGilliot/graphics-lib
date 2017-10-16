#include "ei_event_manager.h"

static struct ei_bindings_t *biblio_bindings = NULL; 

void event_analysis(struct ei_event_t* event){
	ei_bool_t callback_return = EI_FALSE;
	ei_bindings_t *current_bind = biblio_bindings;
	ei_widget_t *widget_clicked = NULL;
	
	while (current_bind != NULL){
		if (event->type == current_bind->type
		    && current_bind->tag != NULL) {
			if (strcmp(current_bind->tag, "all") == 0){
				callback_return = current_bind->callback(current_bind->widget,
									 event,
									 current_bind->user_param);
			} else if (event->type == ei_ev_keyup
				   && event->type == ei_ev_keydown) {
			} else if (strcmp(current_bind->tag, "button") == 0){
				widget_clicked = ei_widget_pick(&event->param.mouse.where);
				if (widget_clicked != NULL &&
				    strcmp(widget_clicked->wclass->name, "button") == 0){
					callback_return =
						current_bind->callback(widget_clicked,
								       event,
								       current_bind->user_param);
				}
			} else if (strcmp(current_bind->tag, "frame") == 0 ){
				widget_clicked = ei_widget_pick(&event->param.mouse.where);
				if (widget_clicked != NULL &&
				    strcmp(widget_clicked->wclass->name, "frame") == 0){
					callback_return =
						current_bind->callback(widget_clicked,
								       event,
								       current_bind->user_param);
				}
			} else if (strcmp(current_bind->tag, "toplevel") == 0 ){
				if (widget_clicked != NULL &&
				    strcmp(widget_clicked->wclass->name, "toplevel") == 0){
					widget_clicked = ei_widget_pick(&event->param.mouse.where);
					/* printf("ID event: %u\n", widget_clicked->pick_id); */
					callback_return =
						current_bind->callback(widget_clicked,
								       event,
								       current_bind->user_param);
				}
			} 
			if (callback_return == EI_TRUE) {
				break;
			}
		} else if (current_bind->widget != NULL
			   && event->type == current_bind->type
			   && event->type != ei_ev_keyup
			   && event->type != ei_ev_keydown){
			widget_clicked = ei_widget_pick(&event->param.mouse.where);
			if (widget_clicked == current_bind->widget) {
				callback_return = current_bind->callback(widget_clicked,
									 event,
									 current_bind->user_param);
			}		
		}
		current_bind = current_bind->next;
	}
}


void register_bindings(ei_bindings_t *to_bind){
	if (biblio_bindings == NULL) {
		biblio_bindings = to_bind;
	} else {
		ei_bindings_t *current = biblio_bindings;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = to_bind;
		to_bind->next = NULL;
	}
}

void delete_binding(ei_bindings_t *to_unbind){
	if (biblio_bindings == NULL) {
		return;
	}
	ei_bindings_t *current = biblio_bindings;
	ei_bindings_t *prec = NULL;
	if (to_unbind->widget == current->widget
	    && to_unbind->type == current->type
	    && to_unbind->tag == current->tag
	    && to_unbind->callback == current->callback
	    /* && to_unbind->user_param == current->user_param */){
		biblio_bindings = current->next;
		/* printf("Supprimé!\n"); */
		return;
	} else {
		prec = current;
		current = current->next;
		while (current != NULL) {
			if (to_unbind->widget == current->widget
			    && to_unbind->type == current->type
			    && to_unbind->tag == current->tag
			    && to_unbind->callback == current->callback
			    /* && to_unbind->user_param == current->user_param*/){
				prec->next = current->next;
				/* printf("Supprimé!\n"); */
				return;
			} else {
				prec = current;
				current = current->next;
			}
		}
		printf("NE PEUT PAS SUPPRIMER CE BINDING\n");
	}
}

