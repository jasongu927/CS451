/* test_ray.c
 * Creates the frames for a GIF showing a cloud of fish; demonstrates z-buffer
 *
 * Melody Mao and Jason Gurevitch
 * CS451 Fall 2018
 * Project 2
 */

#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include "ray.h"
#include <time.h>

void make_teapot(Module *md, int division, int fill);
void make_teapot(Module *md, int division, int fill){
	int i;
	Color red;
	color_set(&red, 0.75, 0.3, 0.3 );
	Color Green = {{0.2, 0.7, 0.2}};

	Point pts[127] = {
	{{  0.2000,  0.0000, 2.70000 , 1.0}}, {{  0.2000, -0.1120, 2.70000 , 1.0}},
    {{  0.1120, -0.2000, 2.70000 , 1.0}}, {{  0.0000, -0.2000, 2.70000 , 1.0}},
    {{  1.3375,  0.0000, 2.53125 , 1.0}}, {{  1.3375, -0.7490, 2.53125 , 1.0}},
    {{  0.7490, -1.3375, 2.53125 , 1.0}}, {{  0.0000, -1.3375, 2.53125 , 1.0}},
    {{  1.4375,  0.0000, 2.53125 , 1.0}}, {{  1.4375, -0.8050, 2.53125 , 1.0}},
    {{  0.8050, -1.4375, 2.53125 , 1.0}}, {{  0.0000, -1.4375, 2.53125 , 1.0}},
    {{  1.5000,  0.0000, 2.40000 , 1.0}}, {{  1.5000, -0.8400, 2.40000 , 1.0}},
    {{  0.8400, -1.5000, 2.40000 , 1.0}}, {{  0.0000, -1.5000, 2.40000 , 1.0}},
    {{  1.7500,  0.0000, 1.87500 , 1.0}}, {{  1.7500, -0.9800, 1.87500 , 1.0}},
    {{  0.9800, -1.7500, 1.87500 , 1.0}}, {{  0.0000, -1.7500, 1.87500 , 1.0}},
    {{  2.0000,  0.0000, 1.35000 , 1.0}}, {{  2.0000, -1.1200, 1.35000 , 1.0}},
    {{  1.1200, -2.0000, 1.35000 , 1.0}}, {{  0.0000, -2.0000, 1.35000 , 1.0}},
    {{  2.0000,  0.0000, 0.90000 , 1.0}}, {{  2.0000, -1.1200, 0.90000 , 1.0}},
    {{  1.1200, -2.0000, 0.90000 , 1.0}}, {{  0.0000, -2.0000, 0.90000 , 1.0}},
    {{ -2.0000,  0.0000, 0.90000 , 1.0}}, {{  2.0000,  0.0000, 0.45000 , 1.0}},
    {{  2.0000, -1.1200, 0.45000 , 1.0}}, {{  1.1200, -2.0000, 0.45000 , 1.0}},
    {{  0.0000, -2.0000, 0.45000 , 1.0}}, {{  1.5000,  0.0000, 0.22500 , 1.0}},
    {{  1.5000, -0.8400, 0.22500 , 1.0}}, {{  0.8400, -1.5000, 0.22500 , 1.0}},
    {{  0.0000, -1.5000, 0.22500 , 1.0}}, {{  1.5000,  0.0000, 0.15000 , 1.0}},
    {{  1.5000, -0.8400, 0.15000 , 1.0}}, {{  0.8400, -1.5000, 0.15000 , 1.0}},
    {{  0.0000, -1.5000, 0.15000 , 1.0}}, {{ -1.6000,  0.0000, 2.02500 , 1.0}},
    {{ -1.6000, -0.3000, 2.02500 , 1.0}}, {{ -1.5000, -0.3000, 2.25000 , 1.0}},
    {{ -1.5000,  0.0000, 2.25000 , 1.0}}, {{ -2.3000,  0.0000, 2.02500 , 1.0}},
    {{ -2.3000, -0.3000, 2.02500 , 1.0}}, {{ -2.5000, -0.3000, 2.25000 , 1.0}},
    {{ -2.5000,  0.0000, 2.25000 , 1.0}}, {{ -2.7000,  0.0000, 2.02500 , 1.0}},
    {{ -2.7000, -0.3000, 2.02500 , 1.0}}, {{ -3.0000, -0.3000, 2.25000 , 1.0}},
    {{ -3.0000,  0.0000, 2.25000 , 1.0}}, {{ -2.7000,  0.0000, 1.80000 , 1.0}},
    {{ -2.7000, -0.3000, 1.80000 , 1.0}}, {{ -3.0000, -0.3000, 1.80000 , 1.0}},
    {{ -3.0000,  0.0000, 1.80000 , 1.0}}, {{ -2.7000,  0.0000, 1.57500 , 1.0}},
    {{ -2.7000, -0.3000, 1.57500 , 1.0}}, {{ -3.0000, -0.3000, 1.35000 , 1.0}},
    {{ -3.0000,  0.0000, 1.35000 , 1.0}}, {{ -2.5000,  0.0000, 1.12500 , 1.0}},
    {{ -2.5000, -0.3000, 1.12500 , 1.0}}, {{ -2.6500, -0.3000, 0.93750 , 1.0}},
    {{ -2.6500,  0.0000, 0.93750 , 1.0}}, {{ -2.0000, -0.3000, 0.90000 , 1.0}},
    {{ -1.9000, -0.3000, 0.60000 , 1.0}}, {{ -1.9000,  0.0000, 0.60000 , 1.0}},
    {{  1.7000,  0.0000, 1.42500 , 1.0}}, {{  1.7000, -0.6600, 1.42500 , 1.0}},
    {{  1.7000, -0.6600, 0.60000 , 1.0}}, {{  1.7000,  0.0000, 0.60000 , 1.0}},
    {{  2.6000,  0.0000, 1.42500 , 1.0}}, {{  2.6000, -0.6600, 1.42500 , 1.0}},
    {{  3.1000, -0.6600, 0.82500 , 1.0}}, {{  3.1000,  0.0000, 0.82500 , 1.0}},
    {{  2.3000,  0.0000, 2.10000 , 1.0}}, {{  2.3000, -0.2500, 2.10000 , 1.0}},
    {{  2.4000, -0.2500, 2.02500 , 1.0}}, {{  2.4000,  0.0000, 2.02500 , 1.0}},
    {{  2.7000,  0.0000, 2.40000 , 1.0}}, {{  2.7000, -0.2500, 2.40000 , 1.0}},
    {{  3.3000, -0.2500, 2.40000 , 1.0}}, {{  3.3000,  0.0000, 2.40000 , 1.0}},
    {{  2.8000,  0.0000, 2.47500 , 1.0}}, {{  2.8000, -0.2500, 2.47500 , 1.0}},
    {{  3.5250, -0.2500, 2.49375 , 1.0}}, {{  3.5250,  0.0000, 2.49375 , 1.0}},
    {{  2.9000,  0.0000, 2.47500 , 1.0}}, {{  2.9000, -0.1500, 2.47500 , 1.0}},
    {{  3.4500, -0.1500, 2.51250 , 1.0}}, {{  3.4500,  0.0000, 2.51250 , 1.0}},
    {{  2.8000,  0.0000, 2.40000 , 1.0}}, {{  2.8000, -0.1500, 2.40000 , 1.0}},
    {{  3.2000, -0.1500, 2.40000 , 1.0}}, {{  3.2000,  0.0000, 2.40000 , 1.0}},
    {{  0.0000,  0.0000, 3.15000 , 1.0}}, {{  0.8000,  0.0000, 3.15000 , 1.0}},
    {{  0.8000, -0.4500, 3.15000 , 1.0}}, {{  0.4500, -0.8000, 3.15000 , 1.0}},
    {{  0.0000, -0.8000, 3.15000 , 1.0}}, {{  0.0000,  0.0000, 2.85000 , 1.0}},
    {{  1.4000,  0.0000, 2.40000 , 1.0}}, {{  1.4000, -0.7840, 2.40000 , 1.0}},
    {{  0.7840, -1.4000, 2.40000 , 1.0}}, {{  0.0000, -1.4000, 2.40000 , 1.0}},
    {{  0.4000,  0.0000, 2.55000 , 1.0}}, {{  0.4000, -0.2240, 2.55000 , 1.0}},
    {{  0.2240, -0.4000, 2.55000 , 1.0}}, {{  0.0000, -0.4000, 2.55000 , 1.0}},
    {{  1.3000,  0.0000, 2.55000 , 1.0}}, {{  1.3000, -0.7280, 2.55000 , 1.0}},
    {{  0.7280, -1.3000, 2.55000 , 1.0}}, {{  0.0000, -1.3000, 2.55000 , 1.0}},
    {{  1.3000,  0.0000, 2.40000 , 1.0}}, {{  1.3000, -0.7280, 2.40000 , 1.0}},
    {{  0.7280, -1.3000, 2.40000 , 1.0}}, {{  0.0000, -1.3000, 2.40000 , 1.0}},
	{{  0.0000,  0.0000, 0.00000, 1.0} }, {{  1.4250, -0.7980, 0.00000, 1.0 }},
	{{  1.5000,  0.0000, 0.07500, 1.0}}, {{  1.4250,  0.0000, 0.00000, 1.0 }},
	{{  0.7980, -1.4250, 0.00000 , 1.0}}, {{  0.0000, -1.5000, 0.07500, 1.0 }},
	{{  0.0000, -1.4250, 0.00000, 1.0 }}, {{  1.5000, -0.8400, 0.07500, 1.0 }},
	{{  0.8400, -1.5000, 0.07500, 1.0 }}
	};
	float temp;
	//these points have the y and z switched so flip them
	for(i = 0; i < 127; i++){
		temp = pts[i].val[Z];
		pts[i].val[Z] = pts[i].val[Y];
		pts[i].val[Y] = temp;
	}
	
	Point bezPts[16];
	BezierSurface bs;
	
	//make rim
	point_copy(&bezPts[0], &pts[102]);
	point_copy(&bezPts[1], &pts[103]);
	point_copy(&bezPts[2], &pts[104]);
	point_copy(&bezPts[3], &pts[105]);
	point_copy(&bezPts[4], &pts[4]);
	point_copy(&bezPts[5], &pts[5]);
	point_copy(&bezPts[6], &pts[6]);
	point_copy(&bezPts[7], &pts[7]);
	point_copy(&bezPts[8], &pts[8]);
	point_copy(&bezPts[9], &pts[9]);
	point_copy(&bezPts[10], &pts[10]);
	point_copy(&bezPts[11], &pts[11]);
	point_copy(&bezPts[12], &pts[12]);
	point_copy(&bezPts[13], &pts[13]);
	point_copy(&bezPts[14], &pts[14]);
	point_copy(&bezPts[15], &pts[15]);
	
	bezierSurface_set(&bs, &bezPts[0]);
	module_bodyColor( md, &red );
	Module* top = module_create();
	Module* top_part = module_create();
	module_bezierSurface(top_part, &bs, division, fill); 
	module_bodyColor( md, &Green );
	module_module(top, top_part);
	for(i =0; i < 3; i++){
		Module* top_part = module_create();
		module_rotateY(top, cos(M_PI/2), sin(M_PI/2));
		module_bezierSurface(top_part, &bs, division, fill);
		module_module(top, top_part);
	}
	module_module(md, top);
	
	module_identity(md);
	//make body
	
	Module* body = module_create();
	Module* body_part = module_create();
	for(i = 0; i < 16; i++){
		point_copy(&bezPts[i], &pts[12 + i]);
	}
	bezierSurface_set(&bs, &bezPts[0]);

	module_bezierSurface(body_part, &bs, division, fill);		
	module_module(body, body_part);

	
	for(i =0; i < 3; i++){
		Module* body_part = module_create();
		module_rotateY(body, cos(M_PI/2), sin(M_PI/2));
		module_bezierSurface(body_part, &bs, division, fill);
		module_module(body, body_part);
	}
	module_identity(body);
	
	for(i = 0; i < 4; i++){
		point_copy(&bezPts[i], &pts[24 + i]);
	}
	
	for(i = 0; i < 12; i++){
		point_copy(&bezPts[4 + i], &pts[29 + i]);
	}
	
	bezierSurface_set(&bs, &bezPts[0]);
	body_part = module_create();

	module_bezierSurface(body_part, &bs, division, fill);	
	module_module(body, body_part);
	for(i =0; i < 3; i++){
		Module* body_part = module_create();
		module_rotateY(body, cos(M_PI/2), sin(M_PI/2));
		module_bezierSurface(body_part, &bs, division, fill);
		module_module(body, body_part);
	}
	module_module(md, body);
	module_identity(md);
	
	//make_lid
	Module *lid = module_create();
	Module* lid_part = module_create();

	for(i = 0; i < 4; i++){
		point_copy(&bezPts[i], &pts[96]);
	}
	
	for(i = 0; i < 4; i++){
		point_copy(&bezPts[4 +i], &pts[97 + i]);
	}
	for(i = 0; i < 4; i++){
		point_copy(&bezPts[8 + i], &pts[101]);
	}
	for(i = 0; i < 4; i++){
		point_copy(&bezPts[12 + i], &pts[i]);
	}
	bezierSurface_set(&bs, &bezPts[0]);

	module_bezierSurface(lid_part, &bs, division, fill);
	module_module(lid, lid_part);
	for(i =0; i < 3; i++){
		Module* lid_part = module_create();
		module_rotateY(lid, cos(M_PI/2), sin(M_PI/2));
		module_bezierSurface(lid_part, &bs, division, fill);	
		module_module(lid, lid_part);		
	}

	module_identity(lid);
	
	for(i = 0; i < 4; i++){
		point_copy(&bezPts[i], &pts[i]);
	}
	
	for(i = 0; i < 12; i++){
		point_copy(&bezPts[4 + i], &pts[106 + i]);
	}
	
	bezierSurface_set(&bs, &bezPts[0]);

	lid_part = module_create();
	module_bezierSurface(lid_part, &bs, division, fill);
	module_module(lid, lid_part);		
	for(i =0; i < 3; i++){
		lid_part = module_create();
		module_rotateY(lid, cos(M_PI/2), sin(M_PI/2));
		module_bezierSurface(lid_part, &bs, division, fill);	
		module_module(lid, lid_part);				
	}
	module_module(md, lid);
	module_identity(md);
	
	//handle
	Module *handle = module_create();
	Module* handle_part = module_create();
	for(i = 0; i < 16; i++){
		point_copy(&bezPts[i], &pts[41 + i]);
	}
	bezierSurface_set(&bs, &bezPts[0]);

	module_bezierSurface(handle_part, &bs, division, fill);	
	module_module(handle, handle_part);
	//flip over y axis
	
	for(i = 0; i < 16; i++){
		bezPts[i].val[Z] = -bezPts[i].val[Z];
	}
	for(i = 0; i <4; i++){
		for(int j = 0; j < 2; j++){
			Point tmp;
			point_copy(&tmp, &bezPts[i*4+j]);
			point_copy(&bezPts[i*4+j], &bezPts[i*4 +(3-j)]);
			point_copy(&bezPts[i*4 +(3-j)], &tmp);
		}
	}
	bezierSurface_set(&bs, &bezPts[0]);
	
	//module_flipZ(md);
	handle_part = module_create();
	module_bezierSurface(handle_part, &bs, division, fill);
	module_module(handle, handle_part);

	module_identity(md);
	
	for(i = 0; i < 12; i++){
		point_copy(&bezPts[i], &pts[53 + i]);
	}
	point_copy(&bezPts[12], &pts[28]);

	for(i = 0; i < 3; i++){
		point_copy(&bezPts[13+i], &pts[65 + i]);
	}
	bezierSurface_set(&bs, &bezPts[0]);

	handle_part = module_create();
	module_bezierSurface(handle_part, &bs, division, fill);	
	module_module(handle, handle_part);	
	
	//flip
	for(i = 0; i < 16; i++){
		bezPts[i].val[Z] = -bezPts[i].val[Z];
	}
	for(i = 0; i <4; i++){
		for(int j = 0; j < 2; j++){
			Point tmp;
			point_copy(&tmp, &bezPts[i*4+j]);
			point_copy(&bezPts[i*4+j], &bezPts[i*4 +(3-j)]);
			point_copy(&bezPts[i*4 +(3-j)], &tmp);
		}
	}
	bezierSurface_set(&bs, &bezPts[0]);

	//module_flipZ(md);
	handle_part = module_create();
	module_bezierSurface(handle_part, &bs, division, fill);
	module_module(handle, handle_part);
	module_identity(md);
	module_module(md, handle);

	//spout
	Module *spout = module_create();
	Module *spout_part = module_create();
	for(i = 0; i < 16; i++){
		point_copy(&bezPts[i], &pts[68 + i]);
	}
	bezierSurface_set(&bs, &bezPts[0]);

	module_bezierSurface(spout_part, &bs, division, fill);	
	module_module(spout, spout_part);
	//flip
	for(i = 0; i < 16; i++){
		bezPts[i].val[Z] = -bezPts[i].val[Z];
	}
	for(i = 0; i <4; i++){
		for(int j = 0; j < 2; j++){
			Point tmp;
			point_copy(&tmp, &bezPts[i*4+j]);
			point_copy(&bezPts[i*4+j], &bezPts[i*4 +(3-j)]);
			point_copy(&bezPts[i*4 +(3-j)], &tmp);
		}
	}
	bezierSurface_set(&bs, &bezPts[0]);

	//module_flipZ(md);
	spout_part = module_create();
	module_bezierSurface(spout_part, &bs, division, fill);
	module_module(spout, spout_part);
	module_identity(md);
	
	for(i = 0; i < 16; i++){
		point_copy(&bezPts[i], &pts[80 + i]);
	}
	bezierSurface_set(&bs, &bezPts[0]);

	spout_part = module_create();
	module_bezierSurface(spout_part, &bs, division, fill);	
	module_module(spout, spout_part);
	
	for(i = 0; i < 16; i++){
		bezPts[i].val[Z] = -bezPts[i].val[Z];
	}
	for(i = 0; i <4; i++){
		for(int j = 0; j < 2; j++){
			Point tmp;
			point_copy(&tmp, &bezPts[i*4+j]);
			point_copy(&bezPts[i*4+j], &bezPts[i*4 +(3-j)]);
			point_copy(&bezPts[i*4 +(3-j)], &tmp);
		}
	}
	
	bezierSurface_set(&bs, &bezPts[0]);
	
	
	//module_flipZ(md);
	spout_part = module_create();
	module_bezierSurface(spout_part, &bs, division, fill);
	module_module(spout, spout_part);
	module_module(md, spout);
	module_identity(md);
	
	//make bottom
	Module *bottom = module_create();
	Module *bottom_part = module_create();
	
	for(i = 0; i < 4; i++){
		point_copy(&bezPts[i], &pts[118]);
	}
	point_copy(&bezPts[4], &pts[124]);
	point_copy(&bezPts[5], &pts[122]);
	point_copy(&bezPts[6], &pts[119]);
	point_copy(&bezPts[7], &pts[121]);
	point_copy(&bezPts[8], &pts[123]);
	point_copy(&bezPts[9], &pts[126]);
	point_copy(&bezPts[10], &pts[125]);
	point_copy(&bezPts[11], &pts[120]);
	for(i = 0; i < 4; i++){
		point_copy(&bezPts[12+i], &pts[40-i]);
	}	
	bezierSurface_set(&bs, &bezPts[0]);

	module_bezierSurface(bottom_part, &bs, division, fill);
	module_module(bottom, bottom_part);
	for(i =0; i < 3; i++){
		bottom_part = module_create();
		module_rotateY(bottom, cos(M_PI/2), sin(M_PI/2));
		module_bezierSurface(bottom_part, &bs, division, fill);
		module_module(bottom, bottom_part);
	}
	module_identity(bottom);
	module_module(md, bottom);
}



void cylinder( Module *md, int sides );
void cylinder( Module *md, int sides ) {
  Module* mod = module_create();
  Polygon p;
  Point xtop, xbot;
  double x1, x2, z1, z2;
  int i;

  polygon_init( &p );
  point_set3D( &xtop, 0, 1.0, 0.0 );
  point_set3D( &xbot, 0, 0.0, 0.0 );

  // make a fan for the top and bottom sides
  // and quadrilaterals for the sides
  for(i=0;i<sides;i++) {
    Point pt[4];
	Vector norms[4];
	
    x1 = cos( i * M_PI * 2.0 / sides );
    z1 = sin( i * M_PI * 2.0 / sides );
    x2 = cos( ( (i+1)%sides ) * M_PI * 2.0 / sides );
    z2 = sin( ( (i+1)%sides ) * M_PI * 2.0 / sides );

    point_copy( &pt[0], &xtop );
    point_set3D( &pt[1], x1, 1.0, z1 );
    point_set3D( &pt[2], x2, 1.0, z2 );
	for(int i = 0; i < 3; i++){
		vector_set(&norms[i], 0.0, 1.0, 0.0);
	}
	
    polygon_set( &p, 3, pt );
	polygon_setNormals(&p, 3, norms);
    module_polygon( mod, &p );

    point_copy( &pt[0], &xbot );
    point_set3D( &pt[1], x1, 0.0, z1 );
    point_set3D( &pt[2], x2, 0.0, z2 );
	for(int i = 0; i < 3; i++){
		vector_set(&norms[i], 0.0, -1.0, 0.0);
	}
	
	
    polygon_set( &p, 3, pt );
	polygon_setNormals(&p, 3, norms);
    module_polygon( mod, &p );


    point_set3D( &pt[0], x1, 0.0, z1 );
    point_set3D( &pt[1], x1, 1.0, z1 );
    point_set3D( &pt[2], x2, 1.0, z2 );
    point_set3D( &pt[3], x2, 0.0, z2 );
    
	vector_set( &norms[0], x1, 0.0, z1 );
    vector_set( &norms[1], x1, 0.0, z1 );
    vector_set( &norms[2], x2, 0.0, z2 );
    vector_set( &norms[3], x2, 0.0, z2 );
	
	for(int i = 0; i < 4; i++){
		vector_normalize(&norms[i]);
	}
    polygon_set( &p, 4, pt );
	polygon_setNormals(&p, 4, norms);
    module_polygon( mod, &p );
  }
  polygon_clear( &p );
  module_module(md, mod);
}

int main(int argc, char *argv[]) {
    Image *src;
   // Module *scene;
    int rows = 720;
    int cols = 1280;
	src = image_create( rows, cols );
	Module *forest;
	Module *treeGroup;
	Module *tree;
	Module *branch;
	Module *branchSegment;
	Module *leaf;
	
	//Color Grey = {{0.6, 0.62, 0.64}};
	Color Green = {{0.2, 0.7, 0.2}};
	Color Brown = {{0.4, 0.2, 0.0}};
	Color lightBrown = {{0.6, 0.3, 0.0}};
	Color Red = {{0.8, 0.1, 0.1}};
	Color BrownRed = {{0.8, 0.3, 0.1}};

	DrawState *ds;
	
	leaf = module_create();
	Polygon p;

	Point verts[6];
	Point pts[3];
	point_set3D(&pts[0], 0.05, 0.5, 0.1);
	point_set2D(&verts[0], 0.0, 0.0);
	point_set2D(&verts[1], 0.4, 0.33);
	point_set2D(&verts[2], 0.3, 0.66);
	point_set2D(&verts[3], 0.1, 1.0);
	point_set2D(&verts[4], -0.3, 0.66);			
	point_set2D(&verts[5], -0.2, 0.25);
	
	module_scale(leaf, 0.8, 0.8, 0.8);
	
	for(int i =0; i < 6; i++){
		polygon_init(&p);
		point_copy(&pts[1], &verts[i]);
		point_copy(&pts[2], &verts[(i+1)%6]);
		polygon_set(&p, 3, pts);
		module_polygon(leaf, &p);
		module_flipZ(leaf);
		module_polygon(leaf, &p);
	}
/*	
	Point pts[3];
	point_set3D(&pts[0], -0.3, 0.0, 0.0);
	point_set3D(&pts[1], 0.2, 0.0, 0.0);
	point_set3D(&pts[2], 0.0, 0.8, 0.01);
	
	polygon_set(&p, 3, pts);
	*/
	

	
	branchSegment = module_create();
	
	module_rotateZ(branchSegment, cos(M_PI/4.0), sin(M_PI/4.0));
	module_translate(branchSegment, -0.19, 0.8, 0.0);
	for(int i = 0; i<4; i++){
		module_module(branchSegment, leaf);
		module_rotateY(branchSegment, cos(M_PI/2), sin(M_PI/2));
	}
	module_identity(branchSegment);

	module_rotateZ(branchSegment, cos((-M_PI)/4.0), sin((-M_PI)/4.0));
	module_translate(branchSegment, 0.19, 0.65, 0.0);
	for(int i = 0; i<4; i++){
		module_module(branchSegment, leaf);
		module_rotateY(branchSegment, cos(M_PI/2), sin(M_PI/2));
	}	
	
	module_identity(branchSegment);

	module_rotateZ(branchSegment, cos((2*M_PI)/4.0), sin((2*M_PI)/4.0));
	module_translate(branchSegment, -0.19, 0.35, 0.0);
	for(int i = 0; i<4; i++){
		module_module(branchSegment, leaf);
		module_rotateY(branchSegment, cos(M_PI/2), sin(M_PI/2));
	}
	module_identity(branchSegment);
	
	module_rotateZ(branchSegment, cos((-2*M_PI)/4.0), sin((-2*M_PI)/4.0));
	module_translate(branchSegment, 0.19, 0.25, 0.0);
	for(int i = 0; i<4; i++){
		module_module(branchSegment, leaf);
		module_rotateY(branchSegment, cos(M_PI/2), sin(M_PI/2));
	}
	module_identity(branchSegment);
	
	
	module_bodyColor(branchSegment, &Brown);
	module_scale(branchSegment, 0.25, 1.0, 0.25);
	
	cylinder(branchSegment, 10);
	
	printf("made branch segment\n");
	branch = module_create();
	//float angle = 0.0;
	int i;
	for(i = 0; i < 5; i++){
		module_module(branch, branchSegment);		
		float diff = (0.5 - ((float) rand())/RAND_MAX)/5.0;
		module_rotateY(branch, cos(2*M_PI/5 * i + diff), sin(2*M_PI/5 * i + diff));
		module_translate(branch, 0.0, 1.0, 0.0);
	}
	
	module_module(branch, leaf);
	printf("made branch\n");
	
	tree = module_create();
	
	for(i = 0; i <5; i++){
		module_identity(tree);
		module_scale(tree, 0.15, 0.15, 0.15);
		float diff = (0.5 - ((float) rand())/RAND_MAX)/5.0;
		module_rotateX(tree, cos(0.3), sin(0.3));
		module_translate(tree, 0.1, 1.0, 0.1);
		module_rotateY(tree, cos(2*M_PI/5 * i + diff), sin(2*M_PI/5 * i + diff));
		module_module(tree, branch);
	}
	
	for(i = 0; i < 8; i++){
		module_identity(tree);
		module_scale(tree, 0.2, 0.17, 0.2);
		float diff = (0.5 - ((float) rand())/RAND_MAX)/5.0;
		module_rotateX(tree, cos(0.6), sin(0.6));
		module_translate(tree, 0.19, 0.8, 0.19);
		module_rotateY(tree, cos(2*M_PI/8 * i + diff), sin(2*M_PI/8 * i + diff));
		module_module(tree, branch);
	}
	
	for(i = 0; i < 6; i++){
		module_identity(tree);
		module_scale(tree, 0.17, 0.2, 0.17);
		float diff = (0.5 - ((float) rand())/RAND_MAX)/2.0;
		module_rotateX(tree, cos(0.7), sin(0.7));
		module_translate(tree, 0.19, 0.5, 0.19);
		module_rotateY(tree, cos(2*M_PI/6 * i + diff), sin(2*M_PI/6 * i + diff));
		module_module(tree, branch);
	}
	
	for(i = 0; i < 4; i++){
		module_identity(tree);
		module_scale(tree, 0.25, 0.15, 0.25);
		float diff = (0.5 - ((float) rand())/RAND_MAX)/2.0;
		module_rotateX(tree, cos(1.0), sin(1.0));
		module_translate(tree, 0.19, 0.3, 0.19);
		module_rotateY(tree, cos(2*M_PI/4 * i + diff), sin(2*M_PI/4 * i + diff));
		module_module(tree, branch);
	}
	
	module_identity(tree);
	module_bodyColor(tree, &Brown);
	module_scale(tree, 0.25, 1.0, 0.25);
	cylinder(tree, 10);
	
	treeGroup = module_create();
	
	
	for(i = 0; i < 4; i++){
		if(i == 3){
			module_bodyColor(treeGroup, &Red);
		}
		else{
			module_bodyColor(treeGroup, &Green);
		}
		module_identity(treeGroup);
		float diff1 = (0.5 - ((float) rand())/RAND_MAX)/2.0;
		float diff2 = (0.5 - ((float) rand())/RAND_MAX)/2.0;
		module_translate(treeGroup, 1 + diff1, 0, 1+ diff2);
		module_rotateY(treeGroup, cos(2*M_PI/4*i), sin(2*M_PI/4 *i));
		module_module(treeGroup, tree);
	}
	module_identity(treeGroup);


	forest = module_create();
	module_bodyColor(forest, &Red);
	module_rotateZ(forest, cos(M_PI/2), sin(M_PI/2)); //add a fallen tree
	module_scale(forest, 0.7, 0.7, 0.7);
	module_module(forest, tree);
	
	
	for(i = 0; i < 4; i++){
		module_identity(forest);
		module_rotateZ(forest, cos(M_PI/2), sin(M_PI/2)); //add a fallen tree
		module_scale(forest, 0.7, 0.7, 0.7);
		float diff1 = (0.5 - ((float) rand())/RAND_MAX)/2.0;
		float diff2 = (0.5 - ((float) rand())/RAND_MAX)/2.0;
		float diff3 = (0.5 - ((float) rand())/RAND_MAX) * 2*M_PI;
		module_rotateX(forest, cos(diff3), sin(diff3));
		module_translate(forest, 2.0 + diff1, 0.0, 1.3 + diff2);
		module_rotateY(forest, cos(2*M_PI/4*i + diff1), sin(2*M_PI/4 *i + diff1));
		module_module(forest, tree);
	}
	
	module_bodyColor(forest, &Green);
	module_identity(forest);
	module_module(forest, treeGroup);
	
	for(i = 0; i < 4; i++){
		module_identity(forest);
		float diff1 = (0.5 - ((float) rand())/RAND_MAX)/2.0;
		float diff2 = (0.5 - ((float) rand())/RAND_MAX)/2.0;
		module_translate(forest, 1.5 + diff1, 0, 1.5 + diff2);
		module_rotateY(forest, cos(2*M_PI/4*i), sin(2*M_PI/4 *i));
		module_module(forest, treeGroup);
	}
	
	module_identity(forest);
	Module* floor = module_create();
	module_bodyColor(floor, &BrownRed);
	
	float randx;
	for(i = 0; i < 50; i++){
		Module* leafrow = module_create();
		for(int j = 0; j < 50; j++){
			module_scale(leafrow, 0.15, 0.15, 0.15);
			randx = (float)rand()/(float)(RAND_MAX) * M_PI*2;
			module_rotateZ(leafrow, cos(randx), sin(randx));
			module_translate(leafrow, 0.25*i-5, -0.0, 0.25*j-5);
			module_module(leafrow, leaf);
			module_identity(leafrow);//
		}
		module_module(floor, leafrow);
	}
	
	module_identity(floor);
	module_scale(floor, 60, 0.2, 60);
	module_bodyColor(floor, &lightBrown);
	module_translate(floor, 0.0, -0.2, 0.0);
	module_cube(floor, 1);
	module_module(forest, floor);
	
	View3D view;
	Matrix VTM;
	Matrix GTM;
	
	// set up a view
	matrix_identity( &VTM );
	matrix_identity( &GTM );
	point_set3D( &(view.vrp),  3.0,  4.0, -7.0 );
	vector_set(  &(view.vpn), -3.0, -4.0,  7.0);
	vector_set(  &(view.vup), 0.0, 1.0, 0.0);
	view.d = 2.0;
	view.du = 1.6;
	view.dv = src->rows * view.du / src->cols;
	view.f = 0.0;
	view.b = 40;
	view.screenx = src->cols;
	view.screeny = src->rows;
	
    Color white;
	color_set(&white, 0.55, 0.55, 0.55 );

	
	Color Dark = {{0.2, 0.2, 0.2}};
	Point ltpt2;
	Point ltpt;
	point_copy(&ltpt2, &view.vrp);
	ltpt2.val[Y] = ltpt2.val[Y] - 1;
	ltpt2.val[X] = ltpt2.val[X] - 2;
	Lighting* light = lighting_create();
	lighting_add( light, LightPoint, &white, NULL, &(ltpt2), 0, 0 );

	point_copy(&ltpt, &view.vrp);
	ltpt2.val[Y] = ltpt2.val[Y] + 10;
	ltpt2.val[X] = ltpt2.val[X] + 6;
	lighting_add( light, LightPoint, &white, NULL, &(ltpt), 0, 0 );
	lighting_add(light, LightAmbient, &Dark, NULL, NULL, 0, 0);
	//lighting_add( light, LightPoint, &white, NULL, &(ltpt2), 0, 0 );
	
    ds = drawstate_create();
	ds->shade = ShadeGouraud;
    drawstate_setBody( ds, white );
    
	clock_t start = clock(), diff;
	
	
	Color colors[20] = { {{0.1, 0.1, 0.1}},
						{{0.1, 0.1, 0.1}},
						{{0.1, 0.1, 0.1}},
						{{0.1, 0.1, 0.1}},
						{{0.1, 0.1, 0.1}},
						{{0.1, 0.1, 0.1}},
						{{0.2, 0.2, 0.2}},
						{{0.2, 0.2, 0.2}},
						{{0.5, 0.5, 0.5}},
						{{0.5, 0.5, 0.5}},
						{{0.5, 0.5, 0.5}},
						{{0.5, 0.5, 0.5}},
						{{0.5, 0.5, 0.5}},
						{{0.5, 0.5, 0.5}},
						{{0.5, 0.5, 0.5}},
						{{0.9, 0.9, 0.9}},
						{{0.9, 0.9, 0.9}},
						{{0.9, 0.9, 0.9}},
						{{0.9, 0.9, 0.9}},
						{{1.0, 1.0, 1.0}}
										};
	
   // module_draw_ray_cel( forest, &GTM, &view, ds , light, src, colors, 20);
	module_draw_ray( forest, &GTM, &view, ds , light, src);

	

//	module_draw(scene, &VTM, &GTM, ds, light, src);
	
	
	
	diff = clock() - start;

	int msec = diff * 1000 / CLOCKS_PER_SEC;

	printf("Time taken %d seconds %d milliseconds", msec/1000, msec%1000);

	
	printf("drew scene\n");
	image_write(src, "../images/ray-forest-cel.ppm");
    
    // free stuff here
    //module_delete( scene );
    image_free( src );
    
    return(0);
}