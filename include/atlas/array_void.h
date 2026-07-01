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
#include <stdbool.h>

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

/**
 * @brief Appends a new element to the end of the generic dynamic array.
 *
 * Copies the bytes of the specified element into the internal storage buffer.
 * If the array is full, its capacity is automatically increased before the
 * element is inserted.
 *
 * @param arr Pointer to the generic dynamic array.
 * @param value Pointer to the element to be copied into the array.
 * The pointed object is not modified.
 *
 * @return 0 on success, or -1 if the array pointer or value pointer
 * is NULL, or if memory reallocation fails.
 */
int atlas_array_void_push(AtlasArrayVoid *arr, const void *value);

/**
 * @brief Retrieves a copy of an element stored in the generic dynamic array.
 *
 * Copies the bytes of the element at the specified index into the
 * user-provided output buffer. The stored element itself is not modified.
 *
 * @param arr Pointer to the generic dynamic array.
 * @param index Zero-based index of the element to retrieve.
 * @param out_value Pointer to the destination buffer that will receive
 * the copied element.
 *
 * @return 0 on success, or -1 if the array pointer, output pointer
 * is NULL, or if the index is out of bounds.
 */
int atlas_array_void_get(const AtlasArrayVoid *arr, size_t index, void *out_value);

/**
 * @brief Replaces the value of an existing element in the generic dynamic array.
 *
 * Copies the bytes of the specified element into the array position
 * identified by the given index, overwriting the previously stored value.
 *
 * @param arr Pointer to the generic dynamic array.
 * @param index Zero-based index of the element to replace.
 * @param new_value Pointer to the element whose bytes will be copied
 * into the array. The pointed object is not modified.
 *
 * @return 0 on success, or -1 if the array pointer, value pointer
 * is NULL, or if the index is out of bounds.
 */
int atlas_array_void_set(AtlasArrayVoid *arr, size_t index, const void *new_value);

/**
 * @brief Removes the last element from the generic dynamic array.
 *
 * Copies the bytes of the last stored element into the user-provided
 * output buffer and removes the element from the array.. The internal storage  * capacity remains unchanged.
 *
 * @param arr Pointer to the generic dynamic array.
 * @param out_value Pointer to the destination buffer that will receive
 * the removed element.
 *
 * @return 0 on success, or -1 if the array pointer, output pointer,
 * or the array is empty.
 */
int atlas_array_void_pop(AtlasArrayVoid *arr, void *out_value);

/**
 * @brief Retrieves the current number of elements stored in the generic dynamic array.
 *
 * Stores the logical size of the array in the user-provided output variable.
 *
 * @param arr Pointer to the generic dynamic array.
 * @param out_value Pointer to the variable that will receive the current size.
 *
 * @return 0 on success, or -1 if the array pointer or output pointer is NULL.
 */
int atlas_array_void_size(const AtlasArrayVoid *arr, size_t *out_value);

/**
 * @brief Retrieves the current storage capacity of the generic dynamic array.
 *
 * Stores the number of elements that can currently be held without
 * reallocating the internal storage buffer.
 *
 * @param arr Pointer to the generic dynamic array.
 * @param out_value Pointer to the variable that will receive the current capacity.
 *
 * @return 0 on success, or -1 if the array pointer or output pointer is NULL.
 */
int atlas_array_void_capacity(const AtlasArrayVoid *arr, size_t *out_value);

/**
 * @brief Checks whether the generic dynamic array is empty.
 *
 * Stores true in the output variable if the array contains no elements,
 * or false otherwise.
 *
 * @param arr Pointer to the generic dynamic array.
 * @param out_value Pointer to the variable that will receive the result.
 *
 * @return 0 on success, or -1 if the array pointer or output pointer is NULL.
 */
int atlas_array_void_empty(const AtlasArrayVoid *arr, bool *out_value);

#endif