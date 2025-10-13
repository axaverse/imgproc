#pragma once

#include <stdlib.h>
#include <stdint.h>
#include "process.h"
#include "types.h"

typedef struct {
    int brightness;
} BrightnessContext;

typedef struct {
    float factor; // >1 increases contrast, <1 decreases contrast
} ContrastContext;

typedef struct {
    int threshold; // 0-255
} ThresholdContext;

int greyscale(Image *dst, const Image *src, size_t i, size_t j, void *ctx);
int invert(Image *dst, const Image *src, size_t i, size_t j, void *ctx);
int threshold(Image *dst, const Image *src, size_t i, size_t j, void *ctx);
int brighten(Image *dst, const Image *src, size_t i, size_t j, void *ctx);
int contrast(Image *dst, const Image *src, size_t i, size_t j, void *ctx);