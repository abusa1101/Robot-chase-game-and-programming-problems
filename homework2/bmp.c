#include "bmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t bmp_calculate_size(bitmap_t *bmp) {
    size_t size = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 921600;
    return size;
}

void bmp_serialize(bitmap_t *bmp, uint8_t *data) {
    BITMAPFILEHEADER file_header = {0};
    file_header.bfType = 0x4d42;
    file_header.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 921600;
    file_header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    file_header.bfReserved1 = 0;
    file_header.bfReserved2 = 0;

    BITMAPINFOHEADER info_header = {0};
    info_header.biSize = sizeof(BITMAPINFOHEADER);
    info_header.biWidth = 640;
    info_header.biHeight = 480;
    info_header.biPlanes = 1;
    info_header.biBitCount = 24;
    info_header.biCompression = 0;
    info_header.biSizeImage = 0;
    info_header.biXPelsPerMeter = 2835;
    info_header.biYPelsPerMeter = 2835;
    info_header.biClrUsed = 0;
    info_header.biClrImportant = 0;

    uint8_t *data_out = data;
    memcpy(data_out, &file_header, sizeof(file_header));
    data_out += sizeof(file_header);
    memcpy(data_out, &info_header, sizeof(info_header));
    data_out += sizeof(info_header);

    for (int i = bmp->height - 1; i >= 0; i--) {
        memcpy(data_out, &bmp->data[i * bmp->width], bmp->width * sizeof(color_bgr_t));
        data_out += bmp->width * sizeof(color_bgr_t);
    }
}
