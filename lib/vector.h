
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// vector.h - Basic array-list implementation
//

#ifndef VECTOR_H
#define VECTOR_H

#include <traffic.h>

typedef void *tr_vector; // An array-list

tr_vector tr_vec_create(unsigned int itemsize, unsigned int capacity);
tr_err tr_vec_delete(tr_vector vec);

unsigned int tr_vec_capacity(tr_vector vec);

unsigned int tr_vec_size(tr_vector vec);
unsigned int tr_vec_count(tr_vector vec);
unsigned int tr_vec_length(tr_vector vec);

tr_err tr_vec_resize(tr_vector vec, unsigned int capacity);

void *tr_vec_item(tr_vector vec, unsigned int index);
void *tr_vec_items(tr_vector vec);

tr_err tr_vec_append(tr_vector vec, void *item);
tr_err tr_vec_prepend(tr_vector vec, void *item);
tr_err tr_vec_insert(tr_vector vec, unsigned int index, void *item);
tr_err tr_vec_remove(tr_vector vec, void *item);
tr_err tr_vec_remove_at(tr_vector vec, unsigned int index);

tr_err tr_vec_push(tr_vector vec, void *item);
void *tr_vec_peek(tr_vector vec);
tr_err tr_vec_pop(tr_vector vec);

#define tr_vec_foreach(type, var, vec)                  \
    for (type var = (type)tr_vec_foreach_first(vec);    \
         !tr_vec_foreach_finished(var);                 \
         var = tr_vec_foreach_next(var))

void *tr_vec_foreach_first(tr_vector vec);
bool  tr_vec_foreach_finished(void *item);
void *tr_vec_foreach_next(void *item);

#endif
