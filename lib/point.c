#include <stdlib.h>
#include <math.h>
#include "point.h"


/*
* set the first two values of the vector to x and y. 
* Set the third value to 0.0 and the fourth value to 1.0.
*/
void point_set2D(Point *p, double x, double y){
	p -> val[X] = x;
	p -> val[Y] = y;
	p -> val[Z] = 0.0;
	p -> val[H] = 1.0;
}

/*
*  set the point’s values to x and y and z. Sets the homogeneous coordinate to 1.0.
*/
void point_set3D(Point *p, double x, double y, double z){
	p -> val[X] = x;
	p -> val[Y] = y;
	p -> val[Z] = z;
	p -> val[H] = 1.0;
}

/*
* set the four values of the vector to x, y, z, and h, respectively.
*/
void point_set(Point *p, double x, double y, double z, double h){
	p -> val[X] = x;
	p -> val[Y] = y;
	p -> val[Z] = z;
	p -> val[H] = h;
}

/*
* normalize the x and y values of a point by its homogeneous coordinate: x = x/h, y = y/h.
*/
void point_normalize(Point *p){
	p -> val[X] = p -> val[X]/p -> val[H];
	p -> val[Y] = p -> val[Y]/p -> val[H];
}

/*
* copy the point data structure.
*/
void point_copy(Point *to, Point *from){
	to -> val[X] = from -> val[X];
	to -> val[Y] = from -> val[Y];
	to -> val[Z] = from -> val[Z];
	to -> val[H] = from -> val[H];
}

/*
* draw the point into src using color c.
*/
void point_draw(Point *p, Image *src, Color c){
	float invZ = 1.0/p->val[Z];
	int x = (int)p -> val[X];
	int y = (int)p -> val[Y];
	if(x >= 0 &&  x < src -> cols && y >= 0&& y < src -> rows){		
		if( invZ > src -> data[y][x].z){
			src -> data[y][x].z = invZ;
			image_setColor(src, x, y, c);
		}
	}	
}

/*
* draw the p into src using FPixel c.
*/
void point_drawf(Point *p, Image *src, FPixel c){
	image_setf(src, (int)p -> val[X], (int)p -> val[Y], c);
}

/*
* use fprintf to print the contents of the Point to the stream fp. The FILE pointer fp can be an actual
* file opened using fopen or a standard output stream like stdout or stderr, both defined in stdio.h. The
* point print function doesn’t care which it is, it just uses fprintf, which takes a FILE pointer as the first
* argument.
*/
void point_print(Point *p, FILE *fp){
//	fprintf(fp, "point value is (x : %f, y : %f, z : %f)\n", p -> val[X], p -> val[Y], p -> val[Z]);
	fprintf(fp, "point value is (x : %f, y : %f, z : %f) with homogeneous coordinate %f\n", p -> val[X], p -> val[Y], p -> val[Z], p -> val[H]);
}