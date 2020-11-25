#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"

int to_bitmap(char* filename, bgr_buffer_t *buffer, int width, int height)
{
    bitmap_header_t bmp_head;
    bitmap_info_t bmp_info;
    int size = (int)(height * width * sizeof(bgr_buffer_t));

    // Initial head
    bmp_head.bfType = 0x4D42; // 'BM'
    bmp_head.bfSize = sizeof(bitmap_header_t) + sizeof(bitmap_info_t) + size;
    bmp_head.bfReserved1 = 0;
    bmp_head.bfReserved2 = 0;
    bmp_head.bfOffBits = bmp_head.bfSize - size;

    // Info head;
    bmp_info.biSize = sizeof(bitmap_info_t);
    bmp_info.biWidth = width;
    bmp_info.biHeight = height;
    bmp_info.biPlanes = 1;
    bmp_info.biBitCount = sizeof(bgr_buffer_t) * 8; // TrueColor 24bit
    bmp_info.biCompression = 0; // NO_COMPRESION
    bmp_info.biSizeImage = size;
    bmp_info.biXPelsPerMeter = 11811; //300 dpi
    bmp_info.biYPelsPerMeter = 11811; //300 dpi
    bmp_info.biClrUsed = 0; // MAX_NUMBER_OF_COLORS
    bmp_info.biClrImportant = 0; // ALL_COLORS_REQUIRED

    // copy the data
    FILE *fp;
    if (!(fp = fopen(filename, "wb"))) return -1;

    fwrite(&bmp_head, 1, sizeof(bitmap_header_t), fp);
    fwrite(&bmp_info, 1, sizeof(bitmap_info_t), fp);
    fwrite(&buffer[0], 1, size, fp);

    fclose(fp);

    return 0;
}

int to_ppm(char* filename, rgb_buffer_t *buffer, int width, int height)
{
    int size = (int)(height * width* sizeof(rgb_buffer_t));

    // copy the data
    FILE *fp;
    if (!(fp = fopen(filename, "wb"))) return -1;

    fprintf(fp, "P6\n%d %d\n255\n", width, height);    
    fwrite(&buffer[0], 1, size, fp);

    fclose(fp);

    return 0;
}