
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// set.h - Basic hashset implementation
//

#ifndef SET_H
#define SET_H

#include <traffic.h>
#include "hash.h" // tr_hashfunc and tr_equalfunc

typedef void *tr_set; // A hashset

tr_set tr_set_create(unsigned int keysize, 
                     tr_hashfunc hashfunc,
                     tr_equalfunc equalfunc);

tr_err tr_set_delete(tr_set set);

unsigned int tr_set_contains(tr_set set, const void *key);
tr_err tr_set_add(tr_set set, const void *key);
tr_err tr_set_remove(tr_set, const void *key);

tr_vector tr_set_items(tr_set set);


tr_set tr_strset_create();
tr_err tr_strset_delete(tr_set set);

unsigned int tr_strset_contains(tr_set set, const char *key);
tr_err tr_strset_add(tr_set set, const char *key);
tr_err tr_strset_remove(tr_set, const char *key);

tr_vector tr_strset_items(tr_set set);


tr_set tr_intset_create();
tr_err tr_intset_delete(tr_set set);

unsigned int tr_intset_contains(tr_set set, int key);
tr_err tr_intset_add(tr_set set, int key);
tr_err tr_intset_remove(tr_set, int key);

tr_vector tr_intset_items(tr_set set);

#endif
