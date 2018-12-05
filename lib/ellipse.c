#include <math.h>
#include <stdlib.h>
#include "circle.h" // include this for drawLineScan
#include "ellipse.h"

void ellipse_set(Ellipse *e, Point tc, double ta, double tb){
	e -> c = tc;
	e -> ra = ta;
	e -> rb = tb;
	e -> a = 0;
}

/*
* helper function for drawing a circle
*/
void ellipsePlotPoints(Image *src, int cX, int cY, int x, int y, Color c){
	image_setColor(src, cX + x, cY + y, c);
	image_setColor(src, cX - x - 1, cY - y - 1, c);
	image_setColor(src, cX + x, cY - y - 1, c);
	image_setColor(src, cX - x, cY + y, c);
}

/*
* taken from notes
*/
void ellipse_draw(Ellipse *e, Image *src, Color c){
	int cx = ROUND(e -> c.val[X]);
	int cy = ROUND(e -> c.val[Y]);
	int Rx = ROUND(e -> ra);
	int Ry = ROUND(e -> rb);
	
	int x = -1;
	int y = -Ry;
	int px = 2 * Ry * Ry;
	int py = 2 * Rx * Rx * (-y);
	
	ellipsePlotPoints(src, cx, cy, x, y, c);
	
	int p = Ry * Ry - Rx * Rx * Ry + Rx * Rx/4 + Ry * Ry + px;
	
	while(px < py){
		x--;
		px += 2 * Ry * Ry;
		if(p < 0){
			p += Ry * Ry + px;			
		}
		else{
			y++;
			py -= 2 * Rx * Rx;
			p += Ry * Ry + px - py;
		}
		ellipsePlotPoints(src, cx, cy, x, y, c);
	}
	
	p = Ry * Ry * (x * x + x) + Rx * Rx * (y * y - 2 * y + 1) - Rx * Rx * Ry * Ry + Rx * Rx - py;
	
	while( y < 0){
		y++;
		py -= 2 * Rx * Rx;
		if(p > 0){
			p += Rx * Rx - py;
		}
		else{
			x--;
			px += 2 * Rx * Rx;
			p += Rx * Rx - py + px;
		}
		ellipsePlotPoints(src, cx, cy, x, y, c);
	}
}

/*
* fills the inside of an ellipse same way as a circle
*/
void ellipse_drawFill(Ellipse *e, Image *src, Color c){
	ellipse_draw(e, src, c);
	//draw ellipse, then do a scan line fill down the center looking for the color c;
	int y = e -> c.val[Y];
	int x = e -> c.val[X];
	draw_line_scan(src, c, x, y);
	int i;
	for(i = 1; i < e -> rb; i++){
		draw_line_scan(src, c, x, y + i);
		draw_line_scan(src, c, x, y - i);
	}
}