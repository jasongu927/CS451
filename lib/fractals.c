
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"
#include "fractals.h"

void mandelbrot(Image *im, float x0, float y0, float dx){
	
	int i;
	int j;
	int c;

	
	//how do I calculate DY why can't I figure this out 
	//width = dx / cols
	//height =  dy / rows
	//width/height = (dx / cols)/(dy / rows)
	// dy / rows = dx / cols
	// dy = dx / cols * rows
	float dy  = (dx / im -> cols) * im -> rows;
	
	float cx;
	float cy;
	
	float zx;
	float zy;
	
	float zx_next;
	float zy_next;
	
	FPixel p;
	
	for(i = 0; i < im -> rows; i++){
		for(j = 0; j < im -> cols; j++){
			cx = dx/im -> cols * j + x0;
			cy = -(dy/ im -> rows * i) + y0 + dy;
			zx = 0;
			zy = 0;
			
			zx_next = 0;
			zy_next = 0;
			
			for(c = 0; c < 255*2; c++){
				zx_next = zx * zx - zy * zy - cx;
				zy_next = 2 * zx * zy - cy;
				
				zx = zx_next;
				zy = zy_next;
				
				if(SQUARE(zx, zy) > 4.0){
					break;
				}
			}
			// zero attractor is white, infinite attractor blended
			p = (FPixel){ {(c/2)/255.0, (c/2)/255.0, (c/2)/255.0}, 0.0, 1.0};
			image_setf(im, i, j, p);
		}
	}
}

void julia(Image *im, float x0, float y0, float dx){
	int i;
	int j;
	int c;

	
	float dy  = (dx / im -> cols) * im -> rows;
	
	float cx = 0.7454054;
	float cy = 0.1130063;
	
	float zx;
	float zy;
	
	float zx_next;
	float zy_next;
	
	FPixel p;
	
	for(i = 0; i < im -> rows; i++){
		for(j = 0; j < im -> cols; j++){
			//zx = dx/im -> cols * j + x0;
			//zy = -(dy/im -> rows * i) + y0 + dy;
			
			//from aziz from http://lodev.org/cgtutor/juliamandelbrot.html
			zx = 1.5 * (j - im -> cols/2)/(im -> cols / 2 * dx) + x0;
			zy = (i - im -> rows / 2) / (im -> rows / 2 * dy) + y0; // had + dy but I didn't like it :p
			
			for(c = 0; c < 255*2; c++){
				zx_next = zx * zx - zy * zy - cx;
				zy_next = 2 * zx * zy - cy;
				
				zx = zx_next;
				zy = zy_next;
				
				if(SQUARE(zx, zy) > 4.0){
					break;
				}
			}
			// zero attractor is white, infinite attractor blended
			p = (FPixel){ {(c/2)/255.0, (c/2)/255.0, (c/2)/255.0}, 0.0, 1.0};
			image_setf(im, i, j, p);		
			}
	}
}

void julia_pick_values(Image *im, float x0, float y0, float dx, float cx, float cy){
	int i;
	int j;
	int c;

	
	float dy  = (dx / im -> cols) * im -> rows;
	
	float zx;
	float zy;
	
	float zx_next;
	float zy_next;
	
	FPixel p;
	
	for(i = 0; i < im -> rows; i++){
		for(j = 0; j < im -> cols; j++){
			//zx = dx/im -> cols * j + x0;
			//zy = -(dy/im -> rows * i) + y0 + dy;
			
			//from aziz from http://lodev.org/cgtutor/juliamandelbrot.html
			zx = 1.5 * (j - im -> cols/2)/(im -> cols / 2 * dx) + x0;
			zy = (i - im -> rows / 2) / (im -> rows / 2 * dy) + y0; // had + dy but I didn't like it :p
			
			for(c = 0; c < 255*2; c++){
				zx_next = zx * zx - zy * zy - cx;
				zy_next = 2 * zx * zy - cy;
				
				zx = zx_next;
				zy = zy_next;
				
				if(SQUARE(zx, zy) > 4.0){
					break;
				}
			}
			// zero attractor is white, infinite attractor blended
			p = (FPixel){ {(c/2)/255.0, (c/2)/255.0, (c/2)/255.0}, 0.0, 1.0};
			image_setf(im, i, j, p);		
			}
	}
}

/*
* creates n julia sets between 0 and 2Pi, intended for making gifs and cool stuff
* I found on wikipedia a set that was z^2 + 0.7885 e^ia for a from 0 to 2pi
*/
void iterate_julia_sets(int rows, int cols, float x0, float y0, float dx, int iterations){
  Image *src;
  
  float di = 2*M_PI/iterations; //amount to vary by iteration
  float cur = 0;
  int i;
  float cx;
  float cy;
  char filename[30];

  for(i = 0; i < iterations; i++){
  	//calculate e^ia using euler's formula
  	src = image_create(rows, cols);
  	cx = 0.7885*cos(cur);
  	cy = 0.7885*sin(cur);
  	julia_pick_values(src, x0, y0, dx, cx, cy);
  	
  	sprintf(filename, "julia%d.ppm",i);
  	
  	image_write(src, filename);
  	image_free(src);
  	cur += di;
  }
  
} 
// perlin noise, taken from wikipedia psudeo code

/*
* linear interpolation between x0 and x1 
* Weight w should be in the range (0.0, 1.0)
*/
float lerp(float x0, float x1, float w){
	return(1.0 - 2) * x0 + w * x1;
}

float dotGridGradient(int x0, int y0, float x, float y, float* gradient, int rows, int cols){
	float dx = x - x0;
	float dy = y - y0;
	
	//compute dot product
	float gradx = gradient[y0 * cols * 2 + x0 * 2];
	float grady = gradient[y0 * cols * 2 + x0 * 2 + 1];
	
	return(dx * gradx + dy * grady);
}

/*
* calculates the noise
* all these print statements are because I did stuff with ints and then 
* was using src instead of src3 in my tests but I want you to understand
* what I went through
*/
float perlin(float x, float y, float* gradient, int rows, int cols){
	int x0 = (int)x;
	int y0 = (int)y;
	
	int x1 = x + 1;
	int y1 = y + 1;
	
	
	float sx = x - x0;
	float sy = y - y0;
	
	//printf("sx and sy are %f , %f \n", sx, sy);
	
	float n0, n1, ix0, ix1, value;
	
	
	n0 = dotGridGradient(x0, y0, x, y, gradient, rows, cols);
	n1 = dotGridGradient(x1, y0, x, y, gradient, rows, cols);
	
	//printf("n0 and n1 are %f , %f \n", n0, n1);
	
	ix0 = lerp(n0, n1, sx);
	
	//printf("ix0 is %f\n", ix0);
	
	n0  = dotGridGradient(x0, y1, x, y, gradient, rows, cols);
	n1 = dotGridGradient(x1, y1, x, y, gradient, rows, cols);
	
	//printf("n0 and n1 are %f , %f \n", n0, n1);
	
	ix1 = lerp(n0, n1, sx);
	
	//printf("ix1 is %f\n", ix1);

	value = lerp(ix0, ix1, sy);
	
	//printf("value is %f\n", value);
	
	return value;
}

/*
* applies the perlin noise function to every pixel in the image
*/
void applyPerlin(Image* im, float offx, float offy){
	int rows = im -> rows;
	int cols = im -> cols;
	
	float* gradients = (float*)malloc(sizeof(int) * rows * cols * 2);
	int i;
	int j;
		
	for(i = 0; i < rows * cols * 2; i++){
		gradients[i] = ((float)rand())/RAND_MAX * 2 - 1.0;
	}
	FPixel p;
	float noise_val;
	for(i = 0; i < rows; i++){
		for(j = 0; j< cols; j++){
			if(i == (rows-1) || j == (cols-1)){
				//printf("at boundary %d, %d \n", i, j);
				// need to deal with boundaries
				// LETS DO NOTHING!!
				continue;
			}
			else{
				noise_val = perlin(((float)j) + offx, ((float)i) + offy, gradients, rows, cols);
				//printf("noise is %f at %d , %d \n", noise_val, i, j);
				noise_val += 1;
				noise_val = noise_val/2;
				//printf("adjusted noise is %f \n", noise_val);

				p = (FPixel){ {noise_val, noise_val, noise_val}, 0.0, 1.0};
				image_setf(im, i, j, p);
				
				//test if it went into the thingy
				//printf("noise in image is %f\n", im -> data[i][j].rgb[RED]);
						
			}
		}
	}
	free(gradients);
}

/*
* creates fractal perlin noise with n iterations
* creates n perlin noise images and then combines them with increasing weight
*/
Image *fractalPerlinNoise(int rows, int cols, float offx, float offy, int iterations){
	Image *img = image_create(rows, cols);
	Image *result = image_create(rows, cols);
	image_fillrgb(result, 0, 0, 0);
	
	int sum = 1;
	int i = iterations;
	while(i > 0){
		sum+= i;
		i--;
	}
	
	int j;
	float pcolor;
	float curcolor;
	FPixel p;
	
	for(i = 0; i < iterations; i++){
		//printf("on %d iteration\n",i);
		applyPerlin(img, offx, offy);
		for(j = 0; j < rows * cols; j++){
			pcolor = (img -> data[0][j].rgb[RED] * (i+1))/sum;
			curcolor = result -> data[0][j].rgb[RED];
			//if(j == 1000){
			//	printf("pcolor is %f, curcolor is %f, addition is %f\n",pcolor, curcolor, pcolor+curcolor);
			//}
			p = (FPixel){ {pcolor+curcolor, pcolor+curcolor, pcolor+curcolor}, 0.0, 1.0};
			result -> data[0][j] = p;
		}
		image_dealloc(img);
		image_alloc(img, rows, cols);
	}
	
	image_free(img);
	return result;
}