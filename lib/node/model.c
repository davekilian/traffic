
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// node/model.c - Virtual network node modeling
//

#include <stdlib.h> // for NULL
#include <string.h> // for memcpy

#include "iface.h"
#include "node.h"

const char *tr_node_name(tr_node trn)
{
    if (!trn) return NULL;

    node *n = (node *)trn;
    return n->name;
}

tr_network tr_node_network(tr_node trn)
{
    if (!trn) return NULL;

    node *n = (node *)trn;
    return n->net;
}

unsigned tr_node_num_ifaces(tr_node trn)
{
    if (!trn) return 0;

    node *n = (node *)trn;
    return tr_strhash_num_keys(n->ifaces);
}

tr_err tr_node_ifaces(tr_node trn, tr_iface *ifaces, unsigned len)
{
    if (!trn) return TR_EPOINTER;
    if (!ifaces) return TR_EPOINTER;

    node *n = (node *)trn;
    tr_vector vec = tr_strhash_values(n->ifaces);

    if (len > tr_vec_count(vec)) {
        len = tr_vec_count(vec);
    }

    memcpy(ifaces, tr_vec_items(vec), len * sizeof(iface));

    tr_vec_delete(vec);
    return TR_OK;
}

bool tr_node_has_iface(tr_node trn, const char *name)
{
    if (!trn) return false;
    if (!name) return false;

    node *n = (node *)trn;
    return tr_strhash_contains(n->ifaces, name);
}

tr_iface tr_node_iface(tr_node trn, const char *name)
{
    if (!trn) return false;
    if (!name) return false;

    node *n = (node *)trn;
    return tr_strhash_get(n->ifaces, name);
}
