
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// vector.c - Vector unit tests
//

#include <traffic.h>

#include <stdlib.h>
#include <stdio.h>

#include "vector.h"
#include "test.h"

int test_vector_basics()
{
    tr_vector vec = tr_vec_create(sizeof(int), 4);
    ASSERT(vec != NULL, "tr_vec_create() returned NULL");

    EQUAL(tr_vec_capacity(vec), 4);
    EQUAL(tr_vec_size(vec), 0);
    EQUAL(tr_vec_count(vec), 0);
    EQUAL(tr_vec_length(vec), 0);

    SUCCEED(tr_vec_resize(vec, 16));
    EQUAL(tr_vec_capacity(vec), 16);
    EQUAL(tr_vec_size(vec), 0);
    EQUAL(tr_vec_count(vec), 0);
    EQUAL(tr_vec_length(vec), 0);

    int values[] = { 0, 1, 2, 3 };

    SUCCEED(tr_vec_append(vec, &values[1]));
    EQUAL(tr_vec_size(vec), 1);
    EQUAL(tr_vec_count(vec), 1);
    EQUAL(tr_vec_length(vec), 1);

    int *test = tr_vec_item(vec, 0);
    EQUAL(*test, 1);

    SUCCEED(tr_vec_prepend(vec, &values[0]));
    EQUAL(tr_vec_size(vec), 2);
    EQUAL(tr_vec_count(vec), 2);
    EQUAL(tr_vec_length(vec), 2);

    test = tr_vec_item(vec, 0);
    EQUAL(*test, 0);
    test = tr_vec_item(vec, 1);
    EQUAL(*test, 1);

    SUCCEED(tr_vec_insert(vec, 2, &values[2]));
    EQUAL(tr_vec_size(vec), 3);
    EQUAL(tr_vec_count(vec), 3);
    EQUAL(tr_vec_length(vec), 3);

    test = tr_vec_item(vec, 0);
    EQUAL(*test, 0);
    test = tr_vec_item(vec, 1);
    EQUAL(*test, 1);
    test = tr_vec_item(vec, 2);
    EQUAL(*test, 2);

    SUCCEED(tr_vec_append(vec, &values[3]));
    EQUAL(tr_vec_size(vec), 4);
    EQUAL(tr_vec_count(vec), 4);
    EQUAL(tr_vec_length(vec), 4);

    test = tr_vec_item(vec, 0);
    EQUAL(*test, 0);
    test = tr_vec_item(vec, 1);
    EQUAL(*test, 1);
    test = tr_vec_item(vec, 2);
    EQUAL(*test, 2);
    test = tr_vec_item(vec, 3);
    EQUAL(*test, 3);

    SUCCEED(tr_vec_remove(vec, &values[2]));
    EQUAL(tr_vec_size(vec), 3);
    EQUAL(tr_vec_count(vec), 3);
    EQUAL(tr_vec_length(vec), 3);

    test = tr_vec_item(vec, 0);
    EQUAL(*test, 0);
    test = tr_vec_item(vec, 1);
    EQUAL(*test, 1);
    test = tr_vec_item(vec, 2);
    EQUAL(*test, 3);

    SUCCEED(tr_vec_remove_at(vec, 0));
    EQUAL(tr_vec_size(vec), 2);
    EQUAL(tr_vec_count(vec), 2);
    EQUAL(tr_vec_length(vec), 2);

    test = tr_vec_item(vec, 0);
    EQUAL(*test, 1);
    test = tr_vec_item(vec, 1);
    EQUAL(*test, 3);

    SUCCEED(tr_vec_delete(vec));
    return 1;
}

int test_vector_enum()
{
    int items[][3] =
    {
        { 1, 2, 3, },
        { 4, 5, 6, },
        { 7, 8, 9, },
    };

    tr_vector outer = tr_vec_create(sizeof(tr_vector), 4);
    ASSERT(outer != NULL, "tr_vec_create failed!");

    for (int io = 0; io < 3; ++io) {
        tr_vector inner = tr_vec_create(sizeof(int), 4);
        ASSERT(inner != NULL, "tr_vec_create failed!");

        for (int ii = 0; ii < 3; ++ii) {
            SUCCEED(tr_vec_append(inner, &items[io][ii]));
        }

        SUCCEED(tr_vec_append(outer, &inner));
    }

    int io = 0;
    int ii = 0;

    tr_vec_foreach(tr_vector *, inner, outer) {
        tr_vector real_inner = tr_vec_item(outer, io++);
        ASSERT(real_inner != NULL, "Couldn't get item %d", io - 1);

        tr_vec_foreach(int*, item, inner) {
            int *real_item = tr_vec_item(*inner, ii++);
            ASSERT(real_item != NULL, "Couldn't get item %d", ii - 1);

            EQUAL(*item, *real_item);
        }
    }

    tr_vec_foreach(tr_vector *, inner, outer) {
        SUCCEED(tr_vec_delete(*inner));
    }

    SUCCEED(tr_vec_delete(outer));
    return 1;
}

int test_vector_stackfuncs()
{
    FAIL("TODO NYI");
}

int test_vector_growshrink()
{
    FAIL("TODO NYI");
}
