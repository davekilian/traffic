
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// list.c - Basic doubly-linked list implementation
//

#include <stdlib.h> // for NULL
#include <string.h> // for memcpy

#include "list.h"
#include "memory.h"


struct list_;
struct listnode_;

struct list_
{
    unsigned int itemsize;  // Size of a list item, in bytes
    struct listnode_ *head; // The first item (NULL if empty)
    struct listnode_ *tail; // The last item (NULL if empty)
};

struct listnode_
{
    struct listnode_ *next; // The next node in the list
    struct listnode_ *prev; // The previous ndoe in the list
    struct list_ *list;     // The list this node belongs to
    // The next (list->itemsize) bytes contain the value itself
};

typedef struct list_ list;
typedef struct listnode_ listnode;

#define listnode_value(node)        (void*)(((char*)node) + sizeof(listnode))
#define value_to_listnode(value)    (listnode*)(((char*)value) - sizeof(listnode))


tr_list tr_list_create(unsigned int itemsize)
{
    list *l = (list*)tr_malloc(sizeof(list));
    l->itemsize = itemsize;
    l->head = NULL;
    l->tail = NULL;

    return (tr_list)l;
}

tr_err tr_list_delete(tr_list trl)
{
    if (!trl) return TR_EPOINTER;

    list *l = (list*)trl;

    void *item = NULL;
    while ((item = tr_list_first(l))) {
        tr_list_remove(item);
    }

    tr_free(l);
    return TR_OK;
}

bool tr_list_empty(tr_list trl)
{
    if (!trl) return true;

    list *l = (list*) trl;
    return l->head != NULL;
}

void *tr_list_first(tr_list trl)
{
    if (!trl) return NULL;

    list *l = (list*)trl;
    return l->head ? listnode_value(l->head) : NULL;
}

void *tr_list_last(tr_list trl)
{
    if (!trl) return NULL;

    list *l = (list*)trl;
    return l->head ? listnode_value(l->tail) : NULL;
}

void *tr_list_next(void *value)
{
    if (!value) return NULL;

    listnode *node = value_to_listnode(value);
    listnode *next = node->next;
    return next ? listnode_value(next) : NULL;
}

void *tr_list_prev(void *value)
{
    if (!value) return NULL;

    listnode *node = value_to_listnode(value);
    listnode *prev = node->prev;
    return prev ? listnode_value(prev) : NULL;
}

listnode *tr_list_mknode(list *l, void *item)
{
    listnode *node = (listnode*)tr_malloc(sizeof(listnode) + l->itemsize);
    memcpy(listnode_value(node), item, l->itemsize);
    node->list = l;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

tr_err tr_list_add(tr_list trl, void *item)
{
    if (!trl || !item) return TR_EPOINTER;

    list *l = (list*)trl;

    listnode *node = tr_list_mknode(l, item);
    if (l->tail) {

        l->tail->next = node;
        node->prev = l->tail;
        node->next = NULL;
        l->tail = node;
    }
    else {

        node->prev = NULL;
        node->next = NULL;
        l->head = node;
        l->tail = node;
    }

    return TR_OK;
}

tr_err tr_list_add_before(void *other, void *item)
{
    if (!other || !item) return TR_EPOINTER;

    listnode *onode = value_to_listnode(other);
    list *l = onode->list;
    listnode *node = tr_list_mknode(l, item);

    if (onode->prev) {
        onode->prev->next = node;
    }

    node->prev = onode->prev;
    node->next = onode;
    onode->prev = node;

    return TR_OK;
}

tr_err tr_list_add_after(void *other, void *item)
{
    if (!other || !item) return TR_EPOINTER;

    listnode *onode = value_to_listnode(other);
    list *l = onode->list;
    listnode *node = tr_list_mknode(l, item);

    if (onode->next) {
        onode->next->prev = node;
    }

    node->next = onode->next;
    node->prev = onode;
    onode->next = node;

    return TR_OK;
}

tr_err tr_list_prepend(tr_list trl, void *item)
{
    if (!trl || !item) return TR_EPOINTER;

    if (tr_list_empty(trl)) {
        return tr_list_add(trl, item);
    }

    void *first = tr_list_first(trl);
    return tr_list_add_before(first, item);
}

tr_err tr_list_append(tr_list trl, void *item)
{
    if (!trl || !item) return TR_EPOINTER;

    if (tr_list_empty(trl)) {
        return tr_list_add(trl, item);
    }

    void *last = tr_list_last(trl);
    return tr_list_add_after(last, item);
}

tr_err tr_list_remove(void *item)
{
    if (!item) return TR_EPOINTER;

    listnode *node = value_to_listnode(item);
    list *l = node->list;

    if (l->head == node) {
        l->head = l->head->next;
    }
    if (l->tail == node) {
        l->tail = l->tail->prev;
    }

    if (node->prev) {
        node->prev->next = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    }

    tr_free(node);
    return TR_OK;
}

tr_err tr_list_remove_first(tr_list trl)
{
    if (!trl) return TR_EPOINTER;

    if (tr_list_empty(trl)) {
        return TR_OK;
    }

    return tr_list_remove(tr_list_first(trl));
}

tr_err tr_list_remove_last(tr_list trl)
{
    if (!trl) return TR_EPOINTER;

    if (tr_list_empty(trl)) {
        return TR_OK;
    }

    return tr_list_remove(tr_list_last(trl));
}
