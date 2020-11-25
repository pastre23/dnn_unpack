#include "draw.h"
#include "fonts/font8x8.h"

#include <stdio.h>

void draw_char(unsigned char *image_buffer, int image_width, int image_height,	int left, int top, const char character, unsigned char color)
{
	if(!image_buffer){
		return;
	}
	
	if (left + 8 >= image_width) {
		return;
	}
	if (top + 8 >= image_height) {
		return;
	}

	for (int h=0; h < 8; h++) {
		for (int w=0; w < 8; w++) {
			int set = font8x8_basic[(int)character][h] & 1 << w;
			int img_index = (top + h) * image_width + (left + w);
			image_buffer[img_index] = set ? color : image_buffer[img_index];
		}
	}
}

void draw_text(unsigned char *image_buffer, int image_width, int image_height, int left, int top, const char *text, unsigned char color)
{
	if(!image_buffer){
		return;
	}

	for (int c = 0; text[c] != '\0'; c++) {
		if( text[c] == '\n'){
			top += 8;
			left = 0;
			continue;
		}
		draw_char(image_buffer, image_width, image_height, left, top, text[c], color);
		left += 8;
	}
}

void draw_rect(unsigned char *image_buffer, int image_width, int image_height, int left, int top, int right, int bottom, unsigned char color, unsigned char line_width)
{
	int sty, endy, stx, endx;
	int temp, width, height;
	unsigned char halfline = (line_width >> 1);

	if(!image_buffer){
		return;
	}

	if (left > right) {
		temp = right;
		right = left;
		left = temp;
	}
	if (top > bottom) {
		temp = bottom;
		bottom = top;
		top = temp;
	}

	if ((left < 0) || (right > image_width)) {
		return;
	}
	if ((top < 0) || (bottom > image_height)) {
		return;
	}

	width = right - left;
	height = bottom - top;

	// h_line
	sty  = MAX(top - halfline, 0);
	endy = MIN(top + halfline, bottom - 1);
	for(int y = sty; y <= endy; y++)
	{
		for(int x = left; x <= right; x++)
		{
			image_buffer[ x + ( y           * image_width)] = color;
			image_buffer[ x + ((y + height) * image_width)] = color;
		}
	}
	
	// v_line
	stx  = MAX(left - halfline, 0);
	endx = MIN(left + halfline, right - 1);
	for(int x = stx; x <= endx; x++)
	{
		for(int y = top; y <= bottom; y++)
		{
			image_buffer[ x          + (y * image_width)] = color;
			image_buffer[(x + width) + (y * image_width)] = color;
		}
	}
}

