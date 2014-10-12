
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

bool test_vector_basics()
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
    return true;
}

bool test_vector_enum()
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
    tr_vec_foreach(tr_vector *, inner, outer) {

        int ii = 0;
        tr_vec_foreach(int*, item, *inner) {
            EQUAL(*item, items[io][ii]);

            ii += 1;
        }

        io += 1;
    }

    tr_vec_foreach(tr_vector *, inner, outer) {
        SUCCEED(tr_vec_delete(*inner));
    }

    SUCCEED(tr_vec_delete(outer));
    return true;
}

bool test_vector_stackfuncs()
{
    int items[] = { 1, 2, 3 };

    tr_vector *vec = tr_vec_create(sizeof(int), 4);
    SUCCEED(tr_vec_push(vec, &items[0]));
    EQUAL(*(int*)tr_vec_peek(vec), items[0]);

    SUCCEED(tr_vec_push(vec, &items[1]));
    EQUAL(*(int*)tr_vec_peek(vec), items[1]);

    SUCCEED(tr_vec_push(vec, &items[2]));
    EQUAL(*(int*)tr_vec_peek(vec), items[2]);

    SUCCEED(tr_vec_pop(vec));
    EQUAL(*(int*)tr_vec_peek(vec), items[1]);

    SUCCEED(tr_vec_pop(vec));
    EQUAL(*(int*)tr_vec_peek(vec), items[0]);

    ERROR(TR_ESTACKEMPTY, tr_vec_pop(vec));
    EQUAL(NULL, tr_vec_peek(vec));

    SUCCEED(tr_vec_delete(vec));
    return true;
}

bool test_vector_growshrink()
{
    tr_vector *vec = tr_vec_create(sizeof(int), 4);

    for (int i = 0; i < 50000; ++i) {
        SUCCEED(tr_vec_append(vec, &i));
        EQUAL(tr_vec_size(vec), i + 1);
    }

    EQUAL(tr_vec_capacity(vec), 65536);

    while (tr_vec_size(vec) > 12) {
        SUCCEED(tr_vec_remove_at(vec, tr_vec_size(vec) - 1));
    }

    EQUAL(tr_vec_capacity(vec), 32);
    
    SUCCEED(tr_vec_delete(vec));
    return true;
}
