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

#endif