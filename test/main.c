
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
    { "test_vector", test_vector },
    { "test_list", test_list },
    { "test_hash", test_hash },
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

