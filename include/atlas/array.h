#ifndef ATLAS_ARRAY_H
#define ATLAS_ARRAY_H

#include <stddef.h>
#include <stdbool.h>

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

/**
 * @brief Inserts a new element at the end of the dynamic array.
 *
 * Adds the specified value to the end of the array. If the current
 * capacity is insufficient, the internal buffer is resized (doubled)
 * to accommodate additional elements.
 *
 * @param arr Pointer to the AtlasArray instance.
 * If arr is NULL, the function returns -1.
 *
 * @param value Value to be inserted into the array.
 *
 * @return 0 on success, or -1 if memory allocation fails
 * or the input pointer is NULL.
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
 * @return 0 on success, or -1 if the array pointer is NULL, the output pointer
 * is NULL, or the index is out of bounds.
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
 * @return 0 on success, or -1 if the array pointer is NULL
 * or the index is out of bounds.
 */
int atlas_array_set(AtlasArray *arr, size_t index, int new_value);

/**
 * @brief Gets the current number of elements stored in the dynamic array.
 *
 * @param arr Pointer to the AtlasArray instance.
 *
 * @return The number of elements currently in the array, or 0 if arr is NULL.
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
 * @return The current capacity of the array, or 0 if arr is NULL.
 */
size_t atlas_array_capacity(const AtlasArray *arr);

/**
 * @brief Removes the last element from the dynamic array.
 *
 * Retrieves the last inserted element, stores it in the provided
 * output pointer, and decreases the logical size of the array.
 *
 * This operation does not reduce the internal capacity.
 *
 * @param arr Pointer to the AtlasArray instance.
 * @param out_value Pointer where the removed value will be stored.
 *
 * @return 0 on success, or -1 if:
 * - arr is NULL
 * - out_value is NULL
 * - the array is empty
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
 * @return 0 on success (including no-op cases), or -1 if memory allocation
 * fails or the input pointer is NULL.
 */
int atlas_array_reserve(AtlasArray *arr, size_t new_capacity);

/**
 * @brief Removes all elements from the dynamic array.
 *
 * Resets the logical size of the array to zero while preserving
 * the currently allocated memory buffer and capacity.
 *
 * This operation allows the array to be reused without triggering
 * additional memory allocations on future insertions.
 *
 * @param arr Pointer to the AtlasArray instance.
 *
 * @return 0 on success, or -1 if the input pointer is NULL.
 */
int atlas_array_clear(AtlasArray *arr);

/**
 * @brief Checks whether the dynamic array is empty.
 *
 * Evaluates the current logical size of the array and stores
 * the result in the provided output parameter.
 *
 * An array is considered empty when its size is equal to zero.
 *
 * @param arr Pointer to the AtlasArray instance.
 * @param empty Pointer where the result will be stored.
 *
 * @return 0 on success, or -1 if any input pointer is NULL.
 */
int atlas_array_empty(const AtlasArray *arr, bool *empty);

/**
 * @brief Retrieves the first element of the dynamic array.
 *
 * Copies the first stored element into the provided output
 * parameter without removing it from the array.
 *
 * If the array contains no elements, the function performs
 * no write operation and leaves the output parameter unchanged.
 *
 * @param arr Pointer to the AtlasArray instance.
 * @param out_value Pointer where the first element will be stored.
 *
 * @return 0 on success, or -1 if any input pointer is NULL.
 */
int atlas_array_front(const AtlasArray *arr, int *out_value);

/**
 * @brief Retrieves the last element of the dynamic array.
 *
 * Copies the last stored element into the provided output
 * parameter without removing it from the array.
 *
 * If the array contains no elements, the function performs
 * no write operation and leaves the output parameter unchanged.
 *
 * @param arr Pointer to the AtlasArray instance.
 * @param out_value Pointer where the last element will be stored.
 *
 * @return 0 on success, or -1 if any input pointer is NULL.
 */
int atlas_array_back(const AtlasArray *arr, int *out_value);

/**
 * @brief Inserts an element at the specified index.
 *
 * Inserts the provided value at the given position and shifts
 * all subsequent elements one position to the right to preserve
 * their order.
 *
 * If the internal buffer is full, the array automatically grows
 * before performing the insertion.
 *
 * The index may be equal to the current size, in which case the
 * operation behaves as an append at the end of the array.
 *
 * @param arr Pointer to the AtlasArray instance.
 * @param index Zero-based insertion position.
 * @param value Value to be inserted.
 *
 * @return 0 on success, or -1 if the array pointer is NULL,
 * the index is out of bounds, or memory reallocation fails.
 */
int atlas_array_insert(AtlasArray *arr, size_t index, int value);

#endif