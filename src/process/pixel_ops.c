#include "pixel_ops.h"

// ----------------------------------------------------------------
int greyscale(Image *dst, const Image *src, size_t i, size_t j, void *ctx) {
    Pixel p = get_pixel(src, i, j);
    int32_t grey = (int32_t)(0.299 * p.v[0] + 0.587 * p.v[1] + 0.114 * p.v[2]);
    grey = CLAMP(grey, 0, 255);

    Pixel out = {0};
    out.v[0] = out.v[1] = out.v[2] = grey;
    
    out.v[3] = p.v[3]; // preserve alpha if present
    
    set_pixel(dst, i, j, out);
    return ERR_OK;
}

int invert(Image *dst, const Image *src, size_t i, size_t j, void *ctx) {
    Pixel p = get_pixel(src, i, j);
    Pixel out = {0};

    for (size_t c = 0; c < MIN(src->channels, 3); ++c) {
        out.v[c] = 255 - p.v[c];
    }

    out.v[3] = p.v[3]; // preserve alpha if present

    set_pixel(dst, i, j, out);
    return ERR_OK;
}

int threshold(Image *dst, const Image *src, size_t i, size_t j, void *ctx) {
    if (!ctx) {
        return ERR_PROCESSING_FAILED;
    }
    
    greyscale(dst, src, i, j, NULL);

    ThresholdContext *tctx = (ThresholdContext *)ctx;
    int threshold = CLAMP(tctx->threshold, 0, 255);

    Pixel p = get_pixel(dst, i, j);
    Pixel out = {0};

    int32_t val = (p.v[0] >= threshold) ? 255 : 0;
    out.v[0] = out.v[1] = out.v[2] = val;
    
    out.v[3] = p.v[3]; // preserve alpha if present

    set_pixel(dst, i, j, out);
    return ERR_OK;
}

int brighten(Image *dst, const Image *src, size_t i, size_t j, void *ctx) {
    if (!ctx) {
        return ERR_PROCESSING_FAILED;
    }

    BrightnessContext *bctx = (BrightnessContext *)ctx;
    int brightness = bctx->brightness;

    Pixel p = get_pixel(src, i, j);
    Pixel out = {0};

    for (size_t c = 0; c < MIN(src->channels, 3); ++c) {
        out.v[c] = CLAMP(p.v[c] + brightness, 0, 255);
    }

    out.v[3] = p.v[3]; // preserve alpha if present

    set_pixel(dst, i, j, out);
    return ERR_OK;
}

int contrast(Image *dst, const Image *src, size_t i, size_t j, void *ctx) {
    if (!ctx) {
        return ERR_PROCESSING_FAILED;
    }

    ContrastContext *cctx = (ContrastContext *)ctx;
    float factor = cctx->factor;

    Pixel p = get_pixel(src, i, j);
    Pixel out = {0};

    for (size_t c = 0; c < MIN(src->channels, 3); ++c) {
        float new_val = (float)(p.v[c] - 128) * factor + 128;
        out.v[c] = CLAMP((int32_t)new_val, 0, 255);
    }

    out.v[3] = p.v[3]; // preserve alpha if present

    set_pixel(dst, i, j, out);
    return ERR_OK;
}