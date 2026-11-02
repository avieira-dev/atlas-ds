/*
 * AtlasDS
 * Deque Implementation
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#include <stdlib.h>

#include "atlas/deque.h"
#include "atlas/status.h"

typedef struct atlas_deque_element AtlasDequeElement;

struct atlas_deque {
    size_t type_size; // Size in bytes of each stored element
    size_t deque_size; // Current number of elements in the deque
    AtlasDequeElement *front_element; // Pointer to the front element
    AtlasDequeElement *back_element; // Pointer to the back element
};

struct atlas_deque_element {
    AtlasDequeElement *previous_element; // Pointer to the previous element in the deque
    AtlasDequeElement *next_element; // Pointer to the next element in the deque
    unsigned char data[]; // Flexible array member storing the element bytes
};

// =====================
// Internal Helpers
// =====================

/*
 * Internal helper that releases every element currently stored
 * in the deque.
 *
 * Traverses the deque from the front element toward the back
 * by following each element's next pointer before releasing
 * its allocated memory.
 *
 * The deque structure itself is not modified or released.
 * Callers are responsible for updating the deque metadata after
 * this function returns.
 */
static void atlas_deque_free_elements(const AtlasDeque *deque) {
    AtlasDequeElement *current_element = deque->front_element;
    while(current_element) {
        AtlasDequeElement *next_element = current_element->next_element;
        free(current_element);
        current_element = next_element;
    }
}

// =====================
// Lifecycle
// =====================

/*
 * Implementation of atlas_deque_create:
 * Allocates memory for the deque structure and initializes an
 * empty deque with no elements.
 *
 * Stores the size of each element and initializes the front
 * and back element pointers to NULL.
 *
 * Returns NULL if type_size is 0 or if the memory allocation
 * for the deque structure fails.
 */
AtlasDeque *atlas_deque_create(size_t type_size) {
    if (type_size == 0) {
        return NULL;
    }

    AtlasDeque *deque = malloc(sizeof(*deque));
    if (!deque) {
        return NULL;
    }

    deque->type_size = type_size;
    deque->deque_size = 0;
    deque->front_element = deque->back_element = NULL;

    return deque;
}

/*
 * Implementation of atlas_deque_destroy:
 * Safely releases all elements contained in the deque before
 * freeing the main deque structure.
 *
 * Traverses the deque by following each element's next
 * pointer, preserving the next element address before
 * releasing the current element to avoid losing access to the
 * remaining elements.
 *
 * Takes a double pointer to reset the user's pointer to NULL,
 * preventing accidental dangling pointer access.
 *
 * Returns ATLAS_ERROR_NULL if the double pointer or referenced
 * deque is NULL.
 */
int atlas_deque_destroy(AtlasDeque **ptr_atlas_deque) {
    if (!ptr_atlas_deque || !*ptr_atlas_deque) {
        return ATLAS_ERROR_NULL;
    }

    AtlasDeque *ptr_deque = *ptr_atlas_deque;

    atlas_deque_free_elements(ptr_deque);

    free(ptr_deque);

    *ptr_atlas_deque = NULL;

    return ATLAS_SUCCESS;
}