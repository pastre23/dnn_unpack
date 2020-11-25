#ifndef _BMP_HEADER_
#define _BMP_HEADER_

#include "utils.h"

#pragma pack(2)
typedef struct                       /**** BMP file header structure ****/
{
    unsigned short bfType;           /* Magic Key */
    unsigned int   bfSize;           /* Size of file */
    unsigned short bfReserved1;      /* Reserved */
    unsigned short bfReserved2;      /* Reserved */
    unsigned int   bfOffBits;        /* Offset to bitmap data */
} bitmap_header_t;

#pragma pack(2)
typedef struct                       /**** BMP file info structure ****/
{
    unsigned int   biSize;           /* Size of info header */
    int            biWidth;          /* Width of image */
    int            biHeight;         /* Height of image */
    unsigned short biPlanes;         /* Number of color planes */
    unsigned short biBitCount;       /* Number of bits per pixel */
    unsigned int   biCompression;    /* Type of compression to use */
    unsigned int   biSizeImage;      /* Size of image data */
    int            biXPelsPerMeter;   /* X pixels per meter */
    int            biYPelsPerMeter;   /* Y pixels per meter */
    unsigned int   biClrUsed;        /* Number of colors used */
    unsigned int   biClrImportant;   /* Number of important colors */
} bitmap_info_t;

int to_bitmap(char* filename, bgr_buffer_t *buffer, int width, int height);
int to_ppm(char* filename, rgb_buffer_t *buffer, int width, int height);

#endif