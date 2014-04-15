
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// list.h - Basic doubly-linked list implementation
//

#ifndef LIST_H
#define LIST_H

#include <traffic.h>

typedef void *tr_list; // A doubly-linked list

tr_list tr_list_create(unsigned int itemsize);
tr_err tr_list_delete(tr_list);

bool tr_list_empty(tr_list);

void *tr_list_first(tr_list list);
void *tr_list_last(tr_list list);

void *tr_list_next(void *value);
void *tr_list_prev(void *value);

tr_err tr_list_add(tr_list list, void *item);
tr_err tr_list_add_before(void *other, void *item);
tr_err tr_list_add_after(void *other, void *item);
tr_err tr_list_prepend(tr_list list, void *item);
tr_err tr_list_append(tr_list list, void *item);
tr_err tr_list_remove(void *item);
tr_err tr_list_remove_first(tr_list list);
tr_err tr_list_remove_last(tr_list list);

#define tr_list_foreach(type, var, list)        \
    for (type var = (type)tr_list_first(list)   \
         var; var = (type)tr_list_next(var))
#endif
