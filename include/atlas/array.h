#ifndef ATLAS_ARRAY_H
#define ATLAS_ARRAY_H

#include <stddef.h>

#define STANDARD_CAPACITY 8

/**
 * @brief Opaque structure representing a dynamic array.
 *
 * This structure is intentionally opaque to enforce encapsulation.
 * Users should only interact with it through the public API.
 */
typedef struct atlas_array AtlasArray;

/**
 * @brief Creates and initializes a dynamic array.
 *
 * Allocates a dynamic array and its internal storage buffer.
 * If the specified capacity is zero, a default capacity
 * (STANDARD_CAPACITY) is used.
 *
 * @param initial_capacity Initial capacity of the dynamic array.
 * If 0, STANDARD_CAPACITY is used instead.
 *
 * @return Pointer to the newly created AtlasArray on success,
 * or NULL if memory allocation fails.
 */
AtlasArray *atlas_array_create(size_t initial_capacity);

/**
 * @brief Destroys a dynamic array and releases its memory.
 *
 * Frees the internal storage buffer and the dynamic array
 * structure itself. After destruction, the pointer is set
 * to NULL to prevent dangling pointer usage.
 *
 * @param ptr_atlas_array Pointer to the AtlasArray pointer.
 * If the pointer or the referenced array is NULL,
 * the function does nothing.
 */
void atlas_array_destroy(AtlasArray **ptr_atlas_array);

#endif