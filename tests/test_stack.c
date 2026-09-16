/*
 * AtlasDS
 * Stack Tests
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#include <stdio.h>
#include <string.h>

#include "atlas/stack.h"
#include "atlas/status.h"
#include "atlas/terminal.h"

static int test_create_destroy(void) {
    AtlasStack *stack = atlas_stack_create(sizeof(int));
    if (!stack) {
        return 1;
    }

    if (atlas_stack_destroy(&stack) != ATLAS_SUCCESS) {
        return 1;
    }

    if (stack != NULL) {
        return 1;
    }

    return 0;
}

static int test_create_invalid_type_size(void) {
    AtlasStack *stack = atlas_stack_create(0);
    if (stack) {
        return 1;
    }

    return 0;
}

static int test_destroy_null(void) {
    AtlasStack *stack = NULL;

    if (atlas_stack_destroy(&stack) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_stack_destroy(NULL) != ATLAS_ERROR_NULL) {
        return 1;
    }

    return 0;
}

static int test_push_empty(void) {
    AtlasStack *stack = atlas_stack_create(sizeof(int));
    if (!stack) {
        return 1;
    }

    int value = 1969;

    if (atlas_stack_push(stack, &value) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_destroy(&stack) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_push_multiple(void) {
    AtlasStack *stack = atlas_stack_create(sizeof(int));
    if (!stack) {
        return 1;
    }

    int first = 19;
    int second = 47;
    int third = 54;

    if (atlas_stack_push(stack, &first) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_push(stack, &second) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_push(stack, &third) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_destroy(&stack) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_push_null(void) {
    AtlasStack *stack = atlas_stack_create(sizeof(int));
    if (!stack) {
        return 1;
    }

    int value = 1941;

    if (atlas_stack_push(NULL, &value) != ATLAS_ERROR_NULL) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_push(stack, NULL) != ATLAS_ERROR_NULL) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_destroy(&stack) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_pop_single(void) {
    AtlasStack *stack = atlas_stack_create(sizeof(int));

    if (!stack) {
        return 1;
    }

    int value = 1969;

    if (atlas_stack_push(stack, &value) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    int out_value = 0;

    if (atlas_stack_pop(stack, &out_value) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (out_value != value) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_destroy(&stack) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_pop_multiple(void) {
    AtlasStack *stack = atlas_stack_create(sizeof(int));

    if (!stack) {
        return 1;
    }

    int first = 19;
    int second = 47;
    int third = 54;

    if (atlas_stack_push(stack, &first) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_push(stack, &second) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_push(stack, &third) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    int out_value = 0;

    if (atlas_stack_pop(stack, &out_value) != ATLAS_SUCCESS || out_value != third) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_pop(stack, &out_value) != ATLAS_SUCCESS || out_value != second) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_pop(stack, &out_value) != ATLAS_SUCCESS || out_value != first) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_destroy(&stack) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_pop_empty(void) {
    AtlasStack *stack = atlas_stack_create(sizeof(int));

    if (!stack) {
        return 1;
    }

    int out_value = 0;

    if (atlas_stack_pop(stack, &out_value) != ATLAS_ERROR_EMPTY) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_destroy(&stack) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_pop_null(void) {
    AtlasStack *stack = atlas_stack_create(sizeof(int));

    if (!stack) {
        return 1;
    }

    int value = 1941;
    int out_value = 0;

    if (atlas_stack_pop(NULL, &out_value) != ATLAS_ERROR_NULL) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_pop(stack, NULL) != ATLAS_ERROR_NULL) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_push(stack, &value) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_pop(NULL, NULL) != ATLAS_ERROR_NULL) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_destroy(&stack) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

int main(void) {
    printf("\n" COLOR_BOLD_BLUE "╭────────────────────────────────────────────────────────╮" COLOR_RESET "\n");
    printf(COLOR_BOLD_BLUE "│" COLOR_RESET "                 AtlasDS - Stack Tests                  " COLOR_BOLD_BLUE "│" COLOR_RESET "\n");
    printf(COLOR_BOLD_BLUE "╰────────────────────────────────────────────────────────╯" COLOR_RESET "\n\n");

    printf(COLOR_YELLOW "ℹ " COLOR_RESET "Starting AtlasDS stack tests...\n\n");

    // =========================================================
    // Lifecycle
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Lifecycle" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_create_destroy()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Create/Destroy operation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Create/Destroy operation\n");

    if (test_create_invalid_type_size()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Type size validation\n");
        return 1;   
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Type size validation\n");

    if (test_destroy_null()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL destroy validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL destroy validation\n\n");

    // =========================================================
    // Insertion
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Insertion" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_push_empty()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Push into empty stack\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Push into empty stack\n");

    if (test_push_multiple()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Push multiple elements\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Push multiple elements\n");

    if (test_push_null()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL push validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL push validation\n\n");

    // =========================================================
    // Removal
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Removal" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_pop_single()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Pop single element\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Pop single element\n");

    if (test_pop_multiple()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Pop multiple elements\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Pop multiple elements\n");

    if (test_pop_empty()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Pop from empty stack\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Pop from empty stack\n");

    if (test_pop_null()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL pop validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL pop validation\n");

    printf(COLOR_BOLD_CYAN "\n════════════════════════════════════════════════════════\n" COLOR_RESET "\n");
    printf(COLOR_BOLD_GREEN " ✔ SUCCESS:" COLOR_RESET " All tests were completed successfully.\n\n");

    return 0;
}