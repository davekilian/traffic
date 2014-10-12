
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// hash.h - Basic hashtable implementation
//

#ifndef HASH_H
#define HASH_H

#include <traffic.h>
#include "vector.h"

typedef void *tr_hash; // A hashtable
typedef unsigned int (*tr_hashfunc)(const void *data); // Hashes key data
typedef bool (*tr_equalfunc)(const void *a, const void *b); // a == b

unsigned int tr_hashfunc_str(const void *str);
unsigned int tr_hashfunc_int(const void *num);

bool tr_equalfunc_str(const void *str1, const void *str2);
bool tr_equalfunc_int(const void *num1, const void *num2);

tr_hash tr_hash_create(unsigned int keysize, 
                       unsigned int valuesize,
                       tr_hashfunc hashfunc,
                       tr_equalfunc equalfunc);

tr_err tr_hash_delete(tr_hash hash);

bool tr_hash_contains(tr_hash hash, const void *key);
void *tr_hash_get(tr_hash hash, const void *key);

tr_err tr_hash_set(tr_hash hash, const void *key, const void *value);
tr_err tr_hash_clear(tr_hash, const void *key);

unsigned int tr_hash_num_keys(tr_hash hash);
tr_vector tr_hash_keys(tr_hash hash);
tr_vector tr_hash_values(tr_hash hash);


tr_hash tr_strhash_create(unsigned int itemsize);
tr_err tr_strhash_delete(tr_hash hash);

bool tr_strhash_contains(tr_hash hash, const char *key);
void *tr_strhash_get(tr_hash hash, const char *key);

tr_err tr_strhash_set(tr_hash hash, const char *key, void *value);
tr_err tr_strhash_clear(tr_hash, const char *key);

unsigned int tr_strhash_num_keys(tr_hash hash);
tr_vector tr_strhash_keys(tr_hash hash);
tr_vector tr_strhash_values(tr_hash hash);


tr_hash tr_inthash_create(unsigned int itemsize);
tr_err tr_inthash_delete(tr_hash hash);

bool tr_inthash_contains(tr_hash hash, int key);
void *tr_inthash_get(tr_hash hash, int key);

tr_err tr_inthash_set(tr_hash hash, int key, void *value);
tr_err tr_inthash_clear(tr_hash, int key);

unsigned int tr_inthash_num_keys(tr_hash hash);
tr_vector tr_inthash_keys(tr_hash hash);
tr_vector tr_inthash_values(tr_hash hash);

#endif
