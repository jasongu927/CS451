#ifndef VIEWING_H

#define VIEWING_H

#include "point.h"
#include "matrix.h"

//type definitions

typedef struct
{
	Point vrp; //center of view rectangle in world coordinates
    double dx; //width of view rectangle in world coordinates
    Vector x; //view window x-axis as as normalized vector
    int screenx; //output image dimension
    int screeny; //output image dimension
} View2D;

typedef struct
{
    Point vrp;
    Vector vpn;
    Vector vup; //cannot be parallel to vpn
    double d;
    double du; //in world coordinate distance
    double dv; //in world coordinate distance
    double f; //front clip plane as distance along positive VPN, > 0
    double b; //back clip plane, > f
    int screenx;
    int screeny;
} View3D;

//functions
void matrix_setView2D( Matrix *vtm, View2D *view );
void matrix_setView3D( Matrix *vtm, View3D *view );

#endif