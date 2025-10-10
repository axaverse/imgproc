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
 * @param path Path to the image file.
 * @return The loaded image structure. Must be freed with free_image().
 * 
 * Precondition:
 * - Path is a valid image file (e.g., .png, .jpg).
 * 
 * Postcondition:
 * - The returned image structure has its data allocated and populated.
 * - The caller is responsible for freeing the image using free_image().
 */
ImageResult* load_image(const char* path);

/**
 * Stores an image to a given path.
 * @param path Path to save the image file.
 * @param img The image structure to save.
 * 
 * Precondition: 
 * - img->data is not NULL.
 * - img->m > 0, img->n > 0, img->channels > 0
 * - img->channels <= 4 (stb_image_write limitation)
 * - Path is a valid writable location.
 * - The file extension in path determines the image format (e.g., .png, .jpg).
 * 
 * Postcondition:
 * - The image is saved to the specified path in the appropriate format.
 */
void store_image(const char* path, const Image* img);

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
