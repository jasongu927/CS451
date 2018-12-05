#ifndef COLOR_H

#define COLOR_H

#include "image.h"
#include <stdio.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
//within here we have RED = 0, GREEN = 1, BLUE = 2

//define some equality checks for color
#define COLOR_EQUAL(c0, c1) ((c0).c[RED] == (c1).c[RED] && (c0).c[GREEN] == (c1).c[GREEN] && (c0).c[BLUE] == (c1).c[BLUE]) 

#define COLOR_EQUAL_PIXEL(c, p) ((c).c[RED] == (p).rgb[RED] && (c).c[GREEN] == (p).rgb[GREEN] && (c).c[BLUE] == (p).rgb[BLUE]) 

typedef struct{
	float c[3];
}Color;


void color_copy(Color *to, Color *from);

void color_copy_bounds(Color *to, Color *from);

void color_set(Color *to, float r, float g, float b);

void image_setColor(Image *src, int x, int y, Color val);

void image_setColorSave(Image *src, int x, int y, Color val, int iteration);

Color image_getColor(Image *src, int r, int c);

void color_multiply(Color *a, Color *b, Color *c);

void color_add(Color *a, Color *b, Color *c);

void color_add_bounds(Color *a, Color *b, Color *c);

void color_multiply_float(Color *a, float b, Color *c);

void color_print(Color *c, FILE *fp);

#endif
