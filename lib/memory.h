
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// memory.h - Memory allocation routines
//

// Returns a pointer to [size] bytes of heap-allocated memory.
// Returns NULL if the pointer could not be allocated.
//
void *tr_malloc(unsigned int size);

// Frees a heap-allocated pointer created with tr_malloc.
// Don't mix with other memory allocation routines.
//
void tr_free(void *);
