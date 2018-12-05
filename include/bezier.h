#ifndef BEZIER_H

#define BEZIER_H

#include <math.h>
#include <stdlib.h>
#include "line.h"
#include "matrix.h"
#include "vector.h"

//type definitions

typedef struct {
    Point pt[4]; //control points
    int zBuffer;
} BezierCurve;

typedef struct {
    Point pt[4][4]; //control points
    int zBuffer;
} BezierSurface;

//function declarations
void bezierCurve_init( BezierCurve *b );
void bezierCurve_set( BezierCurve *b, Point *vlist );
void bezierCurve_zBuffer( BezierCurve *p, int flag );
void bezierCurve_draw( BezierCurve *b, Image *src, Color c );

void bezierSurface_init( BezierSurface *b );
void bezierSurface_set( BezierSurface *b, Point *vlist );
void bezierSurface_zBuffer( BezierSurface *p, int flag );

void bezier_subdivide(Point *bezIn, Point *bezOut);

void bezier_surfaceNormal(Point *contu, Point *contv, float u, float v, Vector *result);


#endif
