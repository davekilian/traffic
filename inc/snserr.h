
//
// sns - Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// snserr.h - Error definitions and uns.h
// ilities
//

#ifndef SNSERR_H
#define SNSERR_H

typedef int snserr;

static const snserr SNS_OK = 0;

/** Gets an English string explaining the given error code */
const char *sns_errstr(snserr error);

#endif
