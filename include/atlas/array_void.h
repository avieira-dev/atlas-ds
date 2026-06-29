/*
 * AtlasDS
 * Generic Dynamic Array Public API
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#ifndef ATLAS_ARRAY_VOID_H
#define ATLAS_ARRAY_VOID_H

#include <stddef.h>

#define ATLAS_ARRAY_VOID_STANDARD_CAPACITY 1

/**
 * @brief Opaque structure representing a generic dynamic array.
 *
 * This structure is intentionally opaque to enforce encapsulation.
 * It uses a void pointer to store elements of any data type.
 * Users should only interact with it through the public API.
 */
typedef struct atlas_array_void AtlasArrayVoid;

/**
 * @brief Creates and initializes a generic dynamic array.
 *
 * Allocates a dynamic array structure and its internal generic storage buffer.
 * If the specified capacity is zero, a default capacity of ATLAS_ARRAY_VOID_STANDARD_CAPACITY elements is used.
 *
 * @param type_size Size in bytes of each element to be stored. Must be greater than 0.
 * @param initial_capacity Initial capacity of the dynamic array.
 * If 0, a capacity of ATLAS_ARRAY_VOID_STANDARD_CAPACITY elements is used instead.
 *
 * @return Pointer to the newly created AtlasArrayVoid on success,
 * or NULL if memory allocation fails or type_size is 0.
 */
AtlasArrayVoid *atlas_array_void_create(size_t type_size, size_t initial_capacity);

/**
 * @brief Destroys a generic dynamic array and releases its memory.
 *
 * Frees the internal generic storage buffer and the dynamic array
 * structure itself. After destruction, the pointer is set to NULL
 * to prevent dangling pointer usage.
 *
 * @param ptr_atlas_array_void Pointer to the AtlasArrayVoid pointer.
 * If the pointer or the referenced array is NULL,
 * the function returns an error code.
 *
 * @return 0 on success, or -1 if the input pointer or the referenced array is NULL.
 */
int atlas_array_void_destroy(AtlasArrayVoid **ptr_atlas_array_void);

#endif