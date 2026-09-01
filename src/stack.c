/*
 * AtlasDS
 * Stack Implementation
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#include <stdlib.h>
#include <string.h>

#include "atlas/stack.h"
#include "atlas/status.h"

typedef struct atlas_stack_element AtlasStackElement;

struct atlas_stack {
    size_t type_size; // Size in bytes of each stored element
    size_t stack_size; // Current number of elements in the stack
    AtlasStackElement *top_element; // Pointer to the top element
};

struct atlas_stack_element {
    AtlasStackElement *previous_element; // Pointer to the previous element in the stack
    unsigned char data[]; // Flexible array member storing the element bytes
};

// =====================
// Internal Helpers
// =====================

/*
 * Internal helper that releases every element currently stored
 * in the stack.
 *
 * Traverses the stack from the top element toward the bottom
 * by following each element's previous pointer before releasing
 * its allocated memory.
 *
 * The stack structure itself is not modified or released.
 * Callers are responsible for updating the stack metadata after
 * this function returns.
 */
static void atlas_stack_free_elements(const AtlasStack *stack) {
    AtlasStackElement *current_element = stack->top_element;
    while(current_element) {
        AtlasStackElement *prev_element = current_element->previous_element;
        free(current_element);
        current_element = prev_element;
    }
}

// =====================
// Lifecycle
// =====================

/*
 * Implementation of atlas_stack_create:
 * Allocates memory for the stack structure and initializes an
 * empty stack with no elements.
 *
 * Stores the size of each element and initializes the top
 * element pointer to NULL.
 *
 * Returns NULL if type_size is 0 or if the memory allocation
 * for the stack structure fails.
 */
AtlasStack *atlas_stack_create(size_t type_size) {
    if (type_size == 0) {
        return NULL;
    }

    AtlasStack *stack = malloc(sizeof(*stack));
    if (!stack) {
        return NULL;
    }

    stack->type_size = type_size;
    stack->stack_size = 0;
    stack->top_element = NULL;

    return stack;
}

/*
 * Implementation of atlas_stack_destroy:
 * Safely releases all elements contained in the stack before
 * freeing the main stack structure.
 *
 * Traverses the stack by following each element's previous
 * pointer, preserving the previous element address before
 * releasing the current element to avoid losing access to the
 * remaining elements.
 *
 * Takes a double pointer to reset the user's pointer to NULL,
 * preventing accidental dangling pointer access.
 *
 * Returns ATLAS_ERROR_NULL if the double pointer or referenced
 * stack is NULL.
 */
int atlas_stack_destroy(AtlasStack **ptr_atlas_stack) {
    if (!ptr_atlas_stack || !*ptr_atlas_stack) {
        return ATLAS_ERROR_NULL;
    }

    AtlasStack *ptr_stack = *ptr_atlas_stack;

    atlas_stack_free_elements(ptr_stack);

    free(ptr_stack);

    *ptr_atlas_stack = NULL;

    return ATLAS_SUCCESS;
}