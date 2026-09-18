/*
 * AtlasDS
 * Generic Dynamic Array Tests
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#include <stdio.h>

#include "atlas/array_void.h"
#include "atlas/status.h"
#include "atlas/terminal.h"

static int compare_int(const void *a, const void *b) {
    int value_a = *(const int *)a;
    int value_b = *(const int *)b;

    if (value_a == value_b) {
        return 0;
    }

    return 1;
}

static int test_create_destroy(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 0);

    if (!array) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    if (array != NULL) {
        return 1;
    }

    return 0;
}

static int test_create_invalid_type_size(void) {
    AtlasArrayVoid *array = atlas_array_void_create(0, 0);

    return array == NULL ? 0 : 1;
}

static int test_destroy_null(void) {
    if (atlas_array_void_destroy(NULL) != ATLAS_ERROR_NULL) {
        return 1;
    }

    AtlasArrayVoid *array = NULL;

    if (atlas_array_void_destroy(&array) != ATLAS_ERROR_NULL) {
        return 1;
    }

    return 0;
}

static int test_push_pop_int(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 1);

    if (!array) {
        return 1;
    }

    int value = 10;

    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    value = 20;

    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    int out = 0;

    if (atlas_array_void_pop(array, &out) != ATLAS_SUCCESS || out != 20) {
        return 1;
    }

    if (atlas_array_void_pop(array, &out) != ATLAS_SUCCESS || out != 10) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_push_pop_double(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(double), 1);

    if (!array) {
        return 1;
    }

    double value = 3.14;

    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    double out = 0.0;

    if (atlas_array_void_pop(array, &out) != ATLAS_SUCCESS || out != value) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_push_null(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 1);

    if (!array) {
        return 1;
    }

    int value = 10;

    if (atlas_array_void_push(NULL, &value) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_array_void_push(array, NULL) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_insert(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 2);

    if (!array) {
        return 1;
    }

    int value = 10;
    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) return 1;

    value = 30;
    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) return 1;

    value = 20;

    if (atlas_array_void_insert(array, 1, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    int out = 0;

    if (atlas_array_void_get(array, 0, &out) != ATLAS_SUCCESS || out != 10) return 1;
    if (atlas_array_void_get(array, 1, &out) != ATLAS_SUCCESS || out != 20) return 1;
    if (atlas_array_void_get(array, 2, &out) != ATLAS_SUCCESS || out != 30) return 1;

    size_t size = 0;

    if (atlas_array_void_size(array, &size) != ATLAS_SUCCESS || size != 3) return 1;

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_insert_invalid(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 2);

    if (!array) {
        return 1;
    }

    int value = 10;

    if (atlas_array_void_insert(NULL, 0, &value) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_array_void_insert(array, 0, NULL) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_array_void_insert(array, 1, &value) != ATLAS_ERROR_BOUNDS) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_pop_empty(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 1);

    if (!array) {
        return 1;
    }

    int out = 0;

    if (atlas_array_void_pop(array, &out) != ATLAS_ERROR_EMPTY) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_pop_null(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 1);

    if (!array) {
        return 1;
    }

    int out = 0;

    if (atlas_array_void_pop(NULL, &out) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_array_void_pop(array, NULL) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_erase(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 3);

    if (!array) {
        return 1;
    }

    int value = 10;
    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) return 1;

    value = 20;
    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) return 1;

    value = 30;
    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) return 1;

    if (atlas_array_void_erase(array, 1) != ATLAS_SUCCESS) {
        return 1;
    }

    int out = 0;

    if (atlas_array_void_get(array, 0, &out) != ATLAS_SUCCESS || out != 10) return 1;
    if (atlas_array_void_get(array, 1, &out) != ATLAS_SUCCESS || out != 30) return 1;

    size_t size = 0;

    if (atlas_array_void_size(array, &size) != ATLAS_SUCCESS || size != 2) return 1;

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_erase_invalid(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 2);

    if (!array) {
        return 1;
    }

    if (atlas_array_void_erase(NULL, 0) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_array_void_erase(array, 0) != ATLAS_ERROR_BOUNDS) {
        return 1;
    }

    int value = 10;

    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) return 1;

    if (atlas_array_void_erase(array, 1) != ATLAS_ERROR_BOUNDS) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_get_set_int(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 1);

    if (!array) {
        return 1;
    }

    int value = 10;

    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    value = 20;

    if (atlas_array_void_set(array, 0, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    int out = 0;

    if (atlas_array_void_get(array, 0, &out) != ATLAS_SUCCESS) {
        return 1;
    }

    if (out != 20) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_get_invalid(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 1);

    if (!array) {
        return 1;
    }

    int out = 0;

    if (atlas_array_void_get(NULL, 0, &out) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_array_void_get(array, 0, NULL) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_array_void_get(array, 0, &out) != ATLAS_ERROR_BOUNDS) {
        return 1;
    }

    int value = 10;

    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    if (atlas_array_void_get(array, 1, &out) != ATLAS_ERROR_BOUNDS) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_set_invalid(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 1);

    if (!array) {
        return 1;
    }

    int value = 20;

    if (atlas_array_void_set(NULL, 0, &value) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_array_void_set(array, 0, NULL) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_array_void_set(array, 0, &value) != ATLAS_ERROR_BOUNDS) {
        return 1;
    }

    int original = 10;

    if (atlas_array_void_push(array, &original) != ATLAS_SUCCESS) {
        return 1;
    }

    if (atlas_array_void_set(array, 1, &value) != ATLAS_ERROR_BOUNDS) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_front_back(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 1);

    if (!array) {
        return 1;
    }

    int value = 10;

    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    value = 20;

    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    int front = 0;
    int back = 0;

    if (atlas_array_void_front(array, &front) != ATLAS_SUCCESS) {
        return 1;
    }

    if (atlas_array_void_back(array, &back) != ATLAS_SUCCESS) {
        return 1;
    }

    if (front != 10 || back != 20) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_front_back_empty(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 1);

    if (!array) {
        return 1;
    }

    int value = 0;

    if (atlas_array_void_front(array, &value) != ATLAS_ERROR_EMPTY) {
        return 1;
    }

    if (atlas_array_void_back(array, &value) != ATLAS_ERROR_EMPTY) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_front_back_null(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 1);

    if (!array) {
        return 1;
    }

    int value = 0;

    if (atlas_array_void_front(NULL, &value) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_array_void_front(array, NULL) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_array_void_back(NULL, &value) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_array_void_back(array, NULL) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_find(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 3);

    if (!array) {
        return 1;
    }

    int value = 10;

    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    value = 20;

    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    value = 30;

    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    size_t index = 0;
    int target = 20;

    if (!atlas_array_void_find(array, &index, &target, compare_int)) {
        return 1;
    }

    if (index != 1) {
        return 1;
    }

    target = 99;

    if (atlas_array_void_find(array, &index, &target, compare_int)) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_find_invalid(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 1);

    if (!array) {
        return 1;
    }

    int value = 10;
    size_t index = 0;

    if (atlas_array_void_find(NULL, &index, &value, compare_int)) {
        return 1;
    }

    if (atlas_array_void_find(array, NULL, &value, compare_int)) {
        return 1;
    }

    if (atlas_array_void_find(array, &index, NULL, compare_int)) {
        return 1;
    }

    if (atlas_array_void_find(array, &index, &value, NULL)) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_contains(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 2);

    if (!array) {
        return 1;
    }

    int value = 10;

    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    value = 20;

    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    int target = 20;

    if (!atlas_array_void_contains(array, &target, compare_int)) {
        return 1;
    }

    target = 50;

    if (atlas_array_void_contains(array, &target, compare_int)) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_contains_invalid(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 1);

    if (!array) {
        return 1;
    }

    int value = 10;

    if (atlas_array_void_contains(NULL, &value, compare_int)) {
        return 1;
    }

    if (atlas_array_void_contains(array, NULL, compare_int)) {
        return 1;
    }

    if (atlas_array_void_contains(array, &value, NULL)) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_reserve(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 1);

    if (!array) {
        return 1;
    }

    if (atlas_array_void_reserve(array, 10) != ATLAS_SUCCESS) {
        return 1;
    }

    size_t capacity = 0;

    if (atlas_array_void_capacity(array, &capacity) != ATLAS_SUCCESS) {
        return 1;
    }

    if (capacity != 10) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_reserve_invalid(void) {
    if (atlas_array_void_reserve(NULL, 10) != ATLAS_ERROR_NULL) {
        return 1;
    }

    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 5);

    if (!array) {
        return 1;
    }

    if (atlas_array_void_reserve(array, 3) != ATLAS_SUCCESS) {
        return 1;
    }

    size_t capacity = 0;

    if (atlas_array_void_capacity(array, &capacity) != ATLAS_SUCCESS) {
        return 1;
    }

    if (capacity != 5) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_shrink_to_fit(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 10);

    if (!array) {
        return 1;
    }

    int value = 10;
    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) return 1;

    value = 20;
    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) return 1;

    if (atlas_array_void_shrink_to_fit(array) != ATLAS_SUCCESS) {
        return 1;
    }

    size_t capacity = 0;

    if (atlas_array_void_capacity(array, &capacity) != ATLAS_SUCCESS) {
        return 1;
    }

    if (capacity != 2) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_shrink_to_fit_invalid(void) {
    if (atlas_array_void_shrink_to_fit(NULL) != ATLAS_ERROR_NULL) {
        return 1;
    }

    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 10);

    if (!array) {
        return 1;
    }

    if (atlas_array_void_shrink_to_fit(array) != ATLAS_SUCCESS) {
        return 1;
    }

    size_t capacity = 0;

    if (atlas_array_void_capacity(array, &capacity) != ATLAS_SUCCESS) {
        return 1;
    }

    if (capacity != 1) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_swap(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 3);

    if (!array) {
        return 1;
    }

    int value = 10;
    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    value = 20;
    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    value = 30;
    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    if (atlas_array_void_swap(array, 0, 2) != ATLAS_SUCCESS) {
        return 1;
    }

    int out = 0;

    if (atlas_array_void_get(array, 0, &out) != ATLAS_SUCCESS || out != 30) {
        return 1;
    }

    if (atlas_array_void_get(array, 1, &out) != ATLAS_SUCCESS || out != 20) {
        return 1;
    }

    if (atlas_array_void_get(array, 2, &out) != ATLAS_SUCCESS || out != 10) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_swap_invalid(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 2);

    if (!array) {
        return 1;
    }

    if (atlas_array_void_swap(NULL, 0, 0) != ATLAS_ERROR_NULL) {
        return 1;
    }

    int value = 10;

    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    if (atlas_array_void_swap(array, 0, 1) != ATLAS_ERROR_BOUNDS) {
        return 1;
    }

    if (atlas_array_void_swap(array, 1, 0) != ATLAS_ERROR_BOUNDS) {
        return 1;
    }

    if (atlas_array_void_swap(array, 1, 1) != ATLAS_ERROR_BOUNDS) {
        return 1;
    }

    if (atlas_array_void_swap(array, 0, 0) != ATLAS_SUCCESS) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_copy(void) {
    AtlasArrayVoid *src = atlas_array_void_create(sizeof(int), 2);
    AtlasArrayVoid *dest = atlas_array_void_create(sizeof(int), 1);

    if (!src || !dest) {
        return 1;
    }

    int value = 10;
    if (atlas_array_void_push(src, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    value = 20;
    if (atlas_array_void_push(src, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    if (atlas_array_void_copy(src, dest) != ATLAS_SUCCESS) {
        return 1;
    }

    size_t size = 0;

    if (atlas_array_void_size(dest, &size) != ATLAS_SUCCESS || size != 2) {
        return 1;
    }

    int out = 0;

    if (atlas_array_void_get(dest, 0, &out) != ATLAS_SUCCESS || out != 10) {
        return 1;
    }

    if (atlas_array_void_get(dest, 1, &out) != ATLAS_SUCCESS || out != 20) {
        return 1;
    }

    atlas_array_void_destroy(&src);
    atlas_array_void_destroy(&dest);

    return 0;
}

static int test_copy_invalid(void) {
    AtlasArrayVoid *a = atlas_array_void_create(sizeof(int), 1);
    AtlasArrayVoid *b = atlas_array_void_create(sizeof(double), 1);

    if (!a || !b) {
        return 1;
    }

    if (atlas_array_void_copy(NULL, a) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_array_void_copy(a, NULL) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_array_void_copy(a, b) != ATLAS_ERROR_TYPE) {
        return 1;
    }

    if (atlas_array_void_copy(a, a) != ATLAS_SUCCESS) {
        return 1;
    }

    atlas_array_void_destroy(&a);
    atlas_array_void_destroy(&b);

    return 0;
}

static int test_clone(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 2);

    if (!array) {
        return 1;
    }

    int value = 10;
    atlas_array_void_push(array, &value);

    value = 20;
    atlas_array_void_push(array, &value);

    AtlasArrayVoid *clone = atlas_array_void_clone(array);

    if (!clone) {
        return 1;
    }

    size_t size = 0;

    if (atlas_array_void_size(clone, &size) != ATLAS_SUCCESS || size != 2) {
        return 1;
    }

    int out = 0;

    if (atlas_array_void_get(clone, 0, &out) != ATLAS_SUCCESS || out != 10) {
        return 1;
    }

    if (atlas_array_void_get(clone, 1, &out) != ATLAS_SUCCESS || out != 20) {
        return 1;
    }

    atlas_array_void_destroy(&clone);
    atlas_array_void_destroy(&array);

    return 0;
}

static int test_clone_invalid(void) {
    if (atlas_array_void_clone(NULL) != NULL) {
        return 1;
    }

    return 0;
}

static int test_size(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 1);

    if (!array) {
        return 1;
    }

    size_t size = 0;

    if (atlas_array_void_size(array, &size) != ATLAS_SUCCESS) {
        return 1;
    }

    if (size != 0) {
        return 1;
    }

    int value = 10;

    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    if (atlas_array_void_size(array, &size) != ATLAS_SUCCESS) {
        return 1;
    }

    if (size != 1) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_capacity(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 1);

    if (!array) {
        return 1;
    }

    size_t capacity = 0;

    if (atlas_array_void_capacity(array, &capacity) != ATLAS_SUCCESS) {
        return 1;
    }

    if (capacity != 1) {
        return 1;
    }

    int value = 10;

    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    if (atlas_array_void_capacity(array, &capacity) != ATLAS_SUCCESS) {
        return 1;
    }

    if (capacity != 2) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_empty(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 1);

    if (!array) {
        return 1;
    }

    bool empty = false;

    if (atlas_array_void_empty(array, &empty) != ATLAS_SUCCESS) {
        return 1;
    }

    if (!empty) {
        return 1;
    }

    int value = 10;

    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    if (atlas_array_void_empty(array, &empty) != ATLAS_SUCCESS) {
        return 1;
    }

    if (empty) {
        return 1;
    }

    if (atlas_array_void_pop(array, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    if (atlas_array_void_empty(array, &empty) != ATLAS_SUCCESS) {
        return 1;
    }

    if (!empty) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_clear(void) {
    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 2);

    if (!array) {
        return 1;
    }

    int value = 10;
    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) return 1;

    value = 20;
    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) return 1;

    if (atlas_array_void_clear(array) != ATLAS_SUCCESS) {
        return 1;
    }

    size_t size = 0;

    if (atlas_array_void_size(array, &size) != ATLAS_SUCCESS) {
        return 1;
    }

    if (size != 0) {
        return 1;
    }

    size_t capacity = 0;

    if (atlas_array_void_capacity(array, &capacity) != ATLAS_SUCCESS) {
        return 1;
    }

    if (capacity != 2) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_clear_invalid(void) {
    if (atlas_array_void_clear(NULL) != ATLAS_ERROR_NULL) {
        return 1;
    }

    return 0;
}

static int test_metadata_invalid(void) {
    size_t size = 0;
    size_t capacity = 0;
    bool empty = false;

    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 1);

    if (!array) {
        return 1;
    }

    if (atlas_array_void_size(NULL, &size) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_array_void_size(array, NULL) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_array_void_capacity(NULL, &capacity) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_array_void_capacity(array, NULL) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_array_void_empty(NULL, &empty) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_array_void_empty(array, NULL) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_array_void_destroy(&array) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

int main(void) {
    printf("\n" COLOR_BOLD_BLUE "╭────────────────────────────────────────────────────────╮" COLOR_RESET "\n");
    printf(COLOR_BOLD_BLUE "│" COLOR_RESET "          AtlasDS - Generic Dynamic Array Tests         " COLOR_BOLD_BLUE "│" COLOR_RESET "\n");
    printf(COLOR_BOLD_BLUE "╰────────────────────────────────────────────────────────╯" COLOR_RESET "\n\n");

    printf(COLOR_YELLOW "ℹ " COLOR_RESET "Starting AtlasDS generic dynamic array tests...\n\n");

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

    if (test_create_invalid_type_size()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Type size validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Type size validation\n");

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

    if (test_push_pop_int()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Push/pop with integers\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Push/pop with integers\n");

    if (test_push_pop_double()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Push/pop with doubles\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Push/pop with doubles\n");

    if (test_push_null()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL push validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL push validation\n");

    if (test_insert()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Insert in middle\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Insert in middle\n");

    if (test_insert_invalid()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL/bounds insert validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL/bounds insert validation\n\n");

    // =========================================================
    // Removal
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Removal" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_pop_empty()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Pop from empty array\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Pop from empty array\n");

    if (test_pop_null()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL pop validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL pop validation\n");

    if (test_erase()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Erase in middle\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Erase in middle\n");

    if (test_erase_invalid()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL/bounds erase validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL/bounds erase validation\n\n");

    // =========================================================
    // Access
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Access" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_get_set_int()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Get/Set element\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Get/Set element\n");

    if (test_get_invalid()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL/bounds get validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL/bounds get validation\n");

    if (test_set_invalid()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL/bounds set validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL/bounds set validation\n");

    if (test_front_back()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Front/Back element retrieval\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Front/Back element retrieval\n");

    if (test_front_back_empty()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Front/Back on empty array\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Front/Back on empty array\n");

    if (test_front_back_null()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL Front/Back validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL Front/Back validation\n\n");

    // =========================================================
    // Search
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Search" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_find()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Find value with comparator\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Find value with comparator\n");

    if (test_find_invalid()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL find validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL find validation\n");

    if (test_contains()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Contains value with comparator\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Contains value with comparator\n");

    if (test_contains_invalid()) {
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

    if (test_reserve_invalid()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL/downward reserve validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL/downward reserve validation\n");

    if (test_shrink_to_fit()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Shrink to fit\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Shrink to fit\n");

    if (test_shrink_to_fit_invalid()) {
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

    if (test_swap_invalid()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL/bounds swap validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL/bounds swap validation\n");

    if (test_copy()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Array copy\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Array copy\n");

    if (test_copy_invalid()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL/type-mismatch copy validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL/type-mismatch copy validation\n");

    if (test_clone()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Clone array\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Clone array\n");

    if (test_clone_invalid()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL clone validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL clone validation\n\n");

    // =========================================================
    // Metadata & Clear
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Metadata & Clear" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_size()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Size tracking\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Size tracking\n");

    if (test_capacity()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Capacity tracking\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Capacity tracking\n");

    if (test_empty()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Empty state detection\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Empty state detection\n");

    if (test_clear()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Clear without truncating capacity\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Clear without truncating capacity\n");

    if (test_clear_invalid()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL clear validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL clear validation\n");

    if (test_metadata_invalid()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL metadata validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL metadata validation\n\n");

    printf(COLOR_BOLD_CYAN "════════════════════════════════════════════════════════\n" COLOR_RESET "\n");
    printf(COLOR_BOLD_GREEN " ✔ SUCCESS:" COLOR_RESET " All tests were completed successfully.\n\n");

    return 0;
}