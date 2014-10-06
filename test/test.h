
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// test.h - Test declarations and utilities
//

extern char *g_lastTestError;

// Causes a test to fail gracefull
//
#define FAIL(...) do {                              \
    asprintf(&g_lastTestError, __VA_ARGS__);        \
    return 0;                                       \
    } while (0)

// Calls FAIL if the given expression evaluates to 0
//
#define ASSERT(expr, ...) do {                      \
        if (!(expr)) {                              \
            FAIL(__VA_ARGS__);                      \
        }                                           \
    } while (0)

// ASSERTs the given expression returns TR_OK
//
#define SUCCEED(expr) do {                          \
        tr_err __error = TR_OK;                     \
        if ((__error = (expr)) != TR_OK) {          \
            FAIL(                                   \
               "In %s:%d: `%s` returned %d: %s",    \
               __FILE__, __LINE__,                  \
               #expr,                               \
               __error,                             \
               arc_errstr(__error));                \
        }                                           \
    } while (0)

//
// Unit test declarations
// Add new items to the table in main.c
//


// Tests for vector utility
//
int test_vector_basics();
int test_vector_enum();
int test_vector_stackfuncs();

// Tests for list utility
//
int test_list_basics();
int test_list_enum();
int test_list_addremove();

// Tests for hashtable utility
//
int test_hash_basics();
int test_hash_enum();

// Tests for hash set utility
//
int test_set_basics();
int test_set_enum();

