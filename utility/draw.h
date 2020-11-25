#ifndef _DRAW_HEADER_
#define _DRAW_HEADER_

#define MIN(x, y)   ((x) < (y) ? (x) : (y))
#define MAX(x, y)   ((x) > (y) ? (x) : (y))

void draw_char(unsigned char *image_buffer, int image_width, int image_height, int left, int top, const char character, unsigned char color);
void draw_text(unsigned char *image_buffer, int image_width, int image_height, int left, int top, const char *text, unsigned char color);
void draw_rect(unsigned char *image_buffer, int image_width, int image_height, int left, int top, int right, int bottom, unsigned char color, unsigned char line_width);

#endif