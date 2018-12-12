#ifndef POLYGON_H

#define POLYGON_H

#include <stdio.h>
#include "point.h"
#include "color.h"
#include "line.h"
#include "image.h"
#include "drawstate.h"
#include "vector.h"
#include "lighting.h"
#include "texture.h"

//type definitions

typedef struct
{
    int oneSided; //whether polygon is one-sided for shading (if not, it's two-sided)
    int numVertex; //number of vertices
    Point *vertex; //array of vertex info
	Vector *normal;
	Color *color;
	int zBuffer;
	Point *orgVertex;
	Vector *orgNormal;
	float reflectance;
	float transmittance;
	Tex_cube_map* tex_ptr;
	Projection_type project;
} Polygon;

//Polygon functions
Polygon *polygon_create(void);
Polygon *polygon_createp( int numV, Point *vlist );
void polygon_free( Polygon *p );
void polygon_init( Polygon *p );
void polygon_set( Polygon *p, int numV, Point *vlist );
void polygon_clear( Polygon *p );
void polygon_setSided( Polygon *p, int oneSided );

void polygon_setOrgNormals(Polygon *p, int numV, Vector *normals);
void polygon_setOrgVertex(Polygon *p, int numV, Point *vertex);
void polygon_setReflectance(Polygon *p, float r);
void polygon_setTransmittance(Polygon *p, float t);
void polygon_setTexture(Polygon*p, Tex_cube_map* t, Projection_type proj);

void polygon_setColors(Polygon *p, int numV, Color *clist);
void polygon_setNormals(Polygon *p, int numV, Vector *normals);
void polygon_setAll(Polygon *p, int numV, Point *vlist, Color *clist, Vector *normals);
void polygonZbuffer(Polygon *p, int flag);

void polygon_copy( Polygon *to, Polygon *from );
void polygon_print( Polygon *p, FILE *fp );
void polygon_normalize( Polygon *p );
void polygon_draw( Polygon *p, Image *src, Color c );
//void polygon_drawFill(Polygon *p, Image *src, Color c ); //original version
void polygon_drawFill(Polygon *p, Image *src, DrawState *ds ); //version for zbuffer

void polygon_drawFillB(Polygon *p, Image *src, Color c );
void polygon_drawFillBGrad(Polygon *p, Image *src, Color cA, Color cB, Color cC);
void polygon_drawFillBGradSave(Polygon *p, Image *src, Color cA, Color cB, Color cC);

void polygon_shade(Polygon *p, Lighting *lighting, DrawState *ds);
void polygon_drawShade(Polygon *p, Image *src, DrawState *ds, Lighting *light);

int return_nump(void);

#endif
