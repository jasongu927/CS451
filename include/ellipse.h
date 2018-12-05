#ifndef ELLIPSE_H

#define ELLIPSE_H

#include "image.h"
#include "point.h"
#include "color.h"

typedef struct{
	double ra;
	double rb;
	Point c;
	double a;
}Ellipse;

void ellipse_set(Ellipse *e, Point tc, double ta, double tb);

void ellipse_draw(Ellipse *e, Image *src, Color p);

void ellipse_drawFill(Ellipse *e, Image *src, Color p);

#endif