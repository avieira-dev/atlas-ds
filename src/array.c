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