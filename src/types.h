#pragma once

/**
 * This header defines useful types and constants used throughout the project.
*/

#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define MAX_CHANNELS 4 // supports RGBA

typedef struct {
    int32_t *data;
    size_t m, n;
    size_t channels;
    const char *format; // supports .png for now
} Image;

typedef Image image_t;

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
    int32_t v[MAX_CHANNELS];
} Pixel;

typedef Pixel pixel_t;

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define CLAMP(x, lower, upper) (MIN(upper, MAX(x, lower)))


/**
 * Creates an image with the specified dimensions and number of channels.
 * Precondition:
 * - img is not NULL.
 * - m > 0
 * - n > 0
 * - 0 < channels <= MAX_CHANNELS
 * 
 * Postcondition:
 * - img is populated with allocated data of size m * n * channels.
 * - img->m == m, img->n == n, img->channels == channels.
 * - img->data is dynamically allocated and should be freed by the caller using free_image().
 * - Returns ERR_OK on success, or an appropriate error code on failure.
 */
int create_image(Image* img, size_t m, size_t n, size_t channels);

/**
 * Frees the memory allocated for an image.
 * @param img The image structure to free.
 * 
 * Precondition:
 * - img is not NULL.
 * - img->data is not NULL.
 * 
 * Postcondition:
 * - The memory allocated for img->data is freed.
 * - img->data is set to NULL to avoid dangling pointers.
 * - img itself is not freed; the caller is responsible for freeing img if it was dynamically allocated.
 */
void free_image(Image* img);
