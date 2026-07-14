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