
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// err.c - Error utilities
//

#include <traffic.h>

static const char *g_errstrs[] = 
{
    /* TR_OK */    "No error",
};

const char *tr_errstr(trerr error)
{
    static const int numerrs = sizeof(g_errstrs) / sizeof(g_errstrs[0]);

    int index = -error;
    if (index < 0 || index >= numerrs) {
        return "Unrecognized error code";
    }

    return g_errstrs[index];
}

