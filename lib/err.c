
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// err.c - Error utilities
//

#include <traffic.h>

static const char *g_errstrs[] = 
{
    /* TR_OK */         "No error",
    /* TR_ENOTFOUND */  "There is no item with the given name",
    /* TR_EARRAYLEN */  "The array is not long enough for the desired list",
    /* TR_ENAMETAKEN */ "The given unique ID is already in use",
    /* TR_ENETINUSE */  "The network can't be unbound because a simulation is runing",
    /* TR_EPOINTER */   "Function received invalid pointer",
};

const char *tr_errstr(tr_err error)
{
    static const int numerrs = sizeof(g_errstrs) / sizeof(g_errstrs[0]);

    int index = -error;
    if (index < 0 || index >= numerrs) {
        return "Unrecognized error code";
    }

    return g_errstrs[index];
}

