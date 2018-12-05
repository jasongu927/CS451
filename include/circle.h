#ifndef CIRCLE_H

#define CIRCLE_H

#include "image.h"
#include "point.h"
#include "color.h"

typedef struct{
	double r;
	Point c;
}Circle;


void circle_set(Circle *c, Point tc, double tr);

void circle_draw(Circle *c, Image *src, Color p);

void circle_drawFill(Circle *c, Image *src, Color p);

#endif