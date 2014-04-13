
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// traffic.h - Main header for public API
//

#ifndef TRAFFIC_H
#define TRAFFIC_H

typedef int trerr;

static const trerr TR_OK = 0;

/** Gets an English string explaining the given error code */
const char *tr_errstr(trerr error);

#endif
