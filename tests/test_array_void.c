/*
 * AtlasDS
 * Generic Dynamic Array Tests
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#include "atlas/array_void.h"
#include "atlas/status.h"

#include <stdio.h>

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
    printf("\n");
    printf("\033[1;33m=========================================================\033[0m\n");
    printf("\033[1;33mAtlasDS - Generic Dynamic Array Tests\033[0m\n");
    printf("\033[1;33m=========================================================\033[0m\n\n");

    printf("\033[0;33m[INFO]\033[0m Starting AtlasDS generic dynamic array tests...\n\n");

    // =========================================================
    // Lifecycle
    // =========================================================
    printf("\033[0;33m[INFO]\033[0m Running lifecycle tests...\n");

    if (test_create_destroy()) {
        printf("\033[0;31m[ERROR]\033[0m test_create_destroy failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Create/Destroy test passed.\n");

    if (test_create_invalid_type_size()) {
        printf("\033[0;31m[ERROR]\033[0m test_create_invalid_type_size failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Invalid type_size validation passed.\n");

    if (test_destroy_null()) {
        printf("\033[0;31m[ERROR]\033[0m test_destroy_null failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m NULL destroy validation passed.\n\n");

    // =========================================================
    // Push / Pop
    // =========================================================
    printf("\033[0;33m[INFO]\033[0m Running push/pop tests...\n");

    if (test_push_pop_int()) {
        printf("\033[0;31m[ERROR]\033[0m test_push_pop_int failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Integer push/pop test passed.\n");

    if (test_push_pop_double()) {
        printf("\033[0;31m[ERROR]\033[0m test_push_pop_double failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Double push/pop test passed.\n");

    if (test_push_null()) {
        printf("\033[0;31m[ERROR]\033[0m test_push_null failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Push NULL validation passed.\n");

    if (test_pop_empty()) {
        printf("\033[0;31m[ERROR]\033[0m test_pop_empty failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Empty array validation passed.\n");

    if (test_pop_null()) {
        printf("\033[0;31m[ERROR]\033[0m test_pop_null failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Pop NULL validation passed.\n\n");

    // =========================================================
    // Get / Set
    // =========================================================
    printf("\033[0;33m[INFO]\033[0m Running get/set tests...\n");

    if (test_get_set_int()) {
        printf("\033[0;31m[ERROR]\033[0m test_get_set_int failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Get/Set test passed.\n");

    if (test_get_invalid()) {
        printf("\033[0;31m[ERROR]\033[0m test_get_invalid failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Get validation passed.\n");

    if (test_set_invalid()) {
        printf("\033[0;31m[ERROR]\033[0m test_set_invalid failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Set validation passed.\n\n");

    // =========================================================
    // Insert / Erase
    // =========================================================
    printf("\033[0;33m[INFO]\033[0m Running insert/erase tests...\n");

    if (test_insert()) {
        printf("\033[0;31m[ERROR]\033[0m test_insert failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Insert test passed.\n");

    if (test_insert_invalid()) {
        printf("\033[0;31m[ERROR]\033[0m test_insert_invalid failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Insert validation passed.\n");

    if (test_erase()) {
        printf("\033[0;31m[ERROR]\033[0m test_erase failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Erase test passed.\n");

    if (test_erase_invalid()) {
        printf("\033[0;31m[ERROR]\033[0m test_erase_invalid failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Erase validation passed.\n\n");

    // =========================================================
    // Swap
    // =========================================================
    printf("\033[0;33m[INFO]\033[0m Running swap tests...\n");

    if (test_swap()) {
        printf("\033[0;31m[ERROR]\033[0m test_swap failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Swap test passed.\n");

    if (test_swap_invalid()) {
        printf("\033[0;31m[ERROR]\033[0m test_swap_invalid failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Swap validation passed.\n\n");

    // =========================================================
    // Copy / Clone
    // =========================================================
    printf("\033[0;33m[INFO]\033[0m Running copy/clone tests...\n");

    if (test_copy()) {
        printf("\033[0;31m[ERROR]\033[0m test_copy failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Copy test passed.\n");

    if (test_copy_invalid()) {
        printf("\033[0;31m[ERROR]\033[0m test_copy_invalid failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Copy validation passed.\n");

    if (test_clone()) {
        printf("\033[0;31m[ERROR]\033[0m test_clone failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Clone test passed.\n");

    if (test_clone_invalid()) {
        printf("\033[0;31m[ERROR]\033[0m test_clone_invalid failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Clone validation passed.\n\n");

    // =========================================================
    // Search
    // =========================================================
    printf("\033[0;33m[INFO]\033[0m Running search tests...\n");

    if (test_find()) {
        printf("\033[0;31m[ERROR]\033[0m test_find failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Find test passed.\n");

    if (test_find_invalid()) {
        printf("\033[0;31m[ERROR]\033[0m test_find_invalid failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Find validation passed.\n");

    if (test_contains()) {
        printf("\033[0;31m[ERROR]\033[0m test_contains failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Contains test passed.\n");

    if (test_contains_invalid()) {
        printf("\033[0;31m[ERROR]\033[0m test_contains_invalid failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Contains validation passed.\n\n");

    // =========================================================
    // Metadata
    // =========================================================
    printf("\033[0;33m[INFO]\033[0m Running metadata tests...\n");

    if (test_size()) {
        printf("\033[0;31m[ERROR]\033[0m test_size failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Size test passed.\n");

    if (test_capacity()) {
        printf("\033[0;31m[ERROR]\033[0m test_capacity failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Capacity test passed.\n");

    if (test_empty()) {
        printf("\033[0;31m[ERROR]\033[0m test_empty failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Empty test passed.\n");

    if (test_front_back()) {
        printf("\033[0;31m[ERROR]\033[0m test_front_back failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Front/Back test passed.\n");

    if (test_front_back_empty()) {
        printf("\033[0;31m[ERROR]\033[0m test_front_back_empty failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Front/Back empty validation passed.\n");

    if (test_front_back_null()) {
        printf("\033[0;31m[ERROR]\033[0m test_front_back_null failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Front/Back NULL validation passed.\n");

    if (test_metadata_invalid()) {
        printf("\033[0;31m[ERROR]\033[0m test_metadata_invalid failed.\n\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Metadata validation passed.\n\n");

    // =========================================================
    // Capacity Management
    // =========================================================
    printf("\033[0;33m[INFO]\033[0m Running capacity management tests...\n");

    if (test_reserve()) {
        printf("\033[0;31m[ERROR]\033[0m test_reserve failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Reserve test passed.\n");

    if (test_reserve_invalid()) {
        printf("\033[0;31m[ERROR]\033[0m test_reserve_invalid failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Reserve validation passed.\n");

    if (test_clear()) {
        printf("\033[0;31m[ERROR]\033[0m test_clear failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Clear test passed.\n");

    if (test_clear_invalid()) {
        printf("\033[0;31m[ERROR]\033[0m test_clear_invalid failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Clear validation passed.\n");

    if (test_shrink_to_fit()) {
        printf("\033[0;31m[ERROR]\033[0m test_shrink_to_fit failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Shrink-to-fit test passed.\n");

    if (test_shrink_to_fit_invalid()) {
        printf("\033[0;31m[ERROR]\033[0m test_shrink_to_fit_invalid failed.\n");
        return 1;
    }
    printf("\033[0;32m[OK]\033[0m Shrink-to-fit validation passed.\n");

    printf("\n\033[1;32m[SUCCESS]\033[0m All Generic Dynamic Array tests passed successfully!\n\n");

    return 0;
}