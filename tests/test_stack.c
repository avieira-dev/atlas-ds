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
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL push validation\n");

    printf(COLOR_BOLD_CYAN "\n════════════════════════════════════════════════════════\n" COLOR_RESET "\n");
    printf(COLOR_BOLD_GREEN " ✔ SUCCESS:" COLOR_RESET " All tests were completed successfully.\n\n");

    return 0;
}