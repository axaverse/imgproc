#pragma once

/**
 * This header defines useful types and constants used throughout the project.
*/

#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>

#define PATH_MAX 4096

typedef struct {
    int32_t *data;
    size_t m, n;
    size_t channels;
} Image;

typedef Image image_t;

typedef struct {
    char inputPath[PATH_MAX];
    char outputPath[PATH_MAX];

    // more options to be added later
} config_t;

typedef enum {
    ERR_OK = 0,
    ERR_FILE_NOT_FOUND,
    ERR_PERMISSION_DENIED,
    ERR_READ_FAILED,
    ERR_WRITE_FAILED,
    ERR_MEMORY_ALLOCATION,
    ERR_UNSUPPORTED_FORMAT,
    ERR_DIRECTORY_NOT_FOUND,
    ERR_UNKNOWN, 
    ERR_INVALID_ARGUMENTS
} ErrorCode;

typedef ErrorCode err_t;

typedef struct {
    int total_images;
    int successful;
    int failed;
    int read_failures;
    int write_failures;
    int transform_failures;
    int memory_failures;
    int permission_errors;
    int missing_directories;
} Report;

typedef Report report_t;

typedef struct {
    ErrorCode code;
    Image* img;
} ImageResult;

typedef ImageResult result_t;