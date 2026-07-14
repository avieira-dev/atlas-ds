/*
 * AtlasDS
 * Linked Lists Public API
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#ifndef ATLAS_LIST_H
#define ATLAS_LIST_H

#include <stddef.h>

/**
 * @brief Opaque structure representing a generic linked list.
 *
 * This structure is intentionally opaque to enforce encapsulation.
 * It stores elements of any data type using a generic byte-based
 * storage mechanism internally.
 *
 * Users should only interact with the list through the public API.
 */
typedef struct atlas_list AtlasList;

/**
 * @brief Creates and initializes a generic linked list.
 *
 * Allocates memory for the linked list structure and initializes
 * an empty list ready to store elements of the specified type.
 *
 * The list does not allocate nodes until elements are inserted.
 *
 * @param type_size Size in bytes of each element to be stored.
 * Must be greater than 0.
 *
 * @return Pointer to the newly created AtlasList on success,
 * or NULL if type_size is 0 or memory allocation fails.
 */
AtlasList *atlas_list_create(size_t type_size);

/**
 * @brief Destroys a generic linked list and releases its memory.
 *
 * Frees all nodes contained in the list, then releases the list
 * structure itself. After destruction, the pointer is set to NULL
 * to prevent dangling pointer usage.
 *
 * @param ptr_atlas_list Pointer to the AtlasList pointer.
 * If the pointer or the referenced list is NULL,
 * the function returns an error code.
 *
 * @return ATLAS_SUCCESS on success, or ATLAS_ERROR_NULL if the
 * input pointer or referenced list is NULL.
 */
int atlas_list_destroy(AtlasList **ptr_atlas_list);

#endif