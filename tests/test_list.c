/*
 * AtlasDS
 * List Tests
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#include <atlas/list.h>
#include <atlas/status.h>

#include <stdio.h>
#include <string.h>

#define COLOR_RED "\033[0;31m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_BLUE "\033[0;34m"

#define COLOR_BOLD_RED "\033[1;31m"
#define COLOR_BOLD_GREEN "\033[1;32m"
#define COLOR_BOLD_YELLOW "\033[1;33m"
#define COLOR_BOLD_BLUE "\033[1;34m"

#define COLOR_RESET "\033[0m"


typedef struct {
    char name[255];
    int age;
    double score;
} Student;


static int test_create_destroy(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    if (list != NULL) {
        return 1;
    }

    return 0;
}


static int test_create_invalid_type_size(void) {
    AtlasList *list = atlas_list_create(0);

    if (list) {
        return 1;
    }

    return 0;
}


static int test_destroy_null(void) {
    AtlasList *list = NULL;

    if (atlas_list_destroy(&list) != ATLAS_ERROR_NULL) {
        return 1;
    }

    if (atlas_list_destroy(NULL) != ATLAS_ERROR_NULL) {
        return 1;
    }

    return 0;
}


static int test_push_front(void) {
    AtlasList *list = atlas_list_create(sizeof(char));

    if (!list) {
        return 1;
    }

    char first = 'A';
    char second = 'D';

    if (atlas_list_push_front(list, &first) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_push_front(list, &second) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    char result;

    if (atlas_list_front(list, &result) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != 'D') {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}


static int test_push_back(void) {
    AtlasList *list = atlas_list_create(sizeof(double));

    if (!list) {
        return 1;
    }

    double first = 3.1416;
    double second = 3.14159;

    if (atlas_list_push_back(list, &first) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_push_back(list, &second) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    double result;

    if (atlas_list_back(list, &result) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != second) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}


static int test_insert_at_beginning(void) {
    AtlasList *list = atlas_list_create(sizeof(Student));

    if (!list) {
        return 1;
    }

    Student student;

    snprintf(student.name, sizeof(student.name), "Alexandre");
    student.age = 26;
    student.score = 10.0;

    if (atlas_list_insert(list, 0, &student) != ATLAS_SUCCESS) {
        return 1;
    }

    Student result;

    if (atlas_list_front(list, &result) != ATLAS_SUCCESS) {
        return 1;
    }

    if (strcmp(result.name, "Alexandre") != 0) {
        return 1;
    }

    if (result.age != 26) {
        return 1;
    }

    if (result.score != 10.0) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_insert_middle(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int element_1 = 1936;
    int element_2 = 1946;

    if (atlas_list_push_front(list, &element_1) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    int result;

    if (atlas_list_front(list, &result) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != element_1) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_push_back(list, &element_2) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_back(list, &result) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != element_2) {
        atlas_list_destroy(&list);
        return 1;
    }

    int element_3 = 1991;

    if (atlas_list_insert(list, 1, &element_3) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_get(list, &result, 0) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != element_1) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_get(list, &result, 1) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != element_3) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_get(list, &result, 2) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != element_2) {
        atlas_list_destroy(&list);
        return 1;
    }

    size_t size;

    if (atlas_list_size(list, &size) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (size != 3) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_insert_end(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int element_1 = 1815;
    int element_2 = 1930;
    int element_3 = 1937;
    int result;

    if (atlas_list_insert(list, 0, &element_1) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_insert(list, 1, &element_2) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_insert(list, 2, &element_3) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_get(list, &result, 0) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != element_1) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_get(list, &result, 1) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != element_2) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_get(list, &result, 2) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != element_3) {
        atlas_list_destroy(&list);
        return 1;
    }

    size_t size;

    if (atlas_list_size(list, &size) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (size != 3) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_insert_invalid_index(void) {
    AtlasList *list = atlas_list_create(sizeof(float));

    if (!list) {
        return 1;
    }

    float element_1 = 20.000f;
    float element_2 = 30.000f;
    float element_3 = 590.000f;
    float result;

    if (atlas_list_insert(list, 0, &element_1) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_insert(list, 1, &element_2) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_insert(list, 3, &element_3) != ATLAS_ERROR_BOUNDS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_get(list, &result, 0) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != element_1) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_get(list, &result, 1) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != element_2) {
        atlas_list_destroy(&list);
        return 1;
    }

    size_t size;

    if (atlas_list_size(list, &size) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (size != 2) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_pop_front(void) {
    AtlasList *list = atlas_list_create(sizeof(char));

    if (!list) {
        return 1;
    }

    char chars[] = {'B', 'D', 'E', 'N', 'N', 'I', 'S'};

    size_t size_arr = sizeof(chars) / sizeof(chars[0]);

    for (size_t i = 0; i < size_arr; i++) {
        if (atlas_list_insert(list, i, &chars[i]) != ATLAS_SUCCESS) {
            atlas_list_destroy(&list);
            return 1;
        }
    }

    size_t size_list;

    if (atlas_list_size(list, &size_list) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (size_list != size_arr) {
        atlas_list_destroy(&list);
        return 1;
    }

    char result;

    if (atlas_list_get(list, &result, 0) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != chars[0]) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_get(list, &result, size_arr - 1) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != chars[size_arr - 1]) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_pop_front(list, &result) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != chars[0]) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_pop_front(list, &result) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != chars[1]) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_front(list, &result) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != chars[2]) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_back(list, &result) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != chars[size_arr - 1]) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_size(list, &size_list) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (size_list != size_arr - 2) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_pop_back(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int elements[] = {20, 34, 42, 57};
    size_t size_arr = sizeof(elements) / sizeof(elements[0]);

    for (size_t i = 0; i < size_arr; i++) {
        if (atlas_list_push_back(list, &elements[i]) != ATLAS_SUCCESS) {
            atlas_list_destroy(&list);
            return 1;
        }
    }

    int result;

    if (atlas_list_pop_back(list, &result) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != elements[size_arr - 1]) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_back(list, &result) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != elements[size_arr - 2]) {
        atlas_list_destroy(&list);
        return 1;
    }

    size_t size;

    if (atlas_list_size(list, &size) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (size != size_arr - 1) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}


static int test_pop_empty(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int result;

    if (atlas_list_pop_front(list, &result) != ATLAS_ERROR_EMPTY) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_pop_back(list, &result) != ATLAS_ERROR_EMPTY) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}


static int test_front_back_empty(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int result;

    if (atlas_list_front(list, &result) != ATLAS_ERROR_EMPTY) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_back(list, &result) != ATLAS_ERROR_EMPTY) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}


static int test_set(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int elements[] = {1979, 1993, 2003};
    size_t size_arr = sizeof(elements) / sizeof(elements[0]);

    for (size_t i = 0; i < size_arr; i++) {
        if (atlas_list_push_back(list, &elements[i]) != ATLAS_SUCCESS) {
            atlas_list_destroy(&list);
            return 1;
        }
    }

    int new_value = 2005;

    if (atlas_list_set(list, &new_value, 1) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    int result;

    if (atlas_list_get(list, &result, 1) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != new_value) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_get(list, &result, 0) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != elements[0]) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}


static int test_get_set_invalid_index(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int element = 1981;

    if (atlas_list_push_back(list, &element) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    int result;

    if (atlas_list_get(list, &result, 5) != ATLAS_ERROR_BOUNDS) {
        atlas_list_destroy(&list);
        return 1;
    }

    int new_value = 1999;

    if (atlas_list_set(list, &new_value, 5) != ATLAS_ERROR_BOUNDS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}


static int test_empty(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    bool is_empty;

    if (atlas_list_empty(list, &is_empty) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (!is_empty) {
        atlas_list_destroy(&list);
        return 1;
    }

    int element = 1;

    if (atlas_list_push_back(list, &element) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_empty(list, &is_empty) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (is_empty) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}


static int test_erase_beginning(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int elements[] = {8, 9, 35, 40};
    size_t size_arr = sizeof(elements) / sizeof(elements[0]);

    for (size_t i = 0; i < size_arr; i++) {
        if (atlas_list_push_back(list, &elements[i]) != ATLAS_SUCCESS) {
            atlas_list_destroy(&list);
            return 1;
        }
    }

    int result;

    if (atlas_list_erase(list, 0, &result) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != elements[0]) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_get(list, &result, 0) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != elements[1]) {
        atlas_list_destroy(&list);
        return 1;
    }

    size_t size;

    if (atlas_list_size(list, &size) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (size != size_arr - 1) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}


static int test_erase_middle(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int elements[] = {10, 20, 30, 40, 50};
    size_t size_arr = sizeof(elements) / sizeof(elements[0]);

    for (size_t i = 0; i < size_arr; i++) {
        if (atlas_list_push_back(list, &elements[i]) != ATLAS_SUCCESS) {
            atlas_list_destroy(&list);
            return 1;
        }
    }

    int result;

    if (atlas_list_erase(list, 2, &result) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != elements[2]) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_get(list, &result, 2) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != elements[3]) {
        atlas_list_destroy(&list);
        return 1;
    }

    size_t size;

    if (atlas_list_size(list, &size) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (size != size_arr - 1) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}


static int test_erase_end(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int elements[] = {5, 15, 25};
    size_t size_arr = sizeof(elements) / sizeof(elements[0]);

    for (size_t i = 0; i < size_arr; i++) {
        if (atlas_list_push_back(list, &elements[i]) != ATLAS_SUCCESS) {
            atlas_list_destroy(&list);
            return 1;
        }
    }

    int result;

    if (atlas_list_erase(list, size_arr - 1, &result) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != elements[size_arr - 1]) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_back(list, &result) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != elements[size_arr - 2]) {
        atlas_list_destroy(&list);
        return 1;
    }

    size_t size;

    if (atlas_list_size(list, &size) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (size != size_arr - 1) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}


static int test_erase_invalid_index(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int element = 7;

    if (atlas_list_push_back(list, &element) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    int result;

    if (atlas_list_erase(list, 5, &result) != ATLAS_ERROR_BOUNDS) {
        atlas_list_destroy(&list);
        return 1;
    }

    size_t size;

    if (atlas_list_size(list, &size) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (size != 1) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

int main(void) {
    printf("\n" COLOR_BOLD_BLUE "========================================================" COLOR_RESET "\n");
    printf(COLOR_BOLD_BLUE "\t\tAtlasDS - List Tests" COLOR_RESET "\n");
    printf(COLOR_BOLD_BLUE "========================================================" COLOR_RESET "\n\n");

    printf(COLOR_YELLOW "[INFO]" COLOR_RESET " Starting AtlasDS list tests...\n\n");

    // =========================================================
    // Lifecycle
    // =========================================================

    printf(COLOR_YELLOW "[INFO]" COLOR_RESET " Running lifecycle tests...\n");

    if (test_create_destroy()) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET " Create/Destroy operation failed.\n");
        return 1;
    }
    printf(COLOR_GREEN "[OK]" COLOR_RESET " Create/Destroy operation passed.\n");

    if (test_create_invalid_type_size()) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET " Type size validation failed.\n");
        return 1;
    }
    printf(COLOR_GREEN "[OK]" COLOR_RESET " Type size validation passed.\n");

    if (test_destroy_null()) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET " NULL destroy validation failed.\n");
        return 1;
    }
    printf(COLOR_GREEN "[OK]" COLOR_RESET " NULL destroy validation passed.\n\n");

    // =========================================================
    // Insertion
    // =========================================================

    printf(COLOR_YELLOW "[INFO]" COLOR_RESET " Running insertion tests...\n");

    if (test_push_front()) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET " Push front operation failed.\n");
        return 1;
    }
    printf(COLOR_GREEN "[OK]" COLOR_RESET " Push front operation passed.\n");

    if (test_push_back()) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET " Push back operation failed.\n");
        return 1;
    }
    printf(COLOR_GREEN "[OK]" COLOR_RESET " Push back operation passed.\n");

    if (test_insert_at_beginning()) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET " Insert at beginning operation failed.\n");
        return 1;
    }
    printf(COLOR_GREEN "[OK]" COLOR_RESET " Insert at beginning operation passed.\n");

    if (test_insert_middle()) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET " Insert middle operation failed.\n");
        return 1;
    }
    printf(COLOR_GREEN "[OK]" COLOR_RESET " Insert middle operation passed.\n");

    if (test_insert_end()) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET " Insert end operation failed.\n");
        return 1;
    }
    printf(COLOR_GREEN "[OK]" COLOR_RESET " Insert end operation passed.\n");

    if (test_insert_invalid_index()) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET " Insert invalid index validation failed.\n");
        return 1;
    }
    printf(COLOR_GREEN "[OK]" COLOR_RESET " Insert invalid index validation passed.\n\n");

    // =========================================================
    // Access
    // =========================================================

    printf(COLOR_YELLOW "[INFO]" COLOR_RESET " Running access tests...\n");

    if (test_set()) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET " Set operation failed.\n");
        return 1;
    }
    printf(COLOR_GREEN "[OK]" COLOR_RESET " Set operation passed.\n");

    if (test_empty()) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET " Empty check failed.\n");
        return 1;
    }
    printf(COLOR_GREEN "[OK]" COLOR_RESET " Empty check passed.\n");

    if (test_get_set_invalid_index()) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET " Get/Set bounds validation failed.\n");
        return 1;
    }
    printf(COLOR_GREEN "[OK]" COLOR_RESET " Get/Set bounds validation passed.\n\n");

    // =========================================================
    // Removal
    // =========================================================

    printf(COLOR_YELLOW "[INFO]" COLOR_RESET " Running removal tests...\n");

    if (test_pop_front()) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET " Pop front operation failed.\n");
        return 1;
    }
    printf(COLOR_GREEN "[OK]" COLOR_RESET " Pop front operation passed.\n");

    if (test_pop_back()) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET " Pop back operation failed.\n");
        return 1;
    }
    printf(COLOR_GREEN "[OK]" COLOR_RESET " Pop back operation passed.\n");

    if (test_pop_empty()) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET " Pop on empty list validation failed.\n");
        return 1;
    }
    printf(COLOR_GREEN "[OK]" COLOR_RESET " Pop on empty list validation passed.\n");

    if (test_front_back_empty()) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET " Front/Back on empty list validation failed.\n");
        return 1;
    }
    printf(COLOR_GREEN "[OK]" COLOR_RESET " Front/Back on empty list validation passed.\n");

    if (test_erase_beginning()) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET " Erase at beginning operation failed.\n");
        return 1;
    }
    printf(COLOR_GREEN "[OK]" COLOR_RESET " Erase at beginning operation passed.\n");

    if (test_erase_middle()) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET " Erase middle operation failed.\n");
        return 1;
    }
    printf(COLOR_GREEN "[OK]" COLOR_RESET " Erase middle operation passed.\n");

    if (test_erase_end()) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET " Erase end operation failed.\n");
        return 1;
    }
    printf(COLOR_GREEN "[OK]" COLOR_RESET " Erase end operation passed.\n");

    if (test_erase_invalid_index()) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET " Erase invalid index validation failed.\n");
        return 1;
    }
    printf(COLOR_GREEN "[OK]" COLOR_RESET " Erase invalid index validation passed.\n");

    printf("\n" COLOR_BOLD_GREEN "[SUCCESS]" COLOR_RESET " All tests completed successfully.\n\n");

    return 0;
}