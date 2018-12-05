#ifndef VECTOR_H

#define VECTOR_H
#include "point.h"

#define SQUARE(x) (x)*(x)

typedef Point Vector;


//vectors
void vector_set(Vector *v, double x, double y, double z);

void vector_print(Vector *v, FILE *fp);

void vector_copy(Vector *dest, Vector *src);

//returns the squared legnth?
double vector_length(Vector *v);

void vector_normalize(Vector *v);

double vector_dot(Vector *a, Vector *b);

void vector_cross(Vector *a, Vector *b, Vector *c);

void vector_subtract(Vector *a, Vector *b, Vector *c);

void vector_scale(Vector *v, double s);

void vector_add(Vector *a, Vector *b, Vector *c);

#endif