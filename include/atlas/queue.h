/*
 * AtlasDS
 * Queue Public API
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#include <stdbool.h>
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

/**
 * @brief Inserts an element at the back of the queue.
 *
 * Allocates a new element, copies the provided value into the
 * element's internal storage, and inserts it at the back of
 * the queue.
 *
 * If the queue is empty, the new element becomes both the front
 * and back element. Otherwise, the current back element is linked
 * to the new element and the back pointer is updated.
 *
 * @param queue Pointer to the queue.
 * @param value Pointer to the value to be inserted.
 *
 * @return ATLAS_SUCCESS on success, ATLAS_ERROR_NULL if the queue
 * or value pointer is NULL, or ATLAS_ERROR_MEMORY if element
 * allocation fails.
 */
int atlas_queue_enqueue(AtlasQueue *queue, const void *value);

/**
 * @brief Removes the front element from the queue.
 *
 * Copies the value stored in the front element into the provided
 * output buffer, removes the element from the queue, and releases
 * its allocated memory.
 *
 * If the removed element is the last element in the queue, both
 * the front and back element pointers are reset to NULL.
 *
 * @param queue Pointer to the queue.
 * @param out_value Pointer to the buffer where the removed value
 * will be copied.
 *
 * @return ATLAS_SUCCESS on success, ATLAS_ERROR_NULL if the queue
 * or output value pointer is NULL, or ATLAS_ERROR_EMPTY if the queue
 * contains no elements.
 */
int atlas_queue_dequeue(AtlasQueue *queue, void *out_value);

/**
 * @brief Copies the first element of the queue into the output buffer.
 *
 * The element remains in the queue after the operation.
 *
 * @param queue Queue to access.
 * @param out_value Buffer that receives the element value.
 * @return ATLAS_SUCCESS on success, ATLAS_ERROR_NULL if queue or out_value
 * is NULL, or ATLAS_ERROR_EMPTY if the queue is empty.
 */
int atlas_queue_front(const AtlasQueue *queue, void *out_value);

/**
 * @brief Copies the last element of the queue into the output buffer.
 *
 * The element remains in the queue after the operation.
 *
 * @param queue Queue to access.
 * @param out_value Buffer that receives the element value.
 * @return ATLAS_SUCCESS on success, ATLAS_ERROR_NULL if queue or out_value
*  is NULL, or ATLAS_ERROR_EMPTY if the queue is empty.
 */
int atlas_queue_back(const AtlasQueue *queue, void *out_value);

/**
 * @brief Retrieves the current number of elements in the queue.
 *
 * @param queue Queue to query.
 * @param out_value Pointer that receives the current queue size.
 * @return ATLAS_SUCCESS on success, or ATLAS_ERROR_NULL if queue or
 * out_value is NULL.
 */
int atlas_queue_size(const AtlasQueue *queue, size_t *out_value);

/**
 * @brief Checks whether the queue is empty.
 *
 * @param queue Queue to query.
 * @param out_value Pointer that receives true if the queue is empty,
 *                  or false otherwise.
 * @return ATLAS_SUCCESS on success, or ATLAS_ERROR_NULL if queue or
 * out_value is NULL.
 */
int atlas_queue_empty(const AtlasQueue *queue, bool *out_value);