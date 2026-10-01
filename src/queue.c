/*
 * AtlasDS
 * Queue Implementation
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#include <stdlib.h>
#include <string.h>

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

/**
 * Creates a new queue element containing a copy of the provided value.
 *
 * Allocates memory for the element and its data storage based on the
 * queue's configured element size.
 *
 * Initializes the new element's next pointer to NULL, since the element
 * is not linked to another element when it is created.
 *
 * Returns NULL if memory allocation fails.
 */
static AtlasQueueElement *atlas_queue_create_element(const AtlasQueue *queue, const void *value) {
    AtlasQueueElement *element = malloc(sizeof(*element) + queue->type_size);
    if (!element) {
        return NULL;
    }

    memcpy(element->data, value, queue->type_size);
    element->next_element = NULL;

    return element;
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

/**
 * Implementation of atlas_queue_enqueue:
 * Creates a new queue element containing a copy of the provided value
 * and inserts it at the back of the queue.
 *
 * If the queue is empty, the new element becomes both the front and
 * back element. Otherwise, the current back element is linked to the
 * new element before updating the queue's back element pointer.
 *
 * Increments the queue size after successfully inserting the new element.
 *
 * Returns ATLAS_ERROR_NULL if the queue or value is NULL, or
 * ATLAS_ERROR_MEMORY if memory allocation fails.
 */
int atlas_queue_enqueue(AtlasQueue *queue, const void *value) {
    if (!queue || !value) {
        return ATLAS_ERROR_NULL;
    }

    AtlasQueueElement *element = atlas_queue_create_element(queue, value);
    if (!element) {
        return ATLAS_ERROR_MEMORY;
    }

    if (queue->queue_size == 0) {
        queue->back_element = queue->front_element = element;
        queue->queue_size++;
        return ATLAS_SUCCESS;
    }

    queue->back_element->next_element = element;
    queue->back_element = element;
    queue->queue_size++;

    return ATLAS_SUCCESS;
}

/**
 * Implementation of atlas_queue_dequeue:
 * Removes the front element from the queue and copies its value
 * into the provided output buffer.
 *
 * Updates the front element pointer to the next element, releases
 * the removed element's memory, and decrements the queue size.
 *
 * If the removed element was the last element in the queue, the
 * back element pointer is also reset to NULL.
 *
 * Returns ATLAS_ERROR_NULL if the queue or output value is NULL,
 * or ATLAS_ERROR_EMPTY if the queue contains no elements.
 */
int atlas_queue_dequeue(AtlasQueue *queue, void *out_value) {
    if (!queue || !out_value) {
        return ATLAS_ERROR_NULL;
    }

    if (queue->queue_size == 0) {
        return ATLAS_ERROR_EMPTY;
    }

    AtlasQueueElement *current_element = queue->front_element;
    queue->front_element = current_element->next_element;
    memcpy(out_value, current_element->data, queue->type_size);
    free(current_element);
    queue->queue_size--;

    if (queue->queue_size == 0) {
        queue->back_element = NULL;
    }

    return ATLAS_SUCCESS;
}

/**
 * Implementation of atlas_queue_front:
 * Copies the value stored in the front element into the provided output buffer.
 *
 * The element remains in the queue after the operation.
 *
 * Returns ATLAS_ERROR_NULL if the queue or output value is NULL, or
 * ATLAS_ERROR_EMPTY if the queue contains no elements.
 */
int atlas_queue_front(const AtlasQueue *queue, void *out_value) {
    if (!queue || !out_value) {
        return ATLAS_ERROR_NULL;
    }

    if (queue->queue_size == 0) {
        return ATLAS_ERROR_EMPTY;
    }

    memcpy(out_value, queue->front_element->data, queue->type_size);

    return ATLAS_SUCCESS;
}

/**
 * Implementation of atlas_queue_back:
 * Copies the value stored in the back element into the provided output buffer.
 *
 * The element remains in the queue after the operation.
 *
 * Returns ATLAS_ERROR_NULL if the queue or output value is NULL, or
 * ATLAS_ERROR_EMPTY if the queue contains no elements.
 */
int atlas_queue_back(const AtlasQueue *queue, void *out_value) {
    if (!queue || !out_value) {
        return ATLAS_ERROR_NULL;
    }

    if (queue->queue_size == 0) {
        return ATLAS_ERROR_EMPTY;
    }

    memcpy(out_value, queue->back_element->data, queue->type_size);

    return ATLAS_SUCCESS;
}

/**
 * Implementation of atlas_queue_size:
 * Retrieves the current number of elements stored in the queue.
 *
 * The queue remains unchanged after the operation.
 *
 * Returns ATLAS_ERROR_NULL if the queue or output value is NULL.
 */
int atlas_queue_size(const AtlasQueue *queue, size_t *out_value) {
    if (!queue || !out_value) {
        return ATLAS_ERROR_NULL;
    }

    *out_value = queue->queue_size;

    return ATLAS_SUCCESS;
}

/**
 * Implementation of atlas_queue_empty:
 * Checks whether the queue currently contains no elements.
 *
 * Stores true in the output value when the queue is empty and false
 * otherwise. The queue remains unchanged after the operation.
 *
 * Returns ATLAS_ERROR_NULL if the queue or output value is NULL.
 */
int atlas_queue_empty(const AtlasQueue *queue, bool *out_value) {
    if (!queue || !out_value) {
        return ATLAS_ERROR_NULL;
    }

    *out_value = queue->queue_size == 0;

    return ATLAS_SUCCESS;
}

/**
 * Implementation of atlas_queue_clear:
 * Removes all elements currently stored in the queue.
 *
 * Releases every allocated element, resets the front and back element
 * pointers, and sets the queue size to zero.
 *
 * The queue structure itself is preserved and remains available for
 * future insertions.
 *
 * Returns ATLAS_ERROR_NULL if the queue is NULL.
 */
int atlas_queue_clear(AtlasQueue *queue) {
    if (!queue) {
        return ATLAS_ERROR_NULL;
    }

    atlas_queue_free_elements(queue);
    queue->front_element = queue->back_element = NULL;
    queue->queue_size = 0;

    return ATLAS_SUCCESS;
}