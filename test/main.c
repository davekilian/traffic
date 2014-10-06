
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// main.c - Entry point for test suite
//

#include <traffic.h>

#include <stdlib.h>
#include <stdio.h>

#include "test.h"

char *g_lastTestError = NULL;


typedef int (*testfunc)();

struct _test
{
    const char *name;
    testfunc  func;
};

typedef struct _test test;


static test g_tests[] = 
{
    { "test_vector_basics", test_vector_basics },
    { "test_vector_enum", test_vector_enum },
    { "test_vector_stackfuncs", test_vector_stackfuncs },
    { "test_vector_growshrink", test_vector_growshrink },

    { "test_list_basics", test_list_basics },
    { "test_list_enum", test_list_enum },
    { "test_list_addremove", test_list_addremove },

    { "test_hash_basics", test_hash_basics },
    { "test_hash_enum", test_hash_enum },
    { "test_inthash_hashfunc", test_inthash_hashfunc },
    { "test_strhash_hashfunc", test_strhash_hashfunc },

    { "test_set_basics", test_set_basics },
    { "test_set_enum", test_set_enum },
};


int main(int argc, const char *argv[])
{
    int numPassed = 0;
    int count = sizeof(g_tests) / sizeof(g_tests[0]);

    for (int i = 0; i < count; ++i) {
        test test = g_tests[i];

        printf("%d/%d: %s ...", i + 1, count, test.name);

        if (test.func()) {
            ++numPassed;
            printf(" OK\n");
        }
        else {
            printf(" FAILED\n");

            if (g_lastTestError) {
                printf("     => %s\n", g_lastTestError);

                free(g_lastTestError);
                g_lastTestError = NULL;
            }
            else {
                printf("The test didn't leave detailed error info\n"
                       "(did you forget to use the FAIL() macro?)\n");
            }
        }
    }

    printf("\nDone. Ran %d tests; %d passed.\n\n", count, numPassed);
    return numPassed == count;
}

