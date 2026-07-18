/*
 * AtlasDS
 * Dynamic Array Implementation
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#include "atlas/array.h"
#include "atlas/status.h"

#include <stdlib.h>

struct atlas_array {
    size_t size; // Current number of elements in the array
    size_t capacity; // Current capacity supported by the dynamic array
    int *data; // Contiguous storage buffer
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
 * Returns ATLAS_ERROR_NULL if the array is null, or
 * ATLAS_ERROR_INVALID_ARGUMENT if requested sizes violate boundaries.
 * Returns ATLAS_ERROR_MEMORY if realloc fails.
 */
static int atlas_array_resize(AtlasArray *arr, size_t new_capacity) {
    if (!arr) {
        return ATLAS_ERROR_NULL;
    }

    if (new_capacity == 0 || new_capacity < arr->size) {
        return ATLAS_ERROR_INVALID_ARGUMENT;
    }

    if (new_capacity != arr->capacity) {
        int *temp = realloc(arr->data, sizeof(int) * new_capacity);
        if (!temp) {
            return ATLAS_ERROR_MEMORY;
        }

        arr->data = temp;
        arr->capacity = new_capacity;
    }

    return ATLAS_SUCCESS;
}

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

    array->capacity = initial_capacity == 0 ? ATLAS_ARRAY_STANDARD_CAPACITY : initial_capacity;
    array->size = 0;

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
 * it uses the internal resize helper to double the capacity. Propagates
 * defensive status errors if the resize fails.
 */
int atlas_array_push(AtlasArray *arr, int value) {
    if (!arr) {
        return ATLAS_ERROR_NULL;
    }

    if (arr->size == arr->capacity) {
        size_t new_capacity = arr->capacity * 2;

        int status = atlas_array_resize(arr, new_capacity);
        if (status != ATLAS_SUCCESS) {
            return status;
        }
    }

    arr->data[arr->size] = value;
    arr->size++;

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_get:
 * Safe bounds-checked getter. Validates the instance, the output target,
 * and ensures the index is strictly less than the current size.
 */
int atlas_array_get(const AtlasArray *arr, size_t index, int *out_value) {
    if (!arr || !out_value) {
        return ATLAS_ERROR_NULL;
    }

    if (index >= arr->size) {
        return ATLAS_ERROR_BOUNDS;
    }

    *out_value = arr->data[index];

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_set:
 * Performs safe indexed overwrite with bounds validation.
 * Replaces the value stored at the specified position without
 * modifying the logical size or the allocated capacity.
 */
int atlas_array_set(AtlasArray *arr, size_t index, int new_value) {
    if (!arr) {
        return ATLAS_ERROR_NULL;
    }

    if (index >= arr->size) {
        return ATLAS_ERROR_BOUNDS;
    }

    arr->data[index] = new_value;

    return ATLAS_SUCCESS;
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
 * its value through the output parameter. Returns ATLAS_ERROR_EMPTY 
 * if no elements are available.
 */
int atlas_array_pop(AtlasArray *arr, int *out_value) {
    if (!arr || !out_value) {
        return ATLAS_ERROR_NULL;
    }

    if (arr->size == 0) {
        return ATLAS_ERROR_EMPTY;
    }

    size_t last_index = arr->size - 1;
    *out_value = arr->data[last_index];
    
    arr->size--;

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_reserve:
 * Ensures that the dynamic array has at least the requested capacity.
 * If new_capacity is less than or equal to the current capacity, the
 * function performs no operation and returns success (idempotent behavior).
 */
int atlas_array_reserve(AtlasArray *arr, size_t new_capacity) {
    if (!arr) {
        return ATLAS_ERROR_NULL;
    }

    if (new_capacity > arr->capacity) {
        int status = atlas_array_resize(arr, new_capacity);
        if (status != ATLAS_SUCCESS) {
            return status;
        }
    }

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_clear:
 * Removes all logical elements from the array by resetting
 * its size to zero. The allocated memory buffer remains intact.
 */
int atlas_array_clear(AtlasArray *arr) {
    if (!arr) {
        return ATLAS_ERROR_NULL;
    }

    arr->size = 0;

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_shrink_to_fit:
 * Reduces the internal buffer to the smallest capacity capable
 * of storing the current elements. Empty arrays retain a minimum capacity.
 */
int atlas_array_shrink_to_fit(AtlasArray *arr) {
    if (!arr) {
        return ATLAS_ERROR_NULL;
    }

    size_t new_capacity = arr->size == 0 ? ATLAS_ARRAY_STANDARD_CAPACITY : arr->size;

    int status = atlas_array_resize(arr, new_capacity);
    if (status != ATLAS_SUCCESS) {
        return status;
    }

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_empty:
 * Checks whether the array contains any logical elements.
 * The result is written to the provided output parameter.
 */
int atlas_array_empty(const AtlasArray *arr, bool *empty) {
    if (!arr || !empty) {
        return ATLAS_ERROR_NULL;
    }

    *empty = (arr->size == 0);

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_front:
 * Retrieves the first element stored in the array and writes it 
 * to the output parameter. Returns ATLAS_ERROR_EMPTY if the array is empty.
 */
int atlas_array_front(const AtlasArray *arr, int *out_value) {
    if (!arr || !out_value) {
        return ATLAS_ERROR_NULL;
    }

    if (arr->size == 0) {
        return ATLAS_ERROR_EMPTY;
    }

    *out_value = arr->data[0];

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_back:
 * Retrieves the last element stored in the array and writes it 
 * to the output parameter. Returns ATLAS_ERROR_EMPTY if the array is empty.
 */
int atlas_array_back(const AtlasArray *arr, int *out_value) {
    if (!arr || !out_value) {
        return ATLAS_ERROR_NULL;
    }

    if (arr->size == 0) {
        return ATLAS_ERROR_EMPTY;
    }

    *out_value = arr->data[arr->size - 1];

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_insert:
 * Inserts a new element at the specified index while preserving
 * the order of existing elements. Returns ATLAS_ERROR_BOUNDS if the
 * index is outside the allowed size range.
 */
int atlas_array_insert(AtlasArray *arr, size_t index, int value) {
    if (!arr) {
        return ATLAS_ERROR_NULL;
    }

    if (index > arr->size) {
        return ATLAS_ERROR_BOUNDS;
    }

    if (arr->capacity == arr->size) {
        size_t new_capacity = arr->capacity * 2;

        int status = atlas_array_resize(arr, new_capacity);
        if (status != ATLAS_SUCCESS) {
            return status;
        }
    }

    if (index < arr->size) {
        for (size_t i = arr->size; i > index; i--) {
            arr->data[i] = arr->data[i - 1];
        }
    }

    arr->data[index] = value;
    arr->size++;

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_erase:
 * Removes the element at the specified index while preserving
 * the order of the remaining elements. Returns ATLAS_ERROR_BOUNDS 
 * if the index is invalid.
 */
int atlas_array_erase(AtlasArray *arr, size_t index) {
    if (!arr) {
        return ATLAS_ERROR_NULL;
    }

    if (index >= arr->size) {
        return ATLAS_ERROR_BOUNDS;
    }

    if (index <= arr->size - 2) {
        for (size_t i = index; i < arr->size - 1; i++) {
            arr->data[i] = arr->data[i + 1];
        }
    }

    arr->size--;

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_find:
 * Performs a linear search through the array looking for the
 * first occurrence of the requested value.
 *
 * If no matching value exists, the function returns 
 * ATLAS_ERROR_NOT_FOUND and leaves the output parameter unchanged.
 */
int atlas_array_find(const AtlasArray *arr, size_t *index_out, int value) {
    if (!arr || !index_out) {
        return ATLAS_ERROR_NULL;
    }

    for (size_t i = 0; i < arr->size; i++) {
        if (arr->data[i] == value) {
            *index_out = i;
            return ATLAS_SUCCESS;
        }
    }

    return ATLAS_ERROR_NOT_FOUND;
}

/*
 * Implementation of atlas_array_contains:
 * Checks whether a given value exists in the array by internally
 * reusing atlas_array_find(). A non-existent value is a valid result
 * and returns ATLAS_SUCCESS with `contains` set to false.
 */
int atlas_array_contains(const AtlasArray *arr, bool *contains, int value) {
    if (!arr || !contains) {
        return ATLAS_ERROR_NULL;
    }

    size_t unused_index = 0;
    *contains = atlas_array_find(arr, &unused_index, value) == ATLAS_SUCCESS;

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_swap:
 * Exchanges the values stored at two valid positions within the array.
 * Returns ATLAS_ERROR_BOUNDS if any index is invalid.
 */
int atlas_array_swap(AtlasArray *arr, size_t index_a, size_t index_b) {
    if (!arr) {
        return ATLAS_ERROR_NULL;
    }

    if (index_a >= arr->size || index_b >= arr->size) {
        return ATLAS_ERROR_BOUNDS;
    }

    if (index_a != index_b) {
        int value_temp = arr->data[index_a];
        arr->data[index_a] = arr->data[index_b];
        arr->data[index_b] = value_temp;
    }

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_copy:
 * Copies all elements from the source array into the destination array.
 * Rejects self-copy operations by returning ATLAS_ERROR_INVALID_ARGUMENT.
 */
int atlas_array_copy(const AtlasArray *src, AtlasArray *dest) {
    if (!src || !dest) {
        return ATLAS_ERROR_NULL;
    }

    if (src == dest) {
        return ATLAS_ERROR_INVALID_ARGUMENT;
    }

    if (dest->capacity < src->size) {
        int status = atlas_array_resize(dest, src->size);
        if (status != ATLAS_SUCCESS) {
            return status;
        }
    }

    size_t i;
    for (i = 0; i < src->size; i++) {
        dest->data[i] = src->data[i];

    }

    dest->size = src->size;

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_array_clone:
 * Creates a new array that is an independent duplicate of the source array.
 * If any internal allocation or copy step fails, releases temporary resources.
 */
AtlasArray *atlas_array_clone(const AtlasArray *src) {
    if (!src) {
        return NULL;
    }

    AtlasArray *clone = atlas_array_create(src->capacity);

    if (!clone) {
        return NULL;
    }

    if (atlas_array_copy(src, clone) != ATLAS_SUCCESS) {
        atlas_array_destroy(&clone);
        return NULL;
    }

    return clone;
}