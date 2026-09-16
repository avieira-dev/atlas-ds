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

static AtlasStackElement *atlas_stack_create_element(const AtlasStack *stack, const void *value) {
    AtlasStackElement *element = malloc(sizeof(AtlasStackElement) + stack->type_size);
    if (!element) {
        return NULL;
    }

    memcpy(element->data, value, stack->type_size);

    element->previous_element = NULL;

    return element;
}

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

/*
 * Implementation of atlas_stack_push:
 * Creates a new stack element containing a copy of the provided value
 * and inserts it at the top of the stack.
 *
 * Preserves the previous top element by assigning it to the new
 * element's previous pointer before updating the stack's top element.
 *
 * Increments the stack size after successfully inserting the new element.
 *
 * Returns ATLAS_ERROR_NULL if the stack or value is NULL, or
 * ATLAS_ERROR_MEMORY if memory allocation fails.
 */
int atlas_stack_push(AtlasStack *stack, const void *value) {
    if (!stack || !value) {
        return ATLAS_ERROR_NULL;
    }

    AtlasStackElement *element = atlas_stack_create_element(stack, value);
    if (!element) {
        return ATLAS_ERROR_MEMORY;
    }

    AtlasStackElement *old_top = stack->top_element;
    stack->top_element = element;
    element->previous_element = old_top;
    stack->stack_size++;

    return ATLAS_SUCCESS;
}

/**
 * @brief Implementation of atlas_stack_pop:
 * Removes the top element from the stack and copies its value
 * into the provided output buffer.
 *
 * Updates the top element pointer to the previous element,
 * releases the removed element's memory, and decrements the
 * stack size.
 *
 * Returns ATLAS_ERROR_NULL if the stack or output value is NULL,
 * or ATLAS_ERROR_EMPTY if the stack contains no elements.
 */
int atlas_stack_pop(AtlasStack *stack, void *out_value) {
    if (!stack || !out_value) {
        return ATLAS_ERROR_NULL;
    }

    if (stack->stack_size == 0) {
        return ATLAS_ERROR_EMPTY;
    }

    AtlasStackElement *element = stack->top_element;
    stack->top_element = element->previous_element;

    memcpy(out_value, element->data, stack->type_size);

    free(element);
    stack->stack_size--;

    return ATLAS_SUCCESS;
}