#include "emitter.h"

Particle * particle_create(Point startLoc){
	//printf("in particle create\n");
	Particle *p = (Particle*) malloc(sizeof(Particle));
	//printf("malloced thingy\n");
	point_copy(&p -> location, &startLoc);
	p -> temperature = 1.0;
	p -> next = NULL;
	p -> state = ParticleStart;
	p -> velocity = (Vector){{0.0, 0.0, 0.0, 0.0}};
	p -> acceleration = (Vector){{0.0, 0.0, 0.0, 0.0}};
	//printf("leaving particle create\n");
	return p;
}

Emitter* emitter_create(Point location, int flag, particleStartFunction sf, particleColorFunction cf, particleUpdateFunction uf){
	printf("creating emitter\n");
	Emitter* e = malloc(sizeof(Emitter));
	point_copy(&e->location, &location);
	e -> create_new = flag;
	e -> startFunc = sf;
	e -> colorFunc = cf;
	e -> updateFunc = uf;
	e -> head = NULL;
	e -> tail = NULL; 
	e -> numParticles = 0;
	if(flag == 0){
		Particle *newp;
		for(int i = 0; i < P_PER_UPDATE * 20; i++){
			if(e->numParticles < MAX_PARTICLES){
				newp = particle_create(e->location);
				e -> startFunc(newp);
				emitter_insert(e, newp);
			}
		}
		
	}
	
	//printf("created emitter\n");
	return e;
}

void emitter_insert(Emitter *e, Particle *p){
	if ( e == NULL || p == NULL || e -> numParticles >= MAX_PARTICLES)
    {
        return;
    }
    
    //if md was empty, start new list
    if ( e->head == NULL )
    {
        e->head = p;
        e->tail = p;
		e->numParticles++;
		return;
    }
    
    //otherwise, attach to tail
    e->tail->next = p;
    e->tail = p;
	e->numParticles++;
}

void emitter_update(Emitter *e){
	//add new particles
	if(e -> create_new == 1){
		Particle *newp;
		for(int i = 0; i < P_PER_UPDATE; i++){
			if(e->numParticles < MAX_PARTICLES){
				newp = particle_create(e->location);
				e -> startFunc(newp);
				emitter_insert(e, newp);
			}
		}
	}
	
	
	Particle* cur = e->head;
	if(cur == NULL){
		return;
	}
	if(cur -> state == ParticleDead){
		e->head = (Particle*) cur->next;
		free(cur);
		e -> numParticles--;
		cur = e->head;
	}
	Particle *next;
	float tmp;
	while(cur != NULL && cur -> next != NULL){
		e->updateFunc(cur);
		cur -> location.val[X] += cur -> velocity.val[X];
		cur -> location.val[Y] += cur -> velocity.val[Y];
		cur -> location.val[Z] += cur -> velocity.val[Z];
		
		
		tmp = cur -> velocity.val[X] + cur -> acceleration.val[X];
		if(tmp * cur -> velocity.val[X] > 0.0){
			cur -> velocity.val[X] = tmp;
		}			
		
		tmp = cur -> velocity.val[Y] + cur -> acceleration.val[Y];
		if(tmp * cur -> velocity.val[Y] > 0.0){
			cur -> velocity.val[Y] = tmp;
		}			
		
		tmp = cur -> velocity.val[Z] + cur -> acceleration.val[Z];
		if(tmp * cur -> velocity.val[Z] > 0.0){
			cur -> velocity.val[Z] = tmp;
		}			
		next = (Particle *) cur-> next;
		if(next->state == ParticleDead){
			cur -> next = next -> next;
			free(next);
			e -> numParticles--;
		}

		cur = (Particle*) cur -> next;
	}
	//do the last node if it was not deleted
	if(cur!= NULL){
		e->updateFunc(cur);
	}
	
}

void emitter_draw(Emitter *e, Matrix *VTM, Matrix *GTM, Image* src){
	//printf("drawing particle\n");
	Particle* cur = e->head;
	Color partC;
	Color imageC;
	Color c;
	Point loc;
	Point tmp;
	while(cur != NULL){
		if(cur -> state == ParticleDead){
			cur = (Particle*) cur->next;
			continue;
		}
		point_copy(&loc, &cur->location);
		matrix_xformPoint(GTM, &loc, &tmp);
		matrix_xformPoint(VTM, &tmp, &loc);
		point_normalize(&loc);
		//point_print(&loc, stdout);
		if((int)loc.val[X] < 0 || (int)loc.val[X] >= src -> cols || (int)loc.val[Y] < 0 || (int)loc.val[Y] >= src -> rows){
			//printf("continuing\n");
			cur = (Particle*) cur->next;
			continue;
		}
		
		e->colorFunc(cur, &partC);
		imageC = image_getColor(src, (int)loc.val[Y], (int)loc.val[X]);
		
		//alpha blend the colors
		
		c.c[RED] = cur -> temperature * partC.c[RED] + (1.0-cur -> temperature) * imageC.c[RED];
		c.c[GREEN] = cur -> temperature * partC.c[GREEN] + (1.0-cur -> temperature) * imageC.c[GREEN];
		c.c[BLUE] = cur -> temperature * partC.c[BLUE] + (1.0-cur -> temperature) * imageC.c[BLUE];
		
		point_draw(&loc, src, c);//maybe this shouldn't be linear and that should be defined in the colorFunc as a drawFunc instead
		
		cur = (Particle*) cur->next;
	}
}

void emitter_clear(Emitter *e){
	Particle* cur = e->head;
	Particle *tmp;
	
	int i = 0;
	while(cur != NULL){
		tmp = cur;
		cur = (Particle*) cur->next;
		free(tmp);
		i++;
	}
	e -> head = NULL;
	e -> tail = NULL; 
	e -> numParticles = 0;
}

void emitter_reset(Emitter *e, int flag){
	emitter_clear(e);
	if(flag == 1){
		Particle *newp;
		for(int i = 0; i < P_PER_UPDATE * 20; i++){
			if(e->numParticles < MAX_PARTICLES){
				newp = particle_create(e->location);
				e -> startFunc(newp);
				emitter_insert(e, newp);
			}
		}
		
	}
}