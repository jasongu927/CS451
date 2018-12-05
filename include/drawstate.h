#ifndef DRAWSTATE_H

#define DRAWSTATE_H

#include "color.h"
#include "point.h"
#include "texture.h"
#include <stdlib.h>
#include <stdio.h>

typedef enum{
	ShadeFrame,
	ShadeConstant,
	ShadeDepth,
	ShadeFlat,
	ShadeGouraud  
} ShadeMethod;

typedef struct{
	Color color;
	Color flatColor;
	Color body;
	Color surface;
	float surfaceCoeff;
	ShadeMethod shade;
	Point viewer;
	
	Tex_cube_map* tex;
	float transmittance;
	float reflectance;
}DrawState;

DrawState *drawstate_create( void );

void drawstate_setColor( DrawState *s, Color c );

void drawstate_copy( DrawState *to, DrawState *from );

void drawstate_setBody(DrawState *s, Color c );

void drawstate_setSurface(DrawState *s, Color c );

void drawstate_setSurfaceCoeff(DrawState *s, float f );

void drawstate_setTransmittance(DrawState *s, float t );

void drawstate_setReflectance(DrawState *s, float r );

void drawstate_setTexture(DrawState *s, Tex_cube_map* tex );

void drawstate_clearTex(DrawState *s);
#endif
