
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// network/unique.id - tr_network creation and cleanup
//

#include <stdlib.h> // for NULL
#include <string.h> // for strlen, strcpy

#include "link.h"
#include "memory.h"
#include "network.h"
#include "node.h"

tr_network tr_net_create(const char *name)
{
    network *net = (network *)tr_malloc(sizeof(network));

    net->name = NULL;
    net->entityids = tr_strset_create();
    net->nodes = tr_strhash_create(sizeof(node));
    net->links = tr_strhash_create(sizeof(link));

    if (name) {
        net->name = tr_malloc(strlen(name) + 1);
        strcpy((char *)net->name, name);
    }

    return net;
}

tr_err tr_net_delete(tr_network trn)
{
    if (!trn) return TR_EPOINTER;

    network *net = (network *)trn;

    tr_vec_foreach(node *, n, net->nodes) {
        tr_err err = tr_net_remove_node(net, n);
        if (err < 0) {
            return err;
        }
    }

    tr_strset_delete(net->entityids);
    tr_strhash_delete(net->nodes);
    tr_strhash_delete(net->links);

    if (net->name) {
        tr_free((void*)net->name);
    }

    tr_free(net);

    return TR_OK;
}
