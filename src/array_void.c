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
    size_t capacity; // Number of elements that can be stored without reallocation
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

static bool atlas_array_void_full(const AtlasArrayVoid *arr) {

    return arr->size == arr->capacity;
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

// =====================
// Operations
// =====================

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

    if (atlas_array_void_full(arr)) {
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

/*
 * Implementation of atlas_array_void_size:
 * Validates the input pointers and stores the current
 * logical size of the array in the user-provided output
 * variable.
 */
int atlas_array_void_size(const AtlasArrayVoid *arr, size_t *out_value) {

    if (!arr || !out_value) {
        return ATLAS_ERROR;
    }

    *out_value = arr->size;

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_void_capacity:
 * Validates the input pointers and stores the current
 * storage capacity of the array in the user-provided
 * output variable.
 */
int atlas_array_void_capacity(const AtlasArrayVoid *arr, size_t *out_value) {

    if (!arr || !out_value) {
        return ATLAS_ERROR;
    }

    *out_value = arr->capacity;

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_void_empty:
 * Validates the input pointers and stores whether the
 * array currently contains no elements in the
 * user-provided output variable.
 */
int atlas_array_void_empty(const AtlasArrayVoid *arr, bool *out_value) {

    if (!arr || !out_value) {
        return ATLAS_ERROR;
    }

    *out_value = (arr->size == 0);

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_void_front:
 * Validates the input pointers and copies the first
 * stored element into the user-provided output buffer.
 */
int atlas_array_void_front(const AtlasArrayVoid *arr, void *out_value) {

    if (!arr || !out_value || arr->size == 0) {
        return ATLAS_ERROR;
    }

    void *element_ptr = atlas_array_void_get_element_ptr(arr, 0);
    memcpy(out_value, element_ptr, arr->type_size);

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_void_back:
 * Validates the input pointers and copies the last
 * stored element into the user-provided output buffer.
 */
int atlas_array_void_back(const AtlasArrayVoid *arr, void *out_value) {

    if (!arr || !out_value || arr->size == 0) {
        return ATLAS_ERROR;
    }

    void *element_ptr = atlas_array_void_get_element_ptr(arr, (arr->size - 1));
    memcpy(out_value, element_ptr, arr->type_size);

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_void_reserve:
 * Ensures that the array has at least the requested
 * storage capacity. If the requested capacity is
 * less than or equal to the current capacity,
 * no reallocation is performed.
 */
int atlas_array_void_reserve(AtlasArrayVoid *arr, size_t new_capacity) {

    if (!arr) {
        return ATLAS_ERROR;
    }

    if (new_capacity > arr->capacity) {
        if (atlas_array_void_resize(arr, new_capacity) != ATLAS_SUCCESS) {
            return ATLAS_ERROR;
        }
    }

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_void_clear:
 * Removes all elements from the array by resetting
 * its logical size. The allocated storage buffer
 * remains unchanged and can be reused by subsequent
 * insertions.
 */
int atlas_array_void_clear(AtlasArrayVoid *arr) {

    if (!arr) {
        return ATLAS_ERROR;
    }

    arr->size = 0;

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_void_shrink_to_fit:
 * Reduces the storage capacity to match the current
 * logical size. When the array is empty, the capacity
 * is reduced to the minimum supported capacity.
 */
int atlas_array_void_shrink_to_fit(AtlasArrayVoid *arr) {

    if (!arr) {
        return ATLAS_ERROR;
    }

    size_t new_capacity = arr->size == 0 ? ATLAS_ARRAY_VOID_STANDARD_CAPACITY : arr->size;

    if (atlas_array_void_resize(arr, new_capacity) != ATLAS_SUCCESS) {
        return ATLAS_ERROR;
    }

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_void_insert:
 * Validates the input pointers and insertion index.
 * Expands the internal storage if necessary, shifts
 * the existing elements one position to the right
 * when inserting before the end of the array, copies
 * the new element into the requested position, and
 * updates the logical size.
 */
int atlas_array_void_insert(AtlasArrayVoid *arr, size_t index, const void *value) {

    if (!arr || !value || index > arr->size) {
        return ATLAS_ERROR;
    }

    if (atlas_array_void_full(arr)) {
        size_t new_capacity = arr->capacity * 2;

        if (atlas_array_void_resize(arr, new_capacity) != ATLAS_SUCCESS) {
            return ATLAS_ERROR;
        }
    }

    if (index < arr->size) {
        for (size_t i = arr->size; i > index; i--) {
            void *destination_ptr = atlas_array_void_get_element_ptr(arr, i);
            void *origin_ptr = atlas_array_void_get_element_ptr(arr, i - 1);
            memcpy(destination_ptr, origin_ptr, arr->type_size);
        }
    }

    void *element_ptr = atlas_array_void_get_element_ptr(arr, index);
    memcpy(element_ptr, value, arr->type_size);

    arr->size++;

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_void_erase:
 * Validates the input pointer and removal index,
 * shifts all elements after the removed position
 * one slot to the left to preserve contiguous
 * storage, then decrements the logical size.
 * The allocated storage capacity remains unchanged.
 */
int atlas_array_void_erase(AtlasArrayVoid *arr, size_t index) {

    if (!arr || index >= arr->size) {
        return ATLAS_ERROR;
    }

    if (index <= arr->size - 2) {
        for (size_t i = index; i < arr->size - 1; i++) {
            void *destination_ptr = atlas_array_void_get_element_ptr(arr, i);
            void *origin_ptr = atlas_array_void_get_element_ptr(arr, i + 1);
            memcpy(destination_ptr, origin_ptr, arr->type_size);
        }
    }

    arr->size--;

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_void_swap:
 * Validates the input pointer and both indices.
 * If the indices are different, exchanges the
 * contents of the two elements using a temporary
 * buffer allocated with the element size. The
 * logical size and storage capacity remain
 * unchanged.
 */
int atlas_array_void_swap(AtlasArrayVoid *arr, size_t index_a, size_t index_b) {

    if (!arr || index_a >= arr->size || index_b >= arr->size) {
        return ATLAS_ERROR;
    }

    if (index_a == index_b) {
        return ATLAS_SUCCESS;
    }

    void *element_a_ptr = atlas_array_void_get_element_ptr(arr, index_a);
    void *element_b_ptr = atlas_array_void_get_element_ptr(arr, index_b);
    void *temp = malloc(arr->type_size);

    if (!temp) {
        return ATLAS_ERROR;
    }
    
    memcpy(temp, element_a_ptr, arr->type_size);
    memcpy(element_a_ptr, element_b_ptr, arr->type_size);
    memcpy(element_b_ptr, temp, arr->type_size);

    free(temp);

    return ATLAS_SUCCESS;
}