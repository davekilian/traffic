
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// set.h - Basic hashset implementation
//

#include <stdlib.h> // for NULL

#include "set.h"

tr_set tr_set_create(unsigned int keysize, 
                     tr_hashfunc hashfunc,
                     tr_equalfunc equalfunc)
{
    return tr_hash_create(keysize, 0, hashfunc, equalfunc);
}

tr_err tr_set_delete(tr_set set)
{
    return tr_hash_delete(set);
}

unsigned int tr_set_contains(tr_set set, const void *key)
{
    return tr_hash_contains(set, key);
}

tr_err tr_set_add(tr_set set, const void *key)
{
    return tr_hash_set(set, key, NULL);
}

tr_err tr_set_remove(tr_set set, const void *key)
{
    return tr_hash_clear(set, key);
}

tr_vector tr_set_items(tr_set set)
{
    return tr_hash_keys(set);
}

tr_set tr_strset_create()
{
    return tr_strhash_create(0);
}

tr_err tr_strset_delete(tr_set set)
{
    return tr_strhash_delete(set);
}

unsigned int tr_strset_contains(tr_set set, const char *key)
{
    return tr_strhash_contains(set, key);
}

tr_err tr_strset_add(tr_set set, const char *key)
{
    return tr_strhash_set(set, key, NULL);
}

tr_err tr_strset_remove(tr_set set, const char *key)
{
    return tr_strhash_clear(set, key);
}

tr_vector tr_strset_items(tr_set set)
{
    return tr_strhash_keys(set);
}

tr_set tr_intset_create()
{
    return tr_inthash_create(0);
}

tr_err tr_intset_delete(tr_set set)
{
    return tr_inthash_delete(set);
}

unsigned int tr_intset_contains(tr_set set, int key)
{
    return tr_inthash_contains(set, key);
}

tr_err tr_intset_add(tr_set set, int key)
{
    return tr_inthash_set(set, key, NULL);
}

tr_err tr_intset_remove(tr_set set, int key)
{
    return tr_inthash_clear(set, key);
}

tr_vector tr_intset_items(tr_set set)
{
    return tr_inthash_keys(set);
}
