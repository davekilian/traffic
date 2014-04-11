
//
// sns - Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// err.c - Error utilities
//

#include <snserr.h>

static const char *g_errstrs[] = 
{
    /* SNS_OK */    "No error",
};

const char *sns_errstr(snserr error)
{
    static const int numerrs = sizeof(g_errstrs) / sizeof(g_errstrs[0]);

    int index = -error;
    if (index < 0 || index >= numerrs) {
        return "Unrecognized error code";
    }

    return g_errstrs[index];
}

