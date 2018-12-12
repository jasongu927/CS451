#include "drawstate.h"

DrawState *drawstate_create( void ){
	DrawState* ds = (DrawState*) malloc(sizeof(DrawState));
	color_set(&(ds->color), 1.0, 1.0, 1.0);
	color_set(&(ds->body), 1.0, 1.0, 1.0);
	color_set(&(ds->surface), 1.0, 1.0, 1.0);
	ds -> surfaceCoeff = 10;
	ds -> shade = ShadeFrame;
	ds -> transmittance = 0;
	ds -> reflectance = 0;
	ds -> tex = NULL;
	ds -> proj = ProjectUV;
	//ds -> viewer = NULL;
}

void drawstate_setColor(DrawState *s, Color c){
	color_copy(&(s -> color), &c);
}

void drawstate_setBody(DrawState *s, Color c ){
	color_copy(&(s -> body), &c);

}

void drawstate_setSurface(DrawState *s, Color c ){
	color_copy(&(s -> surface), &c);
}

void drawstate_setSurfaceCoeff(DrawState *s, float f ){
	s->surfaceCoeff = f;
}

void drawstate_setTransmittance(DrawState *s, float t ){
	s->transmittance = t;
}

void drawstate_setReflectance(DrawState *s, float r ){
	s->reflectance = r;
}

void drawstate_setTexture(DrawState *s, Tex_cube_map* text ){
	s->tex = text;
}

void drawstate_setProjection(DrawState *s, Projection_type p ){
	s->proj = p;
}

void drawstate_clearTex(DrawState *s){
	s->tex = NULL;
}

void drawstate_copy(DrawState* to, DrawState* from){
	drawstate_setColor(to, from->color);
	drawstate_setBody(to, from -> body);
	drawstate_setSurface(to, from->surface);
	drawstate_setSurfaceCoeff(to, from -> surfaceCoeff);
	drawstate_setReflectance(to, from->reflectance);
	drawstate_setTransmittance(to, from->transmittance);
	drawstate_setTexture(to, from->tex);
	drawstate_setProjection(to, from->proj);
	to-> shade = from -> shade;
}