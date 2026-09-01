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
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL destroy validation\n");

    printf(COLOR_BOLD_CYAN "\n════════════════════════════════════════════════════════\n" COLOR_RESET "\n");
    printf(COLOR_BOLD_GREEN " ✔ SUCCESS:" COLOR_RESET " All tests were completed successfully.\n\n");

    return 0;
}