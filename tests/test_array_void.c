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

    if (atlas_array_void_destroy(NULL) != ATLAS_ERROR) {
        return 1;
    }

    AtlasArrayVoid *array = NULL;

    if (atlas_array_void_destroy(&array) != ATLAS_ERROR) {
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

    if (atlas_array_void_push(NULL, &value) != ATLAS_ERROR) {
        return 1;
    }

    if (atlas_array_void_push(array, NULL) != ATLAS_ERROR) {
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

    if (atlas_array_void_pop(array, &out) != ATLAS_ERROR) {
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

    if (atlas_array_void_pop(NULL, &out) != ATLAS_ERROR) {
        return 1;
    }

    if (atlas_array_void_pop(array, NULL) != ATLAS_ERROR) {
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

    if (atlas_array_void_get(NULL, 0, &out) != ATLAS_ERROR) {
        return 1;
    }

    if (atlas_array_void_get(array, 0, NULL) != ATLAS_ERROR) {
        return 1;
    }

    if (atlas_array_void_get(array, 0, &out) != ATLAS_ERROR) {
        return 1;
    }

    int value = 10;

    if (atlas_array_void_push(array, &value) != ATLAS_SUCCESS) {
        return 1;
    }

    if (atlas_array_void_get(array, 1, &out) != ATLAS_ERROR) {
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

    if (atlas_array_void_set(NULL, 0, &value) != ATLAS_ERROR) {
        return 1;
    }

    if (atlas_array_void_set(array, 0, NULL) != ATLAS_ERROR) {
        return 1;
    }

    if (atlas_array_void_set(array, 0, &value) != ATLAS_ERROR) {
        return 1;
    }

    int original = 10;

    if (atlas_array_void_push(array, &original) != ATLAS_SUCCESS) {
        return 1;
    }

    if (atlas_array_void_set(array, 1, &value) != ATLAS_ERROR) {
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

static int test_metadata_invalid(void) {

    size_t size = 0;
    size_t capacity = 0;
    bool empty = false;

    AtlasArrayVoid *array = atlas_array_void_create(sizeof(int), 1);

    if (!array) {
        return 1;
    }

    if (atlas_array_void_size(NULL, &size) != ATLAS_ERROR) {
        return 1;
    }

    if (atlas_array_void_size(array, NULL) != ATLAS_ERROR) {
        return 1;
    }

    if (atlas_array_void_capacity(NULL, &capacity) != ATLAS_ERROR) {
        return 1;
    }

    if (atlas_array_void_capacity(array, NULL) != ATLAS_ERROR) {
        return 1;
    }

    if (atlas_array_void_empty(NULL, &empty) != ATLAS_ERROR) {
        return 1;
    }

    if (atlas_array_void_empty(array, NULL) != ATLAS_ERROR) {
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

    printf("\033[0;32m[OK]\033[0m Double push/pop test passed.\n\n");

    // =========================================================
    // Get / Set
    // =========================================================
    printf("\033[0;33m[INFO]\033[0m Running get/set tests...\n");

    if (test_get_set_int()) {
        printf("\033[0;31m[ERROR]\033[0m test_get_set_int failed.\n");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Get/Set test passed.\n\n");

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

    printf("\033[0;32m[OK]\033[0m Empty test passed.\n\n");

    // =========================================================
    // Error Handling
    // =========================================================
    printf("\033[0;33m[INFO]\033[0m Running error handling tests...\n");

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

    printf("\033[0;32m[OK]\033[0m Pop NULL validation passed.\n");

    if (test_get_invalid()) {
        printf("\033[0;31m[ERROR]\033[0m test_get_invalid failed.\n");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Get validation passed.\n");

    if (test_set_invalid()) {
        printf("\033[0;31m[ERROR]\033[0m test_set_invalid failed.\n");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Set validation passed.\n");

    if (test_metadata_invalid()) {
        printf("\033[0;31m[ERROR]\033[0m test_metadata_invalid failed.\n");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Metadata validation passed.\n");

    printf("\n\033[0;32m[SUCCESS]\033[0m All Generic Dynamic Array tests passed successfully!\n\n");

    return 0;
}