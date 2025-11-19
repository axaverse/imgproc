#pragma once

/**
 * The IO module is crucial for resource management and user interaction. It is 
 * responsible for:
 * - load/save image files in various formats
 * - parse the command line arguments and create the configuration object
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include "types.h"

/**
 * Loads an image given a path.
 * @param img Pointer to an Image structure to populate.
 * @param path Path to the image file.
 * @return Error code indicating success or failure. (Check bits from ErrorCode enum)
 *
 * Precondition:
 * - Path is a valid image file (e.g., .png, .jpg).
 * - img is not NULL.
 *
 * Postcondition:
 * - img is populated with image data, dimensions, and channels.
 * - img->data is dynamically allocated and should be freed by the caller using free_image().
 * - img->m > 0, img->n > 0, img->channels > 0
 * - img->channels <= 4 (stb_image limitation)
 *
 * Possible error codes:
 * - ERR_FILE_NOT_FOUND: If the file does not exist.
 * - ERR_PERMISSION_DENIED: If the file cannot be accessed due to permission issues.
 * - ERR_READ_FAILED: If the file cannot be read or is corrupted.
 * - ERR_UNSUPPORTED_FORMAT: If the file format is not supported.
 * - ERR_MEMORY_ALLOCATION: If memory allocation for image data fails.
 * - ERR_DIRECTORY_NOT_FOUND: If the directory in the path does not exist.
 * - ERR_UNKNOWN: For any other unspecified errors.
 */
int load_image(Image* img, const char* path);

/**
 * Stores an image to a given path.
 * @param path Path to save the image file.
 * @param img The image structure to save.
 * @return Error code indicating success or failure. (Check bits from ErrorCode enum)
 * 
 * Precondition: 
 * - img->data is not NULL.
 * - img->m > 0, img->n > 0, img->channels > 0
 * - img->channels <= 4 (stb_image_write limitation)
 * - Path is a valid writable location.
 * - The file extension in path determines the image format (e.g., .png, .jpg).
 * 
 * Postcondition:
 * - The image is saved to the specified path in the appropriate format with return value 0.
 * - If the operation fails, the function returns a non-zero error code and no file is created/modified.
 * 
 * Possible error codes:
 * - ERR_PERMISSION_DENIED: If the file cannot be written due to permission issues.
 * - ERR_WRITE_FAILED: If the file cannot be written (e.g., disk full, invalid path).
 * - ERR_UNSUPPORTED_FORMAT: If the file format is not supported for writing.
 * - ERR_MEMORY_ALLOCATION: If memory allocation fails during the write process.
 * - ERR_DIRECTORY_NOT_FOUND: If the directory in the path does not exist.
 * - ERR_UNKNOWN: For any other unspecified errors.
 */
int store_image(const char* path, const Image* img);