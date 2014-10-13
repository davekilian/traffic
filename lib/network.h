
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// network.h - Declarations for virtual networks
//

#ifndef NETWORK_H
#define NETWORK_H

#include <traffic.h>

#include "hash.h"
#include "set.h"

struct _node;

struct _network
{
    const char *name;   // The network's friendly name
    tr_set entityids;   // IDs in use by entities in this network
    tr_hash nodes;      // Map from node ID string to node ptr
    tr_hash links;      // Map from link ID string to link ptr
};

typedef struct _network network;

// Checks if some other entity in the network is already using the given
// network entity ID.
//
bool tr_net_id_taken(network *net, const char *id);

// Marks the given network entity ID as in use.
// Won't fail if the ID is already taken; use tr_net_id_taken instead.
//
tr_err tr_net_take_id(network *net, const char *id);

// Marks the given network entity ID as no longer in use.
// Won't fail if the ID isn't already taken; use tr_net_id_taken instead.
//
tr_err tr_net_release_id(network *net, const char *id);

// Adds a node to the network
//
tr_err tr_net_add_node(network *net, struct _node *node);

// Removes a node from the network
//
tr_err tr_net_remove_node(network *net, struct _node *node);

#endif
