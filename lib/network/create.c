
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// network/unique.id - tr_network creation and cleanup
//

#include "link.h"
#include "memory.h"
#include "network.h"
#include "node.h"

tr_network tr_net_create(const char *name)
{
    network *net = (network *)tr_malloc(sizeof(network));
    net->name = name;
    net->entityids = tr_strset_create();
    net->nodes = tr_strhash_create(sizeof(node));
    net->links = tr_strhash_create(sizeof(link));

    return net;
}

tr_err tr_net_delete(tr_network trn)
{
    if (!trn) return TR_EPOINTER;

    network *net = (network *)trn;

    tr_strset_delete(net->entityids);
    tr_strhash_delete(net->nodes);
    tr_strhash_delete(net->links);

    tr_free(net);

    return TR_OK;
}
