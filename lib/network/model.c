
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// network/model.c - Simulated network modeling
//

#include <stdlib.h> // for NULL
#include <string.h> // for memcpy

#include "network.h"
#include "node.h"

const char *tr_net_name(tr_network trn)
{
    if (!trn) return NULL;

    network *net = (network *)trn;
    return net->name;
}

unsigned int tr_net_num_nodes(tr_network trn)
{
    if (!trn) return 0;

    network *net = (network *)trn;
    return tr_strhash_num_keys(net->nodes);
}

tr_err tr_net_nodes(tr_network trn, tr_node *nodes, unsigned int len)
{
    if (!trn) return TR_EPOINTER;
    if (!nodes) return TR_EPOINTER;

    network *net = (network *)trn;
    tr_vector nodevec = tr_hash_values(net->nodes);

    if (len > tr_vec_length(nodevec)) {
        len = tr_vec_length(nodevec);
    }

    memcpy(nodes, tr_vec_items(nodevec), len * sizeof(node));

    tr_vec_delete(nodes);
    return TR_OK;
}

tr_err tr_net_link(tr_network net, tr_iface i1, tr_iface i2, tr_link *link)
{
    return TR_EINTERNAL; // TODO NYI link modeling
}

bool tr_net_has_node(tr_network trn, const char *name)
{
    if (!trn) return TR_EPOINTER;
    if (!name) return TR_EPOINTER;

    network *net = (network *)trn;
    return tr_strhash_contains(net->nodes, name);
}

tr_node tr_net_node(tr_network trn, const char *name)
{
    if (!trn) return NULL;
    if (!name) return NULL;

    network *net = (network *)trn;
    return tr_strhash_get(net->nodes, name);
}

tr_err tr_net_add_node(network *net, struct _node *node)
{
    if (!net) return TR_EPOINTER;
    if (!node) return TR_EPOINTER;

    return tr_strhash_set(net->nodes, node->name, node);
}

tr_err tr_net_remove_node(network *net, struct _node *node)
{
    if (!net) return TR_EPOINTER;
    if (!node) return TR_EPOINTER;

    return tr_strhash_clear(net->nodes, node->name);
}
