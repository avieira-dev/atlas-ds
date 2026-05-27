#include "atlas/array.h"

#include <stdlib.h>

struct atlas_array {

    int *data; // Contiguous storage buffer
    size_t size; // Current number of elements in the array
    size_t capacity; // Current capacity supported by the dynamic array
};

// =====================
// Lifecycle
// =====================

/*
 * Implementation of atlas_array_create:
 * Allocates memory for both the control structure and the underlying
 * data buffer. Handles fallbacks for default capacity and performs
 * cleanup if the second allocation fails to avoid memory leaks.
 */
AtlasArray *atlas_array_create(size_t initial_capacity) {

    AtlasArray *array = malloc(sizeof(struct atlas_array));

    if (!array) {
        return NULL;
    }

    array->size = 0;

    if (initial_capacity == 0) {
        array->capacity = STANDARD_CAPACITY;
    } else {
        array->capacity = initial_capacity;
    }

    int *ptr_data = malloc(sizeof(int) * array->capacity);

    if (!ptr_data) {
        free(array);
        return NULL;
    }

    array->data = ptr_data;

    return array;
}

/*
 * Implementation of atlas_array_destroy:
 * Safely frees the internal buffer first, then the main structure.
 * Takes a double pointer to reset the user's pointer to NULL,
 * preventing accidental dangling pointer access.
 */
void atlas_array_destroy(AtlasArray **ptr_atlas_array) {

    if (!ptr_atlas_array || !*ptr_atlas_array) {
        return;
    }

    AtlasArray *array = *ptr_atlas_array;

    free(array->data);
    free(array);

    *ptr_atlas_array = NULL;
}

// =====================
// Operations
// =====================

/*
 * Implementation of atlas_array_push:
 * Appends a value to the array. If the size reaches current capacity,
 * it uses realloc to double the capacity. Includes defensive checks
 * to prevent crashes if realloc fails.
 */
int atlas_array_push(AtlasArray *arr, int value) {

    if (!arr) {
        return -1;
    }

    if (arr->size == arr->capacity) {
        size_t new_capacity = arr->capacity * 2;
        int *temp = realloc(arr->data, sizeof(int) * new_capacity);

        if (!temp) {
            return -1;
        }

        arr->data = temp;
        arr->capacity = new_capacity;
    }

    arr->data[arr->size] = value;
    arr->size++;

    return 0;
}

/*
 * Implementation of atlas_array_get:
 * Safe bounds-checked getter. Validates the instance, the output target,
 * and ensures the index is strictly less than the current size.
 */
int atlas_array_get(const AtlasArray *arr, size_t index, int *out_value) {

    if (!arr || !out_value || index >= arr->size) {
        return -1;
    }

    *out_value = arr->data[index];

    return 0;
}

/*
 * Implementation of atlas_array_size:
 * Returns the logical size (element count) of the dynamic array.
 * Returns 0 as a safe fallback if the array pointer is NULL.
 */
size_t atlas_array_size(const AtlasArray *arr) {

    if (!arr) {
        return 0;
    }

    return arr->size;
}

/*
 * Implementation of atlas_array_capacity:
 * Returns the total allocated slots in the internal buffer.
 * Returns 0 as a safe fallback if the array pointer is NULL.
 */
size_t atlas_array_capacity(const AtlasArray *arr) {

    if (!arr) {
        return 0;
    }

    return arr->capacity;
}

/*
 * Implementation of atlas_array_pop:
 * Removes the last logical element from the array and returns
 * its value through the output parameter. Does not reduce the
 * allocated capacity of the internal buffer.
 */
int atlas_array_pop(AtlasArray *arr, int *out_value) {

    if (!arr || !out_value || arr->size == 0) {
        return -1;
    }

    size_t last_index = arr->size - 1;
    *out_value = arr->data[last_index];
    
    arr->size--;

    return 0;
}

/*
 * Implementation of atlas_array_reserve:
 * Ensures that the dynamic array has at least the requested capacity.
 * If new_capacity is less than or equal to the current capacity, the
 * function performs no operation and returns success (idempotent behavior).
 *
 * If expansion is required, the function allocates a new memory block
 * using realloc and only updates the internal pointer and capacity after
 * successful allocation, preserving the previous state in case of failure.
 */
int atlas_array_reserve(AtlasArray *arr, size_t new_capacity){

    if (!arr) {
        return -1;
    }

    if (new_capacity > arr->capacity) {
        int *temp = realloc(arr->data, sizeof(int) * new_capacity);

        if (!temp) {
            return -1;
        }

        arr->data = temp;
        arr->capacity = new_capacity;
    }

    return 0;
}