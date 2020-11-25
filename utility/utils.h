#ifndef _UTILS_HEADER_
#define _UTILS_HEADER_

#pragma pack(1)
typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} rgb_buffer_t;

#pragma pack(1)
typedef struct {
    unsigned char b;
    unsigned char g;
    unsigned char r;
} bgr_buffer_t; 

#define RGB(r, g, b) (((r & 0x0000FF) << 16) | ((g & 0x0000FF) << 8) | ((b & 0x0000FF)))
#define BGR(r, g, b) (((b & 0x0000FF) << 16) | ((g & 0x0000FF) << 8) | ((r & 0x0000FF)))

#define R(rgb) ((rgb & 0xFF00FF) >> 16)
#define G(rgb) ((rgb & 0x00FF00) >> 8)
#define B(rgb) ((rgb & 0x0000FF))

#endif