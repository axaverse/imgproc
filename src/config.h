#pragma once

#include <argtable3/argtable3.h>
#include "types.h"
#include "process/process.h"
#include "process/pixel_ops.h"

typedef enum {
    FILTER_NONE = 0,
    FILTER_GRAYSCALE,
    FILTER_INVERT,
    FILTER_THRESHOLD,
    FILTER_BRIGHTEN,
    FILTER_CONTRAST
} FilterType;

typedef struct {
    FilterType type;
    union {
        BrightnessContext brightenCtx;
        ContrastContext contrastCtx;
        ThresholdContext thresholdCtx;
    } ctx;
} FilterSpec;

#define MAX_FILTERS 10
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

typedef struct {
    char input_path[PATH_MAX];
    char output_path[PATH_MAX];
    FilterSpec filters[MAX_FILTERS];
    size_t count;
} Config;

typedef Config config_t;

err_t build_config_from_args(
    config_t *config,
    struct arg_str *brightness,
    struct arg_lit *grayscale,
    struct arg_str *contrast,
    struct arg_lit *invert,
    struct arg_str *threshold,
    struct arg_file *input,
    struct arg_file *output
);