#include <stdio.h>

#include "io.h"
#include "types.h"
#include "process/pixel_ops.h"
#include "process/process.h"

void dump_image(const Image* img) {
    if (!img) { return; }

    printf("Image format: %s\n", img->format);
    printf("Image dimensions: %zu x %zu\n", img->n, img->m);
    printf("Number of channels: %zu\n", img->channels);

    for (size_t i = 0; i < img->m; ++i) {
        for (size_t j = 0; j < img->n; ++j) {
            printf("[");
            for (size_t c = 0; c < img->channels; ++c) {
                size_t idx = (i * img->n + j) * img->channels + c;

                printf("%3d", img->data[idx]);
                if (c < img->channels - 1) { printf(", "); }
            }
            printf("] ");
        }
        printf("\n");
    }
}

int main() {
    // let's test the dump_image function by just entering a sample array
    Image img;
    img.m = 2;
    img.n = 3;
    img.channels = 3;
    img.format = "png";

    img.data = (int32_t *)malloc(img.m * img.n * img.channels * sizeof(int32_t));
    for (size_t i = 0; i < img.m * img.n * img.channels; ++i) {
        img.data[i] = (int32_t)(i * 10);
    }

    dump_image(&img);
    free_image(&img);

    // -------------------------------------------------
    // test_images/bg2.png

    int err = load_image(&img, "test_images/bg2.png");
    if (err) {
        printf("Failed to load image: %d\n", err);
        return err;
    }

    // dump_image(&img);

    Image out;
    out.channels = img.channels;
    out.m = img.m;
    out.n = img.n;
    out.format = img.format;

    out.data = (int32_t *)malloc(out.m * out.n * out.channels * sizeof(int32_t));

    err = map_image(&out, &img, invert, NULL);

    if (err) {
        printf("Failed to process image: %d\n", err);
        free_image(&img);
        free_image(&out);
        return err;
    }

    // dump_image(&out);

    store_image("test_images/out_invert.png", &out);

    free_image(&out);
    free_image(&img);

    return 0;
}
