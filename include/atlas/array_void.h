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
 * output buffer and removes the element from the array. The internal
 * storage capacity remains unchanged.
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

/**
 * @brief Retrieves a copy of the first element stored in the generic dynamic array.
 *
 * Copies the bytes of the first stored element into the user-provided
 * output buffer.
 *
 * @param arr Pointer to the generic dynamic array.
 * @param out_value Pointer to the destination buffer.
 *
 * @return 0 on success, or -1 if the array pointer, output pointer,
 * or the array is empty.
 */
int atlas_array_void_front(const AtlasArrayVoid *arr, void *out_value);

/**
 * @brief Retrieves a copy of the last element stored in the generic dynamic array.
 *
 * Copies the bytes of the last stored element into the user-provided
 * output buffer.
 *
 * @param arr Pointer to the generic dynamic array.
 * @param out_value Pointer to the destination buffer.
 *
 * @return 0 on success, or -1 if the array pointer, output pointer,
 * or the array is empty.
 */
int atlas_array_void_back(const AtlasArrayVoid *arr, void *out_value);

/**
 * @brief Ensures that the generic dynamic array has at least the specified capacity.
 *
 * Expands the internal storage buffer if the requested capacity is greater
 * than the current capacity. If the requested capacity is less than or
 * equal to the current capacity, no changes are made.
 *
 * @param arr Pointer to the generic dynamic array.
 * @param new_capacity Minimum capacity that the array should support.
 *
 * @return 0 on success, or -1 if the array pointer is NULL or memory
 * reallocation fails.
 */
int atlas_array_void_reserve(AtlasArrayVoid *arr, size_t new_capacity);

/**
 * @brief Removes all elements from the generic dynamic array.
 *
 * Clears the logical contents of the array by resetting its size to zero.
 * The allocated storage capacity remains unchanged, allowing future
 * insertions without additional memory allocation.
 *
 * @param arr Pointer to the generic dynamic array.
 *
 * @return 0 on success, or -1 if the array pointer is NULL.
 */
int atlas_array_void_clear(AtlasArrayVoid *arr);

/**
 * @brief Reduces the storage capacity of the generic dynamic array.
 *
 * Shrinks the internal storage buffer to match the current logical size.
 * If the array is empty, the capacity is reduced to
 * ATLAS_ARRAY_VOID_STANDARD_CAPACITY.
 *
 * @param arr Pointer to the generic dynamic array.
 *
 * @return 0 on success, or -1 if the array pointer is NULL or memory
 * reallocation fails.
 */
int atlas_array_void_shrink_to_fit(AtlasArrayVoid *arr);

/**
 * @brief Inserts a new element at the specified position in the generic dynamic array.
 *
 * Copies the bytes of the specified element into the array at the given index.
 * If the insertion occurs before the end of the array, the existing elements
 * are shifted one position to the right to make room for the new element.
 * If the array is full, its storage is automatically expanded before insertion.
 *
 * @param arr Pointer to the generic dynamic array.
 * @param index Zero-based position where the new element will be inserted.
 * Valid values are in the range [0, size].
 * @param value Pointer to the element to be copied into the array.
 *
 * @return 0 on success, or -1 if the array pointer, value pointer
 * is NULL, the index is out of bounds, or memory reallocation fails.
 */
int atlas_array_void_insert(AtlasArrayVoid *arr, size_t index, const void *value);

/**
 * @brief Removes the element at the specified position from the generic dynamic array.
 *
 * Erases the element at the given index by shifting all subsequent elements
 * one position to the left. The logical size of the array is reduced by one,
 * while the allocated storage capacity remains unchanged.
 *
 * @param arr Pointer to the generic dynamic array.
 * @param index Zero-based index of the element to remove.
 *
 * @return 0 on success, or -1 if the array pointer is NULL
 * or the index is out of bounds.
 */
int atlas_array_void_erase(AtlasArrayVoid *arr, size_t index);

/**
 * @brief Exchanges the positions of two elements in the generic dynamic array.
 *
 * Swaps the elements stored at the specified indices by exchanging their
 * contents in place. If both indices are equal, no modification is
 * performed and the function returns success.
 *
 * @param arr Pointer to the generic dynamic array.
 * @param index_a Zero-based index of the first element.
 * @param index_b Zero-based index of the second element.
 *
 * @return 0 on success, or -1 if the array pointer is NULL
 * or either index is out of bounds.
 */
int atlas_array_void_swap(AtlasArrayVoid *arr, size_t index_a, size_t index_b);

/**
 * @brief Copies the contents of one generic dynamic array into another.
 *
 * Copies all stored elements from the source array into the destination
 * array. If necessary, the destination storage is automatically expanded
 * to accommodate the copied elements. Both arrays must store elements of
 * the same size.
 *
 * @param src Pointer to the source generic dynamic array.
 * @param dest Pointer to the destination generic dynamic array.
 *
 * @return 0 on success, or -1 if either array pointer is NULL, the element
 * sizes differ, or memory reallocation fails.
 */
int atlas_array_void_copy(const AtlasArrayVoid *src, AtlasArrayVoid *dest);

/**
 * @brief Creates a copy of a generic dynamic array.
 *
 * Allocates a new generic dynamic array with the same element size and
 * capacity as the source array, then copies all stored elements into the
 * newly created array.
 *
 * @param src Pointer to the source generic dynamic array.
 *
 * @return Pointer to the newly created copy on success, or NULL if the
 * source pointer is NULL or memory allocation fails.
 */
AtlasArrayVoid *atlas_array_void_clone(const AtlasArrayVoid *src);

#endif