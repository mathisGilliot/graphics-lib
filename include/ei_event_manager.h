#ifndef EI_EVENT_MANAGER_H
#define EI_EVENT_MANAGER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "ei_event.h"
#include "ei_application.h"
#include "ei_widget.h"
#include "ei_widgetbutton.h"


typedef struct ei_bindings_t {
	ei_widget_t*          widget;
	ei_eventtype_t        type;
	ei_tag_t	      tag;
	ei_callback_t         callback;
	void*                 user_param;
	struct ei_bindings_t* next;
} ei_bindings_t;

extern void event_analysis(struct ei_event_t* event);

extern void register_bindings(ei_bindings_t *to_bind);

extern void delete_binding(ei_bindings_t *to_unbind);

#endif
