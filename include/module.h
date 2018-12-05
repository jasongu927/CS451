#ifndef MODULE_H

#define MODULE_H

#include "drawstate.h"
#include "bezier.h"
#include "texture.h"

//type definitions

//all valid object types to put in a module
typedef enum {
    ObjNone,
    ObjLine,
    ObjPoint,
    ObjPolyline,
    ObjPolygon,
    ObjIdentity, //sets the transformation to identity in the open module
    ObjMatrix,
    ObjColor,
    ObjBodyColor,
    ObjSurfaceColor,
    ObjSurfaceCoeff,
    ObjLight,
    ObjModule,
    ObjBezier,
	ObjReflectance,
	ObjTransmittance,
	ObjTexture,
} ObjectType;

typedef union {
    Point point;
    Line line;
    Polyline polyline;
    Polygon polygon;
    Matrix matrix;
    Color color;
    float coeff;
    void *module;
    BezierCurve curve;
	Light light;
	Tex_cube_map* texture;
	float reflectance;
	float transmittance;
} Object;

typedef struct {
    ObjectType type;
    Object obj;
    void *next;
} Element;

typedef struct {
    Element *head; // pointer to the head of the linked list
    Element *tail; // keep around a pointer to the last object
} Module;

//function declarations
Element *element_create(void);
Element *element_init( ObjectType type, void *obj );
void element_delete( Element *e );

Module *module_create(void);
void module_clear( Module *md );
void module_delete( Module *md );
void module_insert( Module *md, Element *e );
void module_module( Module *md, Module *sub );
void module_point( Module *md, Point *p );
void module_line( Module *md, Line *p );
void module_polyline( Module *md, Polyline *p );
void module_polygon( Module *md, Polygon *p );
void module_identity( Module *md );
void module_translate2D( Module *md, double tx, double ty );
void module_scale2D( Module *md, double sx, double sy );
void module_rotateZ( Module *md, double cth, double sth );
void module_shear2D( Module *md, double shx, double shy );
void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, Lighting *lighting, Image *src);
void module_translate(Module *md, double tx, double ty, double tz);
void module_scale(Module *md, double sx, double sy, double sz);
void module_rotateX(Module *md, double cth, double sth);
void module_rotateY(Module *md, double cth, double sth);
void module_cube(Module *md, int solid);
void module_color(Module *md, Color *c);
void module_bezierCurve( Module *md, BezierCurve *b );
void module_bezierCurveSubdiv( Module *md, BezierCurve *b, int divisions );
void module_bezierSurface(Module *md, BezierSurface *b, int divisions, int flag);
void module_sphere(Module *md, int division);

void module_light(Module *md, Light *l);

void module_bodyColor(Module *md, Color *c);
void module_surfaceColor(Module *md, Color *c);
void module_surfaceCoeff(Module *md, float coeff);

void module_reflectance(Module *md, float r);
void module_transmittance(Module *md, float t);
void module_setTexture(Module *md, Tex_cube_map* t);
void module_clearTexture(Module *md);

void module_lighting(Module *md, Matrix *VTM, Matrix *GTM, Lighting *lighting);
void module_draw_lighting(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, Image *src);

void module_flipZ( Module *md);




#endif
