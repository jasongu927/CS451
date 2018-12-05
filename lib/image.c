#include "image.h"
#include "ppmIO.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*
* Allocates an Image structure and initializes
* the top level fields to appropriate values. Allocates space for an image of the specified size, unless
* either rows or cols is 0. Returns a pointer to the allocated Image structure. Returns a NULL pointer if
* the operation fails.
*/
Image *image_create(int rows, int cols){
	
	//malloc an image
	Image *image = (Image*)malloc(sizeof(Image));
	
	image -> rows = rows;
	image -> cols = cols;
	
	//check to see rows or cols are valid
	if (rows == 0 || cols == 0){
		return image;
	}
	
	image -> data = (FPixel**)malloc(sizeof(FPixel*)*rows);
	if(image -> data == NULL){
		free(image);
		return NULL;
	}
	image -> data[0] = (FPixel*)malloc(sizeof(FPixel) * rows * cols);
	if(image -> data[0] == NULL){
		free(image);
		return NULL;
	}
	//set all of my outer aray
	int i;
	for(i = 1; i < rows; i++){
		image -> data[i] = &image -> data[0][i * cols];
	}
	
	image_fillz(image, 1.0);
	
	return image;
}

/*
* de-allocates image data and frees the Image structure.
*/
void image_free(Image *src){
	if(src -> data != NULL){
		if(src -> data[0] != NULL){
			free(src -> data[0]);
		}
		free(src -> data);
	}
	
	free(src);
}

/*
* given an uninitialized Image structure, sets the rows and cols
* fields to zero and the data field to NULL.
*/
void image_init(Image *src){
	src -> rows = 0;
	src -> cols = 0;
	src -> data = NULL;
	image_fillz(src, 1.0);
}

/*

*/
int image_alloc(Image *src, int rows, int cols){
	//just always free the memory to be safe since checks are in there anyway
	if(src -> data != NULL){
		if(src -> data[0] != NULL){
			free(src -> data[0]);
		}
		free(src -> data);
	}
	
	printf("reset fields of image\n");
	
	src -> rows = rows;
	src -> cols = cols;
	
	src -> data = (FPixel**)malloc(sizeof(FPixel*)*rows);
	if(src -> data == NULL){
		return -1;
	}
	src -> data[0] = (FPixel*)malloc(sizeof(FPixel) * rows * cols);
	if(src -> data[0] == NULL){
		return -1;
	}
	//set all of my outer aray
	int i;
	for(i = 1; i < rows; i++){
		src -> data[i] = &src -> data[0][i * cols];
	}
	
	int j;
	for(i = 0; i < rows; i++){
		for(j = 0; j < cols; j++){
			SET_ARGBZ(src->data[i][j], 0.0, 0.0, 0.0, 1.0, 1.0);
		}
	}
	return 0;
}

/*
* de-allocates image data and resets the Image structure
* fields. The function does not free the Image structure.
*/
void image_dealloc(Image *src){
	//I'm not sure how this is different from my init so I will just call it here??		
	if(src -> data != NULL){
		if(src -> data[0] != NULL){
			free(src -> data[0]);
		}
		free(src -> data);
		src -> data = NULL;
	}
		image_init(src);
	
}

/*
*  reads a PPM image from the given filename. An optional
* extension is to determine the image type from the filename and permit the use of different file
* types. Initializes the alpha channel to 1.0 and the z channel to 1.0. Returns a NULL pointer if the
* operation fails.
*/
Image *image_read(char *filename){
	int rows;
	int cols;
	int colors;
	Pixel* src = readPPM(&rows, &cols, &colors, filename);
	Image* image = image_create(rows, cols);
	
	int i;
	for(i = 0; i < rows * cols; i++){
		SET_ARGBZ((image -> data[0][i]), src[i].r/255.0, src[i].g/255.0, src[i].b/255.0, 1.0, 1.0);
	}
	
	free(src);
	return image;
}

/*
* writes a PPM image to the given filename. Returns 0 on success. 
*/
int image_write(Image *src, char *filename){
	if(src == NULL){
		return -1;
	}
	if(src -> data == NULL){
		return -1;
	}
	Pixel* image = (Pixel*)malloc(sizeof(Pixel) * src -> rows * src -> cols);
	int i;
	//for now do nothing with the alpha or Z channels
	for(i = 0; i < src -> rows * src -> cols; i++){
		
		//I choose to make a ROUND macro
		image[i].r = ROUND(src -> data[0][i].rgb[RED] * 255);
		image[i].g = ROUND(src -> data[0][i].rgb[GREEN] * 255);
		image[i].b = ROUND(src -> data[0][i].rgb[BLUE] * 255);	
		}
	
	writePPM(image, src -> rows, src -> cols, 255, filename);
	free(image);
	return 0;
}

/*
* returns the FPixel at (r, c).
*/
FPixel image_getf(Image *src, int r, int c){
	return src -> data[r][c];
}

/*
* returns the value of band b at pixel (r, c)
*/
float image_getc(Image *src, int r, int c, int b){
	return src -> data[r][c].rgb[b];
}

/*
* returns the alpha value at pixel(r,c)
*/
float image_geta(Image *src, int r, int c){
		return src -> data[r][c].a;
}

/*
* returns the depth value at pixel(r,c)
*/
float image_getz(Image *src, int r, int c){
	return src -> data[r][c].z;
}

/*
* sets the values of the pixel (r,c) to the FPixel val
*/
void image_setf(Image *src, int r, int c, FPixel val){
	//I'm not sure if we can directly set src[r][c] to val so I'm just gonna be safe and copy
	SET_ARGBZ(src -> data[r][c], val.rgb[RED], val.rgb[GREEN], val.rgb[BLUE], val.a, val.z);
}

/*
* sets the value of pixel (r, c) band b to val.
*/
void image_setc(Image *src, int r, int c, int b, float val){
	src -> data[r][c].rgb[b] = val;
}

/*
* sets the alpha value of pixel (r,c) to val
*/
void image_seta(Image *src, int r, int c, float val){
	src -> data[r][c].a = val;
}

/*
* sets the depth value of pixel (r,c) to val
*/
void image_setz(Image *src, int r, int c, float val){
	src -> data[r][c].z = val;
}

/*
* resets every pixel to a default value (e.g. Black, alpha value of 1.0, z value of 1.0).
*/
void image_reset(Image *src){
	int i;
	for(i = 0; i < src -> rows * src -> cols; i++){
		SET_ARGBZ(src->data[0][i], 0.0, 0.0, 0.0, 1.0, 1.0);
	}
}

/*
* sets every FPixel to the given value.
*/
void image_fill(Image *src, FPixel val){
	int i;
	for(i = 0; i < src -> rows * src -> cols; i++){
		SET_ARGBZ(src -> data[0][i], val.rgb[RED], val.rgb[GREEN], val.rgb[BLUE], val.a, val.z);
	}	
}

/*
* sets the (r, g, b) values of each pixel to the given color.
*/
void image_fillrgb(Image *src, float r, float g, float b){
	int i;
	for(i = 0; i < src -> rows * src -> cols; i++){
		SET_RGB(src -> data[0][i], r, g, b);
	}	
}
 
/*
*  sets the alpha value of each pixel to the given value.
*/
void image_filla(Image *src, float a){	
	int i;
	for(i = 0; i < src -> rows * src -> cols; i++){
		src -> data[0][i].a = a;
	}	
}

/*
*  sets the z value of each pixel to the given value.
*/
void image_fillz(Image *src, float z){
	int i;
	for(i = 0; i < src -> rows * src -> cols; i++){
		src -> data[0][i].z = z;
	}
}