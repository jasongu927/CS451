#ifndef LINE_H

#define LINE_H

#include "image.h"
#include "point.h"
#include "color.h"


typedef struct{
	int zBuffer;
	Point a;
	Point b;
} Line;

void line_set2D(Line *l, double x0, double y0, double x1, double y1);

void line_set(Line *l, Point ta, Point tb);

void line_zBuffer(Line *l, int flag);

void line_normalize(Line *l);

void line_copy(Line *to, Line *from);

int clip_line(Line *l, Image *src, Color c, Line *res);

void line_draw(Line *l, Image *src, Color c);

void line_draw_dashes(Line *l, Image *src, Color c, int dLength);

#endif