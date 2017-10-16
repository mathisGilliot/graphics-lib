#include <stdlib.h>
#include <stdio.h>

#include "ei_event.h"
#include "ei_event_manager.h"


void ei_bind(ei_eventtype_t		eventtype,
	      ei_widget_t*		widget,
	      ei_tag_t	         	tag,
	      ei_callback_t		callback,
	      void*			user_param){

	struct ei_bindings_t *to_bind = malloc(sizeof(struct ei_bindings_t));
	to_bind->widget = widget;
	to_bind->type = eventtype;
	to_bind->tag = tag;
	to_bind->callback = callback;
	to_bind->user_param = user_param;
	to_bind->next = NULL;
	/* Insertion en queue pour appeler en premier les traitants interne */
	register_bindings(to_bind);


}


void ei_unbind (ei_eventtype_t		eventtype,
		ei_widget_t*		widget,
		ei_tag_t		tag,
		ei_callback_t		callback,
		void*			user_param){

	/* printf("OK1\n"); */

	struct ei_bindings_t to_bind = {widget, eventtype, tag, callback, user_param, NULL};
	delete_binding(&to_bind);
	/* free(&to_bind); */
	/* printf("OK\n"); */
}
