#ifndef TEXTURE_H

#define TEXTURE_H

#include "image.h"
#include "color.h"
#include "vector.h"
#include "point.h"
#include <stdlib.h>

typedef enum{
	ProjectUV,
	ProjectCube,
	ProjectCylinder
}Projection_type;

typedef struct{
	Image* top;
	Image* bottom;
	Image* front;
	Image* back;
	Image* right;
	Image* left;	
}Cube_map;

typedef struct{
	Cube_map tex;
	Cube_map bump;
	Cube_map reflect;
}Tex_cube_map;


Tex_cube_map* map_create(char* filename);

Tex_cube_map* map_create_uv(char* filename);

void calculate_tex(Tex_cube_map* map, Point* point, Vector* normal, Projection_type p, Color* result, Vector* bump, float* reflectCoeff);

void calculate_tex_sphere(Tex_cube_map* tex, Point* point, Vector* norm, Color* result, Vector* bump, float* reflectCoeff);
	
void calculate_tex_cube(Tex_cube_map* tex, Point* point, Vector* norm, Color* result, Vector* bump, float* reflectCoeff);

void bilin_interp_image(Image* image, float startu, float startv, Color* result);

void free_cube(Cube_map* cube);

void free_tex(Tex_cube_map* map);

#endif