/*
 * AtlasDS
 * Generic Dynamic Array Implementation
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#include "atlas/array_void.h"

#include <stdlib.h>

struct atlas_array_void {
    size_t type_size; // Size in bytes of each element
    size_t size;      // Current number of elements in the array
    size_t capacity;  // Current capacity supported by the dynamic array
    void *data;       // Contiguous storage buffer
};

// =====================
// Lifecycle
// =====================

/*
 * Implementation of atlas_array_void_create:
 * Allocates memory for both the control structure and the underlying generic
 * data buffer. Sets the array metadata and initializes the internal
 * storage using the requested capacity or the minimum supported
 * capacity when none is specified.
 */
AtlasArrayVoid *atlas_array_void_create(size_t type_size, size_t initial_capacity) {

    if (type_size == 0) {
        return NULL;
    }

    AtlasArrayVoid *array = malloc(sizeof(struct atlas_array_void));

    if (!array) {
        return NULL;
    }

    array->type_size = type_size;
    array->capacity = initial_capacity == 0 ? ATLAS_ARRAY_VOID_STANDARD_CAPACITY : initial_capacity;
    array->size = 0;

    void *ptr_data = malloc(array->type_size * array->capacity);

    if (!ptr_data) {
        free(array);
        return NULL;
    }

    array->data = ptr_data;

    return array;
}

/*
 * Implementation of atlas_array_void_destroy:
 * Safely frees the internal generic buffer first, then the main structure.
 * Takes a double pointer to validate inputs in defensive order and resets
 * the user's pointer to NULL, preventing accidental dangling pointer access.
 */
int atlas_array_void_destroy(AtlasArrayVoid **ptr_atlas_array_void) {

    if (!ptr_atlas_array_void || !*ptr_atlas_array_void) {
        return -1;
    }

    AtlasArrayVoid *array = *ptr_atlas_array_void;

    free(array->data);
    free(array);

    *ptr_atlas_array_void = NULL;

    return 0;
}