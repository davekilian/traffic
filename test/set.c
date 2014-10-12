
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// set.c - Hash set unit tests
//

#include <traffic.h>

#include <stdlib.h>
#include <stdio.h>

#include "set.h"
#include "test.h"

bool test_set_basics()
{
    tr_set set = tr_intset_create();

    for (int i = 0; i < 4; ++i) {
        EQUAL(tr_intset_contains(set, i), false);
        SUCCEED(tr_intset_add(set, i));
    }

    for (int i = 0; i < 4; ++i) {
        EQUAL(tr_intset_contains(set, i), true);
    }

    SUCCEED(tr_set_delete(set));
    return true;
}

bool test_set_enum()
{
    struct _testitem
    {
        int value;
        bool seen;
    };
    typedef struct _testitem testitem;

    testitem items[] =  {
        { 0, false },
        { 1, false },
        { 2, false },
        { 3, false },
    };

    tr_set set = tr_intset_create();
    for (int i = 0; i < 4; ++i) {
        EQUAL(tr_intset_contains(set, items[i].value), false);
        SUCCEED(tr_intset_add(set, items[i].value));
    }

    tr_vector values = tr_intset_items(set);
    EQUAL(tr_vec_size(values), 4);

    for (int i = 0; i < 4; ++i) {
        int value = *(int*)tr_vec_item(values, i);

        bool found = false;
        for (int j = 0; j < 4; ++j) {
            if (items[j].value == value) {

                ASSERT(!items[j].seen, "Saw item for value %d twice", value);
                items[j].seen = true;

                found = true;
                break;
            }
        }

        ASSERT(found, "Didn't find item for value %d", value);
    }

    for (int i = 0; i < 4; ++i) {
        ASSERT(items[i].seen, "Never saw value %d", items[i].value);
    }

    tr_intset_delete(set);
    return true;
}
