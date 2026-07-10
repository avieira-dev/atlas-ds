/*
 * AtlasDS
 * Status Codes
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#ifndef ATLAS_STATUS_H
#define ATLAS_STATUS_H

/**
 * @brief Generic success status.
 *
 * Indicates that an operation completed successfully.
 */
#define ATLAS_SUCCESS 0

/**
 * @brief Null pointer error.
 *
 * Indicates that one or more required pointers are NULL.
 */
#define ATLAS_ERROR_NULL (-1)

/**
 * @brief Index out-of-bounds error.
 *
 * Indicates that the specified index is outside the valid range.
 */
#define ATLAS_ERROR_BOUNDS (-2)

/**
 * @brief Empty container error.
 *
 * Indicates that the requested operation cannot be performed
 * because the container has no stored elements.
 */
#define ATLAS_ERROR_EMPTY (-3)

/**
 * @brief Memory allocation error.
 *
 * Indicates that a memory allocation or reallocation operation failed.
 */
#define ATLAS_ERROR_MEMORY (-4)

/**
 * @brief Type mismatch error.
 *
 * Indicates that the provided data type is incompatible with
 * the requested operation.
 */
#define ATLAS_ERROR_TYPE (-5)

/**
 * @brief Element not found.
 *
 * Indicates that the requested element does not exist in the container.
 */
#define ATLAS_ERROR_NOT_FOUND (-6)

/**
 * @brief Invalid argument error
 * 
 * Indicates that a provided argument is invalid, illegal,
 * or incompatible with the function's requirements.
 */
#define ATLAS_ERROR_INVALID_ARGUMENT (-7)

#endif