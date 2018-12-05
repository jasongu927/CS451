#ifndef IMAGE_H

#define IMAGE_H

//for value bands
#define RED 0 
#define GREEN 1
#define BLUE 2

#define SET_RGB(pixel, r, g, b) (pixel).rgb[RED] = (r); (pixel).rgb[GREEN] = (g); (pixel).rgb[BLUE] = (b)

#define SET_ARGBZ(pixel, r, g, b, alpha, depth) SET_RGB(pixel, r, g, b); (pixel).a = (alpha); (pixel).z = (depth)

#define ROUND(x) (x) - (int)(x) > 0.5 ? (int)(x)+1 : (int)(x) 

typedef struct{
	float rgb[3];
	float a; // alpha
	float z; // depth
} FPixel;

typedef struct{
	int rows;
	int cols;
	FPixel **data;
} Image;

Image *image_create(int rows, int cols);

void image_free(Image *src);

void image_init(Image *src);

int image_alloc(Image *src, int rows, int cols);

void image_dealloc(Image *src);

Image *image_read(char *filename);

int image_write(Image *src, char *filename);

FPixel image_getf(Image *src, int r, int c);

float image_getc(Image *src, int r, int c, int b);

float image_geta(Image *src, int r, int c);

float image_getz(Image *src, int r, int c);

void image_setf(Image *src, int r, int c, FPixel val);

void image_setc(Image *src, int r, int c, int b, float val);

void image_seta(Image *src, int r, int c, float val);

void image_setz(Image *src, int r, int c, float val);

void image_reset(Image *src);

void image_fill(Image *src, FPixel val);

void image_fillrgb(Image *src, float r, float g, float b);
 
void image_filla(Image *src, float a);

void image_fillz(Image *src, float z);
#endif
