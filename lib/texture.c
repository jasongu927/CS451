#include "texture.h"
#include <math.h>

Tex_cube_map* map_create(char* filename){
	Tex_cube_map* tex = malloc(sizeof(Tex_cube_map));
	char path[50];
	char localPath[60];
	sprintf(path, "../textures/%s/texture/", filename);
	//get textures
	sprintf(localPath, "%s/%s.ppm", path, "top");
	tex->tex.top = image_read(localPath);
	sprintf(localPath, "%s/%s.ppm", path, "bottom");
	tex->tex.bottom = image_read(localPath);
	sprintf(localPath, "%s/%s.ppm", path, "right");
	tex->tex.right = image_read(localPath);
	sprintf(localPath, "%s/%s.ppm", path, "left");
	tex->tex.left = image_read(localPath);
	sprintf(localPath, "%s/%s.ppm", path, "front");
	tex->tex.front = image_read(localPath);
	sprintf(localPath, "%s/%s.ppm", path, "back");
	tex->tex.back = image_read(localPath);

	//get bumps
	sprintf(path, "../textures/%s/bump/", filename);
	sprintf(localPath, "%s/%s.ppm", path, "top");
	tex->bump.top = image_read(localPath);
	sprintf(localPath, "%s/%s.ppm", path, "bottom");
	tex->bump.bottom = image_read(localPath);
	sprintf(localPath, "%s/%s.ppm", path, "right");
	tex->bump.right = image_read(localPath);
	sprintf(localPath, "%s/%s.ppm", path, "left");
	tex->bump.left = image_read(localPath);
	sprintf(localPath, "%s/%s.ppm", path, "front");
	tex->bump.front = image_read(localPath);
	sprintf(localPath, "%s/%s.ppm", path, "back");
	tex->bump.back = image_read(localPath);

	return tex;
}

void bilin_interp_image(Image* image, float startu, float startv, Color* result){
	float u, v;
	int lowu, lowv, highu, highv;
	Color tempC0, tempC1, tempC;

	
	u = startu * (image->cols-1);
	v = startv * (image->rows-1);
	//printf("u is %f, v is %f\n",u,v);
	lowu = MAX(0, (int)u);
	lowv = MAX(0, (int)v);
	if(u+1 > image->cols - 1){
		highu = image->cols - 1;
		u = highu;
	}else{
		highu = (int)u+1;
	}
	if(v+1 > image->rows - 1){
		highv = image->rows - 1;
		v = highv;
	}else{
		highv = (int)v+1;
	}
	//highu = MIN(image->cols-1, (int)(u+1));
	//highv = MIN(image->rows-1, (int)(v+1));
	tempC = image_getColor(image, lowv, lowu);
	// scale along x(u)
	color_multiply_float(&tempC, u-lowu, &tempC); 
	color_copy(&tempC0, &tempC);
	
	tempC = image_getColor(image, lowv, highu);
	color_multiply_float(&tempC, highu-u, &tempC);
	color_add(&tempC0, &tempC, &tempC0);
	
	//second part
	tempC = image_getColor(image, highv, lowu);
	color_multiply_float(&tempC, u-lowu, &tempC); 
	color_copy(&tempC1, &tempC);
	
	tempC = image_getColor(image, highv, highu);
	color_multiply_float(&tempC, highu-u, &tempC);
	color_add(&tempC1, &tempC, &tempC1);
	
	//scale in other direction and add
	color_multiply_float(&tempC0, v-lowv, &tempC0);
	color_multiply_float(&tempC1, highv-v, &tempC1);
	color_add(&tempC0, &tempC1, result);
}

//this could be pre-computed into u,v but I'm lazy and now I need to store an origonal norm and points -- or just store a pointer to the origonal obj!?
void calculate_tex_sphere(Tex_cube_map* tex, Point* point, Vector* norm, Color* result, Vector* bump){//, Vector* du, Vector* dv){
	Vector dir;
	vector_copy(&dir, point);
	vector_normalize(&dir);
	
	//vector_copy(&dir, point);
	//vector_normalize(&dir);
	//vector_add(norm, &dir, &dir);
	//vector_normalize(&dir);
	
	int signX = (dir.val[X] > 0)? 1 : -1;
	int signY = (dir.val[Y] > 0)? 1 : -1;
	int signZ = (dir.val[Z] > 0)? 1 : -1;
	
	float absX = fabs(dir.val[X]);
	float absY = fabs(dir.val[Y]);
	float absZ = fabs(dir.val[Z]);
	
	
	float u,v;
	Image* texImage;
	Image* bumpImage;
	
	// I may need to properly project from the point...
	// uggh this will not be fun (can I just add them???)
	
	//up/down
	if(absY > absX && absY > absZ){
		//top
		if(signY == 1){
			texImage = tex->tex.top;
			bumpImage = tex->bump.top;
			u = 0.5 * (dir.val[X] / absY + 1);
			v = 0.5 * (dir.val[Z] / absY + 1);
		}
		//bottom
		else{
			texImage = tex->tex.bottom;
			bumpImage = tex->bump.bottom;
			u = 0.5 * (dir.val[X] / absY + 1);
			v = 0.5 * (dir.val[Z] / absY + 1);
		}
	//front/back
	} else if (absX > absY && absX > absZ){
		if(signX == 1){
			texImage = tex->tex.front;
			bumpImage = tex->bump.front;
			u = 0.5 * (-dir.val[Z] / absX + 1);
			v = 0.5 * (-dir.val[Y] / absX + 1);
		}else{
			texImage = tex->tex.back;
			bumpImage = tex->bump.back;
			u = 0.5 * (dir.val[Z] / absX + 1);
			v = 0.5 * (-dir.val[Y] / absX + 1);
		}
	//right/left
	} else{
		if(signZ == 1){
			texImage = tex->tex.right;
			bumpImage = tex->bump.right;
			u = 0.5 * (dir.val[X] / absZ + 1);
			v = 0.5 * (-dir.val[Y] / absZ + 1);
		}else{
			texImage = tex->tex.left;
			bumpImage = tex->bump.left;
			u = 0.5 * (-dir.val[X] / absZ + 1);
			v = 0.5 * (-dir.val[Y] / absZ + 1);
		}
		
	}
	//printf("got image\n");
	bilin_interp_image(texImage, u, v, result);
	//bump map
	
	if(result->c[X] == 0.0){
		printf("u is %f, v is %f\n", u, v);
	}
	Color tempC;
	bilin_interp_image(bumpImage, u, v, &tempC);
	for(int i = 0; i < 3; i++){
		bump->val[i] = tempC.c[i] - 0.5;
	}
}

void free_cube(Cube_map* cube){
	free(&cube->top);
	free(&cube->bottom);
	free(&cube->front);
	free(&cube->back);
	free(&cube->right);
	free(&cube->left);
}

void free_tex(Tex_cube_map* map){
	free_cube(&map->tex);
	free_cube(&map->bump);
	free(map);
}