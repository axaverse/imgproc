#include <stdio.h>

#include "io.h"
#include "types.h"

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
    return 0;
}
