#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "io.h"

/**
 * Future extensions: 
 * - support for more image formats (JPEG, BMP, etc.)
 * - Support 4 channels (RGBA) in addition to 3 (RGB)
 */

// ----------------------------------------------------------------
int load_image(Image* img, const char* path) {
    int error = 0;

    int width, height, channels;
    unsigned char *data = stbi_load(path, &width, &height, &channels, 3); // force 3 channels (RGB)
    error |= (data == NULL) ? ERR_READ_FAILED : 0;

    if (error == 0) {
        img->data = (int32_t *)malloc(width * height * channels * sizeof(int32_t));
        error |= (img->data == NULL) ? ERR_MEMORY_ALLOCATION : 0;
    }

    if (error == 0) {
        img->m = height;
        img->n = width;
        img->channels = channels;
        img->format = "png"; // force png for now
        
        size_t totalPixels = width * height * channels;
        for (size_t i = 0; i < totalPixels; ++i) { img->data[i] = (int32_t)data[i]; }
    }

    stbi_image_free(data);
    return error;
}

int store_image(const char* path, const Image* img) {
    int error = 0;

    unsigned char* data = (unsigned char *)malloc(img->m * img->n * img->channels);
    error |= (data == NULL) ? ERR_MEMORY_ALLOCATION : 0;

    if (error == 0) {
        size_t totalPixels = img->m * img->n * img->channels;
        for (size_t i = 0; i < totalPixels; ++i) { data[i] = (unsigned char)(img->data[i]); }
    }

    if (error == 0) {
        int success = stbi_write_png(path, img->n, img->m, img->channels, data, img->n * img->channels * sizeof(int32_t));
        error |= (success == 0) ? ERR_WRITE_FAILED : 0;
    }

    free(data);
    return error;
}

void free_image(Image* img) {
    if (img && img->data) {
        free(img->data);
        img->data = NULL;
        img->m = 0;
        img->n = 0;
        img->channels = 0;
        img->format = NULL;
    }
}