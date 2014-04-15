
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// memory.c - Memory allocation routines
//

// libtraffic uses tr_malloc/tr_free for all heap-allocation needs.
// For now malloc/free suit our needs, but it would be easy to switch
// to a custom allocator should the need arise.
//
#include <stdlib.h>
void *tr_malloc(unsigned int size) { return malloc(size); }
void tr_free(void *mem) { free(mem); }
