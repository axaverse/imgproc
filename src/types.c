#include "types.h"

int create_image(Image* img, size_t m, size_t n, size_t channels) {
    if (!img || m == 0 || n == 0 || channels == 0 || channels > MAX_CHANNELS) {
        return ERR_INVALID_ARGUMENTS;
    }

    img->data = (int32_t *)malloc(m * n * channels * sizeof(int32_t));
    if (!img->data) {
        return ERR_MEMORY_ALLOCATION;
    }

    img->m = m;
    img->n = n;
    img->channels = channels;
    img->format = "png"; // default format

    return ERR_OK;
}

void free_image(Image* img) {
    if (img && img->data) {
        free(img->data);
        img->data = NULL;
        img->m = 0;
        img->n = 0;
        img->channels = 0;
        img->format = NULL;
    }
}