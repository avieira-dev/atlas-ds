/*
 * AtlasDS
 * Dynamic Array Public API
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#ifndef ATLAS_ARRAY_H
#define ATLAS_ARRAY_H

#include <stddef.h>
#include <stdbool.h>

#define ATLAS_ARRAY_STANDARD_CAPACITY 1

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

/**
 * @brief Inserts a new element at the end of the dynamic array.
 *
 * Adds the specified value to the end of the array. If the current
 * capacity is insufficient, the internal buffer is resized (doubled)
 * to accommodate additional elements.
 *
 * @param arr Pointer to the AtlasArray instance.
 * @param value Value to be inserted into the array.
 *
 * @return ATLAS_SUCCESS on success, ATLAS_ERROR_NULL if `arr` is NULL,
 * or a propagated allocation error if resizing fails.
 */
int atlas_array_push(AtlasArray *arr, int value);

/**
 * @brief Retrieves an element from the dynamic array at a specific index.
 *
 * Checks if the index is valid and within bounds, then extracts the value
 * and stores it in the provided output pointer.
 *
 * @param arr Pointer to the AtlasArray instance.
 * @param index The zero-based index of the element to retrieve.
 * @param out_value Pointer to the variable where the retrieved value will be stored.
 *
 * @return ATLAS_SUCCESS on success, ATLAS_ERROR_NULL if `arr` or `out_value` is NULL,
 * or ATLAS_ERROR_BOUNDS if the index is out of bounds.
 */
int atlas_array_get(const AtlasArray *arr, size_t index, int *out_value);

/**
 * @brief Replaces the value stored at a specific index in the dynamic array.
 *
 * Performs bounds checking before overwriting the target element.
 * The operation modifies the existing value without changing the
 * logical size or the allocated capacity of the array.
 *
 * @param arr Pointer to the AtlasArray instance.
 * @param index Zero-based index of the element to be replaced.
 * @param new_value New value that will overwrite the current element.
 *
 * @return ATLAS_SUCCESS on success, ATLAS_ERROR_NULL if `arr` is NULL,
 * or ATLAS_ERROR_BOUNDS if the index is out of bounds.
 */
int atlas_array_set(AtlasArray *arr, size_t index, int new_value);

/**
 * @brief Gets the current number of elements stored in the dynamic array.
 *
 * @param arr Pointer to the AtlasArray instance.
 *
 * @return The number of elements currently in the array, or 0 if `arr` is NULL.
 */
size_t atlas_array_size(const AtlasArray *arr);

/**
 * @brief Gets the total capacity of the dynamic array.
 *
 * The capacity represents how many elements the internal buffer can hold
 * before requiring a memory reallocation.
 *
 * @param arr Pointer to the AtlasArray instance.
 *
 * @return The current capacity of the array, or 0 if `arr` is NULL.
 */
size_t atlas_array_capacity(const AtlasArray *arr);

/**
 * @brief Removes the last element from the dynamic array.
 *
 * Retrieves the last inserted element, stores it in the provided
 * output pointer, and decreases the logical size of the array.
 * This operation does not reduce the internal capacity.
 *
 * @param arr Pointer to the AtlasArray instance.
 * @param out_value Pointer where the removed value will be stored.
 *
 * @return ATLAS_SUCCESS on success, ATLAS_ERROR_NULL if `arr` or `out_value` is NULL,
 * or ATLAS_ERROR_EMPTY if the container contains no elements.
 */
int atlas_array_pop(AtlasArray *arr, int *out_value);

/**
 * @brief Ensures the dynamic array has at least the specified capacity.
 *
 * If the current capacity is lower than the requested value, the internal
 * buffer is reallocated to accommodate the new capacity. If the requested
 * capacity is less than or equal to the current capacity, the function
 * performs no operation and returns success.
 *
 * @param arr Pointer to the AtlasArray instance.
 * @param new_capacity Minimum required capacity for the array.
 *
 * @return ATLAS_SUCCESS on success (including no-op cases), ATLAS_ERROR_NULL if `arr` is NULL,
 * or a propagated allocation error if resizing fails.
 */
int atlas_array_reserve(AtlasArray *arr, size_t new_capacity);

/**
 * @brief Removes all elements from the dynamic array.
 *
 * Resets the logical size of the array to zero while preserving
 * the currently allocated memory buffer and capacity.
 *
 * @param arr Pointer to the AtlasArray instance.
 *
 * @return ATLAS_SUCCESS on success, or ATLAS_ERROR_NULL if `arr` is NULL.
 */
int atlas_array_clear(AtlasArray *arr);

/**
 * @brief Reduces the allocated capacity to match the current size.
 *
 * Shrinks the internal buffer to eliminate unused capacity and
 * minimize memory consumption. Empty arrays retain a standard minimum capacity.
 *
 * @param arr Pointer to the AtlasArray instance.
 *
 * @return ATLAS_SUCCESS on success, ATLAS_ERROR_NULL if `arr` is NULL,
 * or a propagated allocation error if resizing fails.
 */
int atlas_array_shrink_to_fit(AtlasArray *arr);

/**
 * @brief Checks whether the dynamic array is empty.
 *
 * Evaluates the current logical size of the array and stores
 * the result in the provided output parameter.
 *
 * @param arr Pointer to the AtlasArray instance.
 * @param empty Pointer where the boolean result will be stored.
 *
 * @return ATLAS_SUCCESS on success, or ATLAS_ERROR_NULL if `arr` or `empty` is NULL.
 */
int atlas_array_empty(const AtlasArray *arr, bool *empty);

/**
 * @brief Retrieves the last element of the dynamic array.
 *
 * Copies the last stored element into the provided output
 * parameter without removing it from the array.
 *
 * @param arr Pointer to the AtlasArray instance.
 * @param out_value Pointer where the last element will be stored.
 *
 * @return ATLAS_SUCCESS on success, ATLAS_ERROR_EMPTY if the array is empty,
 * or ATLAS_ERROR_NULL if `arr` or `out_value` is NULL.
 */
int atlas_array_front(const AtlasArray *arr, int *out_value);

/**
 * @brief Retrieves the last element of the dynamic array.
 *
 * Copies the last stored element into the provided output
 * parameter without removing it from the array.
 *
 * If the array contains no elements, the function performs
 * no write operation and returns an empty container error.
 *
 * @param arr Pointer to the AtlasArray instance.
 * @param out_value Pointer where the last element will be stored.
 *
 * @return ATLAS_SUCCESS on success, ATLAS_ERROR_EMPTY if the array is empty,
 * or ATLAS_ERROR_NULL if any input pointer is NULL.
 */
int atlas_array_back(const AtlasArray *arr, int *out_value);

/**
 * @brief Inserts an element at the specified index.
 *
 * Inserts the provided value at the given position and shifts
 * all subsequent elements one position to the right to preserve
 * their order.
 *
 * @param arr Pointer to the AtlasArray instance.
 * @param index Zero-based insertion position.
 * @param value Value to be inserted.
 *
 * @return ATLAS_SUCCESS on success, ATLAS_ERROR_NULL if `arr` is NULL,
 * ATLAS_ERROR_BOUNDS if the index is out of bounds, or a propagated error if resizing fails.
 */
int atlas_array_insert(AtlasArray *arr, size_t index, int value);

/**
 * @brief Removes the element at the specified index.
 *
 * Removes the element located at the given position and shifts
 * all subsequent elements one position to the left to preserve
 * their order.
 *
 * @param arr Pointer to the AtlasArray instance.
 * @param index Zero-based position of the element to remove.
 *
 * @return ATLAS_SUCCESS on success, ATLAS_ERROR_NULL if `arr` is NULL,
 * or ATLAS_ERROR_BOUNDS if the index is out of bounds.
 */
int atlas_array_erase(AtlasArray *arr, size_t index);

/**
 * @brief Searches for the first occurrence of a value.
 *
 * Performs a linear search through the array and returns the
 * index of the first element whose value matches the requested value.
 *
 * @param arr Pointer to the AtlasArray instance.
 * @param index_out Output parameter that receives the index of the matching element.
 * @param value Value to search for.
 *
 * @return ATLAS_SUCCESS if found, ATLAS_ERROR_NULL if `arr` or `index_out` is NULL,
 * or ATLAS_ERROR_NOT_FOUND if the value does not exist.
 */
int atlas_array_find(const AtlasArray *arr, size_t *index_out, int value);

/**
 * @brief Checks whether a value exists in the array.
 *
 * Performs a linear search through the array looking for the specified value
 * and outputs a boolean result.
 *
 * @param arr Pointer to the AtlasArray instance.
 * @param contains Output parameter indicating whether the value exists.
 * @param value Value to search for.
 *
 * @return ATLAS_SUCCESS on success, or ATLAS_ERROR_NULL if `arr` or `contains` is NULL.
 */
int atlas_array_contains(const AtlasArray *arr, bool *contains, int value);

/**
 * @brief Swaps the values stored at two positions in the array.
 *
 * Exchanges the elements located at the specified indices.
 *
 * @param arr Pointer to the AtlasArray instance.
 * @param index_a Index of the first element.
 * @param index_b Index of the second element.
 *
 * @return ATLAS_SUCCESS on success, ATLAS_ERROR_NULL if `arr` is NULL,
 * or ATLAS_ERROR_BOUNDS if either index is out of bounds.
 */
int atlas_array_swap(AtlasArray *arr, size_t index_a, size_t index_b);

/**
 * @brief Copies all elements from one array into another.
 *
 * Replaces the contents of the destination array with a copy of
 * the elements stored in the source array. Self-copy operations are rejected.
 *
 * @param src Pointer to the source array.
 * @param dest Pointer to the destination array.
 *
 * @return ATLAS_SUCCESS on success, ATLAS_ERROR_NULL if `src` or `dest` is NULL,
 * ATLAS_ERROR_INVALID_ARGUMENT if `src == dest`, or a propagated error if resizing fails.
 */
int atlas_array_copy(const AtlasArray *src, AtlasArray *dest);

/**
 * @brief Creates an independent copy of an existing array.
 *
 * Allocates a new AtlasArray instance and duplicates all
 * elements, metadata, and capacity information from the source array.
 *
 * @param src Pointer to the source array.
 *
 * @return Pointer to the newly allocated clone on success, or NULL if `src` is NULL 
 * or memory allocation fails.
 */
AtlasArray *atlas_array_clone(const AtlasArray *src);

#endif