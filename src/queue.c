/*
 * AtlasDS
 * Queue Implementation
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#include <stdlib.h>

#include "atlas/queue.h"
#include "atlas/status.h"

typedef struct atlas_queue_element AtlasQueueElement;

struct atlas_queue {
    size_t type_size; // Size in bytes of each stored element
    size_t queue_size; // Current number of elements in the queue
    AtlasQueueElement *front_element; // Pointer to the front element
    AtlasQueueElement *back_element; // Pointer to the back element
};

struct atlas_queue_element {
    AtlasQueueElement *next_element; // Pointer to the next element in the queue
    unsigned char data[]; // Flexible array member storing the element bytes
};

// =====================
// Internal Helpers
// =====================

/**
 * @brief Internal helper that releases every element currently stored
 * in the queue.
 *
 * Traverses the queue from the front element toward the back
 * by following each element's next pointer before releasing
 * its allocated memory.
 *
 * The queue structure itself is not modified or released.
 * Callers are responsible for updating the queue metadata after
 * this function returns.
 */
static void atlas_queue_free_elements(const AtlasQueue *queue) {
    AtlasQueueElement *current_element = queue->front_element;
    while (current_element) {
        AtlasQueueElement *next_element = current_element->next_element;
        free(current_element);
        current_element = next_element;
    }
}

// =====================
// Lifecycle
// =====================

/**
 * @brief Implementation of atlas_queue_create.
 *
 * Allocates memory for the queue structure and initializes an
 * empty queue with no elements.
 *
 * Stores the size of each element and initializes the front
 * and back element pointers to NULL.
 *
 * Returns NULL if type_size is 0 or if the memory allocation
 * for the queue structure fails.
 */
AtlasQueue *atlas_queue_create(size_t type_size) {
    if (type_size == 0) {
        return NULL;
    }

    AtlasQueue *queue = malloc(sizeof(*queue));

    if (!queue) {
        return NULL;
    }

    queue->type_size = type_size;
    queue->queue_size = 0;
    queue->front_element = NULL;
    queue->back_element = NULL;

    return queue;
}

/**
 * @brief Implementation of atlas_queue_destroy.
 *
 * Safely releases all elements contained in the queue before
 * freeing the main queue structure.
 *
 * Traverses the queue by following each element's next pointer,
 * preserving the next element address before releasing the
 * current element to avoid losing access to the remaining elements.
 *
 * Takes a double pointer to reset the user's pointer to NULL,
 * preventing accidental dangling pointer access.
 *
 * Returns ATLAS_ERROR_NULL if the double pointer or referenced
 * queue is NULL.
 */
int atlas_queue_destroy(AtlasQueue **ptr_atlas_queue) {
    if (!ptr_atlas_queue || !*ptr_atlas_queue) {
        return ATLAS_ERROR_NULL;
    }

    AtlasQueue *ptr_queue = *ptr_atlas_queue;
    atlas_queue_free_elements(ptr_queue);
    free(ptr_queue);
    *ptr_atlas_queue = NULL;

    return ATLAS_SUCCESS;
}