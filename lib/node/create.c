
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// node/create.c -- Node creation and cleanup
//

#include <stdlib.h> // for NULL
#include <string.h> // for strlen, strcpy

#include "iface.h"
#include "memory.h"
#include "network.h"
#include "node.h"

tr_node tr_node_create(tr_network net, const char *name)
{
    if (!net) return NULL;
    if (!name) return NULL;

    if (tr_net_id_taken(net, name)) {
        return NULL;
    }

    node *n = (node *)tr_malloc(sizeof(node));
    n->net = net;
    n->ifaces = tr_strhash_create(sizeof(iface));

    n->name = tr_malloc(strlen(name) + 1);
    strcpy((char*)n->name, name);

    if (tr_net_add_node(net, n) < 0) {
        return NULL;
    }

    return n;
}

tr_err tr_node_delete(tr_node trn)
{
    if (!trn) return TR_EPOINTER;

    node *n = (node *)trn;

    tr_err err = tr_net_remove_node(n->net, n);
    if (err < 0) {
        return err;
    }

    tr_free((void*)n->name);
    tr_strhash_delete(n->ifaces);
    return TR_OK;
}
