#ifndef DSA_COMMON_H
#define DSA_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/**
 * Status codes returned by data structure operations.
 * Allows safe error checking without crashing or silent failures.
 */
typedef enum {
    STATUS_OK = 0,
    STATUS_ERR_NULL_PTR = -1,
    STATUS_ERR_ALLOC = -2,
    STATUS_ERR_EMPTY = -3,
    STATUS_ERR_NOT_FOUND = -4,
    STATUS_ERR_OUT_OF_BOUNDS = -5,
    STATUS_ERR_DUPLICATE = -6,
    STATUS_ERR_INVALID_ARG = -7
} StatusCode;

/**
 * Convert StatusCode to human-readable string.
 */
static inline const char* status_to_string(StatusCode code) {
    switch (code) {
        case STATUS_OK:                  return "STATUS_OK: Operation completed successfully";
        case STATUS_ERR_NULL_PTR:        return "STATUS_ERR_NULL_PTR: Null pointer provided";
        case STATUS_ERR_ALLOC:           return "STATUS_ERR_ALLOC: Memory allocation failed";
        case STATUS_ERR_EMPTY:           return "STATUS_ERR_EMPTY: Data structure is empty";
        case STATUS_ERR_NOT_FOUND:       return "STATUS_ERR_NOT_FOUND: Element not found";
        case STATUS_ERR_OUT_OF_BOUNDS:   return "STATUS_ERR_OUT_OF_BOUNDS: Index out of range";
        case STATUS_ERR_DUPLICATE:       return "STATUS_ERR_DUPLICATE: Duplicate element not allowed";
        case STATUS_ERR_INVALID_ARG:     return "STATUS_ERR_INVALID_ARG: Invalid argument";
        default:                         return "STATUS_ERR_UNKNOWN: Unknown error";
    }
}

#endif /* DSA_COMMON_H */
