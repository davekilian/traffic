
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// hash.h - Basic hashtable implementation
//

#include <assert.h> // for assert()
#include <stdlib.h> // for NULL
#include <string.h> // for memset

#include "hash.h"
#include "memory.h"
#include "vector.h"

struct _hashitem;
struct _hashtable;

typedef struct _hashitem hashitem;
typedef struct _hashtable hashtable;

struct _hashitem
{
    unsigned int occupied;  // Boolean indicating whether slot is in use
    hashitem *next;         // Next item that shares the same hash value
    // Next hashtable->keysize bytes contain the key
    // Then hashtable->valuesize bytes containing the value
};

struct _hashtable
{
    unsigned int keysize;   // Size of each key, in bytes
    unsigned int valuesize; // Size of each value, in bytes

    tr_hashfunc hashfunc;   // Uniformly hashes input keys
    tr_equalfunc equalfunc; // Determines whether two keys are equivalent

    unsigned int capacity;  // Number of items in the table
    unsigned int numused;   // Number of occupied items in the table
    unsigned int cellar;    // Start of the cellar region (see below)

    void *items;            // Table of hashitems
};

// The portion of the hashtable that is reserved for the 'cellar.'
//
// In coalesced hashing, the cellar is a portion of the table reserved for
// items whose hashes conflict with another item already in the table.
// By separating out this part of the table, we guarantee that chains of
// values with different hash addresses don't coalesce until the cellar 
// is full.
//
static const double CELLAR = .14;

// Gets the index in the hashtable's item table for the given key
//
static unsigned int tr_hash_addr(hashtable *hash, const void *key)
{
    // % hash->cellar: hash to any bucket before the cellar starts
    return hash->hashfunc(key) % hash->cellar;
}

// Gets the i'th item from the hashtable's item table
//
static hashitem *tr_hash_item(hashtable *hash, unsigned int i)
{
    unsigned int size = sizeof(hashitem) + hash->keysize + hash->valuesize;
    return (hashitem*)((char*)hash->items + i * size);
}

// Gets the key for a hash table item
//
static void *tr_hashitem_key(hashtable *hash, hashitem *item)
{
    return (char*)item + sizeof(hashitem);
}

// Gets the value for a hash table item
//
static void *tr_hashitem_value(hashtable *hash, hashitem *item)
{
    return (char*)item + sizeof(hashitem) + hash->keysize;
}

// Resizes an existing hash, migrating over any existing data
//
static void tr_hash_resize(hashtable *hash, unsigned int capacity)
{
    // Store the old item table
    unsigned int prevcap = hash->capacity;
    void *previtems = hash->items;

    // Allocate the new item table
    unsigned int itemsize = sizeof(hashitem) + hash->keysize + hash->valuesize;
    hash->items = tr_malloc(capacity * itemsize);
    memset(hash->items, 0, capacity * itemsize);

    hash->numused = 0;
    hash->capacity = capacity;
    hash->cellar = capacity - CELLAR * capacity;

    if (previtems) {

        // Migrate the existing items to the new table
        for (unsigned int i = 0; i < prevcap; ++i) {

            hashitem *item = (hashitem*)((char*)previtems + i * itemsize);
            if (item->occupied) {

                void *key = ((char*)item + sizeof(hashitem));
                void *value = ((char*)key + hash->keysize);

                tr_hash_set(hash, key, value);
            }
        }

        // Clean up the old item table
        tr_free(previtems);
    }
}

// Resizes the hashtable's intenral item table if needed.
// Follows the same resize rules as tr_vector.
//
static void tr_hash_resize_if_needed(hashtable *hash)
{
    if (hash->numused >= hash->capacity / 2) {
        tr_hash_resize(hash, 2 * hash->capacity);
    }
    else if (hash->numused <= hash->capacity / 4) {
        tr_hash_resize(hash, hash->capacity / 2);
    }
}

// Gets the next free hashitem in the cellar.
// If the cellar is full, returns NULL.
//
static hashitem *tr_hash_next_cellar(hashtable *hash)
{
    for (unsigned int i = hash->cellar; i < hash->capacity; ++i) {

        hashitem *item = tr_hash_item(hash, i);
        if (!item->occupied) {
            return item;
        }
    }

    return NULL;
}

// Stores the given key and value in the given hash item.
//
static void tr_hashitem_claim(hashtable *hash, hashitem *item, 
                              const void *key, const void *value)
{
    assert(!item->occupied);
    item->occupied = true;

    memcpy(tr_hashitem_key(hash, item), key, hash->keysize);
    memcpy(tr_hashitem_value(hash, item), value, hash->valuesize);

    hash->numused += 1;
}

tr_hash tr_hash_create(unsigned int keysize, 
                       unsigned int valuesize,
                       tr_hashfunc hashfunc,
                       tr_equalfunc equalfunc)
{
    hashtable *hash = (hashtable*)tr_malloc(sizeof(hashtable));
    hash->keysize = keysize;
    hash->valuesize = valuesize;
    hash->hashfunc = hashfunc;
    hash->equalfunc = equalfunc;
    hash->capacity = 0;
    hash->numused = 0;
    hash->cellar = 0;
    hash->items = NULL;

    tr_hash_resize(hash, 16);
    return hash;
}

tr_err tr_hash_delete(tr_hash trh)
{
    if (!trh) return TR_EPOINTER;

    hashtable *hash = (hashtable *)trh;
    tr_free(hash->items);

    return TR_OK;
}

unsigned int tr_hash_contains(tr_hash trh, const void *key)
{
    if (!trh) return false;

    hashtable *hash = (hashtable*)trh;
    unsigned int i = tr_hash_addr(hash, key);
    hashitem *bucket = tr_hash_item(hash, i);

    for (hashitem *item = bucket; item; item = item->next) {
        void *itemkey = tr_hashitem_key(hash, item);

        if (hash->equalfunc(key, itemkey)) {
            return true;
        }
    }

    return false;
}

void *tr_hash_get(tr_hash trh, const void *key)
{
    if (!trh) return NULL;

    hashtable *hash = (hashtable*)trh;
    unsigned int i = tr_hash_addr(hash, key);
    hashitem *bucket = tr_hash_item(hash, i);

    for (hashitem *item = bucket; item; item = item->next) {
        void *itemkey = tr_hashitem_key(hash, item);

        if (hash->equalfunc(key, itemkey)) {
            return tr_hashitem_value(hash, item);
        }
    }

    return NULL;
}

tr_err tr_hash_set(tr_hash trh, const void *key, const void *value)
{
    if (!trh) return TR_EPOINTER;
    hashtable *hash = (hashtable*)trh;

    // Insert the item into its bucket if the bucket is free
    unsigned int i = tr_hash_addr(hash, key);
    hashitem *bucket = tr_hash_item(hash, i);

    if (!bucket->occupied) {
        tr_hashitem_claim(hash, bucket, key, value);
        tr_hash_resize_if_needed(hash);

        return TR_OK;
    }

    // The bucket is occupied; try adding to the cellar
    hashitem *cellar = tr_hash_next_cellar(hash);
    if (cellar) {

        assert(!cellar->occupied);
        tr_hashitem_claim(hash, cellar, key, value);

        cellar->next = bucket->next;
        bucket->next = cellar;

        tr_hash_resize_if_needed(hash);
        return TR_OK;
    }

    // The cellar is full; use open addressing
    for (unsigned int i = 0; i < hash->capacity; ++i) {

        hashitem *item = tr_hash_item(hash, i);
        if (!item->occupied) {

            tr_hashitem_claim(hash, item, key, value);

            item->next = bucket->next;
            bucket->next = item;

            tr_hash_resize_if_needed(hash);
            return TR_OK;
        }
    }

    // The hash itself is full (this should never happen)
    assert(0 && "Hash is full. Possible bug with tr_hash_resize_if_needed()");
    return TR_EINTERNAL;
}

tr_err tr_hash_clear(tr_hash trh, const void *key)
{
    if (!trh) return TR_EPOINTER;

    hashtable *hash = (hashtable*)trh;
    unsigned int i = tr_hash_addr(hash, key);
    hashitem *bucket = tr_hash_item(hash, i);

    hashitem *item = bucket;
    hashitem *prev = NULL;
    while (item) {

        void *itemkey = tr_hashitem_key(hash, item);
        if (hash->equalfunc(key, itemkey)) {

            item->occupied = false;
            if (prev) {
                prev->next = item->next;
            }

            hash->numused -= 1;
            tr_hash_resize_if_needed(hash);

            return TR_OK;
        }

        prev = item;
        item = item->next;
    }

    return TR_ENOTFOUND;
}

unsigned int tr_hash_num_keys(tr_hash trh)
{
    if (!trh) return 0;

    hashtable *hash = (hashtable *)trh;
    unsigned int count = 0;

    for (unsigned int i = 0; i < hash->capacity; ++i) {
        hashitem *item = tr_hash_item(hash, i);

        if (item->occupied) {
            count += 1;
        }
    }

    return count;
}

tr_vector tr_hash_keys(tr_hash trh)
{
    if (!trh) return NULL;

    hashtable *hash = (hashtable *)trh;
    tr_vector keys = tr_vec_create(hash->keysize, hash->capacity);

    for (unsigned int i = 0; i < hash->capacity; ++i) {
        hashitem *item = tr_hash_item(hash, i);

        if (item->occupied) {
            void *key = (char*)item + sizeof(hashitem);
            tr_vec_append(keys, key);
        }
    }

    return keys;
}

tr_vector tr_hash_values(tr_hash trh)
{
    if (!trh) return NULL;

    hashtable *hash = (hashtable *)trh;
    tr_vector values = tr_vec_create(hash->valuesize, hash->capacity);

    for (unsigned int i = 0; i < hash->capacity; ++i) {
        hashitem *item = tr_hash_item(hash, i);

        if (item->occupied) {
            void *value = (char*)item + sizeof(hashitem);
            tr_vec_append(values, value);
        }
    }

    return values;
}

unsigned int tr_hashfunc_str(const void *val)
{
    // This is the djb2 hash function (http://www.cse.yorku.ca/~oz/hash.html)
    //
    const char *str = (const char *)val;
    unsigned int hash = 5381;

    while (str) {
        hash = (hash << 5) + hash + *(str++);
    }

    return hash;
}

unsigned int tr_equalfunc_str(const void *val1, const void *val2)
{
    const char *str1 = *((const char **)val1);
    const char *str2 = *((const char **)val2);

    return strcmp(str1, str2) == 0;
}

tr_hash tr_strhash_create(unsigned int itemsize)
{
    return tr_hash_create(sizeof(const char *), 
                          itemsize, 
                          tr_hashfunc_str,
                          tr_equalfunc_str);
}

tr_err tr_strhash_delete(tr_hash hash)
{
    return tr_hash_delete(hash);
}

unsigned int tr_strhash_contains(tr_hash hash, const char *key)
{
    return tr_hash_contains(hash, &key);
}

void *tr_strhash_get(tr_hash hash, const char *key)
{
    return tr_hash_get(hash, &key);
}

tr_err tr_strhash_set(tr_hash hash, const char *key, void *value)
{
    return tr_hash_set(hash, &key, value);
}

tr_err tr_strhash_clear(tr_hash hash, const char *key)
{
    return tr_hash_clear(hash, &key);
}

unsigned int tr_strhash_num_keys(tr_hash hash)
{
    return tr_hash_num_keys(hash);
}

tr_vector tr_strhash_keys(tr_hash hash)
{
    return tr_hash_keys(hash);
}

tr_vector tr_strhash_values(tr_hash hash)
{
    return tr_hash_values(hash);
}

unsigned int tr_hashfunc_int(const void *num)
{
    return *((unsigned int*)num);
}

unsigned int tr_equalfunc_int(const void *val1, const void *val2)
{
    unsigned int num1 = *((unsigned int *)val1);
    unsigned int num2 = *((unsigned int *)val2);

    return num1 == num2;
}

tr_hash tr_inthash_create(unsigned int itemsize)
{
    return tr_hash_create(sizeof(int), 
                          itemsize, 
                          tr_hashfunc_int,
                          tr_equalfunc_int);
}

tr_err tr_inthash_delete(tr_hash hash)
{
    return tr_hash_delete(hash);
}

unsigned int tr_inthash_contains(tr_hash hash, int key)
{
    return tr_hash_contains(hash, &key);
}

void *tr_inthash_get(tr_hash hash, int key)
{
    return tr_hash_get(hash, &key);
}

tr_err tr_inthash_set(tr_hash hash, int key, void *value)
{
    return tr_hash_set(hash, &key, value);
}

tr_err tr_inthash_clear(tr_hash hash, int key)
{
    return tr_hash_clear(hash, &key);
}

unsigned int tr_inthash_num_keys(tr_hash hash)
{
    return tr_hash_num_keys(hash);
}

tr_vector tr_inthash_keys(tr_hash hash)
{
    return tr_hash_keys(hash);
}

tr_vector tr_inthash_values(tr_hash hash)
{
    return tr_hash_values(hash);
}
