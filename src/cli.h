#pragma once
#include <argtable3/argtable3.h>
#include "config.h"
#include "types.h"

typedef struct {
    struct arg_str *module;  // first positional argument
    
    // Enhance filters (only valid if module == "enhance")
    struct arg_str *brightness;
    struct arg_str *contrast;
    struct arg_lit *grayscale;
    struct arg_lit *invert;
    struct arg_str *threshold;

    // Input/output files
    struct arg_file *input;
    struct arg_file *output;

    struct arg_end *end;

    void *table[16]; // argtable
} cli_args_t;

void cli_init(cli_args_t *cli);
void cli_free(cli_args_t *cli);

/**
 * Parses CLI arguments and fills config struct.
 * Returns ERR_OK or error code.
 */
err_t cli_parse(cli_args_t *cli, int argc, char **argv, config_t *cfg);
