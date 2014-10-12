
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// list.c - Linked list unit tests
//

#include <traffic.h>

#include <stdlib.h>
#include <stdio.h>

#include "list.h"
#include "test.h"

bool test_list_basics()
{
    tr_list list = tr_list_create(sizeof(int));
    ASSERT(list != NULL, "tr_list_create failed!");

    EQUAL(tr_list_empty(list), true);
    EQUAL(tr_list_first(list), NULL);
    EQUAL(tr_list_last(list), NULL);

    int value = 123;
    SUCCEED(tr_list_add(list, &value));
    EQUAL(tr_list_empty(list), false);
    EQUAL(*(int*)tr_list_first(list), 123);
    EQUAL(*(int*)tr_list_last(list), 123);
    EQUAL(tr_list_first(list), tr_list_last(list));

    value = 456;
    SUCCEED(tr_list_add(list, &value));
    EQUAL(tr_list_empty(list), false);

    int *ptr = tr_list_first(list);
    EQUAL(*ptr, 123);
    ptr = tr_list_next(ptr);
    EQUAL(*ptr, 456);
    ptr = tr_list_prev(ptr);
    EQUAL(*ptr, 123);
    ptr = tr_list_prev(ptr);
    EQUAL(ptr, NULL);
    ptr = tr_list_first(list);
    ptr = tr_list_next(ptr);
    ptr = tr_list_next(ptr);
    EQUAL(ptr, NULL);
    ptr = tr_list_last(list);
    EQUAL(*ptr, 456);
    ptr = tr_list_prev(ptr);
    EQUAL(*ptr, 123);

    SUCCEED(tr_list_delete(list));
    return true;
}

bool test_list_enum()
{
    int items[][3] =
    {
        { 1, 2, 3, },
        { 4, 5, 6, },
        { 7, 8, 9, },
    };

    tr_list outer = tr_list_create(sizeof(tr_list));
    ASSERT(outer != NULL, "tr_list_create failed!");

    for (int io = 0; io < 3; ++io) {
        tr_list inner = tr_list_create(sizeof(int));
        ASSERT(inner != NULL, "tr_list_create failed!");

        for (int ii = 0; ii < 3; ++ii) {
            SUCCEED(tr_list_append(inner, &items[io][ii]));
        }

        SUCCEED(tr_list_append(outer, &inner));
    }

    int io = 0;
    tr_list_foreach(tr_list *, inner, outer) {
        int ii = 0;
        tr_list_foreach(int *, item, *inner) {
            EQUAL(*item, items[io][ii]);

            ii += 1;
        }

        io += 1;
    }

    tr_list_foreach(tr_list *, inner, outer) {
        SUCCEED(tr_list_delete(*inner));
    }

    SUCCEED(tr_list_delete(outer));
    return true;
}

bool test_list_addremove()
{
    #define EQUAL_LISTS(list, ...) do {                             \
            int __items[] = __VA_ARGS__;                            \
            int __index = 0;                                        \
            tr_list_foreach(int *, __item, list) {                  \
                EQUAL(*__item, __items[__index++]);                 \
            }                                                       \
            EQUAL(__index, sizeof(__items) / sizeof(int));          \
            EQUAL(*(int*)tr_list_first(list), __items[0]);          \
            EQUAL(*(int*)tr_list_last(list), __items[__index - 1]); \
        } while (0)

    tr_list list = tr_list_create(sizeof(int));

    int item = 0;
    for (int i = 0; i < 4; ++i) {
        SUCCEED(tr_list_add(list, &item));
        item += 1;
    }

    EQUAL_LISTS(list, { 0, 1, 2, 3 });

    SUCCEED(tr_list_add_before(tr_list_first(list), &item));
    EQUAL_LISTS(list, { 4, 0, 1, 2, 3 });
    item += 1;

    SUCCEED(tr_list_add_before(tr_list_last(list), &item));
    EQUAL_LISTS(list, { 4, 0, 1, 2, 5, 3 });
    item += 1;

    SUCCEED(tr_list_add_after(tr_list_first(list), &item));
    EQUAL_LISTS(list, { 4, 6, 0, 1, 2, 5, 3 });
    item += 1;

    SUCCEED(tr_list_add_after(tr_list_last(list), &item));
    EQUAL_LISTS(list, { 4, 6, 0, 1, 2, 5, 3, 7 });
    item += 1;

    SUCCEED(tr_list_prepend(list, &item));
    EQUAL_LISTS(list, { 8, 4, 6, 0, 1, 2, 5, 3, 7 });
    item += 1;

    SUCCEED(tr_list_append(list, &item));
    EQUAL_LISTS(list, { 8, 4, 6, 0, 1, 2, 5, 3, 7, 9 });
    item += 1;

    SUCCEED(tr_list_remove_first(list));
    EQUAL_LISTS(list, { 4, 6, 0, 1, 2, 5, 3, 7, 9 });

    SUCCEED(tr_list_remove_last(list));
    EQUAL_LISTS(list, { 4, 6, 0, 1, 2, 5, 3, 7 });

    SUCCEED(tr_list_remove(tr_list_first(list)));
    EQUAL_LISTS(list, { 6, 0, 1, 2, 5, 3, 7 });

    SUCCEED(tr_list_remove(tr_list_last(list)));
    EQUAL_LISTS(list, { 6, 0, 1, 2, 5, 3 });

    SUCCEED(tr_list_remove(tr_list_next(tr_list_first(list))));
    EQUAL_LISTS(list, { 6, 1, 2, 5, 3 });

    tr_list_delete(list);
    return true;
}
