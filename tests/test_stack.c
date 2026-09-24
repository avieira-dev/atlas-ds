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

static int test_top_single(void) {
    AtlasStack *stack = atlas_stack_create(sizeof(int));

    if (!stack) {
        return 1;
    }

    int value = 1924;

    if (atlas_stack_push(stack, &value) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    int out_value = 0;

    if (atlas_stack_top(stack, &out_value) != ATLAS_SUCCESS) {
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

static int test_top_multiple(void) {
    AtlasStack *stack = atlas_stack_create(sizeof(int));

    if (!stack) {
        return 1;
    }

    int first = 17;
    int second = 26;
    int third = 33;

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

    if (atlas_stack_top(stack, &out_value) != ATLAS_SUCCESS || out_value != third) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_destroy(&stack) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_top_preserves_element(void) {
    AtlasStack *stack = atlas_stack_create(sizeof(int));

    if (!stack) {
        return 1;
    }

    int first = 1993;
    int second = 2000;
    int third = 2007;

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

    if (atlas_stack_top(stack, &out_value) != ATLAS_SUCCESS || out_value != third) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_top(stack, &out_value) != ATLAS_SUCCESS || out_value != third) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_pop(stack, &out_value) != ATLAS_SUCCESS || out_value != third) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_destroy(&stack) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_top_empty(void) {
    AtlasStack *stack = atlas_stack_create(sizeof(int));

    if (!stack) {
        return 1;
    }

    int out_value = 0;

    if (atlas_stack_top(stack, &out_value) != ATLAS_ERROR_EMPTY) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_destroy(&stack) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_top_null(void) {
    AtlasStack *stack = atlas_stack_create(sizeof(int));

    if (!stack) {
        return 1;
    }

    int value = 1815;
    int out_value = 0;

    if (atlas_stack_top(NULL, &out_value) != ATLAS_ERROR_NULL) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_top(stack, NULL) != ATLAS_ERROR_NULL) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_push(stack, &value) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_top(NULL, NULL) != ATLAS_ERROR_NULL) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_destroy(&stack) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}
static int test_size_empty(void) {
    AtlasStack *stack = atlas_stack_create(sizeof(int));

    if (!stack) {
        return 1;
    }

    size_t size = 2000;

    if (atlas_stack_size(stack, &size) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (size != 0) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_destroy(&stack) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_size_multiple(void) {
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

    size_t size = 0;

    if (atlas_stack_size(stack, &size) != ATLAS_SUCCESS || size != 3) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_pop(stack, &first) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_size(stack, &size) != ATLAS_SUCCESS || size != 2) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_destroy(&stack) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_size_null(void) {
    AtlasStack *stack = atlas_stack_create(sizeof(int));

    if (!stack) {
        return 1;
    }

    size_t size = 0;

    if (atlas_stack_size(NULL, &size) != ATLAS_ERROR_NULL) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_size(stack, NULL) != ATLAS_ERROR_NULL) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_size(NULL, NULL) != ATLAS_ERROR_NULL) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_destroy(&stack) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_empty_initial(void) {
    AtlasStack *stack = atlas_stack_create(sizeof(int));

    if (!stack) {
        return 1;
    }

    bool empty = false;

    if (atlas_stack_empty(stack, &empty) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (!empty) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_destroy(&stack) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_empty_non_empty(void) {
    AtlasStack *stack = atlas_stack_create(sizeof(int));

    if (!stack) {
        return 1;
    }

    int value = 1969;

    if (atlas_stack_push(stack, &value) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    bool empty = true;

    if (atlas_stack_empty(stack, &empty) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (empty) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_destroy(&stack) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_empty_after_pop(void) {
    AtlasStack *stack = atlas_stack_create(sizeof(int));

    if (!stack) {
        return 1;
    }

    int value = 1941;
    int out_value = 0;

    if (atlas_stack_push(stack, &value) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_pop(stack, &out_value) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    bool empty = false;

    if (atlas_stack_empty(stack, &empty) != ATLAS_SUCCESS || !empty) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_destroy(&stack) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_empty_null(void) {
    AtlasStack *stack = atlas_stack_create(sizeof(int));

    if (!stack) {
        return 1;
    }

    bool empty = false;

    if (atlas_stack_empty(NULL, &empty) != ATLAS_ERROR_NULL) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_empty(stack, NULL) != ATLAS_ERROR_NULL) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_empty(NULL, NULL) != ATLAS_ERROR_NULL) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_destroy(&stack) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_clear_multiple(void) {
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

    if (atlas_stack_clear(stack) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    size_t size = 0;

    if (atlas_stack_size(stack, &size) != ATLAS_SUCCESS || size != 0) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    bool empty = false;

    if (atlas_stack_empty(stack, &empty) != ATLAS_SUCCESS || !empty) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_destroy(&stack) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_clear_empty(void) {
    AtlasStack *stack = atlas_stack_create(sizeof(int));

    if (!stack) {
        return 1;
    }

    if (atlas_stack_clear(stack) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_destroy(&stack) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_clear_reuse(void) {
    AtlasStack *stack = atlas_stack_create(sizeof(int));

    if (!stack) {
        return 1;
    }

    int first = 1969;
    int second = 1941;
    int out_value = 0;

    if (atlas_stack_push(stack, &first) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_push(stack, &second) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_clear(stack) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_push(stack, &first) != ATLAS_SUCCESS) {
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

static int test_clear_null(void) {
    if (atlas_stack_clear(NULL) != ATLAS_ERROR_NULL) {
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
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL pop validation\n\n");

    // =========================================================
    // Access
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Access" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_top_single()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Top single element\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Top single element\n");

    if (test_top_multiple()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Top multiple elements\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Top multiple elements\n");

    if (test_top_preserves_element()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Top preserves element\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Top preserves element\n");

    if (test_top_empty()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Top from empty stack\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Top from empty stack\n");

    if (test_top_null()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL top validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL top validation\n\n");

    // =========================================================
    // State
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ State" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_size_empty()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Size of empty stack\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Size of empty stack\n");

    if (test_size_multiple()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Size with multiple elements\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Size with multiple elements\n");

    if (test_size_null()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL size validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL size validation\n");

    if (test_empty_initial()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Empty initial state\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Empty initial state\n");

    if (test_empty_non_empty()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Empty non-empty stack\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Empty non-empty stack\n");

    if (test_empty_after_pop()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Empty after pop\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Empty after pop\n");

    if (test_empty_null()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL empty validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL empty validation\n\n");

    // =========================================================
    // Maintenance
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Maintenance" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_clear_multiple()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Clear multiple elements\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Clear multiple elements\n");

    if (test_clear_empty()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Clear empty stack\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Clear empty stack\n");

    if (test_clear_reuse()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Reuse after clear\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Reuse after clear\n");

    if (test_clear_null()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL clear validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL clear validation\n\n");

    printf(COLOR_BOLD_CYAN "════════════════════════════════════════════════════════\n" COLOR_RESET "\n");
    printf(COLOR_BOLD_GREEN " ✔ SUCCESS:" COLOR_RESET " All tests were completed successfully.\n\n");

    return 0;
}