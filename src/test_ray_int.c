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

int main(int argc, char *argv[]) {
   
    
    
    // make module

    //make triangle
    int x = 0;
    int y = 0;
    Polygon purfles;
    Point pt[3];
    polygon_init( &purfles );
    point_set3D( &(pt[0]), x+1, y+1, 0 );
    point_set3D( &(pt[1]), x-1, y+1, 0 );
    point_set3D( &(pt[2]), x+1, y-1, 0);

    polygon_set( &purfles, 3, pt );
	
	Point p;
	Ray ray;
	point_set3D(&ray.start, 0, 0, 2);
	vector_set(&ray.ray, 0, 0, -2);
	vector_normalize(&ray.ray);
	double inter = intersect_triangle_MT(&ray, &purfles, NULL);
	printf("intersection is %.2f\n", inter);


	vector_set(&ray.ray, 1, 0, -2);
	vector_normalize(&ray.ray);
	inter =  intersect_triangle_MT(&ray, &purfles, NULL);
	printf("intersection is %.2f\n", inter);	
		
	vector_set(&ray.ray, -1, 1.1, -2);
	vector_normalize(&ray.ray);
	inter =  intersect_triangle_MT(&ray, &purfles, NULL);
	printf("intersection is %.2f\n", inter);
//    module_draw_ray( scene, &GTM, &view, ds, NULL, src);
/*
	Lighting* light = lighting_create();
	lighting_add( light, LightPoint, &white, NULL, &(view.vrp), 0, 0 );
	lighting_add(light, LightAmbient, &Dark, NULL, NULL, 0, 0);
	module_draw(scene, &VTM, &GTM, ds, light, src);
	free(light);
	*/

    
    return(0);
}