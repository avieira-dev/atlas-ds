/*
 * AtlasDS
 * Queue Public API
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#include <stddef.h>

/**
 * @brief Opaque structure representing a generic queue.
 *
 * This structure is intentionally opaque to enforce encapsulation.
 * Internally, it stores elements as dynamically allocated nodes
 * using a byte-based storage mechanism.
 *
 * Users should only interact with the queue through the public API.
 */
typedef struct atlas_queue AtlasQueue;

/**
 * @brief Creates and initializes a generic queue.
 *
 * Allocates memory for the queue structure and initializes an empty
 * queue ready to store elements of the specified type.
 *
 * The queue does not allocate elements until values are inserted.
 *
 * @param type_size Size in bytes of each element to be stored.
 * Must be greater than 0.
 *
 * @return Pointer to the newly created AtlasQueue on success,
 * or NULL if type_size is 0 or memory allocation fails.
 */
AtlasQueue *atlas_queue_create(size_t type_size);

/**
 * @brief Destroys a generic queue and releases its memory.
 *
 * Frees all elements contained in the queue, then releases the
 * queue structure itself. After destruction, the pointer is set
 * to NULL to prevent dangling pointer usage.
 *
 * @param ptr_atlas_queue Pointer to the AtlasQueue pointer.
 * If the pointer or the referenced queue is NULL, the function
 * returns an error code.
 *
 * @return ATLAS_SUCCESS on success, or ATLAS_ERROR_NULL if the
 * input pointer or referenced queue is NULL.
 */
int atlas_queue_destroy(AtlasQueue **ptr_atlas_queue);