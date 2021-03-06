
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// vector.c - Basic array-list implementation
//

#include <assert.h> // for assert
#include <stdlib.h> // for NULL
#include <string.h> // for memcpy

#include "memory.h"
#include "vector.h"

struct _vector
{
    unsigned int itemsize;  // Size of a list item, in bytes
    unsigned int capacity;  // Number of slots available in the vector
    unsigned int size;      // Number of slots occupied
    void *data;             // Current data buffer
};

typedef struct _vector vector;

tr_vector tr_vec_create(unsigned int itemsize, unsigned int capacity)
{
    vector *v = (vector*)tr_malloc(sizeof(vector));
    v->itemsize = itemsize;
    v->capacity = 0;
    v->size = 0;
    v->data = NULL;

    tr_vec_resize(v, capacity);
    return v;
}

tr_err tr_vec_delete(tr_vector trv)
{
    if (!trv) return TR_EPOINTER;

    vector *v = (vector*)trv;
    if (v->data) {
        tr_free(v->data);
    }

    tr_free(v);
    return TR_OK;
}

unsigned int tr_vec_capacity(tr_vector trv)
{
    if (!trv) return 0;
    return ((vector*)trv)->capacity;
}

unsigned int tr_vec_size(tr_vector trv)
{
    if (!trv) return 0;
    return ((vector*)trv)->size;
}

unsigned int tr_vec_count(tr_vector vec)
{
    return tr_vec_size(vec);
}

unsigned int tr_vec_length(tr_vector vec)
{
    return tr_vec_size(vec);
}

tr_err tr_vec_resize(tr_vector trv, unsigned int capacity)
{
    if (!trv) return TR_EPOINTER;

    vector *v = (vector*)trv;
    void *newbuf = malloc(v->itemsize * capacity);

    if (v->data) {
        unsigned int ncopy = v->size < capacity 
                           ? v->size
                           : capacity;

        memcpy(newbuf, v->data, ncopy * v->itemsize);
        tr_free(v->data);
    }

    v->capacity = capacity;
    v->data = newbuf;

    if (v->size > capacity) {
        v->size = capacity;
    }

    return TR_OK;
}

void *tr_vec_item(tr_vector trv, unsigned int index)
{
    if (!trv) return NULL;

    vector *v = (vector*)trv;
    return ((char*)v->data) + (index * v->itemsize);
}

void *tr_vec_items(tr_vector trv)
{
    if (!trv) return NULL;

    vector *v = (vector*)trv;
    return v->data;
}

tr_err tr_vec_append(tr_vector trv, void *item)
{
    if (!trv) return TR_EPOINTER;
    return tr_vec_insert(trv, tr_vec_size(trv), item);
}

tr_err tr_vec_prepend(tr_vector trv, void *item)
{
    if (!trv) return TR_EPOINTER;
    return tr_vec_insert(trv, 0, item);
}

tr_err tr_vec_insert(tr_vector trv, unsigned int index, void *item)
{
    if (!trv) return TR_EPOINTER;

    vector *v = (vector*)trv;
    if (index > v->size) {
        return TR_EOUTOFRANGE;
    }

    assert(v->size < v->capacity);

    unsigned int after = (v->size - index) * v->itemsize;
    memmove(tr_vec_item(v, index + 1), tr_vec_item(v, index), after);
    memcpy(tr_vec_item(v, index), item, v->itemsize);

    v->size += 1;
    if (v->size == v->capacity) {
        tr_vec_resize(trv, 2 * v->capacity);
    }

    return TR_OK;
}

tr_err tr_vec_remove(tr_vector trv, void *item)
{
    if (!trv) return TR_EPOINTER;

    vector *v = (vector*)trv;
    for (unsigned int i = 0; i < v->size; ++i) {

        void *cur = (char*)v->data + i * v->itemsize;
        if (memcmp(cur, item, v->itemsize) == 0) {
            return tr_vec_remove_at(trv, i);
        }
    }

    return TR_ENOTFOUND;
}

tr_err tr_vec_remove_at(tr_vector trv, unsigned int index)
{
    if (!trv) return TR_EPOINTER;

    vector *v = (vector*)trv;
    if (index >= v->size) {
        return TR_EOUTOFRANGE;
    }

    unsigned int after = (v->size - (index + 1)) * v->itemsize;
    if (after > 0) {
        memmove(tr_vec_item(v, index), tr_vec_item(v, index + 1), after);
    }

    v->size -= 1;
    if (v->size <= v->capacity / 4) {
        tr_vec_resize(trv, v->capacity / 2);
    }

    return TR_OK;
}

tr_err tr_vec_push(tr_vector trv, void *item)
{
    if (!trv) return TR_EPOINTER;
    return tr_vec_append(trv, item);
}

void *tr_vec_peek(tr_vector trv)
{
    if (!trv) return NULL;

    unsigned int size = tr_vec_size(trv);
    if (size == 0) {
        return NULL;
    }

    return tr_vec_item(trv, size - 1);
}

tr_err tr_vec_pop(tr_vector trv)
{
    if (!trv) return TR_EPOINTER;

    unsigned int size = tr_vec_size(trv);
    if (size == 0) {
        return TR_ESTACKEMPTY;
    }

    return tr_vec_remove_at(trv, size - 1);
}


struct _foreach_item
{
    vector *parent;
    int index;
};

typedef struct _foreach_item foreach_item;

void *tr_vec_foreach_first(tr_vector trv)
{
    if (!trv) return NULL;
    vector *v = (vector*)trv;

    if (v->size == 0) {
        return NULL;
    }

    foreach_item *item = tr_malloc(sizeof(foreach_item) + v->itemsize);
    item->parent = v;
    item->index = 0;

    void *buf = (char*)item + sizeof(foreach_item);
    memcpy(buf, tr_vec_item(v, item->index++), v->itemsize);

    return buf;
}

bool tr_vec_foreach_finished(void *buf)
{
    if (!buf) {
        return 1;
    }

    foreach_item *item = (foreach_item*)((char*)buf - sizeof(foreach_item));
    return item->index > item->parent->size;
}

void *tr_vec_foreach_next(void *buf)
{
    if (!buf) {
        return NULL;
    }

    if (tr_vec_foreach_finished(buf)) {
        tr_free(buf);
        return NULL;
    }

    foreach_item *item = (foreach_item*)((char*)buf - sizeof(foreach_item));
    vector *v = item->parent;

    memcpy(buf, tr_vec_item(v, item->index++), v->itemsize);
    return buf;
}

