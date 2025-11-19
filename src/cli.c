#include "cli.h"
#include <stdio.h>
#include <string.h>

void cli_init(cli_args_t *cli) {
    memset(cli, 0, sizeof(*cli));

    // ---- module name ----
    cli->module = arg_str1(NULL, NULL, "MODULE", "module to run (e.g., enhance)");

    // ---- enhance filters ----
    cli->brightness = arg_str0(NULL, "brightness", "VAL",  "brightness amount");
    cli->contrast   = arg_str0(NULL, "contrast",   "VAL",  "contrast amount");
    cli->grayscale  = arg_lit0(NULL, "grayscale",          "grayscale filter");
    cli->invert     = arg_lit0(NULL, "invert",             "invert filter");
    cli->threshold  = arg_str0(NULL, "threshold",  "VAL",  "threshold amount");

    // ---- files ----
    cli->input      = arg_file1(NULL, NULL, "INPUT",  "input image");
    cli->output     = arg_file1(NULL, NULL, "OUTPUT", "output image");

    // ---- end of table ----
    cli->end        = arg_end(20);

    // ---- build table ----
    cli->table[0] = cli->module;
    cli->table[1] = cli->brightness;
    cli->table[2] = cli->contrast;
    cli->table[3] = cli->grayscale;
    cli->table[4] = cli->invert;
    cli->table[5] = cli->threshold;
    cli->table[6] = cli->input;
    cli->table[7] = cli->output;
    cli->table[8] = cli->end;
}

void cli_free(cli_args_t *cli) {
    arg_freetable(cli->table, sizeof(cli->table) / sizeof(cli->table[0]));
}

/**
 * Ensures exactly one filter option is provided.
 */
static int count_filters(cli_args_t *cli) {
    return cli->brightness->count +
           cli->contrast->count +
           cli->grayscale->count +
           cli->invert->count +
           cli->threshold->count;
}

err_t cli_parse(cli_args_t *cli, int argc, char **argv, config_t *cfg) {
    int nerrors = arg_parse(argc, argv, cli->table);
    if (nerrors > 0) {
        arg_print_errors(stderr, cli->end, argv[0]);
        fprintf(stderr, "\nUsage:\n");
        arg_print_syntax(stdout, cli->table, "\n");
        fprintf(stderr, "\nOptions:\n");
        arg_print_glossary(stdout, cli->table, "  %-25s %s\n");
        return ERR_INVALID_ARGUMENTS;
    }

    const char *module = cli->module->sval[0];

    if (strcmp(module, "enhance") == 0) {
        int enhance_count = cli->brightness->count + cli->contrast->count +
                            cli->grayscale->count + cli->invert->count +
                            cli->threshold->count;

        if (enhance_count == 0) {
            fprintf(stderr, "Error: 'enhance' module requires at least one filter.\n");
            return ERR_INVALID_ARGUMENTS;
        }

        return build_config_from_args(
            cfg,
            cli->brightness,
            cli->grayscale,
            cli->contrast,
            cli->invert,
            cli->threshold,
            cli->input,
            cli->output);
    }
    else {
        fprintf(stderr, "Error: Unknown module '%s'\n", module);
        return ERR_INVALID_ARGUMENTS;
    }

    return ERR_OK;
}
