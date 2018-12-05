#include "lighting.h"

void light_init (Light *light){
	light -> type = LightNone;
	light -> color = (Color){{0, 0, 0}};
	light -> direction = (Vector){{1, 0, 0, 0}};// I don't think a vector can be 0
	light -> position = (Point){{0, 0, 0, 1}};
	light -> cutoff = 0;
	light -> sharpness = 1;
}

void light_copy (Light *to, Light *from){
	to -> type = from -> type;
	color_copy(&to->color, &from->color);
	vector_copy(&to->direction, &from -> direction);
	point_copy(&to -> position, &from -> position);
	to -> cutoff = from -> cutoff;
	to -> sharpness = from -> sharpness;
}

//lighting functions
Lighting *lighting_create(void){
	Lighting *lights =  malloc(sizeof(Lighting));
	lights -> nLights = 0;
	return lights;
}

void lighting_init( Lighting *l){
	l -> nLights = 0;
	//don't need to empty cuz nlights
}

void lighting_add( Lighting *l, LightType type, Color *c, Vector *dir, Point *pos, float cutoff, float sharpness){
	l -> light[l -> nLights].type = type;
	color_copy(&l -> light[l -> nLights].color, c);
	if(dir != NULL){
		vector_copy(&l -> light[l -> nLights].direction, dir);
	}
	if(pos != NULL){
		point_copy(&l -> light[l -> nLights].position, pos);
	}
	l -> light[l -> nLights].cutoff	= cutoff;
	l -> light[l -> nLights].sharpness = sharpness;
	l -> nLights++;
}

static void ambient_shading( Light *l, Vector *N, Vector *V, Point *p, Vector *L, Color *Cb, Color *Cs, float s, int oneSided, Color *c){
	//printf("doing ambient shading\n");
	color_multiply(&(l-> color), Cb ,c);
}	

static void direct_shading( Light *l, Vector *N, Vector *V, Point *p, Vector *L, Color *Cb, Color *Cs, float s, int oneSided, Color *c){
	//printf("doing direct shading\n");
	Color tmp;
	Vector dir;
	for(int i = 0; i < 3; i++){
		dir.val[i] = -l->direction.val[i];
	}
	color_multiply(Cb, &(l -> color), &tmp);
	color_multiply_float(&tmp, vector_dot(&dir, N), c);
}	

void point_shading( Light *l, Vector *N, Vector *V, Point *p, Vector *L, Color *Cb, Color *Cs, float s, int oneSided, Color *c){
	//printf("doing point shading\n");
	Color body;
	Color tmp;
	Color surface;
	
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	color_multiply(Cb, &(l -> color), &tmp);
	printf("body is color %.2f %.2f %.2f\n", tmp.c[0], tmp.c[1], tmp.c[2] );
	float angle = vector_dot(L, N);
	printf("angle is %f\n", angle);
	color_multiply_float(&tmp, angle, &body);
	printf("body is color %.2f %.2f %.2f\n", body.c[0], body.c[1], body.c[2] );
	
	
	
	Vector Hv;
	Hv.val[X] = (L -> val[X] + V -> val[X])/2;
	Hv.val[Y] = (L -> val[Y] + V -> val[Y])/2;
	Hv.val[Z] = (L -> val[Z] + V -> val[Z])/2;
	vector_normalize(&Hv);
	
	color_multiply(Cs, &(l ->color), &tmp);
	printf("surface is color %.2f %.2f %.2f\n", tmp.c[0], tmp.c[1], tmp.c[2] );
	angle = vector_dot(&Hv, N);
	printf("angle is %f\n", angle);
	angle = pow(angle, s);
	printf("angle is %f after being multiplied by %f\n", angle, s);
	color_multiply_float(&tmp,  angle, &surface);
	printf("surface is color %.2f %.2f %.2f\n", surface.c[0], surface.c[1], surface.c[2] );

	color_add(&body, &surface, c);
	printf("final color is \n");
	color_print(c, stdout);
}	

static int spot_shading( Light *l, Vector *N, Vector *V, Point *p, Vector *L, Color *Cb, Color *Cs, float s, int oneSided, Color *c){
	//printf("doing spot shading\n");
	
	float alpha = vector_dot(L, &(l -> direction));
	
	if(alpha > l -> cutoff){
		Color tmp;
		point_shading(l, N, V, L, p, Cb, Cs, s, oneSided, &tmp);
		color_multiply_float(&tmp, pow(alpha, l -> sharpness), c);
		return 1;
	}
	else 
		return -1;
}	


void lighting_shading( Lighting *l, Vector *N, Vector *V, Point *p, Color *Cb, Color *Cs, float s, int oneSided, Color *c){
	Color tmp = {{0, 0, 0}};
	Color sumC = {{0, 0, 0}};
	int i;
	float angle;
	
	for(i = 0; i < l -> nLights; i++){
		color_set(&tmp, 0.0, 0.0, 0.0);
		Vector L;
	
		//printf("calculating lighting of type %d\n", l -> light[i].type);

		if((l -> light[i].type != LightAmbient) && (l -> light[i].type != LightDirect)){ 
			L.val[X] = l->light[i].position.val[X] - p->val[X];
			L.val[Y] = l->light[i].position.val[Y] - p->val[Y] ;
			L.val[Z] = l->light[i].position.val[Z] - p->val[Z] ;
			
			angle = vector_dot(&L, N);
			vector_normalize(&L);
			//printf("angle is %f\n", angle);
			if (oneSided && angle <= 0){
				continue;
			}
		}
		
		//printf("continuing with light of type %d\n", l -> light[i].type);
		switch(l -> light[i].type){
			case LightNone:
				continue;
			case LightAmbient:
				ambient_shading(&l->light[i], N, V, p, &L, Cb, Cs, s, oneSided, &tmp);
				break;
			case LightDirect:
				direct_shading(&l->light[i], N, V, p, &L, Cb, Cs, s, oneSided, &tmp);
				break;
			case LightPoint:
				point_shading(&l->light[i], N, V, p, &L, Cb, Cs, s, oneSided, &tmp);
				break;
			case LightSpot:
				spot_shading(&l->light[i], N, V, p, &L, Cb, Cs, s, oneSided, &tmp);
				break;
		}
		//printf("adding color %.2f %.2f %.2f\n", tmp.c[0], tmp.c[1], tmp.c[2] );
		color_add(&tmp, &sumC, &sumC);
		//printf("resulting color %.2f %.2f %.2f\n", sumC.c[0], sumC.c[1], sumC.c[2] );
	}
	color_copy(c, &sumC);
	//printf("final color %.2f %.2f %.2f\n", sumC.c[0], sumC.c[1], sumC.c[2] );
	//printf("final color %.2f %.2f %.2f\n", c->c[0], c->c[1], c->c[2] );
}
