#include "atlas/array.h"

#include <stdlib.h>

struct atlas_array {

    int *data; // Contiguous storage buffer
    size_t size; // Current number of elements in the array
    size_t capacity; // Current capacity supported by the dynamic array
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
static int atlas_array_resize(AtlasArray *arr, size_t new_capacity) {

    if (!arr || new_capacity == 0 || new_capacity < arr->size) {
        return -1;
    }

    if (new_capacity != arr->capacity) {
        int *temp = realloc(arr->data, sizeof(int) * new_capacity);

        if (!temp) {
            return -1;
        }

        arr->data = temp;
        arr->capacity = new_capacity;
    }

    return 0;
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

        if (atlas_array_resize(arr, new_capacity) != 0) {
            return -1;
        }
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
 * Implementation of atlas_array_set:
 * Performs safe indexed overwrite with bounds validation.
 * Replaces the value stored at the specified position without
 * modifying the logical size or the allocated capacity.
 */
int atlas_array_set(AtlasArray *arr, size_t index, int new_value) {

    if (!arr || index >= arr->size) {
        return -1;
    }

    arr->data[index] = new_value;

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
int atlas_array_reserve(AtlasArray *arr, size_t new_capacity) {

    if (!arr) {
        return -1;
    }

    if (new_capacity > arr->capacity) {
        if (atlas_array_resize(arr, new_capacity) != 0) {
            return -1;
        }
    }

    return 0;
}

/*
 * Implementation of atlas_array_shrink_to_fit:
 * Reduces the internal buffer to the smallest capacity capable
 * of storing the current elements.
 *
 * When the array contains elements, the capacity becomes equal
 * to the current size. Empty arrays retain a minimum capacity
 * of one element.
 *
 * If the current capacity already matches the target capacity,
 * the operation completes without performing a reallocation.
 */
int atlas_array_shrink_to_fit(AtlasArray *arr) {

    if (!arr) {
        return -1;
    }

    size_t new_capacity = arr->size == 0 ? 1 : arr->size;

    if (atlas_array_resize(arr, new_capacity) != 0) {
        return -1;
    }

    return 0;
}

/*
 * Implementation of atlas_array_clear:
 * Removes all logical elements from the array by resetting
 * its size to zero. The allocated memory buffer remains intact,
 * allowing future insertions without additional reallocations.
 */
int atlas_array_clear(AtlasArray *arr) {

    if (!arr) {
        return -1;
    }

    arr->size = 0;

    return 0;
}

/*
 * Implementation of atlas_array_empty:
 * Checks whether the array contains any logical elements.
 * The result is written to the provided output parameter
 * without modifying the array state.
 */
int atlas_array_empty(const AtlasArray *arr, bool *empty) {

    if (!arr || !empty) {
        return -1;
    }

    *empty = (arr->size == 0);

    return 0;
}

/*
 * Implementation of atlas_array_front:
 * Retrieves the first element stored in the array and
 * writes it to the provided output parameter without
 * modifying the array state.
 */
int atlas_array_front(const AtlasArray *arr, int *out_value) {

    if (!arr || !out_value) {
        return -1;
    }

    if (arr->size > 0) {
        *out_value = arr->data[0];
    }

    return 0;
}

/*
 * Implementation of atlas_array_back:
 * Retrieves the last element stored in the array and
 * writes it to the provided output parameter without
 * modifying the array state.
 */
int atlas_array_back(const AtlasArray *arr, int *out_value) {

    if (!arr || !out_value) {
        return -1;
    }

    if (arr->size > 0) {
        *out_value = arr->data[arr->size - 1];
    }

    return 0;
}

/*
 * Implementation of atlas_array_insert:
 * Inserts a new element at the specified index while preserving
 * the order of existing elements. Elements at and after the
 * insertion point are shifted one position to the right.
 *
 * If the current capacity is insufficient, the internal buffer
 * is expanded before the insertion takes place.
 */
int atlas_array_insert(AtlasArray *arr, size_t index, int value) {

    if (!arr || index > arr->size) {
        return -1;
    }

    if (arr->capacity == arr->size) {
        size_t new_capacity = arr->capacity * 2;

        if (atlas_array_resize(arr, new_capacity) != 0) {
            return -1;
        }
    }

    if (index < arr->size) {
        for (size_t i = arr->size; i > index; i--) {
            arr->data[i] = arr->data[i - 1];
        }
    }

    arr->data[index] = value;
    arr->size++;

    return 0;
}

/*
 * Implementation of atlas_array_erase:
 * Removes the element at the specified index while preserving
 * the order of the remaining elements. Elements located after
 * the removal point are shifted one position to the left.
 *
 * The internal buffer is not resized and the allocated
 * capacity remains unchanged after the operation.
 */
int atlas_array_erase(AtlasArray *arr, size_t index) {

    if (!arr || index >= arr->size) {
        return -1;
    }

    if (index <= arr->size - 2) {
        for (size_t i = index; i < arr->size - 1; i++) {
            arr->data[i] = arr->data[i + 1];
        }
    }

    arr->size--;

    return 0;
}

/*
 * Implementation of atlas_array_find:
 * Performs a linear search through the array looking for the
 * first occurrence of the requested value.
 *
 * Elements are inspected from the beginning of the array
 * towards the end. When a match is found, its index is stored
 * in the output parameter and the search terminates
 * immediately.
 *
 * If no matching value exists, the function returns an error
 * and leaves the output parameter unchanged.
 */
int atlas_array_find(const AtlasArray *arr, size_t *index_out, int value) {

    if (!arr || !index_out) {
        return -1;
    }

    for (size_t i = 0; i < arr->size; i++) {
        if (arr->data[i] == value) {
            *index_out = i;
            return 0;
        }
    }

    return -1;
}

/*
 * Implementation of atlas_array_contains:
 * Checks whether a given value exists in the array.
 *
 * Internally, the operation reuses atlas_array_find() to perform
 * the search. The result is reported through the output parameter
 * as a boolean value rather than returning the position of the
 * matching element.
 *
 * A value that is not present in the array is considered a valid
 * outcome and does not constitute an error.
 */
int atlas_array_contains(const AtlasArray *arr, bool *contains, int value) {

    if (!arr || !contains) {
        return -1;
    }

    size_t unused_index = 0;

    *contains = atlas_array_find(arr, &unused_index, value) == 0;

    return 0;
}

/*
 * Implementation of atlas_array_swap:
 * Exchanges the values stored at two valid positions within
 * the array.
 *
 * The operation preserves the current size and capacity and
 * requires only a temporary variable to perform the exchange.
 *
 * If both indices refer to the same element, the function
 * completes successfully without modifying the array.
 *
 * Invalid indices or a NULL array pointer result in an error
 * and leave the array unchanged.
 */
int atlas_array_swap(AtlasArray *arr, size_t index_a, size_t index_b) {

    if (!arr || index_a >= arr->size || index_b >= arr->size) {
        return -1;
    }

    if (index_a != index_b) {
        int value_temp = arr->data[index_a];

        arr->data[index_a] = arr->data[index_b];
        arr->data[index_b] = value_temp;
    }

    return 0;
}

/*
 * Implementation of atlas_array_copy:
 * Copies all elements from the source array into the
 * destination array.
 *
 * The operation preserves the source array and replaces
 * the contents of the destination array.
 *
 * If the destination capacity is insufficient, the internal
 * buffer is expanded before copying begins.
 *
 * Empty source arrays are treated as valid input and cause
 * the destination array to become empty while preserving its
 * current capacity.
 *
 * Self-copy operations are rejected to prevent ambiguous
 * behavior.
 *
 * In the event of a memory allocation failure, the destination
 * array remains unchanged.
 */
int atlas_array_copy(const AtlasArray *src, AtlasArray *dest) {

    if (!src || !dest || src == dest) {
        return -1;
    }

    if (dest->capacity < src->size) {
        if (atlas_array_resize(dest, src->size) != 0) {
            return -1;
        }
    }

    size_t i;

    for (i = 0; i < src->size; i++) {
        dest->data[i] = src->data[i];

    }

    dest->size = src->size;

    return 0;
}