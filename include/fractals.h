#ifndef FRACTALS_H

#define FRACTALS_H

#define SQUARE(x, y) (x) * (x) + (y) * (y)

void mandelbrot(Image *im, float x0, float y0, float dx);

void julia(Image *im, float x0, float y0, float dx);

float lerp(float x0, float x1, float w);

float dotGridGradient(int x0, int y0, float x, float y, float* gradient, int rows, int cols);

float perlin(float x, float y, float* gradient, int rows, int cols);

void applyPerlin(Image *im, float offx, float offy);

Image *fractalPerlinNoise(int rows, int cols, float offx, float offy, int iterations);

void julia_pick_values(Image *im, float x0, float y0, float dx, float cx, float cy);

void iterate_julia_sets(int rows, int cols, float x0, float y0, float dx, int iterations);

#endif
