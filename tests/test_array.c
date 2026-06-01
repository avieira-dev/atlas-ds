#include "atlas/array.h"

#include <stdio.h>

static void test_fail(AtlasArray **arr, const char *message) {

    printf("\033[0;31m[ERROR]\033[0m %s\n", message);
    atlas_array_destroy(arr);
}

int main(void) {

    printf("\n\033[0;33m[INFO]\033[0m Starting AtlasDS dynamic array tests...\n");

    // =========================================================
    // Creation Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running creation tests...\n");

    size_t initial_capacity = 3;

    AtlasArray *arr = atlas_array_create(initial_capacity);

    if (!arr) {
        printf("\033[0;31m[ERROR]\033[0m Failed to create dynamic array.\n");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Dynamic array created successfully.\n");

    if (atlas_array_size(arr) == 0) {
        printf("\033[0;32m[OK]\033[0m Initial size is correct (0).\n");
    } else {
        test_fail(&arr, "Initial size is incorrect.");
        return 1;
    }

    if (atlas_array_capacity(arr) == initial_capacity) {
        printf("\033[0;32m[OK]\033[0m Initial capacity is correct (%zu).\n", initial_capacity);
    } else {
        test_fail(&arr, "Initial capacity is incorrect.");
        return 1;
    }

    // =========================================================
    // Push and Resize Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running push and resize tests...\n");

    int values[] = {5, 10, 15, 20};
    size_t total_values = sizeof(values) / sizeof(values[0]);

    for (size_t i = 0; i < total_values; i++) {
        if (atlas_array_push(arr, values[i]) != 0) {
            test_fail(&arr, "Failed to insert value into dynamic array.");
            return 1;
        }

        printf("\033[0;32m[OK]\033[0m Inserted value at index %zu: %d\n", i, values[i]);
    }

    printf("\033[0;32m[OK]\033[0m Automatic resizing completed successfully.\n");

    // =========================================================
    // Set Validation Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running set tests...\n");
    
    int new_value = 25;
    size_t last_index = total_values - 1;

    if (atlas_array_set(arr, last_index, new_value) != 0) {
        test_fail(&arr, "Error updating value at index.");
        return 1;
    }

    int old_value = values[last_index];
    values[last_index] = new_value;

    int updated_value = 0;

    if (atlas_array_get(arr, last_index, &updated_value) != 0) {
        test_fail(&arr, "Failed to retrieve updated value.");
        return 1;
    }

    if (updated_value != new_value) {
        test_fail(&arr, "Updated value does not match expected value.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Element updated successfully at index %zu: %d -> %d\n", last_index, old_value, updated_value);

    // =========================================================
    // Size Validation Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running size validation tests...\n");

    size_t current_size = atlas_array_size(arr);

    if (current_size == total_values) {
        printf("\033[0;32m[OK]\033[0m Size validation passed (%zu elements).\n", current_size);
    } else {
        test_fail(&arr, "Size validation failed.");
        return 1;
    }

    // =========================================================
    // Capacity Validation Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running capacity validation tests...\n");

    size_t expected_capacity = initial_capacity * 2;
    size_t current_capacity = atlas_array_capacity(arr);

    if (current_capacity == expected_capacity) {
        printf("\033[0;32m[OK]\033[0m Capacity validation passed (%zu).\n", current_capacity);
    } else {
        test_fail(&arr, "Capacity validation failed.");
        return 1;
    }

    // =========================================================
    // Reserve Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running reserve tests...\n");

    size_t before_capacity = atlas_array_capacity(arr);
    size_t requested_capacity = before_capacity + 10;

    if (atlas_array_reserve(arr, requested_capacity) != 0) {
        test_fail(&arr, "reserve failed to increase capacity.");
        return 1;
    }

    size_t after_capacity = atlas_array_capacity(arr);

    if (after_capacity < requested_capacity) {
        test_fail(&arr, "reserve did not reach required capacity.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Reserve increased capacity successfully (%zu -> %zu).\n", before_capacity, after_capacity);

    size_t stable_capacity = after_capacity;

    if (atlas_array_reserve(arr, stable_capacity - 1) != 0) {
        test_fail(&arr, "reserve failed on no-op case.");
        return 1;
    }

    if (atlas_array_capacity(arr) != stable_capacity) {
        test_fail(&arr, "reserve changed capacity incorrectly on no-op call.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Reserve idempotency verified successfully.\n");

    // =========================================================
    // Element Retrieval Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running element retrieval tests...\n");

    for (size_t i = 0; i < total_values; i++) {
        int retrieved_value = 0;

        if (atlas_array_get(arr, i, &retrieved_value) != 0) {
            test_fail(&arr, "Failed to retrieve element.");
            return 1;
        }

        if (retrieved_value != values[i]) {
            test_fail(&arr, "Retrieved value does not match expected value.");
            return 1;
        }

        printf("\033[0;32m[OK]\033[0m Retrieved index %zu successfully: %d\n", i, retrieved_value);
    }

    // =========================================================
    // Clear Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running clear tests...\n");

    size_t capacity_before_clear = atlas_array_capacity(arr);

    if (atlas_array_clear(arr) != 0) {
        test_fail(&arr, "Failed to clear dynamic array.");
        return 1;
    }

    if (atlas_array_size(arr) != 0) {
        test_fail(&arr, "clear did not reset size to zero.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Clear reset size successfully.\n");

    if (atlas_array_capacity(arr) != capacity_before_clear) {
        test_fail(&arr, "clear modified array capacity.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Clear preserved allocated capacity (%zu).\n", capacity_before_clear);

    if (atlas_array_clear(arr) != 0) {
        test_fail(&arr, "clear failed on already empty array.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Clear idempotency validated successfully.\n");

    // =========================================================
    // Empty Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running empty tests...\n");

    bool is_empty = false;

    if (atlas_array_empty(arr, &is_empty) != 0) {
        test_fail(&arr, "Failed to check empty state.");
        return 1;
    }

    if (!is_empty) {
        test_fail(&arr, "Array should be empty after clear.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Empty array detected correctly.\n");

    if (atlas_array_push(arr, 99) != 0) {
        test_fail(&arr, "Failed to insert element for empty validation.");
        return 1;
    }

    if (atlas_array_empty(arr, &is_empty) != 0) {
        test_fail(&arr, "Failed to check non-empty state.");
        return 1;
    }

    if (is_empty) {
        test_fail(&arr, "Array incorrectly reported as empty.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Non-empty array detected correctly.\n");

    // Validate buffer reuse after clear
    int reused_value = 0;

    if (atlas_array_get(arr, 0, &reused_value) != 0 || reused_value != 99) {
        test_fail(&arr, "Array reuse after clear failed.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Array reuse after clear validated successfully.\n");
    
    // =========================================================
    // Pop Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running pop operation tests...\n");

    int removed_value = 0;

    if (atlas_array_pop(arr, &removed_value) != 0) {
        test_fail(&arr, "Failed to pop last element.");
        return 1;
    }

    if (removed_value != 99) {
        test_fail(&arr, "Pop returned an unexpected value.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Pop operation returned correct value: %d\n", removed_value);

    if (atlas_array_size(arr) == 0) {
        printf("\033[0;32m[OK]\033[0m Size updated correctly after pop (%zu).\n", atlas_array_size(arr));
    } else {
        test_fail(&arr, "Size was not updated correctly after pop.");
        return 1;
    }

    // =========================================================
    // Bounds Checking Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running bounds checking tests...\n");

    int dummy = 0;

    if (atlas_array_get(arr, total_values, &dummy) == -1) {
        printf("\033[0;32m[OK]\033[0m Out-of-bounds access correctly rejected.\n");
    } else {
        test_fail(&arr, "Out-of-bounds access was incorrectly allowed.");
        return 1;
    }

    // =========================================================
    // Empty Pop Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running empty pop tests...\n");

    while (atlas_array_size(arr) > 0) {
        atlas_array_pop(arr, &dummy);
    }

    if (atlas_array_pop(arr, &dummy) == -1) {
        printf("\033[0;32m[OK]\033[0m Empty array pop correctly rejected.\n");
    } else {
        test_fail(&arr, "Empty array pop was incorrectly allowed.");
        return 1;
    }

    // =========================================================
    // NULL Safety Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running NULL safety tests...\n");

    if (atlas_array_size(NULL) == 0) {
        printf("\033[0;32m[OK]\033[0m atlas_array_size(NULL) returned 0.\n");
    } else {
        test_fail(&arr, "atlas_array_size(NULL) failed.");
        return 1;
    }

    if (atlas_array_capacity(NULL) == 0) {
        printf("\033[0;32m[OK]\033[0m atlas_array_capacity(NULL) returned 0.\n");
    } else {
        test_fail(&arr, "atlas_array_capacity(NULL) failed.");
        return 1;
    }

    if (atlas_array_get(NULL, 0, &dummy) == -1) {
        printf("\033[0;32m[OK]\033[0m atlas_array_get(NULL, ...) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_get(NULL, ...) failed.");
        return 1;
    }

    if (atlas_array_get(arr, 0, NULL) == -1) {
        printf("\033[0;32m[OK]\033[0m atlas_array_get(..., NULL) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_get(..., NULL) failed.");
        return 1;
    }

    if (atlas_array_push(NULL, 10) == -1) {
        printf("\033[0;32m[OK]\033[0m atlas_array_push(NULL, ...) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_push(NULL, ...) failed.");
        return 1;
    }

    if (atlas_array_pop(NULL, &dummy) == -1) {
        printf("\033[0;32m[OK]\033[0m atlas_array_pop(NULL, ...) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_pop(NULL, ...) failed.");
        return 1;
    }

    if (atlas_array_pop(arr, NULL) == -1) {
        printf("\033[0;32m[OK]\033[0m atlas_array_pop(..., NULL) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_pop(..., NULL) failed.");
        return 1;
    }

    if (atlas_array_clear(NULL) == -1) {
        printf("\033[0;32m[OK]\033[0m atlas_array_clear(NULL) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_clear(NULL) failed.");
        return 1;
    }

    bool empty_state = false;

    if (atlas_array_empty(NULL, &empty_state) == -1) {
        printf("\033[0;32m[OK]\033[0m atlas_array_empty(NULL, ...) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_empty(NULL, ...) failed.");
        return 1;
    }

    if (atlas_array_empty(arr, NULL) == -1) {
        printf("\033[0;32m[OK]\033[0m atlas_array_empty(..., NULL) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_empty(..., NULL) failed.");
        return 1;
    }

    // =========================================================
    // Cleanup Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Cleaning up resources...\n");

    atlas_array_destroy(&arr);

    if (arr == NULL) {
        printf("\033[0;32m[OK]\033[0m Dynamic array destroyed successfully.\n");
    } else {
        printf("\033[0;31m[ERROR]\033[0m Failed to destroy dynamic array.\n");
        return 1;
    }

    printf("\n\033[0;33m[INFO]\033[0m All tests completed successfully.\n\n");

    return 0;
}