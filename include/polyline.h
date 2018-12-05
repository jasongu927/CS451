#ifndef POLYLINE_H

#define POLYLINE_H

#include <stdio.h>
#include "image.h"
#include "point.h"
#include "color.h"
#include "line.h"

typedef struct{
	int zBuffer;
	int numVertex;
	Point *vertex;
} Polyline;



Polyline *polyline_create(void);

Polyline *polyline_createp(int numV, Point *vlist);

void polyline_free(Polyline *p);

void polyline_init(Polyline *p);

void polyline_set(Polyline *p, int numV, Point *vlist);

void polyline_clear(Polyline *p);


void polyline_zBuffer(Polyline *p, int flag);

void polyline_copy(Polyline *to, Polyline *from);

void polyline_print(Polyline *p, FILE *fp);

void polyline_normalize(Polyline *p);

void polyline_draw(Polyline *p, Image *src, Color c);

void scanline_flood_fill(Image* src, int startx, int starty, Color c);

#endif