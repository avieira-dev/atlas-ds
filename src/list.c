/*
 * AtlasDS
 * Linked Lists Implementation
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#include <atlas/list.h>
#include <atlas/status.h>

#include <stdlib.h>
#include <string.h>

typedef struct atlas_list_node AtlasListNode;

struct atlas_list {
    size_t type_size; // Size in bytes of each stored element
    size_t list_size; // Current number of nodes in the list
    AtlasListNode *first_node; // Pointer to the first node
    AtlasListNode *last_node; // Pointer to the last node
};

struct atlas_list_node {
    AtlasListNode *next_node; // Pointer to the next node in the list
    unsigned char data[]; // Flexible array member storing the element bytes
};

// =====================
// Internal Helpers
// =====================

/*
 * Internal helper that allocates and initializes a new linked list node.
 *
 * Allocates a single memory block containing both the node metadata
 * and the flexible array member used to store the element bytes.
 *
 * The provided value is copied into the node's internal storage using
 * the list element size defined by type_size.
 *
 * Returns NULL if node allocation fails.
 */
static AtlasListNode *atlas_list_create_node(const AtlasList *list, const void *value) {
    AtlasListNode *node = malloc(sizeof(AtlasListNode) + list->type_size);

    if (!node) {
        return NULL;
    }

    memcpy(node->data, value, list->type_size);

    return node;
}

/*
 * Internal helper that initializes the first and last pointers
 * of an empty linked list after inserting its first node.
 *
 * The provided node becomes both the head and tail of the list,
 * and its next pointer is initialized to NULL because there are
 * no subsequent nodes.
 */
static void atlas_list_init_empty_list(AtlasList *list, AtlasListNode *node) {
    list->first_node = node;
    list->last_node = node;
    node->next_node = NULL;
}

// =====================
// Lifecycle
// =====================

/*
 * Implementation of atlas_list_create:
 * Allocates memory for the linked list structure and initializes
 * an empty list with no nodes. Stores the size of each element
 * and initializes the internal node pointers to NULL.
 *
 * Returns NULL if type_size is 0 or if the memory allocation
 * for the list structure fails.
 */
AtlasList *atlas_list_create(size_t type_size) {
    if (type_size == 0) {
        return NULL;
    }

    AtlasList *list = malloc(sizeof(*list));

    if (!list) {
        return NULL;
    }

    list->type_size = type_size;
    list->list_size = 0;
    list->first_node = NULL;
    list->last_node = NULL;

    return list;
}

/*
 * Implementation of atlas_list_destroy:
 * Safely releases all nodes contained in the linked list before
 * freeing the main list structure.
 *
 * Traverses the list by following each node's next pointer,
 * preserving the next node address before releasing the current
 * node to avoid losing access to the remaining elements.
 *
 * Takes a double pointer to reset the user's pointer to NULL,
 * preventing accidental dangling pointer access.
 *
 * Returns ATLAS_ERROR_NULL if the double pointer or referenced
 * list is NULL.
 */
int atlas_list_destroy(AtlasList **ptr_atlas_list) {
    if (!ptr_atlas_list || !*ptr_atlas_list) {
        return ATLAS_ERROR_NULL;
    }

    AtlasList *ptr_list = *ptr_atlas_list;

    AtlasListNode *current_node = ptr_list->first_node;
    while (current_node) {
        AtlasListNode *next_node = current_node->next_node;
        free(current_node);
        current_node = next_node;
    }

    free(ptr_list);

    *ptr_atlas_list = NULL;

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_list_push_front:
 * Allocates a new node, copies the provided element into its
 * internal storage, and inserts it at the beginning of the
 * linked list.
 *
 * If the list is empty, the new node becomes both the first
 * and last node. Otherwise, it is linked before the current
 * first node and becomes the new list head.
 *
 * Returns ATLAS_ERROR_NULL if the list or value pointer is
 * NULL, or ATLAS_ERROR_MEMORY if node allocation fails.
 */
int atlas_list_push_front(AtlasList *list, const void *value) {
    if (!list || !value) {
        return ATLAS_ERROR_NULL;
    }

    AtlasListNode *node = atlas_list_create_node(list, value);
    if (!node) {
        return ATLAS_ERROR_MEMORY;
    }

    if (list->list_size == 0){
        atlas_list_init_empty_list(list, node);
    } else {
        node->next_node = list->first_node;
        list->first_node = node;
    }

    list->list_size++;

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_list_push_back:
 * Allocates a new node, copies the provided element into its
 * internal storage, and appends it to the end of the linked
 * list.
 *
 * If the list is empty, the new node becomes both the first
 * and last node. Otherwise, it is linked after the current
 * last node and becomes the new list tail.
 *
 * Returns ATLAS_ERROR_NULL if the list or value pointer is
 * NULL, or ATLAS_ERROR_MEMORY if node allocation fails.
 */
int atlas_list_push_back(AtlasList *list, const void *value) {
    if (!list || !value) {
        return ATLAS_ERROR_NULL;
    }

    AtlasListNode *node = atlas_list_create_node(list, value);
    if (!node) {
        return ATLAS_ERROR_MEMORY;
    }

    if (list->list_size == 0) {
        atlas_list_init_empty_list(list, node);
    } else {
        list->last_node->next_node = node;
        node->next_node = NULL;
        list->last_node = node;
    }

    list->list_size++;

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_list_pop_front:
 * Removes the first node from the linked list, copies its stored
 * element into the user-provided output buffer, and releases the
 * node memory.
 *
 * If the removed node is the only element in the list, both the
 * first and last node pointers are reset to NULL. Otherwise, the
 * second node becomes the new list head.
 *
 * Returns ATLAS_ERROR_NULL if the list or output buffer pointer
 * is NULL, or ATLAS_ERROR_EMPTY if the list contains no nodes.
 */
int atlas_list_pop_front(AtlasList *list, void *out_value) {
    if (!list || !out_value) {
        return ATLAS_ERROR_NULL;
    }

    if (list->list_size == 0) {
        return ATLAS_ERROR_EMPTY;
    }

    AtlasListNode *temp_ptr = list->first_node;

    if (list->list_size == 1) {
        list->first_node = NULL;
        list->last_node = NULL;
    } else {
        list->first_node = list->first_node->next_node;
    }

    memcpy(out_value, temp_ptr->data, list->type_size);

    list->list_size--;

    free(temp_ptr);

    return ATLAS_SUCCESS;
}

/*
 * Implementation of atlas_list_pop_back:
 * Removes the last node from the linked list, copies its stored
 * element into the user-provided output buffer, and releases the
 * node memory.
 *
 * If the removed node is the only element in the list, both the
 * first and last node pointers are reset to NULL. Otherwise, the
 * list is traversed to locate the node preceding the last one,
 * which becomes the new list tail.
 *
 * Returns ATLAS_ERROR_NULL if the list or output buffer pointer
 * is NULL, or ATLAS_ERROR_EMPTY if the list contains no nodes.
 */
int atlas_list_pop_back(AtlasList *list, void *out_value) {
    if (!list || !out_value) {
        return ATLAS_ERROR_NULL;
    }

    if (list->list_size == 0) {
        return ATLAS_ERROR_EMPTY;
    }

    AtlasListNode *temp_ptr = list->last_node;
    
    if (list->list_size == 1) {
        list->first_node = NULL;
        list->last_node = NULL;
    } else {
        AtlasListNode *current_ptr = list->first_node;
        while (current_ptr->next_node != list->last_node) {
            current_ptr = current_ptr->next_node;
        }

        list->last_node = current_ptr;
        current_ptr->next_node = NULL;
    }

    memcpy(out_value, temp_ptr->data, list->type_size);

    list->list_size--;

    free(temp_ptr);

    return ATLAS_SUCCESS;
}