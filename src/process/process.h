#pragma once

#include <stdlib.h>
#include <stdint.h>

#include "types.h"

/**
 * Given an image, the process module provides functions to manipulate and transform images at various levels (pixel, region, whole image).
 */

//-----------------------------------------------------------------------------------

/**
 * Pixel processing function type. 
 * @param src Pointer to the source Image structure.
 * @param dst Pointer to the destination Image structure.
 * @param i Y coordinate of the pixel to process.
 * @param j X coordinate of the pixel to process.
 * @param ctx Pointer to additional context data (can be NULL).
 * 
 * @return 0 on success, non-zero error code on failure.
 * 
 * Function may mutate the dst image
 */
typedef int (*PixelProcessor)(
    Image *dst, const Image *src, size_t i, size_t j, void *ctx
);

/**
 * Transforms the source image using the provided pixel processing function and stores the result in the destination image.
 * @param dst Pointer to the destination Image structure.
 * @param src Pointer to the source Image structure.
 * @param func Function pointer to the pixel processing function.
 * @param ctx Pointer to additional context data for the pixel processing function (can be NULL).
 * @return Error code indicating success or failure. (Check bits from ErrorCode enum)
 * 
 * Precondition:
 * - src and dst are not NULL.
 * - src->data and dst->data are not NULL.
 * - func is not NULL. 
 * 
 * Postcondition:
 * - dst is populated with the transformed image data.
 * - If the operation fails, the function returns a non-zero error code. dst is undefined in this case (may be partially modified).
 * 
 * Function may mutate the dst image
 * 
 * Possible error codes:
 * - ERR_INVALID_ARGUMENTS: If any of the input pointers are NULL.
 * - ERR_PROCESSING_FAILED: If the pixel processing function fails for any pixel.
 * - ERR_UNKNOWN: For any other unspecified errors.
 */
int map_image(Image *dst, const Image *src, PixelProcessor func, void* ctx);

/**
 * Gets the pixel at (i, j) from the image.
 * @param img Pointer to the Image structure.
 * @param i Y coordinate of the pixel.
 * @param j X coordinate of the pixel.
 * @return The Pixel at the specified coordinates.
 * 
 * Precondition:
 * - img is not NULL.
 * - img->data is not NULL.
 * - 0 <= i < img->m and 0 <= j < img->n
 * 
 * Postcondition:
 * - Returns the Pixel p at (i, j).
 * - |p.v| = img->channels. Unused channels are set to 0. 
 */
Pixel get_pixel(const Image* img, size_t i, size_t j);

/**
 * Sets the pixel at (i, j) in the image.
 * @param img Pointer to the Image structure.
 * @param i Y coordinate of the pixel.
 * @param j X coordinate of the pixel.
 * @param p The Pixel value to set.
 * 
 * Precondition:
 * - img is not NULL.
 * - img->data is not NULL.
 * - 0 <= j < img->n and 0 <= i < img->m
 * - |p.v| = img->channels. Extra channels in p are ignored.
 * 
 * Postcondition:
 * - The pixel at (i, j) in img is set to the value of p.
 */
void set_pixel(Image* img, size_t i, size_t j, const Pixel p);