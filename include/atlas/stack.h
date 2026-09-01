/*
 * AtlasDS
 * Stack Public API
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#ifndef ATLAS_STACK_H
#define ATLAS_STACK_H

#include <stddef.h>

/**
 * @brief Opaque structure representing a generic stack.
 *
 * This structure is intentionally opaque to enforce encapsulation.
 * Internally, it stores elements as dynamically allocated nodes
 * using a byte-based storage mechanism.
 *
 * Users should only interact with the stack through the public API.
 */
typedef struct atlas_stack AtlasStack;

/**
 * @brief Creates and initializes a generic stack.
 *
 * Allocates memory for the stack structure and initializes an empty
 * stack ready to store elements of the specified type.
 *
 * The stack does not allocate elements until values are inserted.
 *
 * @param type_size Size in bytes of each element to be stored.
 * Must be greater than 0.
 *
 * @return Pointer to the newly created AtlasStack on success,
 * or NULL if type_size is 0 or memory allocation fails.
 */
AtlasStack *atlas_stack_create(size_t type_size);

/**
 * @brief Destroys a generic stack and releases its memory.
 *
 * Frees all elements contained in the stack, then releases the
 * stack structure itself. After destruction, the pointer is set
 * to NULL to prevent dangling pointer usage.
 *
 * @param ptr_atlas_stack Pointer to the AtlasStack pointer.
 * If the pointer or the referenced stack is NULL, the function
 * returns an error code.
 *
 * @return ATLAS_SUCCESS on success, or ATLAS_ERROR_NULL if the
 * input pointer or referenced stack is NULL.
 */
int atlas_stack_destroy(AtlasStack **ptr_atlas_stack);

#endif