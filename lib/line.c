#include <math.h>
#include <stdlib.h>
#include "line.h"

/*
* initialize a 2D line.
*/
void line_set2D(Line *l, double x0, double y0, double x1, double y1){
	l->a.val[X] = x0;
	l->a.val[Y] = y0;
	l->a.val[Z] = 0.0;
	l->a.val[H] = 1.0;

	l->b.val[X] = x1;
	l->b.val[Y] = y1;
	l->b.val[Z] = 0.0;
	l->b.val[H] = 1.0;
	
	l -> zBuffer = 1;
}

/*
* initialize a line to ta and tb.
*/
void line_set(Line *l, Point ta, Point tb){
	l -> a = ta;
	l -> b = tb;
	
	l -> zBuffer = 1;
}

/*
* set the z-buffer flag to the given value.
*/
void line_zBuffer(Line *l, int flag){
	l -> zBuffer = flag;
}

/*
* normalize the x and y values of the endpoints by their homogeneous coordinate
*/
void line_normalize(Line *l){
	point_normalize(&(l->a));
	point_normalize(&(l->b));
}

/*
* copies the line data structure
*/
void line_copy(Line *to, Line *from){
	point_copy(&(to -> a), &(from -> a));
	point_copy(&(to -> b), &(from -> b));
	to -> zBuffer = from -> zBuffer;
}

int clip_line(Line *l, Image *src, Color c, Line *res){
	line_zBuffer(res, l->zBuffer);
	float start = 0.0;
	float end = 1.0;
	int i;
	int j;
	float pk;
	float qk;
	float tk;
	for(i = 0; i < 2; i ++){
		for(j = 0; j < 2; j++){
			if(j == 0){
				pk = -(l->b.val[i]  - l->a.val[i]);
				qk =  l ->a.val[i] - 0;
			}
			else{
				pk = (l->b.val[i]  - l->a.val[i]);
				if( i == 0){
					qk = src -> cols - 1 -  l -> a.val[i];
				}
				else{
					qk = src -> rows - 1 - l -> a.val[i];
				}
			}
			//check for vertical
			if(pk == 0){
				if(qk < 0){
					return -1;
				}
				else{
					continue;
				}
			}
			tk = qk/pk;
			if(pk < 0){
				start = MAX(start, tk);
			}
			else{
				end = MIN(end, tk);
			}
			if(start >= end){
				return -1;
			}
		}
	}
	//this is so dumb that I need to do this because yay floats
	res -> a.val[X] = l -> a.val[X] + (l -> b.val[X] - l -> a.val[X]) * start;
	res->a.val[X] = (int)MIN((float)src->cols, MAX(0.0, res->a.val[X]));
	
	res -> a.val[Y] = l -> a.val[Y] + (l -> b.val[Y] - l -> a.val[Y]) * start; 
	res->a.val[Y] = (int)MIN((float)src->rows, MAX(0.0, res->a.val[Y]));
	
	res -> a.val[Z] = l -> a.val[Z] + (l -> b.val[Z] - l -> a.val[Z]) * start; 	
	
	res -> b.val[X] = l -> a.val[X] + (l -> b.val[X] - l -> a.val[X]) * end;
	res->b.val[X] = (int)MIN((float)src->cols, MAX(0.0, res->b.val[X]));
	
	res -> b.val[Y] = l -> a.val[Y] + (l -> b.val[Y] - l -> a.val[Y]) * end; 
	res->b.val[Y] = (int)MIN((float)src->rows, MAX(0.0, res->b.val[Y]));
	
	res -> b.val[Z] = l -> a.val[Z] + (l -> b.val[Z] - l -> a.val[Z]) * end; 	
			
	return 0;
}

static inline void zBuffer_draw(Image *src, int x, int y, float curz, Color c){
	if(curz > src -> data[y][x].z){
		src -> data[y][x].z = curz;
		float scaleZ = cos(1/curz * M_PI/2);
		image_setColor(src, x, y, ((Color){{c.c[RED] * (scaleZ), c.c[GREEN] * (scaleZ), c.c[BLUE] * (scaleZ)}}));
	}		
}

/*
* draw line using Bresenham's Algorithm
*/ 
void line_draw(Line *l0, Image *src, Color c){
	Line lc;
	Line* l = &lc;
	int flag = clip_line(l0, src, c, l);
	
		
	if(flag == -1){
		return;
	}
	
	float dzPerScan;
	float curz;
	//THIS IS BAD CODE BUT WE NEED TO DEAL WITH INF
	float bz = l->b.val[Z] > 0 ? MAX(0.0001, l->b.val[Z]) :  MIN(-0.0001, l->b.val[Z]);
	float az = l->a.val[Z] > 0 ? MAX(0.0001, l->a.val[Z]) :  MIN(-0.0001, l->a.val[Z]);
	dzPerScan = (1/bz - 1/az);
	curz = 1/az;

	//check to see if it is vertical
	if(l -> a.val[X] == l -> b.val[X]){
		int x = l -> a.val[X];
		float dy;

		if(l -> zBuffer == 1){
			dy = l -> b.val[Y] - l -> a.val[Y];
			dzPerScan/= dy;
		}		
		//if we are in the 4th quadrent
		if(l -> a.val[Y] > l -> b.val[Y]){
			int y;
			for(y = l -> a.val[Y] - 1; y >= l -> b.val[Y]; y--){
				if(l -> zBuffer == 1){
					zBuffer_draw(src, x, y, curz, c);		
					curz += dzPerScan;
				}
				else{
					image_setColor(src, x, y, c);
				}
			}
			return;
		}
		else{ // we are in the 2nd quadrent
			int y;
			for(y = l -> a.val[Y]; y < l -> b.val[Y]+1; y++){
				if(l -> zBuffer == 1){
					zBuffer_draw(src, x, y, curz, c);				
					curz += dzPerScan;
				}
				else{
					image_setColor(src, x, y, c);
				}
			}
			return;
		}
	}
	//check to see if it is horizontal
	if(l -> a.val[Y] == l -> b.val[Y]){
		float dx;

		if(l -> zBuffer == 1){
			dx = l -> b.val[X] - l -> a.val[X];
			curz = 1/az;
		}	
		
		int y = l -> a.val[Y];
		
		//if we are in the 3rd quadrent
		if(l -> a.val[X] > l -> b.val[X]){
			int x;
			for(x = l -> a.val[X] -1 ; x >= l -> b.val[X]+1; x--){
				if(l -> zBuffer == 1){
					zBuffer_draw(src, x, y, curz, c);		
					curz += dzPerScan;
				}
				else{
					image_setColor(src, x, y, c);
				}
			}
			return;
		}
		else{ // we are in the 1st quadrent
			int x;
			for(x = l -> a.val[X]; x < l -> b.val[X]; x++){
				if(l -> zBuffer == 1){
					zBuffer_draw(src, x, y, curz, c);						
					curz += dzPerScan;
				}
				else{
					image_setColor(src, x, y, c);
				}
			}
			return;
		}
	}
	
	Point a;
	Point b;
	
	//flip the points if you need to
	if(l -> b.val[Y] < l -> a.val[Y]){
		a = l -> b;
		b = l -> a;
	} else{
		a = l -> a;
		b = l -> b;
	}
		
		
	
	bz = b.val[Z] > 0 ? MAX(0.0001, b.val[Z]) :  MIN(-0.0001, b.val[Z]);
	az = a.val[Z] > 0 ? MAX(0.0001, a.val[Z]) :  MIN(-0.0001, a.val[Z]);	
	
	//draw our line from a to b
	int dx = (int)(b.val[X] - a.val[X]);
	int dy = (int)(b.val[Y] - a.val[Y]);

	dzPerScan = (1/bz - 1/az);
	
	int adx = abs(dx);
	int ady = abs(dy);
	
	
	// we are in octant 1/4
	if(abs(dx) > dy){
		dzPerScan/=dx;
		int x;
		int y = a.val[Y];
		if(l -> zBuffer == 1){
			zBuffer_draw(src, a.val[X], y, curz, c);				
		}
		else{
			image_setColor(src, a.val[X], y, c);
		}
		int error = (int)(3 * ady - 2 * adx);
		//test to see if x should increase or decrease
		if(dx > 0){
			for(x = a.val[X]; x < b.val[X]; x++){
				if(error > 0){
					y++;
					error -= 2 * adx;
				}
				error += 2 * ady;
				if(l -> zBuffer == 1){
					curz += dzPerScan;
					zBuffer_draw(src, x, y, curz, c);						
				}
				else{
					image_setColor(src, x, y, c);
				}
			}
		}
		else{
			for(x  = a.val[X]; x > b.val[X]; x--){
				if(error > 0){
					y++;
					error -= 2 * adx;
				}
				error += 2 * ady;
				if(l -> zBuffer == 1){
					curz += dzPerScan;
					zBuffer_draw(src, x, y, curz, c);					
				}
				else{
					image_setColor(src, x, y, c);
				}
			}
		}
	}
	//y will always increase
	else{
		//printf("case 2\n");
		dzPerScan/= dy;

		int x = a.val[X];
		int y;
		int signx = a.val[X] > b.val[X] ? -1 : 1;
		if(l -> zBuffer == 1){
			zBuffer_draw(src, x, a.val[Y], curz, c);
		}
		else{
			image_setColor(src, x, a.val[Y], c);
		}
		int error = (int)(3 * adx - 2 * ady);
		for(y  = a.val[Y]; y < (int)b.val[Y]; y++){
			if(error > 0){
				x+= signx;
				error -= 2 * ady;
			}
			error += 2 * adx;
			if(l -> zBuffer == 1){
				curz += dzPerScan;
				zBuffer_draw(src, x, y, curz, c);
			}
			else{
				image_setColor(src, x, y, c);
			}
		}
	}
	
}

/*
* draws a line with the desired dash length
*/
void line_draw_dashes(Line *l, Image *src, Color c, int dLength){
	int count = 0; // for drawing dotted lines
	
	//check to see if it is horizontal
	if(l -> a.val[X] == l -> b.val[X]){
		int x = l -> a.val[X];
		//if we are in the 4th quadrent
		if(l -> a.val[Y] > l -> b.val[Y]){
			int y;
			for(y = l -> a.val[Y] - 1; y >= l -> b.val[Y]; y--){
				if(count < dLength){
					image_setColor(src, x, y, c);
				}
				count++;
				count = count % (dLength*2);
			}
			return;
		}
		else{ // we are in the 2nd quadrent
			int y;
			for(y = l -> a.val[Y]; y < l -> b.val[Y]+1; y++){
				if(count < dLength){
					image_setColor(src, x, y, c);
				}
				count++;
				count = count % (dLength*2);
			}
			return;
		}
	}
	//check to see if it is vertical
	if(l -> a.val[Y] == l -> b.val[Y]){
		int y = l -> a.val[Y];
		
		//if we are in the 3rd quadrent
		if(l -> a.val[X] > l -> b.val[X]){
			int x;
			for(x = l -> a.val[X] -1 ; x >= l -> b.val[X]+1; x--){
				if(count < dLength){
					image_setColor(src, x, y, c);
				}
				count++;
				count = count % (dLength*2);
			}
			return;
		}
		else{ // we are in the 1st quadrent
			int x;
			for(x = l -> a.val[X]; x < l -> b.val[X]; x++){
				if(count < dLength){
					image_setColor(src, x, y, c);
				}
				count++;
				count = count % (dLength*2);
			}
			return;
		}
	}
	
	Point a;
	Point b;
	
	//flip the points if you need to
	if(l -> b.val[Y] < l -> a.val[Y]){
		a = l -> b;
		b = l -> a;
	} else{
		a = l -> a;
		b = l -> b;
	}
	
	//draw our line from a to b
	int dx = (int)(b.val[X] - a.val[X]);
	int dy = (int)(b.val[Y] - a.val[Y]);

	int adx = abs(dx);
	int ady = abs(dy);
	
	
	// we are in octant 1/4
	if(abs(dx) > dy){
		int x;
		int y = a.val[Y];
		image_setColor(src, a.val[X], y, c);
		int error = (int)(3 * ady - 2 * adx);
		//test to see if x should increase or decrease
		if(dx > 0){
			for(x = a.val[X]; x < b.val[X]; x++){
				if(error > 0){
					y++;
					error -= 2 * adx;
				}
				error += 2 * ady;
				if(count < dLength){
					image_setColor(src, x, y, c);
				}
				count++;
				count = count % (dLength*2);
			}
		}
		else{
			for(x  = a.val[X]; x > b.val[X]; x--){
				if(error > 0){
					y++;
					error -= 2 * adx;
				}
				error += 2 * ady;
				if(count < dLength){
					image_setColor(src, x, y, c);
				}
				count++;
				count = count % (dLength*2);
			}
		}
	}
	//y will always increase
	else{
		int x = a.val[X];
		int y;
		int signx = a.val[X] > b.val[X] ? -1 : 1;
		image_setColor(src, x, a.val[Y], c);
		int error = (int)(3 * adx - 2 * ady);
		for(y  = a.val[Y]; y < b.val[Y]; y++){
			if(error > 0){
				x+= signx;
				error -= 2 * ady;
			}
			error += 2 * adx;
			if(count < dLength){
				image_setColor(src, x, y, c);
			}
			count++;
			count = count % (dLength*2);
		}
	}
	
}