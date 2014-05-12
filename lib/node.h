
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// node.h - Declarations for virtual network nodes
//

#ifndef NODE_H
#define NODE_H

#include <traffic.h>

#include "hash.h"

struct _network;

struct _node
{
    const char *name;       // This node's unique ID
    struct _network *net;   // The network that contains this node
    tr_hash ifaces;         // This node's virtual network interfaces
};

typedef struct _node node;

#endif
