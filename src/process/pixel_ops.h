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

int greyscale(const Image *dst, Image *src, size_t i, size_t j, void *ctx);
int invert(const Image *dst, Image *src, size_t i, size_t j, void *ctx);
int threshold(const Image *dst, Image *src, size_t i, size_t j, void *ctx);
int brighten(const Image *dst, Image *src, size_t i, size_t j, void *ctx);
int contrast(const Image *dst, Image *src, size_t i, size_t j, void *ctx);