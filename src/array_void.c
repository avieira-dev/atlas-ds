/*
 * AtlasDS
 * Generic Dynamic Array Implementation
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#include "atlas/array_void.h"
#include "atlas/status.h"

#include <stdlib.h>
#include <string.h>

struct atlas_array_void {
    
    size_t type_size; // Size in bytes of each element
    size_t size; // Current number of elements in the array
    size_t capacity; // Current capacity supported by the dynamic array
    void *data; // Contiguous storage buffer
};

// =====================
// Internal Helpers
// =====================

/*
 * Internal resize helper used to change the capacity
 * of the dynamic array. Supports both expansion and
 * shrinking operations while preserving all stored
 * elements.
 *
 * The requested capacity must be greater than zero
 * and large enough to hold the current logical size.
 *
 * If the requested capacity matches the current one,
 * the function performs no operation and returns success.
 */
static int atlas_array_void_resize(AtlasArrayVoid *arr, size_t new_capacity) {

    if (!arr || new_capacity == 0 || new_capacity < arr->size) {
        return ATLAS_ERROR;
    }

    if (new_capacity != arr->capacity) {
        void *temp = realloc(arr->data, arr->type_size * new_capacity);

        if (!temp) {
            return ATLAS_ERROR;
        }

        arr->data = temp;
        arr->capacity = new_capacity;
    }

    return ATLAS_SUCCESS;
}

/*
 * Internal helper that computes the address of the element
 * stored at the specified index within the contiguous
 * storage buffer.
 *
 * The returned pointer refers directly to the internal
 * storage and must not be freed or retained after any
 * operation that may reallocate the array.
 */
static void *atlas_array_void_get_element_ptr(const AtlasArrayVoid *arr, size_t index) {

    void *out = (char *)arr->data + (index * arr->type_size);

    return out;
}

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
        return ATLAS_ERROR;
    }

    AtlasArrayVoid *array = *ptr_atlas_array_void;

    free(array->data);
    free(array);

    *ptr_atlas_array_void = NULL;

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_void_push:
 * Validates the input pointers and appends a copy of the specified element
 * to the end of the array. Automatically expands the internal storage when
 * the current capacity is exhausted, then copies the element bytes into the
 * correct position and updates the array size.
 */
int atlas_array_void_push(AtlasArrayVoid *arr, const void *value) {

    if (!arr || !value) {
        return ATLAS_ERROR;
    }

    if (arr->size == arr->capacity) {
        size_t new_capacity = arr->capacity * 2;

        if (atlas_array_void_resize(arr, new_capacity) != ATLAS_SUCCESS) {
            return ATLAS_ERROR;
        }
    }

    void *element_ptr = atlas_array_void_get_element_ptr(arr, arr->size);
    memcpy(element_ptr, value, arr->type_size);
    arr->size++;

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_void_get:
 * Validates the input pointers and requested index, obtains
 * the address of the requested element within the internal
 * storage buffer, then copies its bytes into the user-provided
 * output buffer.
 */
int atlas_array_void_get(const AtlasArrayVoid *arr, size_t index, void *out_value) {

    if (!arr || !out_value || index >= arr->size) {
        return ATLAS_ERROR;
    }

    void *element_ptr = atlas_array_void_get_element_ptr(arr, index);
    memcpy(out_value, element_ptr, arr->type_size);

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_void_set:
 * Validates the input pointers and requested index, obtains
 * the address of the destination element within the internal
 * storage buffer, then overwrites the stored value by copying
 * the bytes from the user-provided element.
 */
int atlas_array_void_set(AtlasArrayVoid *arr, size_t index, const void *new_value) {

    if (!arr || !new_value || index >= arr->size) {
        return ATLAS_ERROR;
    }

    void *element_ptr = atlas_array_void_get_element_ptr(arr, index);
    memcpy(element_ptr, new_value, arr->type_size);

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_void_pop:
 * Removes the last element from the generic dynamic array and copies
 * its bytes into the provided output buffer.
 *
 * The internal storage capacity remains unchanged. After the element
 * has been copied to the output buffer, the logical size of the array
 * is decremented.
 */
int atlas_array_void_pop(AtlasArrayVoid *arr, void *out_value) {

    if (!arr || !out_value || arr->size == 0) {
        return ATLAS_ERROR;
    }

    void *element_ptr = atlas_array_void_get_element_ptr(arr, (arr->size - 1));
    memcpy(out_value, element_ptr, arr->type_size);
    arr->size--;

    return ATLAS_SUCCESS;
}