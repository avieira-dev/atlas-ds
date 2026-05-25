#include "atlas/array.h"

#include <stdio.h>

int main(void) {

    printf("\n\033[0;33m[INFO]\033[0m Starting AtlasDS dynamic array tests...\n");

    size_t initial_capacity = 3;

    AtlasArray *arr = atlas_array_create(initial_capacity);

    if (!arr) {
        printf("\033[0;31m[ERROR]\033[0m Failed to create dynamic array.\n");
        return 1;
    }

    printf("\033[0;32m[OK]\033[0m Dynamic array created successfully.\n");

    int values[] = {5, 10, 15, 20};
    size_t total_values = sizeof(values) / sizeof(values[0]);

    for (size_t i = 0; i < total_values; i++) {
        if (atlas_array_push(arr, values[i]) == 0) {
            printf("\033[0;32m[OK]\033[0m Inserted value: %d\n", values[i]);
        } else {
            printf("\033[0;31m[ERROR]\033[0m Failed to insert value: %d\n", values[i]);

            atlas_array_destroy(&arr);
            return 1;
        }
    }

    printf("\033[0;32m[OK]\033[0m Automatic resizing test completed.\n");

    atlas_array_destroy(&arr);

    if (arr == NULL) {
        printf("\033[0;32m[OK]\033[0m Dynamic array destroyed successfully.");
    } else {
        printf("\033[0;31m[ERROR]\033[0m Failed to destroy dynamic array.");
        return 1;
    }

    printf("\n\033[0;33m[INFO]\033[0m All tests completed successfully.\n\n");

    return 0;
}