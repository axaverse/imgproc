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
#define MAX_CHANNELS 4 // supports RGBA

typedef struct {
    int32_t *data;
    size_t m, n;
    size_t channels;
    const char *format; // supports .png for now
} Image;

typedef Image image_t;

typedef struct {
    char inputPath[PATH_MAX];
    char outputPath[PATH_MAX];

    // more options to be added later
} config_t;

typedef enum {
    ERR_OK = 0,
    ERR_FILE_NOT_FOUND = 1 << 0,
    ERR_PERMISSION_DENIED = 1 << 1,
    ERR_READ_FAILED = 1 << 2,
    ERR_WRITE_FAILED = 1 << 3,
    ERR_MEMORY_ALLOCATION = 1 << 4,
    ERR_UNSUPPORTED_FORMAT = 1 << 5,
    ERR_DIRECTORY_NOT_FOUND = 1 << 6,
    ERR_UNKNOWN = 1 << 7, 
    ERR_INVALID_ARGUMENTS = 1 << 8,
    ERR_PROCESSING_FAILED = 1 << 9,
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
    int64_t v[MAX_CHANNELS];
} Pixel;

typedef Pixel pixel_t;

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define CLAMP(x, lower, upper) (MIN(upper, MAX(x, lower)))