#ifndef TEXTURE_H

#define TEXTURE_H

#include "image.h"
#include "color.h"
#include "vector.h"
#include "point.h"
#include <stdlib.h>

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
}Tex_cube_map;


Tex_cube_map* map_create(char* filename);

void calculate_tex_sphere(Tex_cube_map* map, Point* point, Vector* normal, Color* resuolt, Vector* bump);

void bilin_interp_image(Image* image, float startu, float startv, Color* result);

void free_cube(Cube_map* cube);

void free_tex(Tex_cube_map* map);

#endif