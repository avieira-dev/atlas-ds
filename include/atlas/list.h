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

/**
 * @brief Inserts an element at the beginning of the linked list.
 *
 * Allocates a new node, copies the provided element into the node's
 * internal storage, and makes it the new first element of the list.
 *
 * If the list is empty, the new node becomes both the first and last
 * node.
 *
 * @param list Pointer to the linked list.
 * @param value Pointer to the element to be copied into the new node.
 *
 * @return ATLAS_SUCCESS on success, ATLAS_ERROR_NULL if either
 * pointer is NULL, or ATLAS_ERROR_MEMORY if node allocation fails.
 */
int atlas_list_push_front(AtlasList *list, const void *value);

/**
 * @brief Appends an element to the end of the linked list.
 *
 * Allocates a new node, copies the provided element into the node's
 * internal storage, and links it after the current last node.
 *
 * If the list is empty, the new node becomes both the first and last
 * node.
 *
 * @param list Pointer to the linked list.
 * @param value Pointer to the element to be copied into the new node.
 *
 * @return ATLAS_SUCCESS on success, ATLAS_ERROR_NULL if either
 * pointer is NULL, or ATLAS_ERROR_MEMORY if node allocation fails.
 */
int atlas_list_push_back(AtlasList *list, const void *value);

/**
 * @brief Removes and returns the first element of the linked list.
 *
 * Copies the value stored in the first node to the user-provided
 * output buffer, removes the node from the list, and releases its
 * allocated memory.
 *
 * If the removed node is the only element in the list, both the
 * first and last node pointers become NULL.
 *
 * @param list Pointer to the linked list.
 * @param out_value Pointer to the destination buffer that receives
 * a copy of the removed element.
 *
 * @return ATLAS_SUCCESS on success, ATLAS_ERROR_NULL if either
 * pointer is NULL, or ATLAS_ERROR_EMPTY if the list contains no
 * elements.
 */
int atlas_list_pop_front(AtlasList *list, void *out_value);

/**
 * @brief Removes and returns the last element of the linked list.
 *
 * Copies the value stored in the last node to the user-provided
 * output buffer, removes the node from the list, and releases its
 * allocated memory.
 *
 * Since the current implementation uses a singly linked list, the
 * operation traverses the list to locate the node preceding the
 * last element before updating the internal links.
 *
 * If the removed node is the only element in the list, both the
 * first and last node pointers become NULL.
 *
 * @param list Pointer to the linked list.
 * @param out_value Pointer to the destination buffer that receives
 * a copy of the removed element.
 *
 * @return ATLAS_SUCCESS on success, ATLAS_ERROR_NULL if either
 * pointer is NULL, or ATLAS_ERROR_EMPTY if the list contains no
 * elements.
 */
int atlas_list_pop_back(AtlasList *list, void *out_value);

/**
 * @brief Retrieves the element stored at the specified index.
 *
 * Traverses the linked list until reaching the requested node,
 * then copies the stored element into the user-provided output
 * buffer.
 *
 * The index is zero-based, where index 0 refers to the first
 * element of the list.
 *
 * @param list Pointer to the linked list.
 * @param out_value Pointer to the destination buffer that receives
 * a copy of the stored element.
 * @param index Zero-based index of the element to retrieve.
 *
 * @return ATLAS_SUCCESS on success, ATLAS_ERROR_NULL if either
 * pointer is NULL, or ATLAS_ERROR_BOUNDS if the index is outside
 * the valid range.
 */
int atlas_list_get(const AtlasList *list, void *out_value, size_t index);

/**
 * @brief Replaces the element stored at the specified index.
 *
 * Traverses the linked list until reaching the requested node,
 * then copies the provided element into the node's internal
 * storage.
 *
 * The index is zero-based, where index 0 refers to the first
 * element of the list.
 *
 * @param list Pointer to the linked list.
 * @param new_value Pointer to the element that will replace the
 * current value stored in the node.
 * @param index Zero-based index of the element to replace.
 *
 * @return ATLAS_SUCCESS on success, ATLAS_ERROR_NULL if either
 * pointer is NULL, or ATLAS_ERROR_BOUNDS if the index is outside
 * the valid range.
 */
int atlas_list_set(AtlasList *list, const void *new_value, size_t index);

#endif