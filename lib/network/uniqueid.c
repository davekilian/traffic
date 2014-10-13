
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// network/unique.id - Unique ID bookkeeping
//

#include "network.h"

bool tr_net_id_taken(network *net, const char *id)
{
    return tr_strset_contains(net->entityids, id);
}

tr_err tr_net_take_id(network *net, const char *id)
{
    return tr_strset_add(net->entityids, id);
}

tr_err tr_net_release_id(network *net, const char *id)
{
    return tr_strset_remove(net->entityids, id);
}
