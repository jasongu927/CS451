#include "module.h"
#include "viewing.h"
#include "ray.h"
#include "color.h"
#include <float.h>
#include <limits.h>

#define MAX_DEPTH 8
#define EPSILON 0.00001
#define SECONDARY_RAYS 3
#define FALLOFF 0.1

int total_rays = 0;

PolygonList* polygon_list_create(){
	PolygonList* list = malloc(sizeof(PolygonList));
	list->head = NULL;
	list->length = 0;
	return list;
}

PolygonList* polygon_node_add(PolygonList* list, Polygon *p){
	// if we are setting the top of the list
	PolygonNode* new = malloc(sizeof(PolygonNode));
	polygon_init(&new->p);
	polygon_copy(&new -> p, p);
	new->next = list->head;
	list->head = new;
	list->length++;
	return list;
}


void polygon_list_free(PolygonList* list){
	PolygonNode *cur = list->head;
	PolygonNode *next;
	while(cur != NULL){
		next = cur->next;
		polygon_clear(&cur->p);
		free(cur);
		cur = next;
	}
}

BoundingBoxNode* bb_node_create(){
	BoundingBoxNode* bbnode = malloc(sizeof(BoundingBoxNode));
	//these shouldn't be 0,0 but meh
	bbnode->minX = INT_MAX;
	bbnode->maxX = INT_MIN;
	bbnode->minY = INT_MAX;
	bbnode->maxY = INT_MIN;
	bbnode->minZ = INT_MAX;
	bbnode->maxZ = INT_MIN;
	bbnode->polygons = NULL;
	bbnode->bblist = NULL;
	bbnode->next = NULL;
	return bbnode;
}

BoundingBoxNode* bb_node_addPolygon(BoundingBoxNode* bbnode, Polygon* p){
	if(bbnode->polygons == NULL){
		bbnode->polygons = polygon_list_create();
	}
	for(int i = 0; i < 3; i++){
		bbnode->minX = MIN(bbnode->minX, p->vertex[i].val[X]);
		bbnode->maxX = MAX(bbnode->maxX, p->vertex[i].val[X]);
		
		bbnode->minY = MIN(bbnode->minY, p->vertex[i].val[Y]);
		bbnode->maxY = MAX(bbnode->maxY, p->vertex[i].val[Y]);
		
		bbnode->minZ = MIN(bbnode->minZ, p->vertex[i].val[Z]);
		bbnode->maxZ = MAX(bbnode->maxZ, p->vertex[i].val[Z]);
	}
	polygon_node_add(bbnode->polygons, p);
	return bbnode;
}

BoundingBoxNode* bb_node_add_bbnode(BoundingBoxNode* bbnode, BoundingBoxNode* new){
	if(bbnode -> bblist == NULL){
		bbnode -> bblist = bb_list_create();
	}
	bbnode->bblist = bb_list_addBoundingBox(bbnode->bblist, new);
	
	bbnode->minX = MIN(bbnode->minX, new->minX);
	bbnode->maxX = MAX(bbnode->maxX, new->maxX);
		
	bbnode->minY = MIN(bbnode->minY, new->minY);
	bbnode->maxY = MAX(bbnode->maxY, new->maxY);
		
	bbnode->minZ = MIN(bbnode->minZ, new->minZ);
	bbnode->maxZ = MAX(bbnode->maxZ, new->maxZ);
	
	return bbnode;
}

BoundingBoxList* bb_list_create(){
	BoundingBoxList* list = malloc(sizeof(BoundingBoxList));
	list-> head = bb_node_create();
	list->length = 0;
	list -> totalP = 0;
	return list;
}

BoundingBoxList* bb_list_addPolygon(BoundingBoxList* bblist, Polygon *p){
	BoundingBoxNode* bbnode = bblist->head;
	bblist->head = bb_node_addPolygon(bbnode, p);
	return bblist;
}


BoundingBoxList* bb_list_addBoundingBox(BoundingBoxList* bblist, BoundingBoxNode* new){
	// if we are setting the top of the list
	new -> next = bblist -> head;
	bblist -> head = new;
	bblist -> length++;
	if(new->polygons){
		bblist -> totalP += new->polygons->length;
	}
	if(new->bblist){
		bblist -> totalP += new->bblist->totalP;
	}
	
	return bblist;
}

void bb_list_free(BoundingBoxList* list){
	BoundingBoxNode *cur = list->head;
	BoundingBoxNode *next;

	while(cur != NULL){
		next = cur->next;
		if(cur->polygons){
			polygon_list_free(cur->polygons);
			free(cur->polygons);
		}
		if(cur->bblist){
			bb_list_free(cur->bblist);
		}
		free(cur);
		cur = next;
	}
	free(list);
}

// Moller-Trumbore intersection algorithm implimented from wikipedia
double intersect_triangle_MT(Ray *ray, Polygon *p, Vector* baryPt){
	Vector edge1, edge2, h, s, q;
	float a, f, u, v;	
	
	vector_subtract(&p->vertex[0], &p->vertex[1], &edge1);
	vector_subtract(&p->vertex[0], &p->vertex[2], &edge2);
	
	vector_cross(&ray -> ray, &edge2, &h);
	a = vector_dot(&edge1, &h);
	
	
	if(a > -EPSILON && a < EPSILON ){//something is fundamentally wrong here
		return 0;
	}
	
	f = 1.0/a;
	vector_subtract(&p->vertex[0], &ray->start, &s);
	u = f * vector_dot(&s, &h);
	if( u < 0.0 || u > 1.0){
		return 0;
	}
	
	vector_cross(&s, &edge1, &q);
	v = f * vector_dot(&ray->ray, &q);
	if( v < 0.0 || u + v > 1.0){
		return 0;
	}
	float t = f * vector_dot(&edge2, &q);
	if ( t > EPSILON){
		if(baryPt != NULL){
			baryPt->val[0] = u;
			baryPt->val[1] = v;
			baryPt->val[2] = 1-u-v;
		}
		return t;
	}
	else return 0;
}

//based on code from here https://tavianator.com/fast-branchless-raybounding-box-intersections-part-2-nans/
int intersect_bounding_box(Ray* ray, BoundingBoxNode* node){		
	if(node->minX == INT_MAX){
		return 0;
	}

	//vector_normalize(&ray->ray);
	float tmin = FLT_MIN;
	float tmax = FLT_MAX;
	float t1, t2;
	
	if(ray->ray.val[X] != 0){
		t1 = ((node->minX) - ray->start.val[X])/ray->ray.val[X];
		t2 = ((node->maxX) - ray->start.val[X])/ray->ray.val[X];
		
		tmin = MAX(tmin, MIN(t1, t2));
		tmax = MIN(tmax, MAX(t1, t2));
		//printf("tmin is %.2f, tmax is %.2f\n", tmin, tmax);
	} else if (ray->start.val[X] <= node->minX || ray->start.val[X] >= node->maxX){
		return 0;
	}
	
	if(ray->ray.val[Y] != 0){
		t1 = ((node->minY) - ray->start.val[Y])/ray->ray.val[Y];
		t2 = ((node->maxY) - ray->start.val[Y])/ray->ray.val[Y];
		
		tmin = MAX(tmin, MIN(t1, t2));
		tmax = MIN(tmax, MAX(t1, t2));
		//printf("tmin is %.2f, tmax is %.2f\n", tmin, tmax);
	} else if (ray->start.val[Y] <= node->minY || ray->start.val[Y] >= node->maxY){
		return 0;
	}
	
	if(ray->ray.val[Z] != 0){
		t1 = ((node->minZ) - ray->start.val[Z])/ray->ray.val[Z];
		t2 = ((node->maxZ) - ray->start.val[Z])/ray->ray.val[Z];
		
		tmin = MAX(tmin, MIN(t1, t2));
		tmax = MIN(tmax, MAX(t1, t2));
		//printf("tmin is %.2f, tmax is %.2f\n", tmin, tmax);
	} else if (ray->start.val[Z] <= node->minZ || ray->start.val[Z] >= node->maxZ){
		return 0;
	}

	if(tmax > tmin && tmax >0){
		return 1;
	} else {
		return 0;
	}
}

/*
* finds the closest polygon to the ray
*/
Polygon* find_closest_p(Ray *ray, PolygonList *list, double maxDist, double* dist, Vector* baryPt){
	double distance = maxDist;
	Polygon *closest = NULL;
	double tmpDist;
	PolygonNode* cur = list->head;
	while(cur != NULL){
		tmpDist = intersect_triangle_MT(ray, &cur->p, baryPt);
		if(tmpDist > EPSILON){
			if(tmpDist < distance){
				closest = &cur->p;
				distance = tmpDist;
			}
		}
		cur = (PolygonNode*) cur->next;
	}
	*dist = distance;
	return closest;
}

double find_closest_p_dist(Ray *ray, PolygonList *list, double maxDist){
	double distance = maxDist;
	double tmpDist;
	PolygonNode* cur = list->head;
	while(cur != NULL){
		tmpDist = intersect_triangle_MT(ray, &cur->p, NULL);
		if(tmpDist > EPSILON){
			if(tmpDist < distance){
				distance = tmpDist;
			}
		}
		cur = (PolygonNode*) cur->next;
	}
	return distance;
}

double find_closest_p_dist_end_early(Ray *ray, PolygonList *list, double maxDist){
	double distance = maxDist;
	double tmpDist;
	PolygonNode* cur = list->head;
	while(cur != NULL){
		tmpDist = intersect_triangle_MT(ray, &cur->p, NULL);
		if(tmpDist > EPSILON){
			if(tmpDist < distance){
				return tmpDist;
			}
		}
		cur = (PolygonNode*) cur->next;
	}
	return distance;
}

double find_closest_p_bb_dist_end_early(Ray *ray, BoundingBoxList* list, double maxDist){
	BoundingBoxNode* bbnode = list->head;
	double tmpdist;
	double distance = maxDist;
	while(bbnode != NULL){
		if(intersect_bounding_box(ray, bbnode)!= 0){
			if(bbnode->polygons){
				tmpdist = find_closest_p_dist_end_early(ray, bbnode->polygons, maxDist);
				if(tmpdist < distance){
					return tmpdist;
				}
			}
			if(bbnode->bblist){
				tmpdist = find_closest_p_bb_dist_end_early(ray, bbnode->bblist, maxDist);
				if(tmpdist < distance){
					return tmpdist;
				}
			}
		}
		bbnode = bbnode->next;
	}
	return distance;
}

Polygon* find_closest_p_bb(Ray *ray, BoundingBoxList* list, double maxDist, double* dist, Vector* baryPt){
	BoundingBoxNode* bbnode = list->head;
	//printf("list length is %d\n", list->length);
	Polygon *closest = NULL;
	Polygon *tmpclose = NULL;
	double tmpdist;
	double distance = maxDist;
	Vector tmpBpt;
	while(bbnode != NULL){
		if(intersect_bounding_box(ray, bbnode) != 0){
			if(bbnode->polygons){
				tmpclose = find_closest_p(ray, bbnode->polygons, maxDist, &tmpdist, &tmpBpt);
				if(tmpdist < distance){
					distance = tmpdist;
					closest = tmpclose;
					vector_copy(baryPt, &tmpBpt);
				}
			}
			if(bbnode->bblist != NULL){
				tmpclose = find_closest_p_bb(ray, bbnode->bblist, maxDist, &tmpdist, &tmpBpt);
				if(tmpdist < distance){
					distance = tmpdist;
					closest = tmpclose;
					vector_copy(baryPt, &tmpBpt);
				}
			}
		}
		bbnode = bbnode->next;
	}
	*dist = distance;
	return closest;
}


double find_closest_p_bb_dist(Ray *ray, BoundingBoxList* list, double maxDist){
	BoundingBoxNode* bbnode = list->head;
	double tmpdist;
	double distance = maxDist;
	while(bbnode != NULL){
		if(intersect_bounding_box(ray, bbnode)!= 0){
			if(bbnode->polygons){
				tmpdist = find_closest_p_dist(ray, bbnode->polygons, maxDist);
				if(tmpdist < distance){
					distance = tmpdist;
				}
			}
			if(bbnode->bblist){
				tmpdist = find_closest_p_bb_dist(ray, bbnode->bblist, maxDist);
				if(tmpdist < distance){
					distance = tmpdist;
				}
			}
		}
		bbnode = bbnode->next;
	}
	return distance;
}

static void set_up_org_polygon(Polygon* p){
	polygon_setOrgVertex(p, 3, p->vertex);
	
	if(p -> normal == NULL){
		Vector norms[3];
		Vector av, bv;
		for(int c = 0; c < 3; c++){
			av.val[X] = p->vertex[c].val[X] - p->vertex[(c+1)%3].val[X];
			av.val[Y] = p->vertex[c].val[Y] - p->vertex[(c+1)%3].val[Y];
			av.val[Z] = p->vertex[c].val[Z] - p->vertex[(c+1)%3].val[Z];
			
			bv.val[X] = p->vertex[c].val[X] - p->vertex[(c-1 + 3)%3].val[X];
			bv.val[Y] = p->vertex[c].val[Y] - p->vertex[(c-1 + 3)%3].val[Y];
			bv.val[Z] = p->vertex[c].val[Z] - p->vertex[(c-1 + 3)%3].val[Z];
			
			vector_cross(&bv, &av, &norms[c]);
			vector_normalize(&norms[c]);
		}
		polygon_setOrgNormals(p, 3, &norms);
	}
	else{
		polygon_setOrgNormals(p, 3, p->normal);
	}
}

static void set_up_polygon(Polygon* p, DrawState *ds){
	if(p -> normal == NULL){
		//set the normals
		Vector norms[3];
		Vector av, bv;
		for(int c = 0; c < 3; c++){
			av.val[X] = p->vertex[c].val[X] - p->vertex[(c+1)%3].val[X];
			av.val[Y] = p->vertex[c].val[Y] - p->vertex[(c+1)%3].val[Y];
			av.val[Z] = p->vertex[c].val[Z] - p->vertex[(c+1)%3].val[Z];
			
			bv.val[X] = p->vertex[c].val[X] - p->vertex[(c-1 + 3)%3].val[X];
			bv.val[Y] = p->vertex[c].val[Y] - p->vertex[(c-1 + 3)%3].val[Y];
			bv.val[Z] = p->vertex[c].val[Z] - p->vertex[(c-1 + 3)%3].val[Z];
			
			vector_cross(&bv, &av, &norms[c]);
			vector_normalize(&norms[c]);
		}
		polygon_setNormals(p, 3, &norms);
	}
	
		
	if(p->color == NULL){
		// set the colors
		Color colors[3];
		for(int i = 0; i <3; i++){
			color_copy(&colors[i], &ds->body);
			// do we need to store surface colors seperately or nahhhh
			// I think we need to add reflectance... etc fields into our polygons
		}
		polygon_setColors(p, 3, &colors);
	}
	
	if(ds->tex!=NULL){
		polygon_setTexture(p, ds->tex, ds->proj);
	}
	
	polygon_setReflectance(p, ds->reflectance);
	//pass reflectance/transmittance here
}

/*
* write pre pass to create polygon list based on module draw
*/
BoundingBoxNode* module_create_polygon_list(Module *md, Matrix *GTM, DrawState *ds){
	Element *cur = md -> head;
	Matrix LTM;
	BoundingBoxNode* bbnode = bb_node_create();
	
	matrix_identity(&LTM);
	//printf("- before while, coeff = %f\n", reflectCoeff);

	while(cur != NULL){
		switch(cur -> type){
		    case ObjNone: break;
            case ObjLine:	break;
			case ObjLight: break;
            case ObjPoint:
			break;
            case ObjPolyline:
                break;
            case ObjPolygon:{
			//printf("- case polygon\n");
			if(cur->obj.polygon.numVertex == 3){
					
				Polygon p;
				polygon_init(&p);

				polygon_copy(&p, &cur -> obj.polygon);
				set_up_org_polygon(&p);
				//printf("copied polygon\n");
				matrix_xformPolygon(&LTM, &p);
				matrix_xformPolygon(GTM, &p);

				set_up_polygon(&p, ds);

				
				bb_node_addPolygon(bbnode, &p);
				polygon_clear(&p);
			} else if(cur->obj.polygon.numVertex == 4){
				Polygon p;
				polygon_init(&p);
				polygon_copy(&p, &cur -> obj.polygon);


				Polygon p0, p1;

				polygon_init(&p0);
				Point pts[3];
				Vector norms[3];
				Color c[3];
				for(int i =0; i < 3;  i++){
					int index = 2-i;
					//printf("index 1 is %d\n", index);
					point_copy(&pts[i], &p.vertex[index]);
					if(p.normal != NULL){
						vector_copy(&norms[i], &p.normal[index]);
					}
					if(p.color != NULL){
						color_copy(&c[i], &p.color[index]);
					}
				}
				polygon_set(&p0, 3, &pts[0]);
				if(p.normal != NULL){
					polygon_setNormals(&p0, 3, &norms[0]);
				}
				if(p.color != NULL){
					polygon_setColors(&p0, 3, &c[0]);
				}
				set_up_org_polygon(&p0);
				matrix_xformPolygon(&LTM, &p0);
				matrix_xformPolygon(GTM, &p0);
				set_up_polygon(&p0, ds);

				//printf("giving polygon reflectance %f\n", reflectCoeff);
				
				bb_node_addPolygon(bbnode, &p0);
				
				
				//second polygon
				polygon_init(&p1);
				for(int i = 0; i < 3; i++){
					int index = ((4-i)%4);
					//printf("index 2 is %d\n", index);
					point_copy(&pts[i], &p.vertex[index]);
					if(p.normal != NULL){
						//printf("has normals\n");
						vector_copy(&norms[i], &p.normal[index]);
					}
					if(p.color != NULL){
						color_copy(&c[i], &p.color[index]);
					}
				}
				polygon_set(&p1, 3, &pts[0]);
				if(p.normal != NULL){
					polygon_setNormals(&p1, 3, &norms[0]);
				}
				if(p.color != NULL){
					polygon_setColors(&p1, 3, &c[0]);
				}
				set_up_org_polygon(&p1);
				matrix_xformPolygon(&LTM, &p1);
				matrix_xformPolygon(GTM, &p1);
				set_up_polygon(&p1, ds);

				//printf("giving polygon reflectance %f\n", reflectCoeff);
				
				bb_node_addPolygon(bbnode, &p1);
				
				polygon_clear(&p);
				polygon_clear(&p0);
				polygon_clear(&p1);
			} else break;
			break;
			}
			case ObjIdentity:
				matrix_identity(&LTM);
			break;
			case ObjMatrix:
				matrix_multiply( &cur -> obj.matrix, &LTM, &LTM);		
				
			break;
			case ObjColor: color_copy(&ds -> color, &cur -> obj.color);
			break;
			case ObjBodyColor: color_copy(&ds -> body , &cur -> obj.color);
			break;
			case ObjSurfaceColor: color_copy(&ds -> surface, &cur -> obj.color);
			break;
			case ObjSurfaceCoeff:  ds -> surfaceCoeff = cur -> obj.coeff;
			break;
            case ObjModule:{
				//printf("- case module\n");
				Matrix tempM;
				DrawState tempDS;

				matrix_identity(&tempM);
				drawstate_copy(&tempDS, ds);
				point_copy(&tempDS.viewer, &ds->viewer);
				
				matrix_multiply(GTM, &LTM, &tempM);
				
				Module* mod = (Module*) (cur -> obj.module);
				BoundingBoxNode* new = module_create_polygon_list(mod, &tempM, &tempDS);
				bb_node_add_bbnode(bbnode, new);
			break;
            }
            case ObjBezier:{
                break;
            }
			case ObjReflectance: {
				drawstate_setReflectance(ds, cur->obj.reflectance);
				break;
			}
			case ObjTexture:{
				drawstate_setTexture(ds, cur->obj.texture);
				break;
			}case ObjProjectionType:{
				drawstate_setProjection(ds, cur->obj.proj);
			}break;
			//printf("- end module_create_polygon_list\n");
		}
	//printf("about to iterate\n");
	cur = (Element*)cur -> next;
	//printf("iterated\n");
	}
	//printf("done\n");
	return bbnode;
}

BoundingBoxList* create_polygon_list(Module *md, Matrix *GTM, DrawState *ds){
	BoundingBoxList* list = bb_list_create();
	BoundingBoxNode* node = module_create_polygon_list(md, GTM, ds);
	bb_list_addBoundingBox(list, node);
	return list;
}

void calculate_barypoint(Vector* v, Vector* baryPt, Vector* result){
	Vector tmpV;
	Vector norm = {{0.0, 0.0, 0.0, 0.0}};
	
	vector_copy(&tmpV, &v[1]);
	vector_scale(&tmpV, baryPt->val[0]);
	vector_add(&norm, &tmpV, &norm);
	
	vector_copy(&tmpV, &v[2]);
	vector_scale(&tmpV, baryPt->val[1]);
	vector_add(&norm, &tmpV, &norm);	
	
	vector_copy(&tmpV, &v[0]);
	vector_scale(&tmpV, (baryPt->val[2]));
	vector_add(&norm, &tmpV, result);
}

/*
* Shoots ray into the scene.
* returns 0 if it does not intersect.
* puts the color into the result pointer.
*/
int shoot_ray(Ray *ray, BoundingBoxList* list, Lighting* l, double maxDist, Color* result, double depth){
	// intersect ray with polygon list
	total_rays++;
	if(depth > MAX_DEPTH){
		return 0;
	}
	double dist;
	Vector baryPt;
	Polygon* p = find_closest_p_bb(ray, list, maxDist, &dist, &baryPt);
	if(!p){
		return 0;
	}	
	float reflectCoeff = p->reflectance;

	Vector intersect;
	vector_copy(&intersect, &ray->ray);
	
	vector_scale(&intersect, dist);

	vector_add(&intersect, &ray->start, &intersect);
	
	// get base color C0 at intersection point Q

	
	// shoot ray from Q to each light
	Color cL = {{0.0, 0.0, 0.0}};
	Vector L;
	Vector NL;
	Ray RL;
	point_copy(&RL.start, &intersect);
	
	Vector norm;
	
	calculate_barypoint(p->normal, &baryPt, &norm);
	vector_normalize(&norm);

	Color c0;
	Vector bump = {{0.0, 0.0, 0.0, 0.0}};
	
	color_copy(&c0, &(p->color[0]));	
	if(p->tex_ptr == NULL){
		color_copy(&c0, &(p->color[0]));	
	}
	else{
		Vector orgNorm;
		Point orgPt;
		calculate_barypoint(p->orgNormal, &baryPt, &orgNorm);
		calculate_barypoint(p->orgVertex, &baryPt, &orgPt);
	
		//maybe use this to calculate height map and get the partial derivatives
		calculate_tex(p->tex_ptr, &orgPt, &orgNorm, p->project, &c0, &bump, &reflectCoeff);
		//printf("bump is \n");
		//vector_print(&bump, stdout);
		vector_scale(&bump, 0.5);
		for(int i = 0; i < 3; i++){
			
			norm.val[i] *= 1+bump.val[i];
		}
		//vector_normalize(&norm);
		//vector_print(&norm, stdout);
	}

	// Handle lighting for ray
	for(int i = 0; i < l -> nLights; i++){
		switch( l -> light[i].type){
			case LightAmbient:{
				Color tmp;
				color_multiply(&c0, &l->light[i].color, &tmp);
				color_add_bounds(&tmp, &cL, &cL);
				break;
			}
			case LightPoint:{
				vector_subtract(&intersect, &l->light[i].position, &L);
				vector_copy(&NL, &L);
				vector_normalize(&NL);
				
				vector_copy(&RL.ray, &NL);
				
				double lightDistance = vector_length(&L);
				
				double closest_polygon = find_closest_p_bb_dist_end_early(&RL, list, lightDistance);
				
				
				if(closest_polygon < lightDistance){
					break;
				}
				
				Color tmp;
				
				// only get L, not specular component
				double intensity = vector_dot(&norm, &NL);
				if(intensity <= 0){
					break;
				}
				color_multiply_float(&l->light[i].color, intensity, &tmp);
				color_multiply(&tmp, &c0, &tmp);
				color_add_bounds(&tmp, &cL, &cL);
				/*
				Vector Hv;
				Hv.val[X] = (NL.val[X] + (-ray -> ray.val[X]))/2;
				Hv.val[Y] = (NL.val[Y] + (-ray -> ray.val[Y]))/2;
				Hv.val[Z] = (NL.val[Z] + (-ray -> ray.val[Z]))/2;
				vector_normalize(&Hv);
				
				color_multiply(&c0, &l->light[i].color, &tmp);
				float angle = vector_dot(&Hv, &norm);
				angle = pow(angle, 10);
				color_multiply_float(&tmp,  angle/2, &tmp);
				color_add_bounds(&tmp, &cL, &cL);
				*/
				break;	
			}
			default:
				continue;
		}
	}
	
	// reflection C2 = C1 + color from reflected ray
	color_copy(result, &cL);
	Color reflectColor;
	if(reflectCoeff > EPSILON){
		calculate_reflection(list, l, maxDist, depth+1, &norm, &(ray->ray), &intersect, reflectCoeff, &reflectColor);
	}
	color_add_bounds(result, &reflectColor, result);

	//distributed ray tracing for ambiant light 
/*
	Ray secondary_ray;
	point_copy(&secondary_ray.start, &intersect);
	// create array for sampling
	float angles[SECONDARY_RAYS];
	for(int i = 0; i < SECONDARY_RAYS; i++){
		angles[i] = (i/SECONDARY_RAYS)-0.5;
	}
	Matrix m;
	Color cA;
	Vector offset = {{1.0, 1.0, 0.0, 0.0}};
	Vector tmpV;
	vector_normalize(&offset);
	
	for(int i = 0; i < SECONDARY_RAYS; i++){
		for(int j = 0; j < SECONDARY_RAYS; j++){
			
			offset.val[X] = angles[i];
			offset.val[Y] = angles[j];
			vector_normalize(&offset);
			double ni = vector_dot(&offset, &norm);
			vector_scale(&offset, ni);
			vector_subtract(&offset, &norm, &secondary_ray.ray);
			int intersected = shoot_ray(&secondary_ray, list, l, maxDist, &cA, depth+(MAX_DEPTH/2)-1);
			if(intersected != 0){
				color_multiply_float(&cA, 1/SECONDARY_RAYS*1/SECONDARY_RAYS, &cA);
				color_multiply_float(&cA, FALLOFF, &cA);
				color_add_bounds(&cA, result, result);
			}
		}
	}
	*/
	// ambient light/diffuse rays CFinal = C2 + ambient * C0

	return 1;
}

/* calculates the color component from reflection and returns it in result
 * returns all 0 (black) if no reflection color component
 * receives bounding box list, lighting, max dist and depth for reflection ray
 * receives surface normal (N) and ray to reflect (I)
 */
void calculate_reflection(BoundingBoxList* list, Lighting* l, double maxDist, double depth, Vector *N, Vector *I, Point *intersect, float reflectCoeff, Color *result)
{
	//calculate perfect reflection direction
	/* R = I−2(N⋅I)N
	 * reflected dir R, incoming light dir I, surface normal N
	 */
	Ray reflectDir;
	point_copy(&(reflectDir.start), intersect);
	Vector partial;
	vector_copy(&partial, N);
	double ni = vector_dot(&partial, I);
	vector_scale(&partial, 2*ni);
	vector_subtract(&partial, I, &(reflectDir.ray));

	//send out reflection ray
	Color reflectColor;
	int intersected = shoot_ray(&reflectDir, list, l, maxDist, &reflectColor, depth);
	//if no intersection, just set to black (will not add anything to surface color)
	if (!intersected)
	{
		color_set(&reflectColor, 0.0, 0.0, 0.0);
	}

	//return resulting color component in result pointer
	color_multiply_float(&reflectColor, reflectCoeff, result);
}

void clamp_color(double intensity, Color* cB, Color* cL, Color* mask, int mask_length, Color* result){
	intensity = intensity < 0 ? 0 : intensity;
	intensity = intensity > 1 ? 1 : intensity;
	int index = ROUND(intensity * (mask_length-1));
	color_multiply(cB, &mask[index], result);
	color_multiply(result, cL, result);
}

double shoot_ray_cel(Ray *ray, BoundingBoxList* list, Lighting* l, double maxDist, Color* result, Color* mask, int mask_length, double depth){
	// intersect ray with polygon list
	if(depth > MAX_DEPTH){
		return 0;
	}
	double dist;
	Vector baryPt;
	Polygon* p = find_closest_p_bb(ray, list, maxDist, &dist, &baryPt);
	if(!p){
		return 0;
	}
	
	Vector intersect;
	vector_copy(&intersect, &ray->ray);
	
	vector_scale(&intersect, dist);

	vector_add(&intersect, &ray->start, &intersect);
	
	// get base color C0 at intersection point Q
	Color c0;
	color_copy(&c0, &(p->color[0]));
	
	// shoot ray from Q to each light
	Color cL = {{0.0, 0.0, 0.0}};
	Vector L;
	Vector NL;
	Ray RL;
	point_copy(&RL.start, &intersect);
	
	Vector tmpV;
	Vector norm = {{0.0, 0.0, 0.0, 0.0}};
	
	vector_copy(&tmpV, &p->normal[1]);
	vector_scale(&tmpV, baryPt.val[0]);
	vector_add(&norm, &tmpV, &norm);
	
	vector_copy(&tmpV, &p->normal[2]);
	vector_scale(&tmpV, baryPt.val[1]);
	vector_add(&norm, &tmpV, &norm);	
	
	vector_copy(&tmpV, &p->normal[0]);
	vector_scale(&tmpV, (baryPt.val[2]));
	vector_add(&norm, &tmpV, &norm);
		
	vector_normalize(&norm);
	double total_intensity = 0;
	// Handle lighting for ray
	Color white = {{1.0, 1.0, 1.0}};

	for(int i = 0; i < l -> nLights; i++){
		switch( l -> light[i].type){
			case LightPoint:{
				vector_subtract(&intersect, &l->light[i].position, &L);
				vector_copy(&NL, &L);
				vector_normalize(&NL);
				
				vector_copy(&RL.ray, &NL);
				
				double lightDistance = vector_length(&L);
				
				double closest_polygon = find_closest_p_bb_dist_end_early(&RL, list, lightDistance);
				Color tmp;				
				
				if(closest_polygon < lightDistance){
					clamp_color(0, &c0, &white, mask, mask_length, &tmp);
					color_add_bounds(&cL, &tmp, &cL);
					break;
				}
				

				
				// only get L, not specular component
				double intensity = vector_dot(&norm, &NL);
				if(intensity <= 0){
					clamp_color(0, &c0, &l->light[i].color, mask, mask_length, &tmp);
					color_add_bounds(&cL, &tmp, &cL);
					break;
				}
				clamp_color(intensity, &c0, &l->light[i].color, mask, mask_length, &tmp);
				color_add_bounds(&cL, &tmp, &cL);
				//total_intensity += intensity;
				break;	
			}
			default:
				continue;
		}
	}
	
	
	// reflection C2 = C1 + color from reflected ray
	
	// ambiant light/diffuse rays CFinal = C2 + ambiant * C0
	color_copy(result, &cL);
	return dist;
}

/*
 * draws module contents with ray-tracing
 */
void module_draw_ray(Module *md, Matrix *GTM, View3D *view, DrawState *ds, Lighting *lighting, Image *src)
{
	int rows = src->rows;
	int cols = src->cols;
	double du = view->du;
	double dv = view->dv;
	
	//------------------------------------get COP as a Point
	Point cop;
	//normalize VPN to unit vector
	Vector negVPN;
	vector_copy( &negVPN, &(view->vpn) );
	vector_normalize( &negVPN );
	//get COP coords
	double copX = view->vrp.val[X] -negVPN.val[X] * view->d;
	double copY = view->vrp.val[Y] -negVPN.val[Y] * view->d;
	double copZ = view->vrp.val[Z] -negVPN.val[Z] * view->d;
	point_set3D( &cop, copX, copY, copZ );
	
	//make polygon list
	
	BoundingBoxList* bblist = create_polygon_list( md, GTM, ds );
	printf("made polygon list\n");
	
	
	printf("there are %d polygons in the scene\n", bblist->totalP);
	
	//------------------------------------get bounds of view plane rectangle
	Vector uAdjust, vupAdjust; //for getting corner of rectangle we start from
	Vector uStep, vStep; //for stepping by pixel in loop through pixel grid
	//get correct axes
	vector_cross( &(view->vup), &(view->vpn), &uAdjust ); //get U
	vector_scale( &uAdjust, -1 ); //flip because view plane vs image index axes are flipped
	vector_copy( &uStep, &uAdjust );
	vector_cross( &(view->vpn), &uAdjust, &vupAdjust ); //get corrected VUP
	vector_copy( &vStep, &vupAdjust );
	//adjust uAdjust and vupAdjust for view plane dimensions
	vector_normalize( &uAdjust ); //normalize U to unit vector
	vector_scale( &uAdjust, 0.5*du ); //scale to 1/2 view plane width
	vector_normalize( &vupAdjust ); //normalize VUP to unit vector
	vector_scale( &vupAdjust, 0.5*dv ); //scale to 1/2 view plane height
	//get coords for one corner of the view rectangle (0.5 du and 0.5 dv away from VRP)
	double cornerX = view->vrp.val[X] - uAdjust.val[X] - vupAdjust.val[X];
	double cornerY = view->vrp.val[Y] - uAdjust.val[Y] - vupAdjust.val[Y];
	double cornerZ = view->vrp.val[Z] - uAdjust.val[Z] - vupAdjust.val[Z];
	//calculate uStep and vStep (normalize, then scale down)
	vector_normalize( &uStep );
	vector_scale( &uStep, du/cols );
	vector_normalize( &vStep );
	vector_scale( &vStep, dv/rows );

	//------------------------------------loop through pixel grid
	//start in middle of corner pixel
	double startX = cornerX + 0.5*uStep.val[X] + 0.5*vStep.val[X];
	double startY = cornerY + 0.5*uStep.val[Y] + 0.5*vStep.val[Y];
	double startZ = cornerZ + 0.5*uStep.val[Z] + 0.5*vStep.val[Z];
	Point curPt;
	point_set3D( &curPt, startX, startY, startZ );
	//for every pixel in view plane grid, shoot ray
	Color c;
	
	for ( int i = 0; i < rows; i++ )
	{
		for ( int j = 0; j < cols; j++ )
		{
			//get ray
			Ray ray;
			vector_subtract( &curPt, &cop, &(ray.ray) ); //from COP through current pixel
			vector_normalize(&ray.ray);
			vector_scale(&ray.ray, -1);
			
			point_copy( &(ray.start), &cop );
			
			
			//find closest polygon
			if (shoot_ray( &ray, bblist, lighting, view->b, &c, 0) != 0 ){ //if there is a polygon
				image_setColor( src, j, i, c );
			}				

			//step along U
			vector_add( &curPt, &uStep, &curPt );

		}
		for(int j = 0; j < cols; j++){
			vector_subtract(&uStep, &curPt, &curPt);
		}
		//step along VUP (v)
		vector_add( &curPt, &vStep, &curPt );
	}

	/*
	COP = distance d along negative VPN axis
	probably turn above distance into a point, store as Point start for every ray
	make list w/ create_polygon_list
	divide view plane into grid of pixels (based on image size)
	for each pixel, extend ray from COP through pixel to wherever it intersects (use find_closest_p)
		get vector direction by subtracting COP start point from pixel point
		if no polygon (returns null) do nothing
		color that pixel using that polygon's info... (or, initially, just white)
	*/
	bb_list_free(bblist);
}

/*
 * draws module contents with ray-tracing using cel shading
 */
void module_draw_ray_cel(Module *md, Matrix *GTM, View3D *view, DrawState *ds, Lighting *lighting, Image *src, Color* cel_colors, int mask_length)
{
	int rows = src->rows;
	int cols = src->cols;
	double du = view->du;
	double dv = view->dv;
	
	//------------------------------------get COP as a Point
	Point cop;
	//normalize VPN to unit vector
	Vector negVPN;
	vector_copy( &negVPN, &(view->vpn) );
	vector_normalize( &negVPN );
	//get COP coords
	double copX = view->vrp.val[X] -negVPN.val[X] * view->d;
	double copY = view->vrp.val[Y] -negVPN.val[Y] * view->d;
	double copZ = view->vrp.val[Z] -negVPN.val[Z] * view->d;
	point_set3D( &cop, copX, copY, copZ );
	
	//make polygon list
	
	BoundingBoxList* bblist = create_polygon_list( md, GTM, ds );
	printf("made polygon list\n");
	
	
	printf("there are %d polygons in the scene\n", bblist->totalP);
	
	//------------------------------------get bounds of view plane rectangle
	Vector uAdjust, vupAdjust; //for getting corner of rectangle we start from
	Vector uStep, vStep; //for stepping by pixel in loop through pixel grid
	//get correct axes
	vector_cross( &(view->vup), &(view->vpn), &uAdjust ); //get U
	vector_scale( &uAdjust, -1 ); //flip because view plane vs image index axes are flipped
	vector_copy( &uStep, &uAdjust );
	vector_cross( &(view->vpn), &uAdjust, &vupAdjust ); //get corrected VUP
	vector_copy( &vStep, &vupAdjust );
	//adjust uAdjust and vupAdjust for view plane dimensions
	vector_normalize( &uAdjust ); //normalize U to unit vector
	vector_scale( &uAdjust, 0.5*du ); //scale to 1/2 view plane width
	vector_normalize( &vupAdjust ); //normalize VUP to unit vector
	vector_scale( &vupAdjust, 0.5*dv ); //scale to 1/2 view plane height
	//get coords for one corner of the view rectangle (0.5 du and 0.5 dv away from VRP)
	double cornerX = view->vrp.val[X] - uAdjust.val[X] - vupAdjust.val[X];
	double cornerY = view->vrp.val[Y] - uAdjust.val[Y] - vupAdjust.val[Y];
	double cornerZ = view->vrp.val[Z] - uAdjust.val[Z] - vupAdjust.val[Z];
	//calculate uStep and vStep (normalize, then scale down)
	vector_normalize( &uStep );
	vector_scale( &uStep, du/cols );
	vector_normalize( &vStep );
	vector_scale( &vStep, dv/rows );

	//------------------------------------loop through pixel grid
	//start in middle of corner pixel
	double startX = cornerX + 0.5*uStep.val[X] + 0.5*vStep.val[X];
	double startY = cornerY + 0.5*uStep.val[Y] + 0.5*vStep.val[Y];
	double startZ = cornerZ + 0.5*uStep.val[Z] + 0.5*vStep.val[Z];
	Point curPt;
	point_set3D( &curPt, startX, startY, startZ );
	
	//for every pixel in view plane grid, shoot ray
	Color c;
	double depth_image[rows][cols];
	double maxDepth = -1;
	double dist;
	for ( int i = 0; i < rows; i++ )
	{
		for ( int j = 0; j < cols; j++ )
		{
			//get ray
			Ray ray;
			vector_subtract( &curPt, &cop, &(ray.ray) ); //from COP through current pixel
			vector_normalize(&ray.ray);
			vector_scale(&ray.ray, -1);
			
			point_copy( &(ray.start), &cop );
			
			dist = shoot_ray_cel( &ray, bblist, lighting, view->b, &c, cel_colors, mask_length, 0);
			depth_image[i][j] = dist;
			if (dist > maxDepth)
			{
				maxDepth = dist;
			}
			
			//find closest polygon
			if (dist != 0 ){ //if there is a polygon
				image_setColor( src, j, i, c );
			}				

			//step along U
			vector_add( &curPt, &uStep, &curPt );

		}
		for(int j = 0; j < cols; j++){
			vector_subtract(&uStep, &curPt, &curPt);
		}
		//step along VUP (v)
		vector_add( &curPt, &vStep, &curPt );
	}

	bb_list_free(bblist);

	//apply Sobel filter to depth image to get edges

	//-------------uncomment for depth edge image
	// Image *dimg;
	// dimg = image_create(rows, cols);
	//-------------------------------------------

	double subsection[3][3]; //holds the 3x3 grid centered at the current pixel
	int ssRow, ssCol; //index of subsection within full image
	double gx, gy, g; //derivative approximations
	double *gradient_img = malloc(sizeof(double) * rows * cols);
	double maxGrad = 0;

	for ( int i = 0; i < rows; i++ )
	{
		for ( int j = 0; j < cols; j++ )
		{
			//copy subsection, filling invalid indices with zeros
			for ( int m = 0; m < 3; m++ )
			{
				for ( int n = 0; n < 3; n++ )
				{
					ssRow = i - 1 + m;
					ssCol = j - 1 + n;
					
					if ( ssRow < 0 || ssRow > rows - 1 || ssCol < 0 || ssCol > cols-1 )
					{
						//outside image, set to max depth
						subsection[m][n] = maxDepth;
					}
					else
					{
						if (depth_image[ssRow][ssCol] > 0)
						{
							subsection[m][n] = depth_image[ssRow][ssCol];
						}
						else //max depth for not intersecting anything
						{
							subsection[m][n] = maxDepth;
						}
					}
				}
			}
			
			gx = subsection[0][2] - subsection[0][0] + 2*subsection[1][2] - 2*subsection[1][0] + subsection[2][2] - subsection[2][0];
			gy = subsection[2][0] - subsection[0][0] + 2*subsection[2][1] - 2*subsection[0][1] + subsection[2][2] - subsection[0][2];

			g = sqrt(gx*gx + gy*gy);
			gradient_img[i*cols + j] = g;
			if (g > maxGrad)
				maxGrad = g;
		}
	}

	//adjust gradient to range 0-1
	Color black = {{0.0, 0.0, 0.0}};
	//Color pink = {{1.0, 0.6, 0.9}};
	for ( int i = 0; i < rows; i++ )
	{
		for ( int j = 0; j < cols; j++ )
		{
			gradient_img[i*cols + j] /= maxGrad;

			//if over threshold, draw edge
			if ( gradient_img[i*cols + j] > 0.01 )
			{
				image_setColor( src, j, i, black );
				//image_setColor( src, j, i, pink );
			}

			//-------------uncomment for depth edge image
			// double v = gradient_img[i*cols + j] * 255;
			// Color c = {{v, v, v}};
			// image_setColor( dimg, j, i, c );
			//-------------------------------------------

			//printf("%f", gradient_img[i*cols + j]);
		}
		//printf("\n");
	}

	printf("max depth %f, max gradient %f\n", maxDepth, maxGrad);

	//----------------------------uncomment for depth edge image
	// image_write(dimg, "../images/depth-edges.ppm");
    // image_free( dimg );
	//----------------------------------------------------------

	free(gradient_img);
}