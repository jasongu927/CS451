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

void make_teapot(Module *mod, int division, int fill){
	int i;
	Color red;
	color_set(&red, 0.75, 0.3, 0.3 );
	Color Green = {{0.2, 0.7, 0.2}};
	Module *md = module_create();
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
	module_module(mod, md);
}


int main(int argc, char *argv[]) {
    Image *src;
    Matrix VTM;
	
	
    Matrix GTM;
    Module *scene;
    float angle;
    int rows = 720;
    int cols = (rows * 16)/9 ;
    int i, j, k, idx;
    
	Tex_cube_map* tex = map_create("orange");
	Tex_cube_map* rainbow = map_create("rainbow");
	
    DrawState *ds;
    View3D view;
    
    Color white;
	Color green;
	Color red;
	Color brown;
    color_set(&white, 0.6, 0.6, 0.6 );
	color_set(&green, 0.1, 0.7, 0.1);
	color_set(&red, 0.7, 0.1, 0.1);
	color_set(&brown, 0.7, 0.35, 0.1);
	Color Dark = {{0.2, 0.2, 0.2}};

    // initialize the image
    src = image_create(rows, cols);
    
    // initialize matrices
    matrix_identity(&GTM);
    matrix_identity(&VTM);
    
    // set the View parameters
    point_set3D(&(view.vrp), 0.0, 4.0, 10.0);
    vector_set(&(view.vpn), 0.0, -4.0, -10.0);
    vector_set(&(view.vup), 0.0, 1.0, 0.0);
	view.d = 2.0;
	view.du = 1.6;
	view.dv = src->rows * view.du / src->cols;
    view.f = 0.0;
    view.b = 100;
    view.screenx = cols;
    view.screeny = rows;
    matrix_setView3D(&VTM, &view);
    
    // print out VTM
    printf("Final VTM: \n");
    matrix_print(&VTM, stdout);
    
    // make module
    scene = module_create();


	module_translate(scene, -1, -1, -2);
	Module* teapot = module_create();
	//module_rotateX(teapot, cos(M_PI/4), sin(M_PI/4));
	module_setTexture(teapot, rainbow);
	make_teapot(teapot, 5, 1);
	module_module(scene, teapot);
	module_identity(scene);
	
	
	
	Module* test = module_create();
	module_translate(test, 2, 1, 0);
	
	module_sphere(test, 0);
	Module* test2 = module_create();
	//module_rotateY(test2, cos(3*M_PI/2), sin(3*M_PI/2));
	module_translate(test2, -2, 0, 0);
	module_setTexture(test2, rainbow);
	module_sphere(test2, 4);
	module_module(test, test2);
	Module* test3 = module_create();
	module_translate(test3, 0, -2, 0);
	module_setTexture(test3, tex);
	module_sphere(test3, 4);
	module_module(test, test3);
	module_module(scene, test);
	
	
	
	module_identity(scene);
		
	module_scale(scene, 20, 20, 0.5);	
	module_translate(scene, 0, 0, -10);

	module_bodyColor(scene, &brown);
	
	module_cube(scene, 2);
	module_identity(scene);
	Lighting* light;
	Point ltpt2;
	point_copy(&ltpt2, &view.vrp);
	ltpt2.val[Y] = ltpt2.val[Y] - 1;
	light = lighting_create();
	lighting_add( light, LightPoint, &white, NULL, &(ltpt2), 0, 0 );
	Point ltpt = {{2.0, 2.0, 10, 1.0}};
	lighting_add( light, LightPoint, &white, NULL, &(ltpt), 0, 0 );
	lighting_add(light, LightAmbient, &Dark, NULL, NULL, 0, 0);
	//lighting_add( light, LightPoint, &white, NULL, &(ltpt2), 0, 0 );
	
    ds = drawstate_create();
	ds->shade = ShadeGouraud;
    drawstate_setBody( ds, red );
    
	clock_t start = clock(), diff;
	
    module_draw_ray( scene, &GTM, &view, ds , light, src);
	module_clear(scene);
	

//	module_draw(scene, &VTM, &GTM, ds, light, src);
	
	
	
	diff = clock() - start;

	int msec = diff * 1000 / CLOCKS_PER_SEC;

	printf("Time taken %d seconds %d milliseconds", msec/1000, msec%1000);

	
	printf("drew scene\n");
	image_write(src, "../images/ray-test.ppm");
    
    // free stuff here
    module_clear( scene );
	image_free( src );
    
	
    return(0);
}