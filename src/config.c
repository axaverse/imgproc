#include "config.h"

float smoothContrastFactor(int contrast) {
    // contrast in [-255, 255]
    return (259.0f * (contrast + 255.0f)) / (255.0f * (259.0f - contrast));
}

err_t build_config_from_args(
    config_t *cfg,
    struct arg_str *brightness,
    struct arg_lit *grayscale,
    struct arg_str *contrast,
    struct arg_lit *invert,
    struct arg_str *threshold,
    struct arg_file *input,
    struct arg_file *output
) {
    if (!cfg) return ERR_INVALID_ARGUMENTS;

    memset(cfg, 0, sizeof(*cfg));

    // input/output paths
    strncpy(cfg->input_path, input->filename[0], PATH_MAX - 1);
    strncpy(cfg->output_path, output->filename[0], PATH_MAX - 1);

    cfg->input_path[PATH_MAX - 1] = '\0';
    cfg->output_path[PATH_MAX - 1] = '\0';

    // filters
    if (brightness->count > 0) {
        cfg->filters[cfg->count].type = FILTER_BRIGHTEN;
        cfg->filters[cfg->count].ctx.brightenCtx.brightness =
            atoi(brightness->sval[0]);
        cfg->count++;
    }

    if (grayscale->count > 0) {
        cfg->filters[cfg->count].type = FILTER_GRAYSCALE;
        cfg->count++;
    }

    if (contrast->count > 0) {
        cfg->filters[cfg->count].type = FILTER_CONTRAST;
        cfg->filters[cfg->count].ctx.contrastCtx.factor =
            smoothContrastFactor(CLAMP(atoi(contrast->sval[0]), -255, 255));
        cfg->count++;
    }

    if (invert->count > 0) {
        cfg->filters[cfg->count].type = FILTER_INVERT;
        cfg->count++;
    }

    if (threshold->count > 0) {
        cfg->filters[cfg->count].type = FILTER_THRESHOLD;
        cfg->filters[cfg->count].ctx.thresholdCtx.threshold =
            atoi(threshold->sval[0]);
        cfg->count++;
    }

    if (cfg->count == 0)
        return ERR_INVALID_ARGUMENTS;

    return ERR_OK;
}

