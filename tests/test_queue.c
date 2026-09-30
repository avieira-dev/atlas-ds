/*
 * AtlasDS
 * Queue Tests
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#include <stdio.h>

#include "atlas/queue.h"
#include "atlas/status.h"
#include "atlas/terminal.h"

static int test_create_destroy(void) {
    AtlasQueue *queue = atlas_queue_create(sizeof(int));
    if (!queue) {
        return 1;
    }

    if (atlas_queue_destroy(&queue) != ATLAS_SUCCESS) {
        return 1;
    }

    if (queue != NULL) {
        return 1;
    }

    return 0;
}

static int test_create_invalid_type_size(void) {
    AtlasQueue *queue = atlas_queue_create(0);
    if (queue) {
        return 1;
    }

    return 0;
}

static int test_destroy_null(void) {
    AtlasQueue *queue = NULL;

    if (atlas_queue_destroy(&queue) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_queue_destroy(NULL) != ATLAS_ERROR_NULL) {
        return 1;
    }

    return 0;
}

static int test_enqueue_empty(void) {
    AtlasQueue *queue = atlas_queue_create(sizeof(int));
    if (!queue) {
        return 1;
    }

    int value = 1969;

    if (atlas_queue_enqueue(queue, &value) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_destroy(&queue) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_enqueue_multiple(void) {
    AtlasQueue *queue = atlas_queue_create(sizeof(int));
    if (!queue) {
        return 1;
    }

    int first = 19;
    int second = 47;
    int third = 54;

    if (atlas_queue_enqueue(queue, &first) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_enqueue(queue, &second) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_enqueue(queue, &third) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_destroy(&queue) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_enqueue_null(void) {
    AtlasQueue *queue = atlas_queue_create(sizeof(int));
    if (!queue) {
        return 1;
    }

    int value = 1941;

    if (atlas_queue_enqueue(NULL, &value) != ATLAS_ERROR_NULL) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_enqueue(queue, NULL) != ATLAS_ERROR_NULL) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_destroy(&queue) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_dequeue_single(void) {
    AtlasQueue *queue = atlas_queue_create(sizeof(int));
    if (!queue) {
        return 1;
    }

    int value = 1969;

    if (atlas_queue_enqueue(queue, &value) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    int out_value = 0;

    if (atlas_queue_dequeue(queue, &out_value) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (out_value != value) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_destroy(&queue) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_dequeue_multiple(void) {
    AtlasQueue *queue = atlas_queue_create(sizeof(int));
    if (!queue) {
        return 1;
    }

    int first = 19;
    int second = 47;
    int third = 54;

    if (atlas_queue_enqueue(queue, &first) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_enqueue(queue, &second) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_enqueue(queue, &third) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    int out_value = 0;

    if (atlas_queue_dequeue(queue, &out_value) != ATLAS_SUCCESS || out_value != first) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_dequeue(queue, &out_value) != ATLAS_SUCCESS || out_value != second) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_dequeue(queue, &out_value) != ATLAS_SUCCESS || out_value != third) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_destroy(&queue) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_dequeue_empty(void) {
    AtlasQueue *queue = atlas_queue_create(sizeof(int));
    if (!queue) {
        return 1;
    }

    int out_value = 0;

    if (atlas_queue_dequeue(queue, &out_value) != ATLAS_ERROR_EMPTY) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_destroy(&queue) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_dequeue_null(void) {
    AtlasQueue *queue = atlas_queue_create(sizeof(int));
    if (!queue) {
        return 1;
    }

    int value = 1941;
    int out_value = 0;

    if (atlas_queue_dequeue(NULL, &out_value) != ATLAS_ERROR_NULL) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_dequeue(queue, NULL) != ATLAS_ERROR_NULL) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_enqueue(queue, &value) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_dequeue(NULL, NULL) != ATLAS_ERROR_NULL) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_destroy(&queue) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_dequeue_fifo(void) {
    AtlasQueue *queue = atlas_queue_create(sizeof(int));
    if (!queue) {
        return 1;
    }

    int first = 10;
    int second = 20;
    int third = 30;

    if (atlas_queue_enqueue(queue, &first) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_enqueue(queue, &second) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_enqueue(queue, &third) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    int out_value = 0;

    if (atlas_queue_dequeue(queue, &out_value) != ATLAS_SUCCESS || out_value != first) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_dequeue(queue, &out_value) != ATLAS_SUCCESS || out_value != second) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_dequeue(queue, &out_value) != ATLAS_SUCCESS || out_value != third) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_destroy(&queue) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_dequeue_last_reuse(void) {
    AtlasQueue *queue = atlas_queue_create(sizeof(int));
    if (!queue) {
        return 1;
    }

    int first = 100;
    int second = 200;
    int third = 300;
    int out_value = 0;

    if (atlas_queue_enqueue(queue, &first) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_enqueue(queue, &second) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_dequeue(queue, &out_value) != ATLAS_SUCCESS || out_value != first) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_dequeue(queue, &out_value) != ATLAS_SUCCESS || out_value != second) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_enqueue(queue, &third) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_dequeue(queue, &out_value) != ATLAS_SUCCESS || out_value != third) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_destroy(&queue) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_front(void) {
    AtlasQueue *queue = atlas_queue_create(sizeof(int));
    if (!queue) {
        return 1;
    }

    int first = 10;
    int second = 20;
    int third = 30;
    int out_value = 0;

    if (atlas_queue_enqueue(queue, &first) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_enqueue(queue, &second) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_enqueue(queue, &third) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_front(queue, &out_value) != ATLAS_SUCCESS || out_value != first) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_dequeue(queue, &out_value) != ATLAS_SUCCESS || out_value != first) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_front(queue, &out_value) != ATLAS_SUCCESS || out_value != second) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_destroy(&queue) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_front_empty(void) {
    AtlasQueue *queue = atlas_queue_create(sizeof(int));
    if (!queue) {
        return 1;
    }

    int out_value = 0;

    if (atlas_queue_front(queue, &out_value) != ATLAS_ERROR_EMPTY) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_destroy(&queue) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_front_null(void) {
    AtlasQueue *queue = atlas_queue_create(sizeof(int));
    if (!queue) {
        return 1;
    }

    int value = 10;
    int out_value = 0;

    if (atlas_queue_front(NULL, &out_value) != ATLAS_ERROR_NULL) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_front(queue, NULL) != ATLAS_ERROR_NULL) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_front(NULL, NULL) != ATLAS_ERROR_NULL) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_enqueue(queue, &value) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_front(queue, NULL) != ATLAS_ERROR_NULL) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_destroy(&queue) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_back(void) {
    AtlasQueue *queue = atlas_queue_create(sizeof(int));
    if (!queue) {
        return 1;
    }

    int first = 10;
    int second = 20;
    int third = 30;
    int out_value = 0;

    if (atlas_queue_enqueue(queue, &first) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_enqueue(queue, &second) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_enqueue(queue, &third) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_back(queue, &out_value) != ATLAS_SUCCESS || out_value != third) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_dequeue(queue, &out_value) != ATLAS_SUCCESS || out_value != first) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_back(queue, &out_value) != ATLAS_SUCCESS || out_value != third) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_destroy(&queue) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_back_empty(void) {
    AtlasQueue *queue = atlas_queue_create(sizeof(int));
    if (!queue) {
        return 1;
    }

    int out_value = 0;

    if (atlas_queue_back(queue, &out_value) != ATLAS_ERROR_EMPTY) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_destroy(&queue) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_back_null(void) {
    AtlasQueue *queue = atlas_queue_create(sizeof(int));
    if (!queue) {
        return 1;
    }

    int value = 10;
    int out_value = 0;

    if (atlas_queue_back(NULL, &out_value) != ATLAS_ERROR_NULL) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_back(queue, NULL) != ATLAS_ERROR_NULL) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_back(NULL, NULL) != ATLAS_ERROR_NULL) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_enqueue(queue, &value) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_back(queue, NULL) != ATLAS_ERROR_NULL) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_destroy(&queue) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_size(void) {
    AtlasQueue *queue = atlas_queue_create(sizeof(int));
    if (!queue) {
        return 1;
    }

    size_t size = 0;

    if (atlas_queue_size(queue, &size) != ATLAS_SUCCESS || size != 0) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    int first = 10;
    int second = 20;

    if (atlas_queue_enqueue(queue, &first) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_size(queue, &size) != ATLAS_SUCCESS || size != 1) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_enqueue(queue, &second) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_size(queue, &size) != ATLAS_SUCCESS || size != 2) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    int out_value = 0;

    if (atlas_queue_dequeue(queue, &out_value) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_size(queue, &size) != ATLAS_SUCCESS || size != 1) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_dequeue(queue, &out_value) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_size(queue, &size) != ATLAS_SUCCESS || size != 0) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_destroy(&queue) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_size_null(void) {
    AtlasQueue *queue = atlas_queue_create(sizeof(int));
    if (!queue) {
        return 1;
    }

    size_t size = 0;

    if (atlas_queue_size(NULL, &size) != ATLAS_ERROR_NULL) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_size(queue, NULL) != ATLAS_ERROR_NULL) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_size(NULL, NULL) != ATLAS_ERROR_NULL) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_destroy(&queue) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_empty(void) {
    AtlasQueue *queue = atlas_queue_create(sizeof(int));
    if (!queue) {
        return 1;
    }

    bool empty = false;

    if (atlas_queue_empty(queue, &empty) != ATLAS_SUCCESS || !empty) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    int first = 10;

    if (atlas_queue_enqueue(queue, &first) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_empty(queue, &empty) != ATLAS_SUCCESS || empty) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    int out_value = 0;

    if (atlas_queue_dequeue(queue, &out_value) != ATLAS_SUCCESS) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_empty(queue, &empty) != ATLAS_SUCCESS || !empty) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_destroy(&queue) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_empty_null(void) {
    AtlasQueue *queue = atlas_queue_create(sizeof(int));
    if (!queue) {
        return 1;
    }

    bool empty = false;

    if (atlas_queue_empty(NULL, &empty) != ATLAS_ERROR_NULL) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_empty(queue, NULL) != ATLAS_ERROR_NULL) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_empty(NULL, NULL) != ATLAS_ERROR_NULL) {
        atlas_queue_destroy(&queue);
        return 1;
    }

    if (atlas_queue_destroy(&queue) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

int main(void) {
    printf("\n" COLOR_BOLD_BLUE "╭────────────────────────────────────────────────────────╮" COLOR_RESET "\n");
    printf(COLOR_BOLD_BLUE "│" COLOR_RESET "                 AtlasDS - Queue Tests                  " COLOR_BOLD_BLUE "│" COLOR_RESET "\n");
    printf(COLOR_BOLD_BLUE "╰────────────────────────────────────────────────────────╯" COLOR_RESET "\n\n");

    printf(COLOR_YELLOW "ℹ " COLOR_RESET "Starting AtlasDS queue tests...\n\n");

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

    if (test_enqueue_empty()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Enqueue into empty queue\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Enqueue into empty queue\n");

    if (test_enqueue_multiple()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Enqueue multiple elements\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Enqueue multiple elements\n");

    if (test_enqueue_null()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL enqueue validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL enqueue validation\n\n");

    // =========================================================
    // Removal
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Removal" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_dequeue_single()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Dequeue single element\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Dequeue single element\n");

    if (test_dequeue_multiple()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Dequeue multiple elements\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Dequeue multiple elements\n");

    if (test_dequeue_empty()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Dequeue from empty queue\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Dequeue from empty queue\n");

    if (test_dequeue_null()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL dequeue validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL dequeue validation\n");

    if (test_dequeue_fifo()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " FIFO order validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " FIFO order validation\n");

    if (test_dequeue_last_reuse()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Queue reuse after removing last element\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Queue reuse after removing last element\n\n");

    // =========================================================
    // Access
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Access" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_front()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Front element access\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Front element access\n");

    if (test_front_empty()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Front access on empty queue\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Front access on empty queue\n");

    if (test_front_null()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL front validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL front validation\n");

    if (test_back()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Back element access\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Back element access\n");

    if (test_back_empty()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Back access on empty queue\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Back access on empty queue\n");

    if (test_back_null()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL back validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL back validation\n\n");

    // =========================================================
    // State
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ State" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_size()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Queue size tracking\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Queue size tracking\n");

    if (test_size_null()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL size validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL size validation\n");

    if (test_empty()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Empty state validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Empty state validation\n");

    if (test_empty_null()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " NULL empty validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " NULL empty validation\n\n");

    printf(COLOR_BOLD_CYAN "════════════════════════════════════════════════════════\n" COLOR_RESET "\n");
    printf(COLOR_BOLD_GREEN " ✔ SUCCESS:" COLOR_RESET " All tests were completed successfully.\n\n");

    return 0;
}