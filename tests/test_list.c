/*
 * AtlasDS
 * List Tests
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#include "atlas/list.h"
#include "atlas/status.h"
#include "atlas/terminal.h"

#include <stdio.h>
#include <string.h>

typedef struct {
    char name[255];
    int age;
    double score;
} Student;

static int compare_int(const void *first, const void *second) {
    const int *a = first;
    const int *b = second;

    if (*a < *b) {
        return -1;
    }

    if (*a > *b) {
        return 1;
    }

    return 0;
}

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

static int test_clear_single_element(void) {
    AtlasList *list = atlas_list_create(sizeof(char));

    if (!list) {
        return 1;
    }

    char letter = 'C';

    if (atlas_list_insert(list, 0, &letter) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    size_t list_size;

    if (atlas_list_size(list, &list_size) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (list_size != 1) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_clear(list) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_size(list, &list_size) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (list_size > 0) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_clear_empty_list(void) {
    AtlasList *list = atlas_list_create(sizeof(float));

    if (!list) {
        return 1;
    }

    if (atlas_list_clear(list) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_clear_multiple_elements(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int years[] = {1947, 1958, 1971, 1998};

    for (size_t i = 0; i < 4; i++) {
        if (atlas_list_insert(list, i, &years[i]) != ATLAS_SUCCESS) {
            atlas_list_destroy(&list);
            return 1;
        }
    }

    size_t list_size;

    if (atlas_list_size(list, &list_size) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (list_size != 4) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_clear(list) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_clear_reuse_list(void) {
    AtlasList *list = atlas_list_create(sizeof(float));

    float nos[] = {0.001, 0.016};

    for (size_t i = 0; i < 2; i++) {
        if (atlas_list_insert(list, i, &nos[i]) != ATLAS_SUCCESS) {
            atlas_list_destroy(&list);
            return 1;
        }
    }

    size_t list_size;

    if (atlas_list_size(list, &list_size) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (list_size != 2) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_clear(list) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    float gravity = 9.81;

    if (atlas_list_insert(list, 0, &gravity) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_size(list, &list_size) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (list_size != 1) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_clear_null_list(void) {
    AtlasList *list = NULL;

    if (atlas_list_clear(list) == ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_swap(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int elements[] = {10, 20, 30, 40};
    size_t size_arr = sizeof(elements) / sizeof(elements[0]);

    for (size_t i = 0; i < size_arr; i++) {
        if (atlas_list_push_back(list, &elements[i]) != ATLAS_SUCCESS) {
            atlas_list_destroy(&list);
            return 1;
        }
    }

    if (atlas_list_swap(list, 1, 3) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    int result;

    if (atlas_list_get(list, &result, 1) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != elements[3]) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_get(list, &result, 3) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != elements[1]) {
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

    if (atlas_list_get(list, &result, 2) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != elements[2]) {
        atlas_list_destroy(&list);
        return 1;
    }

    size_t size;

    if (atlas_list_size(list, &size) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (size != size_arr) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}


static int test_swap_same_index(void) {
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

    if (atlas_list_swap(list, 1, 1) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    int result;

    if (atlas_list_get(list, &result, 0) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != elements[0]) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_get(list, &result, 1) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != elements[1]) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_get(list, &result, 2) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != elements[2]) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}


static int test_swap_invalid_index(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int elements[] = {1, 2, 3};
    size_t size_arr = sizeof(elements) / sizeof(elements[0]);

    for (size_t i = 0; i < size_arr; i++) {
        if (atlas_list_push_back(list, &elements[i]) != ATLAS_SUCCESS) {
            atlas_list_destroy(&list);
            return 1;
        }
    }

    if (atlas_list_swap(list, 0, 10) != ATLAS_ERROR_BOUNDS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_swap(list, 10, 0) != ATLAS_ERROR_BOUNDS) {
        atlas_list_destroy(&list);
        return 1;
    }

    int result;

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


static int test_swap_single_element(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int element = 42;

    if (atlas_list_push_back(list, &element) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_swap(list, 0, 0) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    int result;

    if (atlas_list_get(list, &result, 0) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != element) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}


static int test_swap_empty_list(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    if (atlas_list_swap(list, 0, 0) != ATLAS_ERROR_BOUNDS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_reverse(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int elements[] = {10, 20, 30, 40};
    size_t size_arr = sizeof(elements) / sizeof(elements[0]);

    for (size_t i = 0; i < size_arr; i++) {
        if (atlas_list_push_back(list, &elements[i]) != ATLAS_SUCCESS) {
            atlas_list_destroy(&list);
            return 1;
        }
    }

    if (atlas_list_reverse(list) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    int result;

    for (size_t i = 0; i < size_arr; i++) {
        if (atlas_list_get(list, &result, i) != ATLAS_SUCCESS) {
            atlas_list_destroy(&list);
            return 1;
        }

        if (result != elements[size_arr - 1 - i]) {
            atlas_list_destroy(&list);
            return 1;
        }
    }

    if (atlas_list_front(list, &result) != ATLAS_SUCCESS) {
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

    if (result != elements[0]) {
        atlas_list_destroy(&list);
        return 1;
    }

    size_t size;

    if (atlas_list_size(list, &size) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (size != size_arr) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_reverse_single_element(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int element = 42;

    if (atlas_list_push_back(list, &element) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_reverse(list) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    int result;

    if (atlas_list_get(list, &result, 0) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != element) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_front(list, &result) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != element) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_back(list, &result) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (result != element) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_reverse_empty_list(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    if (atlas_list_reverse(list) != ATLAS_ERROR_EMPTY) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_reverse_null_list(void) {
    if (atlas_list_reverse(NULL) != ATLAS_ERROR_NULL) {
        return 1;
    }

    return 0;
}

static int test_copy(void) {
    AtlasList *source = atlas_list_create(sizeof(int));
    AtlasList *destination = atlas_list_create(sizeof(int));

    if (!source || !destination) {
        atlas_list_destroy(&source);
        atlas_list_destroy(&destination);
        return 1;
    }

    int source_elements[] = {10, 20, 30, 40};
    int destination_element = 99;

    for (size_t i = 0; i < 4; i++) {
        if (atlas_list_push_back(source, &source_elements[i]) != ATLAS_SUCCESS) {
            atlas_list_destroy(&source);
            atlas_list_destroy(&destination);
            return 1;
        }
    }

    if (atlas_list_push_back(destination, &destination_element) != ATLAS_SUCCESS) {
        atlas_list_destroy(&source);
        atlas_list_destroy(&destination);
        return 1;
    }

    if (atlas_list_copy(source, destination) != ATLAS_SUCCESS) {
        atlas_list_destroy(&source);
        atlas_list_destroy(&destination);
        return 1;
    }

    size_t size;

    if (atlas_list_size(destination, &size) != ATLAS_SUCCESS) {
        atlas_list_destroy(&source);
        atlas_list_destroy(&destination);
        return 1;
    }

    if (size != 4) {
        atlas_list_destroy(&source);
        atlas_list_destroy(&destination);
        return 1;
    }

    int result;

    for (size_t i = 0; i < 4; i++) {
        if (atlas_list_get(destination, &result, i) != ATLAS_SUCCESS) {
            atlas_list_destroy(&source);
            atlas_list_destroy(&destination);
            return 1;
        }

        if (result != source_elements[i]) {
            atlas_list_destroy(&source);
            atlas_list_destroy(&destination);
            return 1;
        }
    }

    atlas_list_destroy(&source);
    atlas_list_destroy(&destination);

    return 0;
}


static int test_copy_independence(void) {
    AtlasList *source = atlas_list_create(sizeof(int));
    AtlasList *destination = atlas_list_create(sizeof(int));

    if (!source || !destination) {
        atlas_list_destroy(&source);
        atlas_list_destroy(&destination);
        return 1;
    }

    int elements[] = {10, 20, 30};

    for (size_t i = 0; i < 3; i++) {
        if (atlas_list_push_back(source, &elements[i]) != ATLAS_SUCCESS) {
            atlas_list_destroy(&source);
            atlas_list_destroy(&destination);
            return 1;
        }
    }

    if (atlas_list_copy(source, destination) != ATLAS_SUCCESS) {
        atlas_list_destroy(&source);
        atlas_list_destroy(&destination);
        return 1;
    }

    int new_value = 99;

    if (atlas_list_set(destination, &new_value, 1) != ATLAS_SUCCESS) {
        atlas_list_destroy(&source);
        atlas_list_destroy(&destination);
        return 1;
    }

    int result;

    if (atlas_list_get(source, &result, 1) != ATLAS_SUCCESS) {
        atlas_list_destroy(&source);
        atlas_list_destroy(&destination);
        return 1;
    }

    if (result != elements[1]) {
        atlas_list_destroy(&source);
        atlas_list_destroy(&destination);
        return 1;
    }

    if (atlas_list_get(destination, &result, 1) != ATLAS_SUCCESS) {
        atlas_list_destroy(&source);
        atlas_list_destroy(&destination);
        return 1;
    }

    if (result != new_value) {
        atlas_list_destroy(&source);
        atlas_list_destroy(&destination);
        return 1;
    }

    atlas_list_destroy(&source);
    atlas_list_destroy(&destination);

    return 0;
}


static int test_clone(void) {
    AtlasList *source = atlas_list_create(sizeof(int));

    if (!source) {
        return 1;
    }

    int elements[] = {15, 25, 35, 45};

    for (size_t i = 0; i < 4; i++) {
        if (atlas_list_push_back(source, &elements[i]) != ATLAS_SUCCESS) {
            atlas_list_destroy(&source);
            return 1;
        }
    }

    AtlasList *clone = atlas_list_clone(source);

    if (!clone) {
        atlas_list_destroy(&source);
        return 1;
    }

    size_t size;

    if (atlas_list_size(clone, &size) != ATLAS_SUCCESS) {
        atlas_list_destroy(&source);
        atlas_list_destroy(&clone);
        return 1;
    }

    if (size != 4) {
        atlas_list_destroy(&source);
        atlas_list_destroy(&clone);
        return 1;
    }

    int result;

    for (size_t i = 0; i < 4; i++) {
        if (atlas_list_get(clone, &result, i) != ATLAS_SUCCESS) {
            atlas_list_destroy(&source);
            atlas_list_destroy(&clone);
            return 1;
        }

        if (result != elements[i]) {
            atlas_list_destroy(&source);
            atlas_list_destroy(&clone);
            return 1;
        }
    }

    atlas_list_destroy(&source);
    atlas_list_destroy(&clone);

    return 0;
}


static int test_clone_independence(void) {
    AtlasList *source = atlas_list_create(sizeof(int));

    if (!source) {
        return 1;
    }

    int elements[] = {100, 200, 300};

    for (size_t i = 0; i < 3; i++) {
        if (atlas_list_push_back(source, &elements[i]) != ATLAS_SUCCESS) {
            atlas_list_destroy(&source);
            return 1;
        }
    }

    AtlasList *clone = atlas_list_clone(source);

    if (!clone) {
        atlas_list_destroy(&source);
        return 1;
    }

    int new_value = 999;

    if (atlas_list_set(clone, &new_value, 1) != ATLAS_SUCCESS) {
        atlas_list_destroy(&source);
        atlas_list_destroy(&clone);
        return 1;
    }

    int result;

    if (atlas_list_get(source, &result, 1) != ATLAS_SUCCESS) {
        atlas_list_destroy(&source);
        atlas_list_destroy(&clone);
        return 1;
    }

    if (result != elements[1]) {
        atlas_list_destroy(&source);
        atlas_list_destroy(&clone);
        return 1;
    }

    if (atlas_list_get(clone, &result, 1) != ATLAS_SUCCESS) {
        atlas_list_destroy(&source);
        atlas_list_destroy(&clone);
        return 1;
    }

    if (result != new_value) {
        atlas_list_destroy(&source);
        atlas_list_destroy(&clone);
        return 1;
    }

    atlas_list_destroy(&source);
    atlas_list_destroy(&clone);

    return 0;
}

static int test_find(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int elements[] = {10, 20, 30, 40};
    size_t size_arr = sizeof(elements) / sizeof(elements[0]);

    for (size_t i = 0; i < size_arr; i++) {
        if (atlas_list_push_back(list, &elements[i]) != ATLAS_SUCCESS) {
            atlas_list_destroy(&list);
            return 1;
        }
    }

    int value = 30;
    size_t index;

    if (atlas_list_find(list, &index, &value, compare_int) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (index != 2) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_find_not_found(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int elements[] = {10, 20, 30, 40};

    for (size_t i = 0; i < 4; i++) {
        if (atlas_list_push_back(list, &elements[i]) != ATLAS_SUCCESS) {
            atlas_list_destroy(&list);
            return 1;
        }
    }

    int value = 99;
    size_t index;

    if (atlas_list_find(list, &index, &value, compare_int) != ATLAS_ERROR_NOT_FOUND) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_find_empty_list(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int value = 10;
    size_t index;

    if (atlas_list_find(list, &index, &value, compare_int) != ATLAS_ERROR_EMPTY) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_find_null(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int value = 10;
    size_t index;

    if (atlas_list_find(NULL, &index, &value, compare_int) != ATLAS_ERROR_NULL) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_find(list, NULL, &value, compare_int) != ATLAS_ERROR_NULL) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_find(list, &index, NULL, compare_int) != ATLAS_ERROR_NULL) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_find(list, &index, &value, NULL) != ATLAS_ERROR_NULL) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_contains(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int elements[] = {10, 20, 30, 40};
    size_t size_arr = sizeof(elements) / sizeof(elements[0]);

    for (size_t i = 0; i < size_arr; i++) {
        if (atlas_list_push_back(list, &elements[i]) != ATLAS_SUCCESS) {
            atlas_list_destroy(&list);
            return 1;
        }
    }

    int value = 30;
    bool contains;

    if (atlas_list_contains(list, &contains, &value, compare_int) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (!contains) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_contains_not_found(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int elements[] = {10, 20, 30, 40};

    for (size_t i = 0; i < 4; i++) {
        if (atlas_list_push_back(list, &elements[i]) != ATLAS_SUCCESS) {
            atlas_list_destroy(&list);
            return 1;
        }
    }

    int value = 99;
    bool contains;

    if (atlas_list_contains(list, &contains, &value, compare_int) != ATLAS_SUCCESS) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (contains) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_contains_empty_list(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int value = 10;
    bool contains;

    if (atlas_list_contains(list, &contains, &value, compare_int) != ATLAS_ERROR_EMPTY) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

static int test_contains_null(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int value = 10;
    bool contains;

    if (atlas_list_contains(NULL, &contains, &value, compare_int) != ATLAS_ERROR_NULL) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_contains(list, NULL, &value, compare_int) != ATLAS_ERROR_NULL) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_contains(list, &contains, NULL, compare_int) != ATLAS_ERROR_NULL) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_contains(list, &contains, &value, NULL) != ATLAS_ERROR_NULL) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_destroy(&list) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}

int main(void) {
    printf("\n" COLOR_BOLD_BLUE "╭────────────────────────────────────────────────────────╮" COLOR_RESET "\n");
    printf(COLOR_BOLD_BLUE "│" COLOR_RESET "                  AtlasDS - List Tests                  " COLOR_BOLD_BLUE "│" COLOR_RESET "\n");
    printf(COLOR_BOLD_BLUE "╰────────────────────────────────────────────────────────╯" COLOR_RESET "\n\n");

    printf(COLOR_YELLOW "ℹ " COLOR_RESET "Starting AtlasDS list tests...\n\n");

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

    if (test_push_front()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Push front operation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Push front operation\n");

    if (test_push_back()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Push back operation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Push back operation\n");

    if (test_insert_at_beginning()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Insert at beginning operation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Insert at beginning operation\n");

    if (test_insert_middle()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Insert middle operation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Insert middle operation\n");

    if (test_insert_end()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Insert end operation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Insert end operation\n");

    if (test_insert_invalid_index()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Insert invalid index validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Insert invalid index validation\n\n");


    // =========================================================
    // Access
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Access" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_set()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Set operation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Set operation\n");

    if (test_empty()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Empty check\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Empty check\n");

    if (test_get_set_invalid_index()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Get/Set bounds validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Get/Set bounds validation\n\n");


    // =========================================================
    // Search
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Search" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_find()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Find operation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Find operation\n");

    if (test_find_not_found()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Find not found validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Find not found validation\n");

    if (test_find_empty_list()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Find on empty list validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Find on empty list validation\n");

    if (test_find_null()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Find NULL validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Find NULL validation\n");

    if (test_contains()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Contains operation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Contains operation\n");

    if (test_contains_not_found()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Contains not found validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Contains not found validation\n");

    if (test_contains_empty_list()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Contains on empty list validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Contains on empty list validation\n");

    if (test_contains_null()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Contains NULL validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Contains NULL validation\n\n");


    // =========================================================
    // Swap
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Swap" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_swap()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Swap operation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Swap operation\n");

    if (test_swap_same_index()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Swap same index validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Swap same index validation\n");

    if (test_swap_invalid_index()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Swap invalid index validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Swap invalid index validation\n");

    if (test_swap_single_element()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Swap on single-element list\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Swap on single-element list\n");

    if (test_swap_empty_list()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Swap on empty list validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Swap on empty list validation\n\n");

    // =========================================================
    // Reverse
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Reverse" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_reverse()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Reverse operation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Reverse operation\n");

    if (test_reverse_single_element()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Reverse single-element list\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Reverse single-element list\n");

    if (test_reverse_empty_list()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Reverse on empty list validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Reverse on empty list validation\n");

    if (test_reverse_null_list()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Reverse NULL validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Reverse NULL validation\n\n");

    // =========================================================
    // Removal
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Removal" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_pop_front()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Pop front operation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Pop front operation\n");

    if (test_pop_back()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Pop back operation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Pop back operation\n");

    if (test_pop_empty()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Pop on empty list validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Pop on empty list validation\n");

    if (test_front_back_empty()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Front/Back on empty list validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Front/Back on empty list validation\n");

    if (test_erase_beginning()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Erase at beginning operation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Erase at beginning operation\n");

    if (test_erase_middle()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Erase middle operation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Erase middle operation\n");

    if (test_erase_end()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Erase end operation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Erase end operation\n");

    if (test_erase_invalid_index()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Erase invalid index validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Erase invalid index validation\n\n");


    // =========================================================
    // Clear
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Clear" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_clear_empty_list()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Clear operation on empty list\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Clear operation on empty list\n");

    if (test_clear_single_element()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Clear operation on single-element list\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Clear operation on single-element list\n");

    if (test_clear_multiple_elements()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Clear operation on multi-element list\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Clear operation on multi-element list\n");

    if (test_clear_reuse_list()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " List reuse after clear\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " List reuse after clear\n");

    if (test_clear_null_list()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Clear NULL validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Clear NULL validation\n\n");


    // =========================================================
    // Copy and Clone
    // =========================================================
    printf(COLOR_BOLD_CYAN "➤ Copy and Clone" COLOR_RESET "\n");
    printf(COLOR_CYAN "────────────────────────────────────────────────────────" COLOR_RESET "\n");

    if (test_copy()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Copy operation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Copy operation\n");

    if (test_copy_independence()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Copy independence validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Copy independence validation\n");

    if (test_clone()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Clone operation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Clone operation\n");

    if (test_clone_independence()) {
        printf("  " COLOR_BOLD_RED "✖" COLOR_RESET " Clone independence validation\n");
        return 1;
    }
    printf("  " COLOR_BOLD_GREEN "✔" COLOR_RESET " Clone independence validation\n\n");

    printf(COLOR_BOLD_CYAN "════════════════════════════════════════════════════════\n" COLOR_RESET "\n");
    printf(COLOR_BOLD_GREEN " ✔ SUCCESS:" COLOR_RESET " All tests were completed successfully.\n\n");

    return 0;
}