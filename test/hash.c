
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// hash.c - Hashtable unit tests
//

#include <traffic.h>

#include <stdlib.h>
#include <stdio.h>

#include "hash.h"
#include "memory.h"
#include "test.h"

bool test_hash_basics()
{
    tr_hash hash = tr_inthash_create(sizeof(int));
    EQUAL(tr_inthash_num_keys(hash), 0);

    int keys[]  = { 0, 1, 2 };
    int items[] = { 3, 4, 5 };

    for (int i = 0; i < 3; ++i) {
        tr_inthash_set(hash, keys[i], &items[i]);
        EQUAL(tr_inthash_num_keys(hash), i + 1);
    }

    for (int i = 0; i < 3; ++i) {
        ASSERT(tr_inthash_contains(hash, i), "Missing key: %d", i);
        EQUAL(*(int*)tr_inthash_get(hash, i), items[i]);
    }

    SUCCEED(tr_inthash_clear(hash, 1));
    ASSERT(!tr_inthash_contains(hash, 1), "Key was not removed: 1");

    ASSERT(tr_inthash_contains(hash, 0), "Missing key: 0");
    EQUAL(*(int*)tr_inthash_get(hash, 0), 3);

    ASSERT(tr_inthash_contains(hash, 2), "Missing key: 2");
    EQUAL(*(int*)tr_inthash_get(hash, 2), 5);

    tr_inthash_delete(hash);
    return true;
}

bool test_hash_enum()
{
    struct _testitem
    {
        int key;
        int value;
        bool seen;
    };
    typedef struct _testitem testitem;

    testitem items[] =  {
        { 0, 1, false },
        { 2, 3, false },
        { 4, 5, false },
        { 6, 7, false },
    };

    tr_hash hash = tr_inthash_create(sizeof(int));
    for (int i = 0; i < 4; ++i) {
        SUCCEED(tr_inthash_set(hash, items[i].key, &items[i].value));
    }

    EQUAL(tr_inthash_num_keys(hash), 4);

    tr_vector keys = tr_inthash_keys(hash);
    EQUAL(tr_vec_size(keys), 4);

    tr_vector values = tr_inthash_values(hash);
    EQUAL(tr_vec_size(values), 4);

    for (int i = 0; i < 4; ++i) {
        int key = *(int*)tr_vec_item(keys, i);
        int value = *(int*)tr_vec_item(values, i);

        bool found = false;
        for (int j = 0; j < 4; ++j) {

            if (items[j].key == key) {
                EQUAL(items[j].value, value);

                ASSERT(!items[j].seen, "Saw item for key %d twice", key);
                items[j].seen = true;

                found = true;
                break;
            }
        }

        ASSERT(found, "Didn't find item for key %d", key);
    }

    for (int i = 0; i < 4; ++i) {
        ASSERT(items[i].seen, "Never saw key %d", items[i].key);
    }

    tr_inthash_delete(hash);
    return true;
}

bool test_inthash_hashfunc()
{
    int counts[] = { 0, 0, 0, 0 };

    for (int i = 0; i < 1000000; ++i) {
        unsigned int hash = tr_hashfunc_int(&i);
        counts[hash % 4] += 1;
    }

    static const int EXPECTED = 250000;
    static const int TOLERANCE =   100;

    for (int i = 0; i < 4; ++i) {
        int difference = counts[i] - EXPECTED;
        if (difference < 0) {
            difference = -difference;
        }

        ASSERT(difference < TOLERANCE, "Hash function is biased");
    }

    return true;
}

bool test_strhash_hashfunc()
{
    int counts[] = { 0, 0, 0, 0 };

    for (int i = 0; i < 1000000; ++i) {

        char *str = tr_malloc(128);
        sprintf(str, "test_string_%d", i);

        unsigned int hash = tr_hashfunc_str(&str);
        counts[hash % 4] += 1;
    }

    static const int EXPECTED = 250000;
    static const int TOLERANCE =   100;

    for (int i = 0; i < 4; ++i) {
        int difference = counts[i] - EXPECTED;
        if (difference < 0) {
            difference = -difference;
        }

        ASSERT(difference < TOLERANCE, "Hash function is biased");
    }

    return true;
}
