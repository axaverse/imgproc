#include "process.h"

// ----------------------------------------------------------------
int map_image(Image *dst, const Image *src, PixelProcessor func, void* ctx) {
    int error = 0;

    if (!src || !dst || !func || !src->data || !dst->data) {
        return ERR_INVALID_ARGUMENTS | ERR_PROCESSING_FAILED;
    }

    for (size_t i = 0; i < src->m; ++i) {
        for (size_t j = 0; j < src->n; ++j) {
            int ret = func(src, dst, i, j, ctx);
            error |= (ret & ~ERR_OK) ? ERR_PROCESSING_FAILED : 0x0;
        }
    }

    return error;
}

Pixel get_pixel(const Image* img, size_t i, size_t j) {
    Pixel p = {0};

    if (!img || !img->data || i >= img->m || j >= img->n) {
        return p; // return zeroed pixel on error
    }

    size_t idx = (i * img->n + j) * img->channels;
    
    for (size_t c = 0; c < img->channels; ++c) {
        p.v[c] = img->data[idx + c];
    }
    
    return p;
}

void set_pixel(Image* img, size_t i, size_t j, const Pixel p) {
    if (!img || !img->data || i >= img->m || j >= img->n) {
        return; // do nothing on error
    }

    size_t idx = (i * img->n + j) * img->channels;

    for (size_t c = 0; c < img->channels; ++c) {
        img->data[idx + c] = (int32_t)p.v[c];
    }
}