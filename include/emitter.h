#ifndef EMITTER_H

#define EMITTER_H

#include <stdlib.h>
#include <stdio.h>
#include "image.h"
#include "point.h"
#include "matrix.h"

#define MAX_PARTICLES 100000
#define P_PER_UPDATE 100

typedef enum{
	ParticleStart,
	ParticleHot,
	ParticleCold,
	ParticleDead
} ParticleState;

typedef struct{
	ParticleState state;
	float temperature;
	Point location;
	Vector velocity;
	Vector acceleration;
	void *next;
} Particle;

Particle * particle_create(Point startLoc);
//don't need free function

typedef void (*particleColorFunction) (Particle* p, Color* c);
typedef void (*particleUpdateFunction) (Particle* p);
typedef void (*particleStartFunction) (Particle* p);


typedef struct{
	Particle *head;
	Particle *tail;
	int numParticles;
	int create_new;
	particleColorFunction colorFunc;
	particleUpdateFunction updateFunc;
	particleStartFunction startFunc;
	Point location;
} Emitter;

Emitter* emitter_create(Point location, int flag, particleStartFunction sf, particleColorFunction cf, particleUpdateFunction uf);
void emitter_insert(Emitter *e, Particle *p);
void emitter_update(Emitter *e);
void emitter_draw(Emitter *e, Matrix *VTM, Matrix *GTM, Image* src);
void emitter_clear(Emitter *e);
void emitter_reset(Emitter *e, int flag);


#endif