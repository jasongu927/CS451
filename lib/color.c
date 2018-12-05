#include "color.h"

/*
* copies the Color data
*/
void color_copy(Color *to, Color *from){
	to -> c[RED] = from -> c[RED];
	to -> c[GREEN] = from -> c[GREEN];
	to -> c[BLUE] = from -> c[BLUE];
}

void color_copy_bounds(Color *to, Color *from){
	to -> c[RED] = MAX(0.0, MIN(1.0, from -> c[RED]));
	to -> c[GREEN] = MAX(0.0, MIN(1.0, from -> c[GREEN]));
	to -> c[BLUE] = MAX(0.0, MIN(1.0, from -> c[BLUE]));
}

/*
* sets the Color data.
*/
void color_set(Color *to, float r, float g, float b){
	to -> c[RED] = r;
	to -> c[GREEN] = g;
	to -> c[BLUE] = b;
}

//multiplies two colors
void color_multiply(Color *a, Color *b, Color *c){
	c -> c[RED] = a -> c[RED] * b -> c[RED];
	c -> c[GREEN] = a -> c[GREEN] * b -> c[GREEN];
	c -> c[BLUE] = a -> c[BLUE] * b -> c[BLUE];	
}

//adds two colors
void color_add(Color *a, Color *b, Color *c){
	c -> c[RED] = a -> c[RED] + b -> c[RED];
	c -> c[GREEN] =a -> c[GREEN] + b -> c[GREEN];
	c -> c[BLUE] = a -> c[BLUE] + b -> c[BLUE];	
}

//adds two colors
void color_add_bounds(Color *a, Color *b, Color *c){
	c -> c[RED] = a -> c[RED] + b -> c[RED];
	c -> c[GREEN] =a -> c[GREEN] + b -> c[GREEN];
	c -> c[BLUE] = a -> c[BLUE] + b -> c[BLUE];	
	
	for(int i = 0; i < 3; i++){
		c->c[i] = MAX(0.0, MIN(1.0, c->c[i]));
	}
}

//multiplies a color by a float
void color_multiply_float(Color *a, float b, Color *c){
	c -> c[RED] = a -> c[RED] * b;
	c -> c[GREEN] = a -> c[GREEN] * b;
	c -> c[BLUE] = a -> c[BLUE] * b;
}

/*
* copies the Color data to the proper pixel
* I changed this to x,y because I was trying to run it using row,col, but got confused with x,y
*/
void image_setColor(Image *src, int x, int y, Color val){
	SET_RGB(src -> data[y][x], val.c[RED], val.c[GREEN], val.c[BLUE]);
}

/*
* copies the Color data to the proper pixel
* I changed this to x,y because I was trying to run it using row,col, but got confused with x,y
*/
void image_setColorSave(Image *src, int x, int y, Color val, int iteration){
	SET_RGB(src -> data[y][x], val.c[RED], val.c[GREEN], val.c[BLUE]);
	char buffer[30];
	sprintf(buffer, "image%d.ppm",iteration);
	image_write(src, buffer);
}

/*
*  returns a Color structure built from the pixel values.
*/
Color image_getColor(Image *src, int r, int c){
	Color result;
	color_set(&result, src -> data[r][c].rgb[RED], src -> data[r][c].rgb[GREEN], src -> data[r][c].rgb[BLUE]);
	return result;
}

void color_print(Color *c, FILE *fp){
	fprintf(fp, "Color is r: %f, g: %f, b: %f\n", c->c[RED], c->c[GREEN], c->c[BLUE]);
}
