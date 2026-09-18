/*
 * AtlasDS
 * Dynamic Array Tests
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#include <stdbool.h>
#include <stdio.h>

#include "atlas/array.h"
#include "atlas/status.h"
#include "atlas/terminal.h"

static int test_create_destroy(void) {
    AtlasArray *arr = atlas_array_create(3);
    if (!arr) {
        return 1;
    }

    atlas_array_destroy(&arr);

    if (arr != NULL) {
        return 1;
    }

    return 0;
}

static int test_initial_state(void) {
    size_t initial_capacity = 3;
    AtlasArray *arr = atlas_array_create(initial_capacity);
    if (!arr) {
        return 1;
    }

    if (atlas_array_size(arr) != 0) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_capacity(arr) != initial_capacity) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_destroy_null(void) {
    AtlasArray *arr = NULL;

    atlas_array_destroy(NULL);
    atlas_array_destroy(&arr);

    return 0;
}

static int test_push_and_resize(void) {
    AtlasArray *arr = atlas_array_create(3);
    if (!arr) {
        return 1;
    }

    int values[] = {5, 10, 15, 20};
    size_t total = sizeof(values) / sizeof(values[0]);

    for (size_t i = 0; i < total; i++) {
        if (atlas_array_push(arr, values[i]) != ATLAS_SUCCESS) {
            atlas_array_destroy(&arr);
            return 1;
        }
    }

    if (atlas_array_size(arr) != total) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_capacity(arr) != 6) {
        atlas_array_destroy(&arr);
        return 1;
    }

    for (size_t i = 0; i < total; i++) {
        int value = 0;
        if (atlas_array_get(arr, i, &value) != ATLAS_SUCCESS || value != values[i]) {
            atlas_array_destroy(&arr);
            return 1;
        }
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_insert_middle(void) {
    AtlasArray *arr = atlas_array_create(4);
    if (!arr) {
        return 1;
    }

    atlas_array_push(arr, 10);
    atlas_array_push(arr, 20);
    atlas_array_push(arr, 30);
    atlas_array_push(arr, 40);

    if (atlas_array_insert(arr, 2, 99) != ATLAS_SUCCESS) {
        atlas_array_destroy(&arr);
        return 1;
    }

    int expected[] = {10, 20, 99, 30, 40};
    for (size_t i = 0; i < 5; i++) {
        int value = 0;
        if (atlas_array_get(arr, i, &value) != ATLAS_SUCCESS || value != expected[i]) {
            atlas_array_destroy(&arr);
            return 1;
        }
    }

    if (atlas_array_size(arr) != 5) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_insert_begin(void) {
    AtlasArray *arr = atlas_array_create(4);
    if (!arr) {
        return 1;
    }

    atlas_array_push(arr, 10);
    atlas_array_push(arr, 20);
    atlas_array_push(arr, 30);

    if (atlas_array_insert(arr, 0, 99) != ATLAS_SUCCESS) {
        atlas_array_destroy(&arr);
        return 1;
    }

    int expected[] = {99, 10, 20, 30};
    for (size_t i = 0; i < 4; i++) {
        int value = 0;
        if (atlas_array_get(arr, i, &value) != ATLAS_SUCCESS || value != expected[i]) {
            atlas_array_destroy(&arr);
            return 1;
        }
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_insert_end(void) {
    AtlasArray *arr = atlas_array_create(4);
    if (!arr) {
        return 1;
    }

    atlas_array_push(arr, 10);
    atlas_array_push(arr, 20);
    atlas_array_push(arr, 30);

    if (atlas_array_insert(arr, atlas_array_size(arr), 99) != ATLAS_SUCCESS) {
        atlas_array_destroy(&arr);
        return 1;
    }

    int expected[] = {10, 20, 30, 99};
    for (size_t i = 0; i < 4; i++) {
        int value = 0;
        if (atlas_array_get(arr, i, &value) != ATLAS_SUCCESS || value != expected[i]) {
            atlas_array_destroy(&arr);
            return 1;
        }
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_insertion_validation(void) {
    AtlasArray *arr = atlas_array_create(3);
    if (!arr) {
        return 1;
    }

    if (atlas_array_push(NULL, 10) != ATLAS_ERROR_NULL) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_insert(NULL, 0, 10) != ATLAS_ERROR_NULL) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_insert(arr, 999, 10) != ATLAS_ERROR_BOUNDS) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_erase_middle(void) {
    AtlasArray *arr = atlas_array_create(5);
    if (!arr) {
        return 1;
    }

    atlas_array_push(arr, 10);
    atlas_array_push(arr, 20);
    atlas_array_push(arr, 30);
    atlas_array_push(arr, 40);
    atlas_array_push(arr, 50);

    if (atlas_array_erase(arr, 2) != ATLAS_SUCCESS) {
        atlas_array_destroy(&arr);
        return 1;
    }

    int expected[] = {10, 20, 40, 50};
    for (size_t i = 0; i < 4; i++) {
        int value = 0;
        if (atlas_array_get(arr, i, &value) != ATLAS_SUCCESS || value != expected[i]) {
            atlas_array_destroy(&arr);
            return 1;
        }
    }

    if (atlas_array_size(arr) != 4) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_erase_begin(void) {
    AtlasArray *arr = atlas_array_create(3);
    if (!arr) {
        return 1;
    }

    atlas_array_push(arr, 10);
    atlas_array_push(arr, 20);
    atlas_array_push(arr, 30);

    if (atlas_array_erase(arr, 0) != ATLAS_SUCCESS) {
        atlas_array_destroy(&arr);
        return 1;
    }

    int expected[] = {20, 30};
    for (size_t i = 0; i < 2; i++) {
        int value = 0;
        if (atlas_array_get(arr, i, &value) != ATLAS_SUCCESS || value != expected[i]) {
            atlas_array_destroy(&arr);
            return 1;
        }
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_erase_end(void) {
    AtlasArray *arr = atlas_array_create(3);
    if (!arr) {
        return 1;
    }

    atlas_array_push(arr, 10);
    atlas_array_push(arr, 20);
    atlas_array_push(arr, 30);

    if (atlas_array_erase(arr, 2) != ATLAS_SUCCESS) {
        atlas_array_destroy(&arr);
        return 1;
    }

    int expected[] = {10, 20};
    for (size_t i = 0; i < 2; i++) {
        int value = 0;
        if (atlas_array_get(arr, i, &value) != ATLAS_SUCCESS || value != expected[i]) {
            atlas_array_destroy(&arr);
            return 1;
        }
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_erase_single(void) {
    AtlasArray *arr = atlas_array_create(1);
    if (!arr) {
        return 1;
    }

    atlas_array_push(arr, 10);

    if (atlas_array_erase(arr, 0) != ATLAS_SUCCESS) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_size(arr) != 0) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_erase_validation(void) {
    AtlasArray *arr = atlas_array_create(3);
    if (!arr) {
        return 1;
    }

    atlas_array_push(arr, 10);

    if (atlas_array_erase(NULL, 0) != ATLAS_ERROR_NULL) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_erase(arr, 999) != ATLAS_ERROR_BOUNDS) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_pop(void) {
    AtlasArray *arr = atlas_array_create(2);
    if (!arr) {
        return 1;
    }

    atlas_array_push(arr, 10);
    atlas_array_push(arr, 99);

    int removed = 0;
    if (atlas_array_pop(arr, &removed) != ATLAS_SUCCESS || removed != 99) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_size(arr) != 1) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_pop_validation(void) {
    AtlasArray *arr = atlas_array_create(2);
    if (!arr) {
        return 1;
    }

    int dummy = 0;

    if (atlas_array_pop(NULL, &dummy) != ATLAS_ERROR_NULL) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_pop(arr, NULL) != ATLAS_ERROR_NULL) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_pop(arr, &dummy) != ATLAS_ERROR_EMPTY) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_get_elements(void) {
    AtlasArray *arr = atlas_array_create(3);
    if (!arr) {
        return 1;
    }

    int values[] = {5, 10, 15};
    for (size_t i = 0; i < 3; i++) {
        atlas_array_push(arr, values[i]);
    }

    for (size_t i = 0; i < 3; i++) {
        int value = 0;
        if (atlas_array_get(arr, i, &value) != ATLAS_SUCCESS || value != values[i]) {
            atlas_array_destroy(&arr);
            return 1;
        }
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_get_validation(void) {
    AtlasArray *arr = atlas_array_create(2);
    if (!arr) {
        return 1;
    }

    atlas_array_push(arr, 10);

    int value = 0;
    if (atlas_array_get(arr, 999, &value) != ATLAS_ERROR_BOUNDS) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_get(NULL, 0, &value) != ATLAS_ERROR_NULL) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_get(arr, 0, NULL) != ATLAS_ERROR_NULL) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_front_back(void) {
    AtlasArray *arr = atlas_array_create(3);
    if (!arr) {
        return 1;
    }

    atlas_array_push(arr, 10);
    atlas_array_push(arr, 20);
    atlas_array_push(arr, 30);

    int front = 0;
    int back = 0;

    if (atlas_array_front(arr, &front) != ATLAS_SUCCESS || front != 10) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_back(arr, &back) != ATLAS_SUCCESS || back != 30) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_front_back_validation(void) {
    AtlasArray *arr = atlas_array_create(2);
    if (!arr) {
        return 1;
    }

    int dummy = 0;

    if (atlas_array_front(NULL, &dummy) != ATLAS_ERROR_NULL) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_front(arr, NULL) != ATLAS_ERROR_NULL) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_front(arr, &dummy) != ATLAS_ERROR_EMPTY) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_back(NULL, &dummy) != ATLAS_ERROR_NULL) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_back(arr, NULL) != ATLAS_ERROR_NULL) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_back(arr, &dummy) != ATLAS_ERROR_EMPTY) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_find_existing(void) {
    AtlasArray *arr = atlas_array_create(5);
    if (!arr) {
        return 1;
    }

    atlas_array_push(arr, 10);
    atlas_array_push(arr, 20);
    atlas_array_push(arr, 30);
    atlas_array_push(arr, 20);

    size_t index = 999;
    if (atlas_array_find(arr, &index, 20) != ATLAS_SUCCESS || index != 1) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_find_missing(void) {
    AtlasArray *arr = atlas_array_create(3);
    if (!arr) {
        return 1;
    }

    atlas_array_push(arr, 10);
    atlas_array_push(arr, 20);

    size_t index = 777;
    if (atlas_array_find(arr, &index, 99) != ATLAS_ERROR_NOT_FOUND) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (index != 777) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_find_validation(void) {
    AtlasArray *arr = atlas_array_create(2);
    if (!arr) {
        return 1;
    }

    size_t index = 0;

    if (atlas_array_find(NULL, &index, 10) != ATLAS_ERROR_NULL) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_find(arr, NULL, 10) != ATLAS_ERROR_NULL) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_contains_existing(void) {
    AtlasArray *arr = atlas_array_create(3);
    if (!arr) {
        return 1;
    }

    atlas_array_push(arr, 10);
    atlas_array_push(arr, 20);

    bool contains = false;
    if (atlas_array_contains(arr, &contains, 20) != ATLAS_SUCCESS || !contains) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_contains_missing(void) {
    AtlasArray *arr = atlas_array_create(3);
    if (!arr) {
        return 1;
    }

    atlas_array_push(arr, 10);
    atlas_array_push(arr, 20);

    bool contains = true;
    if (atlas_array_contains(arr, &contains, 99) != ATLAS_SUCCESS || contains) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_contains_validation(void) {
    AtlasArray *arr = atlas_array_create(2);
    if (!arr) {
        return 1;
    }

    bool dummy = false;

    if (atlas_array_contains(NULL, &dummy, 10) != ATLAS_ERROR_NULL) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_contains(arr, NULL, 10) != ATLAS_ERROR_NULL) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_reserve(void) {
    AtlasArray *arr = atlas_array_create(3);
    if (!arr) {
        return 1;
    }

    size_t requested = atlas_array_capacity(arr) + 10;
    if (atlas_array_reserve(arr, requested) != ATLAS_SUCCESS) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_capacity(arr) != requested) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_reserve_idempotent(void) {
    AtlasArray *arr = atlas_array_create(3);
    if (!arr) {
        return 1;
    }

    size_t requested = atlas_array_capacity(arr) + 10;
    atlas_array_reserve(arr, requested);

    if (atlas_array_reserve(arr, requested - 5) != ATLAS_SUCCESS) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_capacity(arr) != requested) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_reserve_validation(void) {
    if (atlas_array_reserve(NULL, 100) != ATLAS_ERROR_NULL) {
        return 1;
    }

    return 0;
}

static int test_shrink_to_fit(void) {
    AtlasArray *arr = atlas_array_create(3);
    if (!arr) {
        return 1;
    }

    atlas_array_push(arr, 10);
    atlas_array_push(arr, 20);
    atlas_array_push(arr, 30);
    atlas_array_push(arr, 40);

    if (atlas_array_shrink_to_fit(arr) != ATLAS_SUCCESS) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_capacity(arr) != atlas_array_size(arr)) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_shrink_to_fit_idempotent(void) {
    AtlasArray *arr = atlas_array_create(3);
    if (!arr) {
        return 1;
    }

    atlas_array_push(arr, 10);
    atlas_array_shrink_to_fit(arr);

    size_t capacity_before = atlas_array_capacity(arr);
    if (atlas_array_shrink_to_fit(arr) != ATLAS_SUCCESS) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_capacity(arr) != capacity_before) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_shrink_to_fit_validation(void) {
    if (atlas_array_shrink_to_fit(NULL) != ATLAS_ERROR_NULL) {
        return 1;
    }

    return 0;
}

static int test_swap(void) {
    AtlasArray *arr = atlas_array_create(4);
    if (!arr) {
        return 1;
    }

    atlas_array_push(arr, 10);
    atlas_array_push(arr, 20);
    atlas_array_push(arr, 30);
    atlas_array_push(arr, 40);

    if (atlas_array_swap(arr, 0, 3) != ATLAS_SUCCESS) {
        atlas_array_destroy(&arr);
        return 1;
    }

    int value = 0;
    atlas_array_get(arr, 0, &value);
    if (value != 40) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_get(arr, 3, &value);
    if (value != 10) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_swap_idempotent(void) {
    AtlasArray *arr = atlas_array_create(4);
    if (!arr) {
        return 1;
    }

    atlas_array_push(arr, 10);
    atlas_array_push(arr, 20);

    size_t size_before = atlas_array_size(arr);
    size_t capacity_before = atlas_array_capacity(arr);

    if (atlas_array_swap(arr, 1, 1) != ATLAS_SUCCESS) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_size(arr) != size_before || atlas_array_capacity(arr) != capacity_before) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_swap_validation(void) {
    AtlasArray *arr = atlas_array_create(2);
    if (!arr) {
        return 1;
    }

    atlas_array_push(arr, 10);

    if (atlas_array_swap(NULL, 0, 1) != ATLAS_ERROR_NULL) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_swap(arr, 999, 0) != ATLAS_ERROR_BOUNDS) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_copy(void) {
    AtlasArray *src = atlas_array_create(4);
    AtlasArray *dest = atlas_array_create(1);
    if (!src || !dest) {
        atlas_array_destroy(&src);
        atlas_array_destroy(&dest);
        return 1;
    }

    atlas_array_push(src, 10);
    atlas_array_push(src, 20);
    atlas_array_push(src, 30);
    atlas_array_push(src, 40);

    if (atlas_array_copy(src, dest) != ATLAS_SUCCESS) {
        atlas_array_destroy(&src);
        atlas_array_destroy(&dest);
        return 1;
    }

    if (atlas_array_size(dest) != 4) {
        atlas_array_destroy(&src);
        atlas_array_destroy(&dest);
        return 1;
    }

    for (size_t i = 0; i < 4; i++) {
        int src_value = 0;
        int dest_value = 0;
        atlas_array_get(src, i, &src_value);
        atlas_array_get(dest, i, &dest_value);
        if (src_value != dest_value) {
            atlas_array_destroy(&src);
            atlas_array_destroy(&dest);
            return 1;
        }
    }

    atlas_array_destroy(&src);
    atlas_array_destroy(&dest);
    return 0;
}

static int test_copy_preserves_source(void) {
    AtlasArray *src = atlas_array_create(2);
    AtlasArray *dest = atlas_array_create(1);
    if (!src || !dest) {
        atlas_array_destroy(&src);
        atlas_array_destroy(&dest);
        return 1;
    }

    atlas_array_push(src, 10);
    atlas_array_copy(src, dest);
    atlas_array_set(dest, 0, 999);

    int original = 0;
    atlas_array_get(src, 0, &original);
    if (original != 10) {
        atlas_array_destroy(&src);
        atlas_array_destroy(&dest);
        return 1;
    }

    atlas_array_destroy(&src);
    atlas_array_destroy(&dest);
    return 0;
}

static int test_copy_from_empty(void) {
    AtlasArray *empty_src = atlas_array_create(2);
    AtlasArray *dest = atlas_array_create(3);
    if (!empty_src || !dest) {
        atlas_array_destroy(&empty_src);
        atlas_array_destroy(&dest);
        return 1;
    }

    atlas_array_push(dest, 1);
    atlas_array_push(dest, 2);

    if (atlas_array_copy(empty_src, dest) != ATLAS_SUCCESS) {
        atlas_array_destroy(&empty_src);
        atlas_array_destroy(&dest);
        return 1;
    }

    if (atlas_array_size(dest) != 0) {
        atlas_array_destroy(&empty_src);
        atlas_array_destroy(&dest);
        return 1;
    }

    atlas_array_destroy(&empty_src);
    atlas_array_destroy(&dest);
    return 0;
}

static int test_copy_validation(void) {
    AtlasArray *arr = atlas_array_create(2);
    if (!arr) {
        return 1;
    }

    if (atlas_array_copy(NULL, arr) != ATLAS_ERROR_NULL) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_copy(arr, NULL) != ATLAS_ERROR_NULL) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_copy(arr, arr) != ATLAS_ERROR_INVALID_ARGUMENT) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_clone(void) {
    AtlasArray *src = atlas_array_create(6);
    if (!src) {
        return 1;
    }

    atlas_array_push(src, 10);
    atlas_array_push(src, 20);
    atlas_array_push(src, 30);
    atlas_array_push(src, 40);

    AtlasArray *clone = atlas_array_clone(src);
    if (!clone) {
        atlas_array_destroy(&src);
        return 1;
    }

    if (atlas_array_size(clone) != 4 || atlas_array_capacity(clone) != 6) {
        atlas_array_destroy(&clone);
        atlas_array_destroy(&src);
        return 1;
    }

    atlas_array_destroy(&clone);
    atlas_array_destroy(&src);
    return 0;
}

static int test_clone_empty(void) {
    AtlasArray *src = atlas_array_create(3);
    if (!src) {
        return 1;
    }

    AtlasArray *clone = atlas_array_clone(src);
    if (!clone) {
        atlas_array_destroy(&src);
        return 1;
    }

    if (atlas_array_size(clone) != 0 || atlas_array_capacity(clone) != 3) {
        atlas_array_destroy(&clone);
        atlas_array_destroy(&src);
        return 1;
    }

    atlas_array_destroy(&clone);
    atlas_array_destroy(&src);
    return 0;
}

static int test_clone_null(void) {
    if (atlas_array_clone(NULL) != NULL) {
        return 1;
    }

    return 0;
}

static int test_set(void) {
    AtlasArray *arr = atlas_array_create(3);
    if (!arr) {
        return 1;
    }

    atlas_array_push(arr, 10);

    if (atlas_array_set(arr, 0, 25) != ATLAS_SUCCESS) {
        atlas_array_destroy(&arr);
        return 1;
    }

    int value = 0;
    if (atlas_array_get(arr, 0, &value) != ATLAS_SUCCESS || value != 25) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_set_validation(void) {
    AtlasArray *arr = atlas_array_create(2);
    if (!arr) {
        return 1;
    }

    if (atlas_array_set(NULL, 0, 123) != ATLAS_ERROR_NULL) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_set(arr, atlas_array_size(arr), 123) != ATLAS_ERROR_BOUNDS) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_clear(void) {
    AtlasArray *arr = atlas_array_create(4);
    if (!arr) {
        return 1;
    }

    atlas_array_push(arr, 10);
    atlas_array_push(arr, 20);

    size_t capacity_before = atlas_array_capacity(arr);
    if (atlas_array_clear(arr) != ATLAS_SUCCESS) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_size(arr) != 0) {
        atlas_array_destroy(&arr);
        return 1;
    }

    if (atlas_array_capacity(arr) != capacity_before) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

static int test_clear_validation(void) {
    if (atlas_array_clear(NULL) != ATLAS_ERROR_NULL) {
        return 1;
    }

    return 0;
}

static int test_empty_state(void) {
    AtlasArray *arr = atlas_array_create(2);
    if (!arr) {
        return 1;
    }

    bool is_empty = false;
    if (atlas_array_empty(arr, &is_empty) != ATLAS_SUCCESS || !is_empty) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_push(arr, 99);

    if (atlas_array_empty(arr, &is_empty) != ATLAS_SUCCESS || is_empty) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);
    return 0;
}

int main(void) {
    printf("\n" COLOR_BOLD_BLUE "╭────────────────────────────────────────────────────────╮" COLOR_RESET "\n");
    printf(COLOR_BOLD_BLUE "│" COLOR_RESET "               AtlasDS - Dynamic Array Tests            " COLOR_BOLD_BLUE "│" COLOR_RESET "\n");
    printf(COLOR_BOLD_BLUE "╰────────────────────────────────────────────────────────╯" COLOR_RESET "\n\n");

    printf(COLOR_YELLOW "ℹ " COLOR_RESET "Starting AtlasDS dynamic array tests...\n\n");

    // =========================================================
    // Lifecycle
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Lifecycle" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_create_destroy()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Create/Destroy operation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Create/Destroy operation\n");

    if (test_initial_state()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Initial size/capacity validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Initial size/capacity validation\n");

    if (test_destroy_null()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL destroy validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL destroy validation\n\n");

    // =========================================================
    // Insertion
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Insertion" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_push_and_resize()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Push and automatic resize\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Push and automatic resize\n");

    if (test_insert_middle()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Insert in middle\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Insert in middle\n");

    if (test_insert_begin()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Insert at beginning\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Insert at beginning\n");

    if (test_insert_end()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Insert at end\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Insert at end\n");

    if (test_insertion_validation()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL/bounds insertion validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL/bounds insertion validation\n\n");

    // =========================================================
    // Removal
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Removal" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_erase_middle()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Erase in middle\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Erase in middle\n");

    if (test_erase_begin()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Erase at beginning\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Erase at beginning\n");

    if (test_erase_end()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Erase at end\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Erase at end\n");

    if (test_erase_single()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Erase single element\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Erase single element\n");

    if (test_erase_validation()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL/bounds erase validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL/bounds erase validation\n");

    if (test_pop()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Pop last element\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Pop last element\n");

    if (test_pop_validation()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL/empty pop validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL/empty pop validation\n\n");

    // =========================================================
    // Access
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Access" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_get_elements()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Element retrieval\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Element retrieval\n");

    if (test_get_validation()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL get validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL get validation\n");

    if (test_front_back()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Front/Back element retrieval\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Front/Back element retrieval\n");

    if (test_front_back_validation()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL/empty Front/Back validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL/empty Front/Back validation\n\n");

    // =========================================================
    // Search
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Search" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_find_existing()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Find existing value\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Find existing value\n");

    if (test_find_missing()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Find missing value\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Find missing value\n");

    if (test_find_validation()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL find validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL find validation\n");

    if (test_contains_existing()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Contains existing value\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Contains existing value\n");

    if (test_contains_missing()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Contains missing value\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Contains missing value\n");

    if (test_contains_validation()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL contains validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL contains validation\n\n");

    // =========================================================
    // Capacity
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Capacity" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_reserve()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Reserve expands capacity\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Reserve expands capacity\n");

    if (test_reserve_idempotent()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Downward reserve ignored\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Downward reserve ignored\n");

    if (test_reserve_validation()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL reserve validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL reserve validation\n");

    if (test_shrink_to_fit()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Shrink to fit\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Shrink to fit\n");

    if (test_shrink_to_fit_idempotent()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Shrink to fit idempotency\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Shrink to fit idempotency\n");

    if (test_shrink_to_fit_validation()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL shrink_to_fit validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL shrink_to_fit validation\n\n");

    // =========================================================
    // Utility
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Utility" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_swap()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Element exchange\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Element exchange\n");

    if (test_swap_idempotent()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Swap idempotency\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Swap idempotency\n");

    if (test_swap_validation()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL/bounds swap validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL/bounds swap validation\n");

    if (test_copy()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Array copy\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Array copy\n");

    if (test_copy_preserves_source()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Source array preserved after copy\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Source array preserved after copy\n");

    if (test_copy_from_empty()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Copy from empty source\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Copy from empty source\n");

    if (test_copy_validation()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL/self copy validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL/self copy validation\n");

    if (test_clone()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Clone array\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Clone array\n");

    if (test_clone_empty()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Clone empty array\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Clone empty array\n");

    if (test_clone_null()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL clone validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL clone validation\n");

    if (test_set()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Set element at index\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Set element at index\n");

    if (test_set_validation()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL/bounds set validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL/bounds set validation\n\n");

    // =========================================================
    // Clear & Empty
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Clear & Empty" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_clear()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Clear without truncating capacity\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Clear without truncating capacity\n");

    if (test_clear_validation()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL clear validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL clear validation\n");

    if (test_empty_state()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Empty state detection\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Empty state detection\n\n");

    printf(COLOR_BOLD_CYAN "════════════════════════════════════════════════════════\n" COLOR_RESET "\n");
    printf(COLOR_BOLD_GREEN " ✔ SUCCESS:" COLOR_RESET " All tests were completed successfully.\n\n");

    return 0;
}