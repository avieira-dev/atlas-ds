/*
 * AtlasDS
 * Dynamic Array Tests
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#include "atlas/array.h"
#include "atlas/status.h"

#include <stdio.h>
#include <stdbool.h>

static void test_fail(AtlasArray **arr, const char *message) {
    printf("\033[0;31m[ERROR]\033[0m %s\n", message);
    atlas_array_destroy(arr);
}

int main(void) {
    printf("\n");
    printf("\033[1;33m=========================================================\033[0m\n");
    printf("\033[1;33mAtlasDS - Dynamic Array Tests\033[0m\n");
    printf("\033[1;33m=========================================================\033[0m\n\n");

    printf("\033[0;33m[INFO]\033[0m Starting AtlasDS dynamic array tests...\n");

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
        if (atlas_array_push(arr, values[i]) != ATLAS_SUCCESS) {
            test_fail(&arr, "Failed to insert value into dynamic array.");
            return 1;
        }
        printf("\033[0;32m[OK]\033[0m Inserted value at index %zu: %d\n", i, values[i]);
    }

    printf("\033[0;32m[OK]\033[0m Automatic resizing completed successfully.\n");

    // =========================================================
    // Insert Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running insert tests...\n");

    AtlasArray *insert_arr = atlas_array_create(4);
    if (!insert_arr) {
        test_fail(&arr, "Failed to create insert test array.");
        return 1;
    }

    if (atlas_array_push(insert_arr, 10) != ATLAS_SUCCESS || 
        atlas_array_push(insert_arr, 20) != ATLAS_SUCCESS || 
        atlas_array_push(insert_arr, 30) != ATLAS_SUCCESS || 
        atlas_array_push(insert_arr, 40) != ATLAS_SUCCESS) {
        atlas_array_destroy(&insert_arr);
        test_fail(&arr, "Failed to prepare insert test array.");
        return 1;
    }

    if (atlas_array_insert(insert_arr, 2, 99) != ATLAS_SUCCESS) {
        atlas_array_destroy(&insert_arr);
        test_fail(&arr, "Failed to insert element.");
        return 1;
    }

    int expected[] = {10, 20, 99, 30, 40};
    for (size_t i = 0; i < 5; i++) {
        int value = 0;
        if (atlas_array_get(insert_arr, i, &value) != ATLAS_SUCCESS) {
            atlas_array_destroy(&insert_arr);
            test_fail(&arr, "Failed to validate insert operation.");
            return 1;
        }

        if (value != expected[i]) {
            atlas_array_destroy(&insert_arr);
            test_fail(&arr, "Insert operation produced incorrect ordering.");
            return 1;
        }
    }

    if (atlas_array_size(insert_arr) != 5) {
        atlas_array_destroy(&insert_arr);
        test_fail(&arr, "Insert did not update size correctly.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Insert operation validated successfully.\n");
    atlas_array_destroy(&insert_arr);

    // Insert at beginning
    AtlasArray *insert_begin = atlas_array_create(4);
    if (!insert_begin) {
        test_fail(&arr, "Failed to create beginning insert test array.");
        return 1;
    }

    atlas_array_push(insert_begin, 10);
    atlas_array_push(insert_begin, 20);
    atlas_array_push(insert_begin, 30);

    if (atlas_array_insert(insert_begin, 0, 99) != ATLAS_SUCCESS) {
        atlas_array_destroy(&insert_begin);
        test_fail(&arr, "Insert at beginning failed.");
        return 1;
    }

    int expected_begin[] = {99, 10, 20, 30};
    for (size_t i = 0; i < 4; i++) {
        int value = 0;
        if (atlas_array_get(insert_begin, i, &value) != ATLAS_SUCCESS || value != expected_begin[i]) {
            atlas_array_destroy(&insert_begin);
            test_fail(&arr, "Insert at beginning produced incorrect ordering.");
            return 1;
        }
    }

    printf("\033[0;32m[OK]\033[0m Insert at beginning validated successfully.\n");
    atlas_array_destroy(&insert_begin);

    // Insert at end
    AtlasArray *insert_end = atlas_array_create(4);
    if (!insert_end) {
        test_fail(&arr, "Failed to create end insert test array.");
        return 1;
    }

    atlas_array_push(insert_end, 10);
    atlas_array_push(insert_end, 20);
    atlas_array_push(insert_end, 30);

    if (atlas_array_insert(insert_end, atlas_array_size(insert_end), 99) != ATLAS_SUCCESS) {
        atlas_array_destroy(&insert_end);
        test_fail(&arr, "Insert at end failed.");
        return 1;
    }

    int expected_end[] = {10, 20, 30, 99};
    for (size_t i = 0; i < 4; i++) {
        int value = 0;
        if (atlas_array_get(insert_end, i, &value) != ATLAS_SUCCESS || value != expected_end[i]) {
            atlas_array_destroy(&insert_end);
            test_fail(&arr, "Insert at end produced incorrect ordering.");
            return 1;
        }
    }

    printf("\033[0;32m[OK]\033[0m Insert at end validated successfully.\n");
    atlas_array_destroy(&insert_end);

    // =========================================================
    // Erase Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running erase tests...\n");

    // Erase in middle
    AtlasArray *erase_middle = atlas_array_create(5);
    if (!erase_middle) {
        test_fail(&arr, "Failed to create middle erase test array.");
        return 1;
    }

    atlas_array_push(erase_middle, 10);
    atlas_array_push(erase_middle, 20);
    atlas_array_push(erase_middle, 30);
    atlas_array_push(erase_middle, 40);
    atlas_array_push(erase_middle, 50);

    if (atlas_array_erase(erase_middle, 2) != ATLAS_SUCCESS) {
        atlas_array_destroy(&erase_middle);
        test_fail(&arr, "Erase in middle failed.");
        return 1;
    }

    int expected_middle[] = {10, 20, 40, 50};
    for (size_t i = 0; i < 4; i++) {
        int value = 0;
        if (atlas_array_get(erase_middle, i, &value) != ATLAS_SUCCESS || value != expected_middle[i]) {
            atlas_array_destroy(&erase_middle);
            test_fail(&arr, "Erase in middle produced incorrect ordering.");
            return 1;
        }
    }

    if (atlas_array_size(erase_middle) != 4) {
        atlas_array_destroy(&erase_middle);
        test_fail(&arr, "Erase in middle did not update size correctly.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Erase in middle validated successfully.\n");
    atlas_array_destroy(&erase_middle);

    // Erase at beginning
    AtlasArray *erase_begin = atlas_array_create(3);
    if (!erase_begin) {
        test_fail(&arr, "Failed to create beginning erase test array.");
        return 1;
    }

    atlas_array_push(erase_begin, 10);
    atlas_array_push(erase_begin, 20);
    atlas_array_push(erase_begin, 30);

    if (atlas_array_erase(erase_begin, 0) != ATLAS_SUCCESS) {
        atlas_array_destroy(&erase_begin);
        test_fail(&arr, "Erase at beginning failed.");
        return 1;
    }

    int expected_begin_erase[] = {20, 30};
    for (size_t i = 0; i < 2; i++) {
        int value = 0;
        if (atlas_array_get(erase_begin, i, &value) != ATLAS_SUCCESS || value != expected_begin_erase[i]) {
            atlas_array_destroy(&erase_begin);
            test_fail(&arr, "Erase at beginning produced incorrect ordering.");
            return 1;
        }
    }

    printf("\033[0;32m[OK]\033[0m Erase at beginning validated successfully.\n");
    atlas_array_destroy(&erase_begin);

    // Erase at end
    AtlasArray *erase_end = atlas_array_create(3);
    if (!erase_end) {
        test_fail(&arr, "Failed to create end erase test array.");
        return 1;
    }

    atlas_array_push(erase_end, 10);
    atlas_array_push(erase_end, 20);
    atlas_array_push(erase_end, 30);

    if (atlas_array_erase(erase_end, 2) != ATLAS_SUCCESS) {
        atlas_array_destroy(&erase_end);
        test_fail(&arr, "Erase at end failed.");
        return 1;
    }

    int expected_end_erase[] = {10, 20};
    for (size_t i = 0; i < 2; i++) {
        int value = 0;
        if (atlas_array_get(erase_end, i, &value) != ATLAS_SUCCESS || value != expected_end_erase[i]) {
            atlas_array_destroy(&erase_end);
            test_fail(&arr, "Erase at end produced incorrect ordering.");
            return 1;
        }
    }

    printf("\033[0;32m[OK]\033[0m Erase at end validated successfully.\n");
    atlas_array_destroy(&erase_end);

    // Erase only element
    AtlasArray *erase_single = atlas_array_create(1);
    if (!erase_single) {
        test_fail(&arr, "Failed to create single element erase test array.");
        return 1;
    }

    atlas_array_push(erase_single, 10);

    if (atlas_array_erase(erase_single, 0) != ATLAS_SUCCESS) {
        atlas_array_destroy(&erase_single);
        test_fail(&arr, "Erase single element failed.");
        return 1;
    }

    if (atlas_array_size(erase_single) != 0) {
        atlas_array_destroy(&erase_single);
        test_fail(&arr, "Erase single element did not empty array.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Erase single element validated successfully.\n");
    atlas_array_destroy(&erase_single);

    // =========================================================
    // Find Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running find tests...\n");

    AtlasArray *find_arr = atlas_array_create(5);
    if (!find_arr) {
        test_fail(&arr, "Failed to create find test array.");
        return 1;
    }

    atlas_array_push(find_arr, 10);
    atlas_array_push(find_arr, 20);
    atlas_array_push(find_arr, 30);
    atlas_array_push(find_arr, 20);
    atlas_array_push(find_arr, 40);

    size_t found_index = 999;
    if (atlas_array_find(find_arr, &found_index, 20) != ATLAS_SUCCESS) {
        atlas_array_destroy(&find_arr);
        test_fail(&arr, "Failed to find existing value.");
        return 1;
    }

    if (found_index != 1) {
        atlas_array_destroy(&find_arr);
        test_fail(&arr, "find returned incorrect index.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m First occurrence found successfully at index %zu.\n", found_index);

    size_t unchanged_index = 777;
    if (atlas_array_find(find_arr, &unchanged_index, 99) != ATLAS_ERROR_NOT_FOUND) {
        atlas_array_destroy(&find_arr);
        test_fail(&arr, "find incorrectly reported a missing value or wrong error code.");
        return 1;
    }

    if (unchanged_index != 777) {
        atlas_array_destroy(&find_arr);
        test_fail(&arr, "find modified output parameter when value was not found.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Missing value handled correctly.\n");
    atlas_array_destroy(&find_arr);

    // =========================================================
    // Contains Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running contains tests...\n");

    AtlasArray *contains_arr = atlas_array_create(5);
    if (!contains_arr) {
        test_fail(&arr, "Failed to create contains test array.");
        return 1;
    }

    atlas_array_push(contains_arr, 10);
    atlas_array_push(contains_arr, 20);
    atlas_array_push(contains_arr, 30);
    atlas_array_push(contains_arr, 20);
    atlas_array_push(contains_arr, 40);

    bool contains = false;
    if (atlas_array_contains(contains_arr, &contains, 20) != ATLAS_SUCCESS) {
        atlas_array_destroy(&contains_arr);
        test_fail(&arr, "contains failed for existing value.");
        return 1;
    }

    if (!contains) {
        atlas_array_destroy(&contains_arr);
        test_fail(&arr, "contains returned false for existing value.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Existing value detected successfully.\n");

    if (atlas_array_contains(contains_arr, &contains, 99) != ATLAS_SUCCESS) {
        atlas_array_destroy(&contains_arr);
        test_fail(&arr, "contains failed for missing value.");
        return 1;
    }

    if (contains) {
        atlas_array_destroy(&contains_arr);
        test_fail(&arr, "contains returned true for missing value.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Missing value handled correctly.\n");
    atlas_array_destroy(&contains_arr);

    // =========================================================
    // Swap Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running swap tests...\n");

    AtlasArray *swap_arr = atlas_array_create(4);
    if (!swap_arr) {
        test_fail(&arr, "Failed to create temporary array for swap tests.");
        return 1;
    }

    atlas_array_push(swap_arr, 10);
    atlas_array_push(swap_arr, 20);
    atlas_array_push(swap_arr, 30);
    atlas_array_push(swap_arr, 40);

    if (atlas_array_swap(swap_arr, 0, 3) != ATLAS_SUCCESS) {
        atlas_array_destroy(&swap_arr);
        test_fail(&arr, "swap failed on valid indices.");
        return 1;
    }

    int value = 0;
    atlas_array_get(swap_arr, 0, &value);
    if (value != 40) {
        atlas_array_destroy(&swap_arr);
        test_fail(&arr, "swap did not update first position correctly.");
        return 1;
    }

    atlas_array_get(swap_arr, 3, &value);
    if (value != 10) {
        atlas_array_destroy(&swap_arr);
        test_fail(&arr, "swap did not update second position correctly.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Element exchange validated successfully.\n");

    size_t size_before_swap = atlas_array_size(swap_arr);
    size_t capacity_before_swap = atlas_array_capacity(swap_arr);

    if (atlas_array_swap(swap_arr, 1, 1) != ATLAS_SUCCESS) {
        atlas_array_destroy(&swap_arr);
        test_fail(&arr, "swap failed when using identical indices.");
        return 1;
    }

    if (atlas_array_size(swap_arr) != size_before_swap) {
        atlas_array_destroy(&swap_arr);
        test_fail(&arr, "swap changed array size.");
        return 1;
    }

    if (atlas_array_capacity(swap_arr) != capacity_before_swap) {
        atlas_array_destroy(&swap_arr);
        test_fail(&arr, "swap changed array capacity.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Swap idempotency validated successfully.\n");
    atlas_array_destroy(&swap_arr);

    // =========================================================
    // Copy Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running copy tests...\n");

    AtlasArray *copy_src = atlas_array_create(4);
    AtlasArray *copy_dest = atlas_array_create(1);

    if (!copy_src || !copy_dest) {
        atlas_array_destroy(&copy_src);
        atlas_array_destroy(&copy_dest);
        test_fail(&arr, "Failed to create copy test arrays.");
        return 1;
    }

    atlas_array_push(copy_src, 10);
    atlas_array_push(copy_src, 20);
    atlas_array_push(copy_src, 30);
    atlas_array_push(copy_src, 40);

    if (atlas_array_copy(copy_src, copy_dest) != ATLAS_SUCCESS) {
        atlas_array_destroy(&copy_src);
        atlas_array_destroy(&copy_dest);
        test_fail(&arr, "copy failed.");
        return 1;
    }

    if (atlas_array_size(copy_dest) != 4) {
        atlas_array_destroy(&copy_src);
        atlas_array_destroy(&copy_dest);
        test_fail(&arr, "copy did not update destination size correctly.");
        return 1;
    }

    for (size_t i = 0; i < 4; i++) {
        int src_value = 0;
        int dest_value = 0;

        atlas_array_get(copy_src, i, &src_value);
        atlas_array_get(copy_dest, i, &dest_value);
        if (src_value != dest_value) {
            atlas_array_destroy(&copy_src);
            atlas_array_destroy(&copy_dest);
            test_fail(&arr, "copy produced incorrect data.");
            return 1;
        }
    }

    printf("\033[0;32m[OK]\033[0m Array copy validated successfully.\n");

    if (atlas_array_set(copy_dest, 0, 999) != ATLAS_SUCCESS) {
        atlas_array_destroy(&copy_src);
        atlas_array_destroy(&copy_dest);
        test_fail(&arr, "Failed to modify destination after copy.");
        return 1;
    }

    int original_value = 0;
    atlas_array_get(copy_src, 0, &original_value);
    if (original_value != 10) {
        atlas_array_destroy(&copy_src);
        atlas_array_destroy(&copy_dest);
        test_fail(&arr, "copy modified source array.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Source array preserved successfully.\n");

    AtlasArray *empty_src = atlas_array_create(2);
    if (!empty_src) {
        atlas_array_destroy(&copy_src);
        atlas_array_destroy(&copy_dest);
        test_fail(&arr, "Failed to create empty source array.");
        return 1;
    }

    if (atlas_array_copy(empty_src, copy_dest) != ATLAS_SUCCESS) {
        atlas_array_destroy(&copy_src);
        atlas_array_destroy(&copy_dest);
        atlas_array_destroy(&empty_src);
        test_fail(&arr, "copy from empty source failed.");
        return 1;
    }

    if (atlas_array_size(copy_dest) != 0) {
        atlas_array_destroy(&copy_src);
        atlas_array_destroy(&copy_dest);
        atlas_array_destroy(&empty_src);
        test_fail(&arr, "copy from empty source did not clear destination size.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Empty source copy validated successfully.\n");
    atlas_array_destroy(&empty_src);
    atlas_array_destroy(&copy_src);
    atlas_array_destroy(&copy_dest);

    // =========================================================
    // Clone Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running clone tests...\n");

    AtlasArray *clone_src = atlas_array_create(6);
    if (!clone_src) {
        test_fail(&arr, "Failed to create clone source array.");
        return 1;
    }

    atlas_array_push(clone_src, 10);
    atlas_array_push(clone_src, 20);
    atlas_array_push(clone_src, 30);
    atlas_array_push(clone_src, 40);

    AtlasArray *clone = atlas_array_clone(clone_src);
    if (!clone) {
        atlas_array_destroy(&clone_src);
        test_fail(&arr, "Failed to clone array.");
        return 1;
    }

    if (atlas_array_size(clone) != 4) {
        atlas_array_destroy(&clone);
        atlas_array_destroy(&clone_src);
        test_fail(&arr, "Clone size mismatch.");
        return 1;
    }

    if (atlas_array_capacity(clone) != 6) {
        atlas_array_destroy(&clone);
        atlas_array_destroy(&clone_src);
        test_fail(&arr, "Clone capacity mismatch.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Clone validated successfully.\n");

    AtlasArray *empty_clone_src = atlas_array_create(3);
    AtlasArray *empty_clone = atlas_array_clone(empty_clone_src);
    if (!empty_clone) {
        atlas_array_destroy(&empty_clone_src);
        atlas_array_destroy(&clone);
        atlas_array_destroy(&clone_src);
        test_fail(&arr, "Failed to clone empty array.");
        return 1;
    }

    if (atlas_array_size(empty_clone) != 0 || atlas_array_capacity(empty_clone) != 3) {
        atlas_array_destroy(&empty_clone);
        atlas_array_destroy(&empty_clone_src);
        atlas_array_destroy(&clone);
        atlas_array_destroy(&clone_src);
        test_fail(&arr, "Empty clone size or capacity mismatch.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Empty clone validated successfully.\n");
    atlas_array_destroy(&empty_clone);
    atlas_array_destroy(&empty_clone_src);
    atlas_array_destroy(&clone);
    atlas_array_destroy(&clone_src);

    // =========================================================
    // Set Validation Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running set tests...\n");

    int new_value = 25;
    size_t last_index = total_values - 1;

    if (atlas_array_set(arr, last_index, new_value) != ATLAS_SUCCESS) {
        test_fail(&arr, "Error updating value at index.");
        return 1;
    }

    int old_value = values[last_index];
    values[last_index] = new_value;

    int updated_value = 0;
    if (atlas_array_get(arr, last_index, &updated_value) != ATLAS_SUCCESS) {
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

    if (atlas_array_reserve(arr, requested_capacity) != ATLAS_SUCCESS) {
        test_fail(&arr, "atlas_array_reserve failed to allocate requested size.");
        return 1;
    }

    if (atlas_array_capacity(arr) == requested_capacity) {
        printf("\033[0;32m[OK]\033[0m Reserve successfully expanded capacity to %zu.\n", requested_capacity);
    } else {
        test_fail(&arr, "Capacity does not match explicitly reserved target.");
        return 1;
    }

    if (atlas_array_reserve(arr, requested_capacity - 5) != ATLAS_SUCCESS) {
        test_fail(&arr, "Idempotent reserve call failed.");
        return 1;
    }

    if (atlas_array_capacity(arr) == requested_capacity) {
        printf("\033[0;32m[OK]\033[0m Downward reserve call correctly ignored.\n");
    } else {
        test_fail(&arr, "Reserve unexpectedly shrank the dynamic capacity.");
        return 1;
    }

    // =========================================================
    // Shrink to Fit Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running shrink_to_fit tests...\n");

    if (atlas_array_shrink_to_fit(arr) != ATLAS_SUCCESS) {
        test_fail(&arr, "shrink_to_fit buffer optimization failed.");
        return 1;
    }

    if (atlas_array_capacity(arr) == atlas_array_size(arr)) {
        printf("\033[0;32m[OK]\033[0m shrink_to_fit successfully tightly packed the buffer.\n");
    } else {
        test_fail(&arr, "Capacity does not equal size after strict shrink operation.");
        return 1;
    }

    size_t capacity_before = atlas_array_capacity(arr);
    if (atlas_array_shrink_to_fit(arr) != ATLAS_SUCCESS) {
        test_fail(&arr, "shrink_to_fit idempotency failed.");
        return 1;
    }

    if (atlas_array_capacity(arr) != capacity_before) {
        test_fail(&arr, "shrink_to_fit changed capacity unnecessarily.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m shrink_to_fit idempotency validated successfully.\n");

    // =========================================================
    // Element Retrieval Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running element retrieval tests...\n");

    for (size_t i = 0; i < total_values; i++) {
        int retrieved_value = 0;
        if (atlas_array_get(arr, i, &retrieved_value) != ATLAS_SUCCESS) {
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
    // Front/Back Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running front/back tests...\n");

    int first_value = 1941;
    if (atlas_array_front(arr, &first_value) != ATLAS_SUCCESS) {
        test_fail(&arr, "Failed to retrieve first element.");
        return 1;
    }

    if (first_value != values[0]) {
        test_fail(&arr, "Front element does not match expected value.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Front element retrieved successfully: %d\n", first_value);

    int last_value = 1972;
    if (atlas_array_back(arr, &last_value) != ATLAS_SUCCESS) {
        test_fail(&arr, "Failed to retrieve last element.");
        return 1;
    }

    if (last_value != values[total_values - 1]) {
        test_fail(&arr, "Back element does not match expected value.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Back element retrieved successfully: %d\n", last_value);

    // =========================================================
    // Clear Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running clear tests...\n");

    size_t cap_before_clear = atlas_array_capacity(arr);
    if (atlas_array_clear(arr) != ATLAS_SUCCESS) {
        test_fail(&arr, "clear operation failed.");
        return 1;
    }

    if (atlas_array_size(arr) != 0) {
        test_fail(&arr, "clear operation did not reset container size to zero.");
        return 1;
    }

    if (atlas_array_capacity(arr) != cap_before_clear) {
        test_fail(&arr, "clear operation modified capacity unexpectedly.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Array buffer cleared without memory truncation.\n");

    // =========================================================
    // Empty Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running empty tests...\n");

    bool is_empty = false;
    if (atlas_array_empty(arr, &is_empty) != ATLAS_SUCCESS) {
        test_fail(&arr, "Failed to check empty state.");
        return 1;
    }

    if (!is_empty) {
        test_fail(&arr, "Array should be empty after clear.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Empty array detected correctly.\n");

    if (atlas_array_push(arr, 99) != ATLAS_SUCCESS) {
        test_fail(&arr, "Failed to insert element for empty validation.");
        return 1;
    }

    if (atlas_array_empty(arr, &is_empty) != ATLAS_SUCCESS) {
        test_fail(&arr, "Failed to check non-empty state.");
        return 1;
    }

    if (is_empty) {
        test_fail(&arr, "Array incorrectly reported as empty.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Non-empty array detected correctly.\n");

    int reused_value = 0;
    if (atlas_array_get(arr, 0, &reused_value) != ATLAS_SUCCESS || reused_value != 99) {
        test_fail(&arr, "Array reuse after clear failed.");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Array reuse after clear validated successfully.\n");

    // =========================================================
    // Pop Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running pop operation tests...\n");

    int removed_value = 0;
    if (atlas_array_pop(arr, &removed_value) != ATLAS_SUCCESS) {
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
    // Bounds Checking and Robustness Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running bounds checking tests...\n");

    int dummy = 0;
    if (atlas_array_get(arr, 999, &dummy) == ATLAS_ERROR_BOUNDS) {
        printf("\033[0;32m[OK]\033[0m Out of bounds index rejected correctly with ATLAS_ERROR_BOUNDS.\n");
    } else {
        test_fail(&arr, "Out-of-bounds protection failed.");
        return 1;
    }

    if (atlas_array_get(arr, 0, NULL) == ATLAS_ERROR_NULL) {
        printf("\033[0;32m[OK]\033[0m atlas_array_get(..., NULL) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_get(..., NULL) failed.");
        return 1;
    }

    if (atlas_array_push(NULL, 10) == ATLAS_ERROR_NULL) {
        printf("\033[0;32m[OK]\033[0m atlas_array_push(NULL, ...) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_push(NULL, ...) failed.");
        return 1;
    }

    if (atlas_array_set(arr, atlas_array_size(arr), 123) == ATLAS_ERROR_BOUNDS) {
        printf("\033[0;32m[OK]\033[0m Invalid set index rejected correctly with ATLAS_ERROR_BOUNDS.\n");
    } else {
        test_fail(&arr, "Invalid set index was incorrectly accepted.");
        return 1;
    }

    if (atlas_array_set(NULL, 0, 123) == ATLAS_ERROR_NULL) {
        printf("\033[0;32m[OK]\033[0m atlas_array_set(NULL, ...) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_set(NULL, ...) failed.");
        return 1;
    }

    if (atlas_array_reserve(NULL, 100) == ATLAS_ERROR_NULL) {
        printf("\033[0;32m[OK]\033[0m atlas_array_reserve(NULL, ...) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_reserve(NULL, ...) failed.");
        return 1;
    }

    if (atlas_array_pop(NULL, &dummy) == ATLAS_ERROR_NULL) {
        printf("\033[0;32m[OK]\033[0m atlas_array_pop(NULL, ...) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_pop(NULL, ...) failed.");
        return 1;
    }

    if (atlas_array_pop(arr, NULL) == ATLAS_ERROR_NULL) {
        printf("\033[0;32m[OK]\033[0m atlas_array_pop(..., NULL) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_pop(..., NULL) failed.");
        return 1;
    }

    if (atlas_array_pop(arr, &dummy) == ATLAS_ERROR_EMPTY) {
        printf("\033[0;32m[OK]\033[0m atlas_array_pop from empty array returned ATLAS_ERROR_EMPTY.\n");
    } else {
        test_fail(&arr, "atlas_array_pop from empty array failed.");
        return 1;
    }

    if (atlas_array_front(NULL, &dummy) == ATLAS_ERROR_NULL) {
        printf("\033[0;32m[OK]\033[0m atlas_array_front(NULL, ...) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_front(NULL) failed.");
        return 1;
    }

    if (atlas_array_front(arr, NULL) == ATLAS_ERROR_NULL) {
        printf("\033[0;32m[OK]\033[0m atlas_array_front(..., NULL) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_front(..., NULL) failed.");
        return 1;
    }

    if (atlas_array_front(arr, &dummy) == ATLAS_ERROR_EMPTY) {
        printf("\033[0;32m[OK]\033[0m atlas_array_front from empty array returned ATLAS_ERROR_EMPTY.\n");
    } else {
        test_fail(&arr, "atlas_array_front from empty array failed.");
        return 1;
    }

    if (atlas_array_back(NULL, &dummy) == ATLAS_ERROR_NULL) {
        printf("\033[0;32m[OK]\033[0m atlas_array_back(NULL, ...) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_back(NULL) failed.");
        return 1;
    }

    if (atlas_array_back(arr, NULL) == ATLAS_ERROR_NULL) {
        printf("\033[0;32m[OK]\033[0m atlas_array_back(..., NULL) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_back(..., NULL) failed.");
        return 1;
    }

    if (atlas_array_back(arr, &dummy) == ATLAS_ERROR_EMPTY) {
        printf("\033[0;32m[OK]\033[0m atlas_array_back from empty array returned ATLAS_ERROR_EMPTY.\n");
    } else {
        test_fail(&arr, "atlas_array_back from empty array failed.");
        return 1;
    }

    if (atlas_array_insert(NULL, 0, 10) == ATLAS_ERROR_NULL) {
        printf("\033[0;32m[OK]\033[0m atlas_array_insert(NULL, ...) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_insert(NULL) failed.");
        return 1;
    }

    if (atlas_array_insert(arr, 999, 10) == ATLAS_ERROR_BOUNDS) {
        printf("\033[0;32m[OK]\033[0m Invalid insert index rejected correctly with ATLAS_ERROR_BOUNDS.\n");
    } else {
        test_fail(&arr, "Invalid insert index failed.");
        return 1;
    }

    if (atlas_array_erase(NULL, 0) == ATLAS_ERROR_NULL) {
        printf("\033[0;32m[OK]\033[0m atlas_array_erase(NULL, ...) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_erase(NULL) failed.");
        return 1;
    }

    if (atlas_array_erase(arr, 999) == ATLAS_ERROR_BOUNDS) {
        printf("\033[0;32m[OK]\033[0m Invalid erase index rejected correctly with ATLAS_ERROR_BOUNDS.\n");
    } else {
        test_fail(&arr, "Invalid erase index failed.");
        return 1;
    }

    size_t dummy_index = 0;
    if (atlas_array_find(NULL, &dummy_index, 10) == ATLAS_ERROR_NULL) {
        printf("\033[0;32m[OK]\033[0m atlas_array_find(NULL, ...) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_find(NULL) failed.");
        return 1;
    }

    if (atlas_array_find(arr, NULL, 10) == ATLAS_ERROR_NULL) {
        printf("\033[0;32m[OK]\033[0m atlas_array_find(..., NULL, ...) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_find(..., NULL) failed.");
        return 1;
    }

    bool dummy_contains = false;
    if (atlas_array_contains(NULL, &dummy_contains, 10) == ATLAS_ERROR_NULL) {
        printf("\033[0;32m[OK]\033[0m atlas_array_contains(NULL, ...) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_contains(NULL) failed.");
        return 1;
    }

    if (atlas_array_contains(arr, NULL, 10) == ATLAS_ERROR_NULL) {
        printf("\033[0;32m[OK]\033[0m atlas_array_contains(..., NULL, ...) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_contains(..., NULL) failed.");
        return 1;
    }

    if (atlas_array_clear(NULL) == ATLAS_ERROR_NULL) {
        printf("\033[0;32m[OK]\033[0m atlas_array_clear(NULL) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_clear(NULL) failed.");
        return 1;
    }

    if (atlas_array_copy(NULL, arr) == ATLAS_ERROR_NULL) {
        printf("\033[0;32m[OK]\033[0m atlas_array_copy(NULL, ...) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_copy(NULL, ...) failed.");
        return 1;
    }

    if (atlas_array_copy(arr, NULL) == ATLAS_ERROR_NULL) {
        printf("\033[0;32m[OK]\033[0m atlas_array_copy(..., NULL) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_copy(..., NULL) failed.");
        return 1;
    }

    if (atlas_array_copy(arr, arr) == ATLAS_ERROR_INVALID_ARGUMENT) {
        printf("\033[0;32m[OK]\033[0m Self-copy rejected correctly with ATLAS_ERROR_INVALID_ARGUMENT.\n");
    } else {
        test_fail(&arr, "atlas_array_copy(arr, arr) failed.");
        return 1;
    }

    if (atlas_array_shrink_to_fit(NULL) == ATLAS_ERROR_NULL) {
        printf("\033[0;32m[OK]\033[0m atlas_array_shrink_to_fit(NULL) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_shrink_to_fit(NULL) failed.");
        return 1;
    }

    if (atlas_array_swap(NULL, 0, 1) == ATLAS_ERROR_NULL) {
        printf("\033[0;32m[OK]\033[0m atlas_array_swap(NULL, ...) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_swap(NULL, ...) failed.");
        return 1;
    }

    if (atlas_array_swap(arr, 999, 0) == ATLAS_ERROR_BOUNDS) {
        printf("\033[0;32m[OK]\033[0m Invalid swap index rejected correctly with ATLAS_ERROR_BOUNDS.\n");
    } else {
        test_fail(&arr, "atlas_array_swap(..., invalid, ...) failed.");
        return 1;
    }

    if (atlas_array_clone(NULL) == NULL) {
        printf("\033[0;32m[OK]\033[0m atlas_array_clone(NULL) rejected correctly.\n");
    } else {
        test_fail(&arr, "atlas_array_clone(NULL) should return NULL.");
        return 1;
    }

    // =========================================================
    // Cleanup Tests
    // =========================================================
    printf("\n\033[0;33m[INFO]\033[0m Running cleanup tests...\n");

    atlas_array_destroy(&arr);

    if (!arr) {
        printf("\033[0;32m[OK]\033[0m Pointer reset to NULL after destruction.\n");
    } else {
        printf("\033[0;31m[ERROR]\033[0m Pointer was not reset to NULL.\n");
        return 1;
    }

    atlas_array_destroy(NULL);
    printf("\033[0;32m[OK]\033[0m Passing NULL to destroy handled safely.\n");

    AtlasArray *null_arr = NULL;
    atlas_array_destroy(&null_arr);
    printf("\033[0;32m[OK]\033[0m Passing pointer to NULL to destroy handled safely.\n");

    printf("\n\033[1;32mAll AtlasDS dynamic array tests passed successfully!\033[0m\n\n");
        
    return 0;
}