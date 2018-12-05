#include "module.h"
#include "viewing.h"
#include "ray.h"
#include "color.h"
#include <limits.h>

#define MAX_DEPTH 8
#define EPSILON 0.0001


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
	PolygonNode *last;
	while(cur != NULL){
		last = cur;
		cur = (PolygonNode*) last->next;
		free(last);
	}
}

BoundingBoxNode* bb_node_create(){
	BoundingBoxNode* bbnode = malloc(sizeof(BoundingBoxNode));
	bbnode->minX = 0;
	bbnode->maxX = 0;
	bbnode->maxY = 0;
	bbnode->minY = 0;
	bbnode->minZ = 0;
	bbnode->minZ = 0;
	bbnode->polygons = polygon_list_create();
	bbnode->bblist = NULL;
	bbnode->next = NULL;
	return bbnode;
}

BoundingBoxNode* bb_node_add_polygon(BoundingBoxNode* bbnode, Polygon* p){
	polygon_node_add(bbnode->polygons, p);
	for(int i = 0; i < 3; i++){
		bbnode->minX = MIN(bbnode->minX, p->vertex[i].val[X]);
		bbnode->maxX = MAX(bbnode->maxX, p->vertex[i].val[X]);
		
		bbnode->minY = MIN(bbnode->minY, p->vertex[i].val[Y]);
		bbnode->maxY = MAX(bbnode->maxY, p->vertex[i].val[Y]);
		
		bbnode->minX = MIN(bbnode->minZ, p->vertex[i].val[Z]);
		bbnode->maxZ = MAX(bbnode->maxZ, p->vertex[i].val[Z]);
	}
	return bbnode;
}

BoundingBoxNode* bb_node_add_bbnode(BoundingBoxNode* bbnode, BoundingBoxNode* new){
	if(bbnode -> bblist == NULL){
		bbnode -> bblist = bb_list_create();
	}
	bbnode->bblist = bb_list_addBoundingBox(bbnode->bblist, new);
	return bbnode;
}

BoundingBoxList* bb_list_create(){
	BoundingBoxList* list = malloc(sizeof(BoundingBoxList));
	list-> head = bb_node_create();
	list->length = 0;
	list -> totalp = 0;
	return list;
}

BoundingBoxList* bb_list_addPolygon(BoundingBoxList* bblist, Polygon *p){
	BoundingBoxNode* bbnode = bblist->head;
	bblist->head = bb_node_add_polygon(bbnode, p);
	return bblist;
}


BoundingBoxList* bb_list_addBoundingBox(BoundingBoxList* bblist, BoundingBoxNode* new){
	// if we are setting the top of the list
	new -> next = bblist -> head;
	bblist -> head = new;
	bblist -> length++;
	bblist -> totalP += new->polygons->length + new->bblist->totalP;
	return bblist;
}

void bb_list_free(BoundingBoxList* list){
	BoundingBoxNode *cur = list->head;
	BoundingBoxNode *last;

	while(cur != NULL){
		last = cur;
		cur = (PolygonNode*) last->next;
		if(last->bblist){
			bb_list_free(last->bblist);
		}
		if(last->polygons){
			polygon_list_free(last->polygons);
		}
		free(last);
	}
}

// Moller-Trumbore intersection algorithm implimented from wikipedia
double intersect_triangle_MT(Ray *ray, Polygon *p, Vector* baryPt){
	Vector edge1, edge2, h, s, q;
	float a, f, u, v;	
	
	vector_subtract(&p->vertex[0], &p->vertex[1], &edge1);
	vector_subtract(&p->vertex[0], &p->vertex[2], &edge2);
	
	vector_cross(&ray -> ray, &edge2, &h);
	a = vector_dot(&edge1, &h);
	
	
	if(a > -EPSILON && a < EPSILON){//something is fundamentally wrong here
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
		}
		return t;
	}
	else 0;
}

//based on code from here https://tavianator.com/fast-branchless-raybounding-box-intersections-part-2-nans/
int intersect_bounding_box(Ray* ray, BoundingBoxNode* node){
	float tmin = FLT_MIN;
	float tmax = FLT_MAX
	float t1, t2;
	
	if(ray->ray[X] != 0){
		t1 = node.minX - ray->start[X]/ray->ray[X];
		t2 = node.maxX - ray->start[X]/ray->ray[X];
		
		tmin = MAX(tmin, MIN(t1, t2));
		tmax = MIN(tmax, MAX(t1, t2));
	} else if (ray->start[X] <= node.minX || ray->start[X] >= node.maxX){
		return 0;
	}
	
	if(ray->ray[Y] != 0){
		t1 = node.minY - ray->start[Y]/ray->ray[Y];
		t2 = node.maxY - ray->start[Y]/ray->ray[Y];
		
		tmin = MAX(tmin, MIN(t1, t2));
		tmax = MIN(tmax, MAX(t1, t2));
	} else if (ray->start[Y] <= node.minY || ray->start[Y] >= node.maxY){
		return 0;
	}
	
		if(ray->ray[Z] != 0){
		t1 = node.minZ - ray->start[Z]/ray->ray[Z];
		t2 = node.maxZ - ray->start[Z]/ray->ray[Z];
		
		tmin = MAX(tmin, MIN(t1, t2));
		tmax = MIN(tmax, MAX(t1, t2));
	} else if (ray->start[Z] <= node.minZ || ray->start[Z] >= node.maxZ){
		return 0;
	}
	
	if(tmax > tmin && tmax > 0.0){
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
	Point pt;
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
	Polygon *closest = NULL;
	double tmpDist;
	Point pt;
	PolygonNode* cur = list->head;
	while(cur != NULL){
		tmpDist = intersect_triangle_MT(ray, &cur->p, NULL);
		if(tmpDist > EPSILON){
			if(tmpDist < distance){
				closest = &cur->p;
				distance = tmpDist;
			}
		}
		cur = (PolygonNode*) cur->next;
	}
	return distance;
}

Polygon* find_closest_p_bb(Ray *ray, BoundingBoxList* list, double maxDist, double* dist, Vector* baryPt){
	BoundingBoxNode* bbnode = list->head;
	Polygon *closest = NULL;
	Polygon *tmpclose = NULL;
	double tmpdist;
	double distance = maxDist;
	Vector tmpBpt;
	while(bbnode != NULL){
		if(intersect_bounding_box(ray, bbnode)){
			tmpclose = find_closest_p(ray, head->polygons, maxDist, &tmpdist, &tmpBt);
			if(tmpdist < distance){
				distance = tmpdist;
				closest = tmpclose;
				vector_copy(baryPt, tmpBpt);
			}
			if(bbnode->bblist){
				tmpclose = find_closest_p_bb(ray, bbnode->bblist, distance, &tmpdist, &tmpBpt)
				if(tmpdist < distance){
					distance = tmpdist;
					closest = tmpclose;
					vector_copy(baryPt, tmpBpt);
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
	Polygon *closest = NULL;
	Polygon *tmpclose = NULL;
	double tmpdist;
	double distance = maxDist;
	Vector tmpBpt;
	while(bbnode != NULL){
		if(intersect_bounding_box(ray, bbnode)){
			tmpdist = find_closest_p_dist(ray, head->polygons, maxDist);
			if(tmpdist < distance){
				distance = tmpdist;
			}
			if(bbnode->bblist){
				tmpdist = find_closest_p_bb_dist(ray, bbnode->bblist, distance)
				if(tmpdist < distance){
				distance = tmpdist;
			}
			}
		}
		bbnode = bbnode->next;
	}
	return distance;
}


static void set_up_polygon(Polygon* p, DrawState *ds){
	if(p -> normal == NULL){
		//set the normals
		//printf("making normals\n");
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
}

/*
* write pre pass to create polygon list based on module draw
*/
BoundingBoxList* module_create_polygon_list(Module *md, Matrix *GTM, DrawState *ds){
	Element *cur = md -> head;
	Matrix LTM;
	BoundingBoxList* bblist = bb_list_create();
	BoundingBoxNode* bbnode = bb_node_create();
	bb_list_addBoundingBox(bblist, bbnode);
	
	matrix_identity(&LTM);
	printf("- before while\n");

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
				//printf("copied polygon\n");
				matrix_xformPolygon(&LTM, &p);
				matrix_xformPolygon(GTM, &p);

				set_up_polygon(&p, ds);
				
				bb_list_addPolygon(bblist, &p);
			} else if(cur->obj.polygon.numVertex == 4){
				Polygon p;
				polygon_init(&p);
				polygon_copy(&p, &cur -> obj.polygon);
				matrix_xformPolygon(&LTM, &p);
				matrix_xformPolygon(GTM, &p);

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
				
				set_up_polygon(&p0, ds);
				
				bb_list_addPolygon(bblist, &p);
				
				
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
				
				set_up_polygon(&p1, ds);
				
				bb_list_addPolygon(bblist, &p);
				
				
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
            case ObjModule:{
				printf("- case module\n");
				Matrix tempM;
				DrawState tempDS;
				
				matrix_identity(&tempM);
				drawstate_copy(&tempDS, ds);
				point_copy(&tempDS.viewer, &ds->viewer);
				
				matrix_multiply(GTM, &LTM, &tempM);
				
				Module* mod = (Module*) (cur -> obj.module);
				BoundingBoxNode* new = module_create_polygon_list(mod, &tempM, &tempDS);
				bb_list_addBoundingBox(bblist->head->bblist, new);
			break;
            }
            case ObjBezier:{
                break;
            }
			//printf("- end module_create_polygon_list\n");
		}
	//printf("about to iterate\n");
	cur = (Element*)cur -> next;
	//printf("iterated\n");
	}
	printf("done\n");
	return bblist;
}

/*
* Shoots ray into the scene.
* returns 0 if it does not intersect.
* puts the color into the result pointer.
*/
int shoot_ray(Ray *ray, PolygonList* list, Lighting* l, double maxDist, Color* result, double depth){
	// intersect ray with polygon list
	//printf("shooting ray\n");
	if(depth > MAX_DEPTH){
		return 0;
	}
	double dist;
	Vector baryPt;
	Polygon* p = find_closest_p(ray, list, maxDist, &dist, &baryPt);
	
	//polygon_print(p, stdout);
	if(!p){
		return 0;
	}
	
	// calculate the intersection point
	Vector intersect;
	vector_copy(&intersect, &ray->ray);
	
	//printf("ray is \n");
	//vector_print(&ray->ray, stdout);
	//point_print(&ray->start, stdout);
	vector_scale(&intersect, dist);

	vector_add(&intersect, &ray->start, &intersect);
	
	//printf("intersection point is \n");
	//point_print(&intersect, stdout);
	
	// get base color C0 at intersection point Q
	Color c0;
	//color_print(&c0, stdout);
	color_copy(&c0, &(p->color[0]));
	// shoot ray from Q to each light
	Color cL = {{0.0, 0.0, 0.0}};
	Vector L;
	Vector NL;
	Ray RL;
	point_copy(&RL.start, &intersect);
	
	Vector tmpV;
	Vector norm = {{0.0, 0.0, 0.0, 0.0}};
	
	//printf("norm is \n");
	vector_copy(&tmpV, &p->normal[0]);
	vector_scale(&tmpV, baryPt.val[0]);
	vector_add(&norm, &tmpV, &norm);
	//vector_print(&norm, stdout);
	
	vector_copy(&tmpV, &p->normal[1]);
	vector_scale(&tmpV, baryPt.val[1]);
	vector_add(&norm, &tmpV, &norm);	
	//vector_print(&norm, stdout);
	
	vector_copy(&tmpV, &p->normal[2]);
	vector_scale(&tmpV, (1.0 - baryPt.val[0]- baryPt.val[1]));
	vector_add(&norm, &tmpV, &norm);
	//vector_print(&norm, stdout);
		
	vector_normalize(&norm);
	
	//printf("~~~~~ new color ~~~~~\n");
	for(int i = 0; i < l -> nLights; i++){
		switch( l -> light[i].type){
			case LightAmbient:{
				Color tmp;
				//printf(" ambient color is \n");
				color_multiply(&c0, &l->light[i].color, &tmp);
				//color_print(&tmp, stdout);
				color_add(&tmp, &cL, &cL);
				break;
			}
			case LightPoint:{
				vector_subtract(&intersect, &l->light[i].position, &L);
				vector_copy(&NL, &L);
				vector_normalize(&NL);
				
				vector_copy(&RL.ray, &NL);
				
				//vector_print(&RL.ray, stdout);
				//point_print(&RL.start, stdout);
				double lightDistance = vector_length(&L);
				
				double closest_polygon; 
				
				Polygon* p1 = find_closest_p(&RL, list, lightDistance, &closest_polygon, NULL);
				
				//polygon_print(p1, stdout);
				
				if(closest_polygon < lightDistance){
					//printf("closest p is %f, light is %.2f\n", closest_polygon, lightDistance);
					break;
				}
				
				Color tmp;
				
				// only get L, not specular component
				double intensity = vector_dot(&norm, &NL);
				//printf("intensity is %f\n", intensity);
				if(intensity <= 0){
					break;
				}
				color_multiply_float(&l->light[i].color, intensity, &tmp);
				color_multiply(&tmp, &c0, &tmp);
				//color_print(&tmp, stdout);
				color_add(&tmp, &cL, &cL);
				//printf("done shading \n");
				break;	
			}
			default:
				continue;
		}
		//color_print(&cL, stdout);
	}
	// C1 = sum(C0 * CL) 
	
	// reflection C2 = C1 + color from reflected ray
	
	// ambiant light/diffuse rays CFinal = C2 + ambiant * C0
	color_copy(result, &cL);
	//printf("done shooting\n");
	return 1;
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
			//printf("cop is \n");
			//point_print(&cop, stdout);
			point_copy( &(ray.start), &cop );
			
			
			//find closest polygon
			
			//max dist = back clip plane
			if (shoot_ray( &ray, bblist, lighting, view->b, &c, 0) != 0 ){ //if there is a polygon
				image_setColor( src, j, i, c );
			}
				

			//step along U
			vector_add( &curPt, &uStep, &curPt );
			//printf("cur pixel is\n");
			//vector_print(&curPt, stdout);
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
}	