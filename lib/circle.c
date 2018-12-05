#include <math.h>
#include <stdlib.h>
#include "circle.h"


void circle_set(Circle *c, Point tc, double tr){
	c -> r = tr;
	c -> c = tc;
}

/*
* helper function for drawing a circle
*/
void circlePlotPoints(Image *src, int cX, int cY, int x, int y, Color c){
	image_setColor(src, cX + x, cY + y, c);
	image_setColor(src, cX + y, cY + x, c);
	image_setColor(src, cX + y, cY - x - 1, c);
	image_setColor(src, cX + x, cY - y - 1, c);
	image_setColor(src, cX - x - 1, cY - y - 1, c);
	image_setColor(src, cX - y - 1, cY - x - 1, c);
	image_setColor(src, cX - y - 1, cY + x, c);
	image_setColor(src, cX - x - 1, cY + y, c);	
}

/*
* draws a circle based on the code given in the lecture notes
*/
void circle_draw(Circle *c, Image *src, Color color){
	//start in 3rd quadrent
	int cx = ROUND(c -> c.val[X]);
	int cy = ROUND(c -> c.val[Y]);
	int x = (int) -1;
	int y = - (ROUND(c -> r));
	
	int error = 1 - ROUND(c -> r);
	
	circlePlotPoints(src, cx, cy, x, y, color);
		

	while(x > y){
		x--;
		if(error < 0){
			error += 1 - 2 * x;
		}
		else{
			y++;
			error += 1 - 2 * (x - y);
		}
		circlePlotPoints(src, cx, cy, x, y, color);
	}
}

//helper function to draw to the left, and draw to the right
void draw_line_scan(Image *src, Color c, int x, int y){
	int curx = x;
	while(!(COLOR_EQUAL_PIXEL(c, src -> data[y][curx])) && curx > 0){
		image_setColor(src, curx, y, c);
		curx--;
	}
	curx = x+1;
	while(!(COLOR_EQUAL_PIXEL(c, src -> data[y][curx]))  && curx < src -> cols){
		image_setColor(src, curx, y, c);
		curx++;
		}
}

/*
* fills the inside of a circle 2 lines at a time moving from the center
*/
void circle_drawFill(Circle *c, Image *src, Color color){
	circle_draw(c, src, color);
	//draw circle, then do a scan line fill down the center looking for the color c;
	int y = c -> c.val[Y];
	int x = c -> c.val[X];
	draw_line_scan(src, color, x, y);
	int i;
	for(i = 1; i < c -> r-1; i++){
		draw_line_scan(src, color, x, y + i);
		draw_line_scan(src, color, x, y - i);
	}
}