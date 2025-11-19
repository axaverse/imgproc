#include <stdio.h>
#include "cli.h"
#include "config.h"
#include "io.h"
#include "types.h"

void resolve_output_path(config_t *cfg) {

    size_t len = strlen(cfg->output_path);
    if (cfg->output_path[len - 1] == '/' || cfg->output_path[len - 1] == '\\') {

        const char *in = cfg->input_path;
        const char *slash = strrchr(in, '/');
        if (!slash) slash = strrchr(in, '\\');

        const char *filename = slash ? slash + 1 : in;

        strncat(cfg->output_path, filename, PATH_MAX - strlen(cfg->output_path) - 1);
    }
}

err_t brighten_image(
    const image_t *img, image_t *out,
    const BrightnessContext *ctx
) {
    return map_image(
        out, img,
        &brighten,
        (void*)ctx
    );
}

err_t contrast_image(
    const image_t *img, image_t *out,
    const ContrastContext *ctx
) {
    return map_image(
        out, img,
        &contrast,
        (void*)ctx
    );
}

err_t threshold_image(
    const image_t *img, image_t *out,
    const ThresholdContext *ctx
) {
    return map_image(
        out, img,
        &threshold,
        (void*)ctx
    );
}

err_t grayscale_image(
    const image_t *img, image_t *out
) {
    return map_image(
        out, img,
        &greyscale,
        NULL
    );
}

err_t invert_image(
    const image_t *img, image_t *out
) {
    return map_image(
        out, img,
        &invert,
        NULL
    );
}

err_t apply_filter(
    const image_t *img, image_t *out,
    const FilterSpec *filter
) {
    if (!img || !filter)
        return ERR_INVALID_ARGUMENTS;

    err_t err = ERR_OK;

    switch (filter->type) {
    case FILTER_BRIGHTEN:
        err = create_image(out, img->m, img->n, img->channels);
        if (err != ERR_OK) return err;
        return brighten_image(
            img, out,
            &filter->ctx.brightenCtx
        );
    case FILTER_CONTRAST:
        err = create_image(out, img->m, img->n, img->channels);
        if (err != ERR_OK) return err;
        return contrast_image(
            img, out,
            &filter->ctx.contrastCtx
        );
    case FILTER_THRESHOLD:
        err = create_image(out, img->m, img->n, img->channels);
        if (err != ERR_OK) return err;
        return threshold_image(
            img, out,
            &filter->ctx.thresholdCtx
        );
    case FILTER_GRAYSCALE:
        err = create_image(out, img->m, img->n, img->channels);
        if (err != ERR_OK) return err;
        return grayscale_image(img, out);
    case FILTER_INVERT:
        err = create_image(out, img->m, img->n, img->channels);
        if (err != ERR_OK) return err;
        return invert_image(img, out);
    default:
        return ERR_INVALID_ARGUMENTS;
    }
}

err_t apply_filters(
    const image_t *img, image_t *out,
    const config_t *config
) {
    image_t curr_in = *img;
    image_t curr_out = {0};

    for (size_t i = 0; i < config->count; ++i) {
        curr_out.data = NULL;

        err_t err = apply_filter(
            &curr_in, &curr_out,
            &config->filters[i]
        );

        if (err != ERR_OK) {
            if (curr_out.data) {
                free_image(&curr_out);
            }
            return err;
        }

        if (i > 0) {
            free_image(&curr_in);
        }

        curr_in = curr_out;
    }

    *out = curr_out;
    return ERR_OK;
}

err_t run_from_config(config_t* config) {
    Image img = {0};
    err_t err = ERR_OK;

    err = load_image(&img, config->input_path);
    if (err != ERR_OK) {
        return err;
    }

    err = apply_filters(&img, &img, config);
    if (err != ERR_OK) {
        free_image(&img);
        return err;
    }

    err = store_image(config->output_path, &img);
    free_image(&img);
    
    return err;
}

void init_report(Report *r) {
    memset(r, 0, sizeof(*r));
}

void report_update(Report *r, err_t err) {
    r->total_images++;

    if (err == ERR_OK) {
        r->successful++;
        return;
    }

    r->failed++;

    if (err & ERR_READ_FAILED)         r->read_failures++;
    if (err & ERR_WRITE_FAILED)        r->write_failures++;
    if (err & ERR_MEMORY_ALLOCATION)   r->memory_failures++;
    if (err & ERR_PERMISSION_DENIED)   r->permission_errors++;
    if (err & ERR_DIRECTORY_NOT_FOUND) r->missing_directories++;
    if (err & ERR_PROCESSING_FAILED)   r->transform_failures++;
}

void print_report(const Report *r) {
    printf("\n==================== PROCESS REPORT ====================\n");

    printf("Total images processed : %d\n", r->total_images);
    printf("Successful             : %d\n", r->successful);
    printf("Failed                 : %d\n", r->failed);

    if (r->failed > 0) {
        printf("\n----------------------- ERRORS -------------------------\n");
        if (r->read_failures)
            printf("Read failures          : %d\n", r->read_failures);
        if (r->write_failures)
            printf("Write failures         : %d\n", r->write_failures);
        if (r->memory_failures)
            printf("Memory allocation errs : %d\n", r->memory_failures);
        if (r->permission_errors)
            printf("Permission errors      : %d\n", r->permission_errors);
        if (r->missing_directories)
            printf("Missing directory errs : %d\n", r->missing_directories);
        if (r->transform_failures)
            printf("Transform failures     : %d\n", r->transform_failures);
    }

    printf("=========================================================\n");
}

int main(int argc, char **argv) {
    cli_args_t cli;
    cli_init(&cli);
    
    config_t config;
    err_t err = cli_parse(&cli, argc, argv, &config);
    if (err != ERR_OK) {
        cli_free(&cli);
        return 1;
    }
    
    resolve_output_path(&config);
    
    Report report;
    init_report(&report);
    err = run_from_config(&config);
    report_update(&report, err);

    print_report(&report);
    cli_free(&cli);

    return (err == ERR_OK) ? 0 : 1;
}
