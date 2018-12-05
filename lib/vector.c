#include "vector.h"
#include <math.h>

//vectors
void vector_set(Vector *v, double x, double y, double z){
	v -> val[X] = x;
	v -> val[Y] = y;
	v -> val[Z] = z;
	v -> val[H] = 0; // do I do this?
}

void vector_print(Vector *v, FILE *fp){
	int i;
	fprintf(fp, "vector is: [");
	for(i = 0; i < 3; i++){
		fprintf(fp, "%f, ", v -> val[i]);
	}
	fprintf(fp, "%f ]\n",v -> val[H]);
}

void vector_copy(Vector *dest, Vector *src){
	int i;
	for(i = 0; i < 4; i++){
		dest -> val[i] = src -> val[i];
	}
}


double fast_inverse_sqroot(double x){
	double y = x;
	double x2 = y * 0.5;
	long i = *(long *) &y;
	i = 0x5fe6eb50c7b537a9 - ( i >> 1);
	y = *(double *) &i;
	y = y * (1.5 - ( x2 * y * y));
	return y;
}

//returns the squared legnth?
//I tried implimenting fast inverse square root for doubles and it was accurate but slower than sqrt
double vector_length(Vector *v){
	return sqrt(SQUARE(v -> val[X]) + SQUARE(v -> val[Y]) +SQUARE(v -> val[Z]));
}

void vector_normalize(Vector *v){
	double legnth = 1/vector_length(v);
	int i;
	for(i = 0; i < 3; i++){
		v -> val[i] = v -> val[i] * legnth;
	}
}

double vector_dot(Vector *a, Vector *b){
	return a -> val[X] * b -> val[X] + a -> val[Y] * b -> val[Y] + a -> val[Z] * b -> val[Z]; 
}

void vector_cross(Vector *a, Vector *b, Vector *c){
	c -> val[X] = a -> val[Y] * b -> val[Z] - a -> val[Z] * b -> val[Y];
	c -> val[Y] = a -> val[Z] * b -> val[X] - a -> val[X] * b -> val[Z];
	c -> val[Z] = a -> val[X] * b -> val[Y] - a -> val[Y] * b -> val[X];
}

void vector_subtract(Vector *a, Vector *b, Vector *c){
	c -> val[X] = b -> val[X] - a -> val[X];
	c -> val[Y] = b -> val[Y] - a -> val[Y];
	c -> val[Z] = b -> val[Z] - a -> val[Z];
}

void vector_scale(Vector *v, double s)
{
	v->val[X] = v->val[X] * s;
	v->val[Y] = v->val[Y] * s;
	v->val[Z] = v->val[Z] * s;
}

void vector_add(Vector *a, Vector *b, Vector *c){
	c -> val[X] = b -> val[X] + a -> val[X];
	c -> val[Y] = b -> val[Y] + a -> val[Y];
	c -> val[Z] = b -> val[Z] + a -> val[Z];
}