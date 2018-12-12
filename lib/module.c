/* module.c
 * Defines functions for using Elements and Modules in hierarchical modeling
 * 
 * Melody Mao and Jason Gurevitch
 * CS351 Spring 2018
 * Project 6
 */

#include <stdlib.h>
#include "graphics.h"
#include "module.h"

/*** Element functions ***/

/* Allocates and returns an initialized but empty Element
 */


//for debugging
char enumStrings[14][20] = {"ObjNone", "ObjLine", "ObjPoint", "ObjPolyline", "ObjPolygon",
							"ObjIdentity", "ObjMatrix", "ObjColor", "ObjBodyColor", "ObjSurfaceColor",
							"ObjSurfaceCoeff", "ObjLight", "ObjModule", "ObjBezier"};


Element *element_create()
{
    Element *e = (Element *)malloc( sizeof( Element ) );
    e->type = ObjNone;
    e->next = NULL;
    
    return e;
}

/* Allocates an Element and stores a duplicate of the data pointed to by obj in the Element
 */
Element *element_init( ObjectType type, void *obj )
{
	//printf("creating element of type %d\n", type);
	//printf("which is a %s\n", enumStrings[type]);
    Element *e = (Element *)malloc( sizeof( Element ) );
    e->type = type;
    e->next = NULL;
    
    switch( type )
    {
        case ObjNone :
            break;
        case ObjLine :
            line_copy( &(e->obj.line), (Line *)obj );
            break;
        case ObjPoint :
            point_copy( &(e->obj.point), (Point *)obj );
            break;
        case ObjPolyline :
			polyline_init(&(e->obj.polyline));
            polyline_copy( &(e->obj.polyline), (Polyline *)obj );
            break;
        case ObjPolygon :
			polygon_init(&(e->obj.polygon));
            polygon_copy( &(e->obj.polygon), (Polygon *)obj );
            break;
        case ObjIdentity :
            break;
        case ObjMatrix :
            matrix_copy( &(e->obj.matrix), (Matrix *)obj );
            break;
        case ObjColor :
            color_copy( &(e->obj.color), (Color *)obj );
            break;
        case ObjBodyColor :
            color_copy( &(e->obj.color), (Color *)obj );
            break;
        case ObjSurfaceColor :
            color_copy( &(e->obj.color), (Color *)obj );
            break;
        case ObjSurfaceCoeff :
            e->obj.coeff = *((float *)obj) ;
            break;
        case ObjLight :
            light_copy( (&e->obj.light), (Light*)obj);
            break;
        case ObjModule :
            e->obj.module = obj;
            break;
        case ObjBezier :
            e->obj.curve = *((BezierCurve *)obj);
            break;
		case ObjReflectance:
			e->obj.reflectance = *((float*)obj);
			break;
		case ObjTransmittance:
			e->obj.transmittance = *((float*)obj);
			break;
		case ObjTexture:
			e->obj.texture = (Tex_cube_map*)obj;
			break;
		case ObjProjectionType:
			e->obj.proj = *(Projection_type*)obj;
			break;
    }
    
    return e;
}

/* frees the element and the object it contains, as appropriate
 */
void element_delete( Element *e )
{
    switch( e->type )
    {
        case ObjNone :
            break;
        case ObjLine :
            break;
        case ObjPoint :
            break;
        case ObjPolyline :
            polyline_clear( &(e->obj.polyline) );
            break;
        case ObjPolygon :
            polygon_clear( &(e->obj.polygon) );
            break;
        case ObjIdentity :
            break;
        case ObjMatrix :
            break;
        case ObjColor :
            break;
        case ObjBodyColor :
            break;
        case ObjSurfaceColor :
            break;
        case ObjSurfaceCoeff :
            break;
        case ObjLight :
            
            break;
        case ObjModule :
			module_clear(e->obj.module);
            break;
        case ObjBezier :
            break;
		default:
			break;
    }
    
    free( e );
}

/*** Module functions ***/

/* Allocates an empty module
 */
Module *module_create()
{
    Module *m = (Module *)malloc( sizeof(Module) );
    m->head = NULL;
    m->tail = NULL;
    return m;
}

/* clears the module’s list of Elements, freeing memory as appropriate
 */
void module_clear( Module *md )
{
    //invalid input
    if ( md == NULL )
    {
        return;
    }
    
    Element *here = md->head;
    Element *next;
    
    //step through list in order until last element
    while ( here != NULL )
    {
        next = (Element *)here->next;
        element_delete( here );
        here = next;
    }
    
    //reset Module fields
    md->head = NULL;
    md->tail = NULL;
}

/* Frees all of the memory associated with a module, including the memory pointed to by md
 */
void module_delete( Module *md )
{
    //invalid input
    if ( md == NULL )
    {
        return;
    }
    
    module_clear( md ); //delete all elements
    free( md );
}

/* Generic insert of an element into the module at the tail of the list
 */
void module_insert( Module *md, Element *e )
{
    //invalid input
    if ( md == NULL || e == NULL )
    {
        return;
    }
    
    //if md was empty, start new list
    if ( md->head == NULL )
    {
        md->head = e;
        md->tail = e;
        return;
    }
    
    //otherwise, attach to tail
    md->tail->next = e;
    md->tail = e;
}

/* Adds a pointer to the Module sub to the tail of the module’s list
 */
void module_module( Module *md, Module *sub )
{
    //invalid input
    if ( md == NULL || sub == NULL )
    {
        return;
    }
	void* subp = (void*) sub;
    Element *e = element_init( ObjModule, subp );
    module_insert( md, e );
}

/* Adds p to the tail of the module’s list
 */
void module_point( Module *md, Point *p )
{
    //invalid input
    if ( md == NULL || p == NULL )
    {
        return;
    }
    
    Element *e = element_init( ObjPoint, p );
    module_insert( md, e );
}

/* Adds p to the tail of the module’s list
 */
void module_line( Module *md, Line *p )
{
    //invalid input
    if ( md == NULL || p == NULL )
    {
        return;
    }
    
    Element *e = element_init( ObjLine, p );
    module_insert( md, e );
}

/* Adds p to the tail of the module’s list
 */
void module_polyline( Module *md, Polyline *p )
{
    //invalid input
    if ( md == NULL || p == NULL )
    {
        return;
    }
    
    Element *e = element_init( ObjPolyline, p );
    module_insert( md, e );
}

/* Adds p to the tail of the module’s list
 */
void module_polygon( Module *md, Polygon *p )
{
    //invalid input
    if ( md == NULL || p == NULL )
    {
        return;
    }
    Element *e = element_init( ObjPolygon, p );
	module_insert( md, e );
}

void module_light(Module *md, Light *l){
	{
    //invalid input
    if ( md == NULL || l == NULL )
    {
        return;
    }
    Element *e = element_init( ObjLight, l );
	module_insert( md, e );
}
}

/* Object that sets the current transform to the identity, placed at the tail of the module’s list
 */
void module_identity( Module *md )
{
    //invalid input
    if ( md == NULL )
    {
        return;
    }
    
    Element *e = element_init( ObjIdentity, NULL );
    module_insert( md, e );
}


/* Matrix operand to add a translation matrix to the tail of the module’s list
 */
void module_translate2D( Module *md, double tx, double ty )
{
    //invalid input
    if ( md == NULL )
    {
        return;
    }
    
    Matrix *t = (Matrix *)malloc( sizeof(Matrix) );
    matrix_identity( t );
    t->m[0][3] = tx;
    t->m[1][3] = ty;
    
    Element *e = element_init( ObjMatrix, t );
    module_insert( md, e );
	free(t);
}

/* Matrix operand to add a scale matrix to the tail of the module’s list
 */
void module_scale2D( Module *md, double sx, double sy )
{
    //invalid input
    if ( md == NULL )
    {
        return;
    }
    
    Matrix *s = (Matrix *)malloc( sizeof(Matrix) );
    matrix_identity( s );
    s->m[0][0] = sx;
    s->m[1][1] = sy;
    
    Element *e = element_init( ObjMatrix, s );
    module_insert( md, e );
	free(s);
}

/* Matrix operand to add a rotation about the Z axis to the tail of the module’s list
 */
void module_rotateZ( Module *md, double cth, double sth )
{
    //invalid input
    if ( md == NULL )
    {
        return;
    }
    
    Matrix *rotate = (Matrix *)malloc( sizeof(Matrix) );
    matrix_identity( rotate );
    rotate->m[0][0] = cth;
    rotate->m[0][1] = -sth;
    rotate->m[1][0] = sth;
    rotate->m[1][1] = cth;
    
    Element *e = element_init( ObjMatrix, rotate );
    module_insert( md, e );
	free(rotate);
}

/* Matrix operand to add a 2D shear matrix to the tail of the module’s list
 */
void module_shear2D( Module *md, double shx, double shy )
{
    //invalid input
    if ( md == NULL )
    {
        return;
    }
    
    Matrix *sh = (Matrix *)malloc( sizeof(Matrix) );
    matrix_identity( sh );
    sh->m[0][1] = shx;
    sh->m[1][0] = shy;
    
    Element *e = element_init( ObjMatrix, sh );
    module_insert( md, e );
	free(sh);
}

void module_flipZ( Module *md )
{
    //invalid input
    if ( md == NULL )
    {
        return;
    }
    
    Matrix *f = (Matrix *)malloc( sizeof(Matrix) );
    matrix_identity( f );
	
	f->m[2][2] = -1;
    
    Element *e = element_init( ObjMatrix, f );
    module_insert( md, e );
	free(f);
}

void module_lighting(Module *md, Matrix *VTM, Matrix *GTM, Lighting *lighting){
	Element *cur = md -> head;
	Matrix LTM;
	//initialize lighting
	//lighting_init(lighting);
	
	matrix_identity(&LTM);
	//printf("ltm is: \n");
	//	matrix_print(&LTM, stdout);
	
	while(cur != NULL){// || lighting -> nLights < MAX_LIGHTS){
		//printf("drawing element %d \n", cur -> type);
		//printf("which has type %s \n", enumStrings[cur -> type]);
		switch(cur -> type){
		    case ObjNone: break;
            case ObjLine:	break;
			case ObjLight: {
				switch (cur-> obj.light.type){		
					case LightNone:
						break;
					case LightAmbient:
						lighting_add(lighting, LightAmbient, &cur -> obj.light.color, NULL, NULL, 0.0, 0.0);
						break;
					case LightDirect:{
						Vector direction = cur -> obj.light.direction;
						Vector tempd;
						matrix_xformVector(&LTM, &direction, &tempd);
						matrix_xformVector(GTM, &tempd, &direction);
						
						lighting_add(lighting, LightDirect, &cur -> obj.light.color, &direction, NULL, 0.0, 0.0);
					}
						break;
					case LightPoint:{
						//Vector direction = cur -> obj.light.direction;
						//Vector tmpv;
						//matrix_xformVector(&LTM, &direction, &tmpv);
						//matrix_xformVector(GTM, &tmpv, &direction);
						
						Point position = cur -> obj.light.position;
						Point tmppt;
						matrix_xformPoint(&LTM, &position, &tmppt);
						matrix_xformPoint(GTM, &tmppt, &position);
						
						lighting_add(lighting, LightPoint, &cur -> obj.light.color, NULL, &position, cur -> obj.light.cutoff, cur -> obj.light.sharpness);
					}
						break;
					case LightSpot:{
						Vector direction = cur -> obj.light.direction;
						Vector tmpv;
						matrix_xformVector(&LTM, &direction, &tmpv);
						matrix_xformVector(GTM, &tmpv, &direction);
						
						Point position = cur -> obj.light.position;
						Point tmppt;
						matrix_xformPoint(&LTM, &position, &tmppt);
						matrix_xformPoint(GTM, &tmppt, &position);
						
						lighting_add(lighting, LightSpot, &cur -> obj.light.color, &direction, &position, cur -> obj.light.cutoff, cur -> obj.light.sharpness);
					}					
					break;
				}
				break;
			}
            case ObjPoint:
			break;
			case ObjColor:
			break;
            case ObjPolyline:
                break;
            case ObjPolygon:
			break;
			case ObjIdentity:
				matrix_identity(&LTM);
			break;
			case ObjMatrix:
				matrix_multiply( &cur -> obj.matrix, &LTM, &LTM);		
				
			break;
			case ObjBodyColor: break;
			case ObjSurfaceColor: break;
			case ObjSurfaceCoeff:  break;
			//stop this section
            case ObjModule:{
				Matrix tempM;
				
				matrix_identity(&tempM);

				matrix_multiply(GTM, &LTM, &tempM);
				Module* mod = (Module*) (cur -> obj.module);
				//printf("entering another module\n");
				module_lighting(mod, VTM, &tempM, lighting);
				//printf("left module\n");
			break;
            }
            case ObjBezier:{
                break;
            }
		}
	//printf("about to iterate\n");
	cur = (Element*)cur -> next;
	//printf("iterated\n");
	}
}

void module_draw_lighting(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, Image *src){
	Lighting l;
	lighting_init(&l);
	//printf("lighting pass\n");
	module_lighting(md, VTM, GTM, &l);
	//printf("drawing pass\n");
	module_draw(md, VTM, GTM, ds, &l, src);
}

void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, Lighting *lighting, Image *src){
	Element *cur = md -> head;
	//printf("starting module draw\n");
	Matrix LTM;
	
	//printf("initial matrix state\n");
	
	//printf("gtm is: \n");
	//matrix_print(GTM, stdout);
	matrix_identity(&LTM);
	//printf("ltm is: \n");
	//matrix_print(&LTM, stdout);
	
	while(cur != NULL){
		printf("drawing element %d \n", cur -> type);
		printf("which has type %s \n", enumStrings[cur -> type]);
		switch(cur -> type){
		    case ObjNone: break;
            case ObjLine:{
                Line l;
				line_copy(&l, &cur -> obj.line);
				matrix_xformLine(&LTM, &l);
				matrix_xformLine(GTM, &l);
				matrix_xformLine(VTM, &l);
				line_normalize(&l);
				line_draw(&l, src, ds->color);
			break;
            }
            case ObjPoint:{
				Point pt1;
                Point pt2;
				point_copy(&pt1, &cur -> obj.point);
				matrix_xformPoint(&LTM, &pt1, &pt2);
				matrix_xformPoint(GTM, &pt2, &pt1);
				matrix_xformPoint(VTM, &pt1, &pt2);
				point_normalize(&pt2);
				point_draw(&pt2, src, ds->color);
			break;
            }
            case ObjPolyline:{
				Polyline p;
				polyline_init(&p);
				polyline_copy(&p, &cur -> obj.polyline);
				matrix_xformPolyline(&LTM, &p);
				matrix_xformPolyline(GTM, &p);
				matrix_xformPolyline(VTM, &p);
				polyline_normalize(&p);
				polyline_draw(&p, src, ds -> color);
				polyline_free(&p);
                break;
            }
            case ObjPolygon:{

				Polygon p;
				polygon_init(&p);
				polygon_copy(&p, &cur -> obj.polygon);
				//printf("copied polygon\n");
				matrix_xformPolygon(&LTM, &p);
				matrix_xformPolygon(GTM, &p);
				//printf("transformed polygon\n");
				Line l;
				Point pn;
				Color red = {{1.0, 0.0, 0.0}};
				if(ds -> shade == ShadeGouraud || ds -> shade == ShadeFlat){
					
					for(int i = 0; i < p.numVertex; i++){
						printf("normalizing vertex %d\n", i);
						vector_normalize(&p.normal[i]);
						/*
						pn.val[X] = p.vertex[i].val[X] + p.normal[i].val[X];
						pn.val[Y] = p.vertex[i].val[Y] + p.normal[i].val[Y];
						pn.val[Z] = p.vertex[i].val[Z] + p.normal[i].val[Z];
						pn.val[H] = 1;
						line_set(&l, p.vertex[i], pn);
						matrix_xformLine(VTM, &l);
						line_normalize(&l);
						//point_print(&l.a, stdout);
						//point_print(&l.b, stdout);
						
						if(p.normal[i].val[i] !=p.normal[i].val[i]){
							
						}
						else{
							line_draw(&l, src, red);
						}*/
					}					
					polygon_shade(&p, lighting, ds);
				
					
					if(ds -> shade == ShadeFlat){
						Color avg = {{0.0, 0.0, 0.0}};
						for(int i = 0; i < p.numVertex; i++){
							color_add(&avg, &p.color[i], &avg);
						}
						color_multiply_float(&avg, 1.0/p.numVertex, &avg);
						color_copy(&ds -> flatColor, &avg);
						//printf("c: %.2f %.2f %.2f\n", avg.c[0], avg.c[1], avg.c[2] );

					}
					
				}
				
				matrix_xformPolygon(VTM, &p);
				//printf("about to normalize polygon\n");
				polygon_normalize(&p);
				
				//printf("drawing polygon\n");
				//polygon_print(&p, stdout);
				polygon_drawShade(&p, src, ds, NULL);
				polygon_clear(&p);
			break;
            }
			case ObjIdentity:
				matrix_identity(&LTM);
			break;
			case ObjMatrix:
				matrix_multiply( &cur -> obj.matrix, &LTM, &LTM);		
				

				//printf("stored matrix is\n");
				//matrix_print(&cur ->obj.matrix, stdout);

				//printf("\n \nafter multiplying\n");
				//printf("LTM is: \n");
				//matrix_print(&LTM, stdout);

			break;
			case ObjColor: color_copy(&ds -> color, &cur -> obj.color);
			break;
			case ObjBodyColor: color_copy(&ds -> body , &cur -> obj.color);
			break;
			case ObjSurfaceColor: color_copy(&ds -> surface, &cur -> obj.color);
			break;
			case ObjSurfaceCoeff:  ds -> surfaceCoeff = cur -> obj.coeff;
			break;
			case ObjLight: break;
			//stop this section
            case ObjModule:{
				Matrix tempM;
				DrawState tempDS;
				
				matrix_identity(&tempM);
				//printf("temp matrix is \n");
				//matrix_print(&tempM, stdout);
				drawstate_copy(&tempDS, ds);
				point_copy(&tempDS.viewer, &ds->viewer);
				
				//printf("GTM is:\n");
				//matrix_print(GTM, stdout);

				//printf("LTM is: \n");
				//matrix_print(&LTM, stdout);

				matrix_multiply(GTM, &LTM, &tempM);
				//printf("temp updated matrix is\n");
				//matrix_print(&tempM, stdout);
				Module* mod = (Module*) (cur -> obj.module);
				module_draw(mod, VTM, &tempM, &tempDS, lighting, src);
			break;
            }
            case ObjBezier:{
                BezierCurve b = cur->obj.curve;
                Point temp;
                Point *transformed = (Point *)malloc( sizeof( Point ) * 4 );
                
                //transform stored points to view space
                matrix_xformPoint(&LTM, &b.pt[0], transformed);
                matrix_xformPoint(GTM, transformed, &temp);
                matrix_xformPoint(VTM, &temp, transformed);
                point_normalize(transformed);
                
                matrix_xformPoint(&LTM, &b.pt[1], transformed+1);
                matrix_xformPoint(GTM, transformed+1, &temp);
                matrix_xformPoint(VTM, &temp, transformed+1);
                point_normalize(transformed+1);
                
                matrix_xformPoint(&LTM, &b.pt[2], transformed+2);
                matrix_xformPoint(GTM, transformed+2, &temp);
                matrix_xformPoint(VTM, &temp, transformed+2);
                point_normalize(transformed+2);
                
                matrix_xformPoint(&LTM, &b.pt[3], transformed+3);
                matrix_xformPoint(GTM, transformed+3, &temp);
                matrix_xformPoint(VTM, &temp, transformed+3);
                point_normalize(transformed+3);
                
                BezierCurve *b2 = (BezierCurve *)malloc( sizeof( BezierCurve ) );
                bezierCurve_set( b2, transformed );
                
                bezierCurve_draw( b2, src, ds->color );
                free( transformed );
                free( b2 );
                break;
            }
		}
	
	cur = (Element*)cur -> next;
	}
}

//3D module functions

void module_translate(Module *md, double tx, double ty, double tz){
    //invalid input
    if ( md == NULL )
    {
        return;
    }
    
    Matrix t;
	matrix_identity(&t);
	matrix_translate(&t, tx, ty, tz);
    
    Element *e = element_init( ObjMatrix, &t );
    module_insert( md, e );
}

void module_scale(Module *md, double sx, double sy, double sz){
    //invalid input
    if ( md == NULL )
    {
        return;
    }
    
    Matrix s;
	matrix_identity(&s);
	matrix_scale(&s, sx, sy, sz);
    
    Element *e = element_init( ObjMatrix, &s );
    module_insert( md, e );
}

void module_rotateX(Module *md, double cth, double sth){
	
	if ( md == NULL )
    {
        return;
    }
    
    Matrix r;
	matrix_identity(&r);
	matrix_rotateX(&r, cth, sth);
    
    Element *e = element_init( ObjMatrix, &r );
    module_insert( md, e );
}


void module_rotateY(Module *md, double cth, double sth){
	
	if ( md == NULL )
    {
        return;
    }
    
    Matrix r;
	matrix_identity(&r);
	matrix_rotateY(&r, cth, sth);
    
    Element *e = element_init( ObjMatrix, &r );
    module_insert( md, e );
}

void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w){
	
	if ( md == NULL )
    {
        return;
    }
    
    Matrix r;
	matrix_identity(&r);
	matrix_rotateXYZ(&r, u, v, w);
    
    Element *e = element_init( ObjMatrix, &r );
    module_insert( md, e );
}

void module_cube(Module *md, int solid){
	Point pts[8];
	int i;
	//I'm trying to figure out how to do this because our module_free does not free points, so I cannot 
	//malloc points inside of here...
	point_set3D(&pts[0], -0.5, -0.5, -0.5);
	point_set3D(&pts[1], -0.5, -0.5,  0.5);
	point_set3D(&pts[2], -0.5,  0.5, -0.5);
	point_set3D(&pts[3], -0.5,  0.5,  0.5);
	point_set3D(&pts[4],  0.5, -0.5, -0.5);
	point_set3D(&pts[5],  0.5, -0.5,  0.5);
	point_set3D(&pts[6],  0.5,  0.5, -0.5);
	point_set3D(&pts[7],  0.5,  0.5,  0.5);
	if(solid == 0){ // I know I can do if(solid) but I prefer to do this
		//make unit cube
		Line l;
		line_set(&l, pts[0], pts[1]);
		module_line(md, &l);
		line_set(&l, pts[1], pts[5]);
		module_line(md, &l);
		line_set(&l, pts[5], pts[4]);
		module_line(md, &l);
		line_set(&l, pts[0], pts[4]);
		module_line(md, &l);
		
		
		line_set(&l, pts[0], pts[2]);
		module_line(md, &l);
		line_set(&l, pts[2], pts[3]);
		module_line(md, &l);
		line_set(&l, pts[3], pts[1]);
		module_line(md, &l);
		
		
		line_set(&l, pts[3], pts[7]);
		module_line(md, &l);
		line_set(&l, pts[7], pts[6]);
		module_line(md, &l);
		line_set(&l, pts[6], pts[2]);
		module_line(md, &l);
		
		line_set(&l, pts[6], pts[4]);
		module_line(md, &l);
		line_set(&l, pts[7], pts[5]);
		module_line(md, &l);
	}
	else{
		Point plist[4];
		Vector normals[4];
		Polygon p;
		polygon_init(&p);
		int i;
		//how do we do surface normals
		
		plist[0] = pts[0];
		plist[1] = pts[1];
		plist[2] = pts[5];
		plist[3] = pts[4];
		
		polygon_set(&p, 4, &plist[0]);
		Vector down = {{0.0, -1.0, 0.0, 0.0}};
		for(i = 0; i < 4; i++){
			normals[i] = down;
		}
		
		polygon_setNormals(&p, 4, &normals[0]);
		module_polygon(md, &p);
		plist[0] = pts[0];
		plist[1] = pts[2];
		plist[2] = pts[3];
		plist[3] = pts[1];
		
		polygon_set(&p, 4, &plist[0]);
		Vector left = {{-1.0, 0.0, 0.0, 0.0}};
		for(i = 0; i < 4; i++){
			normals[i] = left;
		}

		polygon_setNormals(&p, 4, &normals[0]);
		module_polygon(md, &p);


		plist[0] = pts[3];
		plist[1] = pts[7];
		plist[2] = pts[6];
		plist[3] = pts[2];
		
		polygon_set(&p, 4, &plist[0]);
		
		Vector up = {{0.0, 1.0, 0.0, 0.0}};
		for(i = 0; i < 4; i++){
			normals[i] = up;
		}

		polygon_setNormals(&p, 4, &normals[0]);
		module_polygon(md, &p);

		plist[0] = pts[7];
		plist[1] = pts[6];
		plist[2] = pts[4];
		plist[3] = pts[5];
		
		polygon_set(&p, 4, &plist[0]);
		
		Vector right = {{1.0, 0.0, 0.0, 0.0}};
		for(i = 0; i < 4; i++){
			normals[i] = right;
		}
		polygon_setNormals(&p, 4, &normals[0]);
		module_polygon(md, &p);
		plist[0] = pts[1];
		plist[1] = pts[5];
		plist[2] = pts[7];
		plist[3] = pts[3];
		
		polygon_set(&p, 4, &plist[0]);
		Vector front = {{0.0, 0.0, 1.0, 0.0}};
		for(i = 0; i < 4; i++){
			normals[i] = front;
		}
		polygon_setNormals(&p, 4, &normals[0]);
		module_polygon(md, &p);
		
		plist[0] = pts[0];
		plist[1] = pts[2];
		plist[2] = pts[6];
		plist[3] = pts[4];
		
		polygon_set(&p, 4, &plist[0]);
		
		Vector back = {{0.0, 0.0, -1.0, 0.0}};
		for(i = 0; i < 4; i++){
			normals[i] = back;
		}
		polygon_setNormals(&p, 4, &normals[0]);
		module_polygon(md, &p);
	}
}

void module_color(Module *md, Color *c){

    //invalid input
    if ( md == NULL || c == NULL )
    {
        return;
    }
    
    Element *e = element_init( ObjColor, c );
    module_insert( md, e );
}

void module_bodyColor(Module *md, Color *c){

    //invalid input
    if ( md == NULL || c == NULL )
    {
        return;
    }
    
    Element *e = element_init( ObjBodyColor, c );
    module_insert( md, e );
}

void module_surfaceColor(Module *md, Color *c){

    //invalid input
    if ( md == NULL || c == NULL )
    {
        return;
    }
    
    Element *e = element_init( ObjSurfaceColor, c );
    module_insert( md, e );
}

void module_surfaceCoeff(Module *md, float coeff){

    //invalid input
    if ( md == NULL)
    {
        return;
    }
    
    Element *e = element_init( ObjSurfaceCoeff, &coeff );
    module_insert( md, e );
}


void module_reflectance(Module *md, float r){
	//invalid input
    if ( md == NULL)
    {
        return;
    }
    
    Element *e = element_init( ObjReflectance, &r );
    module_insert( md, e );
}
void module_transmittance(Module *md, float t){
	//invalid input
    if ( md == NULL)
    {
        return;
    }
    
    Element *e = element_init( ObjTransmittance, &t );
    module_insert( md, e );
}
void module_setTexture(Module *md, Tex_cube_map* t){
    //invalid input
    if ( md == NULL)
    {
        return;
    }
    
    Element *e = element_init( ObjTexture, t );
    module_insert( md, e );
}

void module_setProjectionType(Module *md, Projection_type p){
    if ( md == NULL)
    {
        return;
    }
    
    Element *e = element_init( ObjProjectionType,  &p);
    module_insert( md, e );
}

void module_clearTexture(Module *md){
	    //invalid input
    if ( md == NULL)
    {
        return;
    }
    
    Element *e = element_init( ObjTexture, NULL);
    module_insert( md, e );
}


/* Adds b to the tail of the module’s list
 */
void module_bezierCurve( Module *md, BezierCurve *b )
{
    //invalid input
    if ( md == NULL || b == NULL )
    {
        return;
    }
    
    Element *e = element_init( ObjBezier, b );
    module_insert( md, e );
}

/*
* converts a bezier curve into lines using subdivision
*/
void module_bezierCurveSubdiv( Module *md, BezierCurve *b, int divisions )
{
    //invalid input
    if ( md == NULL || b == NULL )
    {
        return;
    }
	int i;
	
	if(divisions == 0){
		Line l;
		for(i = 0; i < 3; i++){
			line_set(&l, b->pt[i], b->pt[i+1]);
			module_line(md, &l);
		}
	}
	else{
		BezierCurve b1;
		Point ptsout[8];
		BezierCurve b2;

		
	    //first level of subdivision
		bezier_subdivide(&b->pt[0], &ptsout[0]);
		
	    bezierCurve_set( &b1, &ptsout[0]);
		module_bezierCurveSubdiv(md, &b1, divisions-1);	
		
		
	    bezierCurve_set( &b2, &ptsout[4] );
		module_bezierCurveSubdiv(md, &b2, divisions-1);
	}

}
/* doesn't work
void calculate_bezier_nromals(BezierSurface *b, int u, int v, Vector *norm){
	Vector vCurve[4];
	Vector uCurve[4];
	Vector dV = {{0.0, 0.0, 0.0}};
	Vector dU = {{0.0, 0.0, 0.0}};
	float fu = u/4.0;
	float fv = v/4.0;
	
	for(int i =0; i< 4; i++){
		vector_copy(&vCurve[i], &b->pt[i][v]);
	}
	vector_scale(&vCurve[0], -3 * (1-fu) * (1-fu));
	vector_add(&vCurve[0], &dV, &dV);
	
	vector_scale(&vCurve[1], 3 * (1-fu) * (1-fu) - (6 * fu * (1-fu)));
	vector_add(&vCurve[1], &dV, &dV);
	
	vector_scale(&vCurve[2], 6 * fu * (1-fu) - (3 *fu * fu));
	vector_add(&vCurve[2], &dV, &dV);
	
	vector_scale(&vCurve[3], 3 * fu * fu * fu);
	vector_add(&vCurve[3], &dV, &dV);
	
	for(int i =0; i< 4; i++){
		vector_copy(&uCurve[i], &b->pt[u][i]);
	}
	vector_scale(&uCurve[0], -3 * (1-fv) * (1-fv));
	vector_add(&uCurve[0], &dU, &dU);
	
	vector_scale(&uCurve[1], 3 * (1-fv) * (1-fv) - (6 * fv * (1-fv)));
	vector_add(&uCurve[1], &dU, &dU);
	
	vector_scale(&uCurve[2], 6 * fv * (1-fv) - (3 *fv * fv));
	vector_add(&uCurve[2], &dU, &dU);
	
	vector_scale(&uCurve[3], 3 * fv * fv * fv);
	vector_add(&uCurve[3], &dU, &dU);
	
	vector_cross(&dV, &dU, norm);
	vector_normalize(norm);
}
*/
/*
* subdivides a bezier surface using N divisions.
* solid specifies if we make lines or triangles between all of the control points
*/
void module_bezierSurface(Module *md, BezierSurface *b, int divisions, int solid){
	//printf("drawing surfae with %d divisions\n",divisions);
	if ( md == NULL || b == NULL )
    {
        return;
    }
	int i;
	int j;
	
	
	if(divisions == 0){
		
		//ok so class/notes say look at corners, but expected output implies draw between control points
		if(solid == 0){
			//draw lines
			Line l;
			for(i = 0; i < 4; i++){
				for(j = 0; j < 4; j++){
					if(j < 3){
						line_set(&l, b->pt[i][j], b->pt[i][j+1]);
						module_line(md, &l);
					}
					if(i < 3){
						line_set(&l, b->pt[i][j], b->pt[i+1][j]);
						module_line(md, &l);
					}
				}
			}
		}
		else{
			//make polygons
			/*
			Vector normals[4][4];
			Vector avg = {{0.0, 0.0, 0.0, 0.0}};
			Point ptsu[4];
			Point ptsv[4];
			for(i = 0; i < 4; i++){
				point_copy(&ptsu[0], &b->pt[0][i]);
				point_copy(&ptsu[1], &b->pt[1][i]);
				point_copy(&ptsu[2], &b->pt[2][i]);
				point_copy(&ptsu[3], &b->pt[3][i]);
				
				for(j = 0; j < 4; j++){
					point_copy(&ptsv[0], &b->pt[j][0]);
					point_copy(&ptsv[1], &b->pt[j][1]);
					point_copy(&ptsv[2], &b->pt[j][2]);
					point_copy(&ptsv[3], &b->pt[j][3]);
				//	printf("normal %d, %d is\n", i, j);
					bezier_surfaceNormal(&ptsu[0], &ptsv[0], (0.25 * i), (0.25 * j), &normals[i][j]);
					avg.val[X] += normals[i][j].val[X];
					avg.val[Y] += normals[i][j].val[Y];
					avg.val[Z] += normals[i][j].val[Z];
				
					vector_normalize(&normals[i][j]);
					
					//vector_print(&normals[i][j], stdout);
				}
			}
			
			avg.val[X] /= 16;			
			avg.val[Y] /= 16;
			avg.val[Z] /= 16;
			vector_normalize(&avg);
	*/
	
			Polygon p;
			Point pts[3];
			Vector norms[3];
			Vector av,bv;
			for(i = 0; i < 3; i++){
				for(j = 0; j < 3; j++){
					//printf("making polygon 1\n");
					point_copy(&pts[0], &b->pt[i][j]);
					point_copy(&pts[1], &b->pt[i][j+1]);
					point_copy(&pts[2], &b->pt[i+1][j]);
					
					for(int c = 0; c < 3; c++){
						av.val[X] = pts[c].val[X] - pts[(c+1)%3].val[X];
						av.val[Y] = pts[c].val[Y] - pts[(c+1)%3].val[Y];
						av.val[Z] = pts[c].val[Z] - pts[(c+1)%3].val[Z];
						
						bv.val[X] = pts[c].val[X] - pts[(c-1 + 3)%3].val[X];
						bv.val[Y] = pts[c].val[Y] - pts[(c-1 + 3)%3].val[Y];
						bv.val[Z] = pts[c].val[Z] - pts[(c-1 + 3)%3].val[Z];
						
						vector_cross(&bv, &av, &norms[c]);
						vector_normalize(&norms[c]);
					}
					
					/*
					calculate_bezier_nromals(b, i, j, &norms[0]);
					calculate_bezier_nromals(b, i, j+1, &norms[1]);
					calculate_bezier_nromals(b, i+1, j, &norms[2]);
					*/ /*
					vector_copy(&norms[0], &normals[i][j]);
					vector_copy(&norms[1], &normals[i][j+1]);
					vector_copy(&norms[2], &normals[i+1][j]);
					*/
					/*
					vector_copy(&norms[0], &avg);
					vector_copy(&norms[1], &avg);
					vector_copy(&norms[2], &avg);					
					*/
					polygon_init(&p);
					polygon_set(&p, 3, &pts[0]);
					polygon_setNormals(&p, 3, &norms[0]);
					module_polygon(md, &p);
					polygon_clear(&p);
					
					//printf("making polygon 2\n");
					point_copy(&pts[0], &b->pt[i+1][j]);
					point_copy(&pts[1], &b->pt[i][j+1]);
					point_copy(&pts[2], &b->pt[i+1][j+1]);
					
					for(int c = 0; c < 3; c++){
						av.val[X] = pts[c].val[X] - pts[(c+1)%3].val[X];
						av.val[Y] = pts[c].val[Y] - pts[(c+1)%3].val[Y];
						av.val[Z] = pts[c].val[Z] - pts[(c+1)%3].val[Z];
						
						bv.val[X] = pts[c].val[X] - pts[(c-1 + 3)%3].val[X];
						bv.val[Y] = pts[c].val[Y] - pts[(c-1 + 3)%3].val[Y];
						bv.val[Z] = pts[c].val[Z] - pts[(c-1 + 3)%3].val[Z];
						
						vector_cross(&bv, &av, &norms[c]);
						vector_normalize(&norms[c]);
					}
					
					/*
					calculate_bezier_nromals(b, i+1, j, &norms[0]);
					calculate_bezier_nromals(b, i, j+1, &norms[1]);
					calculate_bezier_nromals(b, i+1, j+1, &norms[2]);
					*//*
					vector_copy(&norms[0], &normals[i+1][j]);
					vector_copy(&norms[1], &normals[i][j+1]);
					vector_copy(&norms[2], &normals[i+1][j+1]);
					*/
					/*
					vector_copy(&norms[0], &avg);
					vector_copy(&norms[1], &avg);
					vector_copy(&norms[2], &avg);					
					*/
					polygon_init(&p);
					polygon_set(&p, 3, &pts[0]);
					polygon_setNormals(&p, 3, &norms[0]);
					module_polygon(md, &p);
					polygon_clear(&p);
				}
			}
		}
	}
	else{
		//subdivide
		//ok so it feels like this whole things is a mess and a hack, is there a better way to do this
		Point vertical[4][8];

		Point pts[8][8];
		
		Point tempHorz[4];
		
		//subdivide curves
		/*
		for(i = 0; i < 4; i++){
			bezier_subdivide(&b->pt[i][0], &vertical[i][0]);
		}
		
		
		for(i = 0; i < 8; i++){
			for(j = 0; j < 4; j++){
				tempHorz[j] = vertical[j][i];
			}
			bezier_subdivide(&tempHorz[0], &pts[i][0]); 
		}
		*/
		
		Point horz[4][8];
		for(i = 0; i < 4; i++){
			for(j = 0; j < 4; j++){
				tempHorz[j] = b->pt[j][i];
			}
			bezier_subdivide(&tempHorz[0], &horz[i][0]);
		}
		
		Point tmpVert[4];
		for(i = 0; i < 8; i++){
			for(j = 0; j < 4; j++){
				tmpVert[j] = horz[j][i];
			}
			bezier_subdivide(&tmpVert[0], &pts[i][0]); 
		}
		
		//printf("starting a new surface\n");
		//put those subdivisions into surfaces
		for(i = 0; i < 4; i++){
			Point bezPts[16];
			BezierSurface bs;
			
			//so we're using this here to determine which section of the array we are taking from
			//while putting both halves into an array for curves was elegant and a quick fix it left
			//a bit of a mess here
			int half = i < 2 ? 0 : 4;
			
			//printf("doing point %d, %d \n", i%2, half + j);			
			for(j = 0; j < 4; j++){
				point_copy(&bezPts[j], &pts[i%2 * 4][half + j]);
				point_copy(&bezPts[4 + j], &pts[i%2 * 4 +1][half + j]);
				point_copy(&bezPts[8 + j],	&pts[i%2 * 4 +2][half + j]);
				point_copy(&bezPts[12 + j], &pts[i%2 * 4 +3][half + j]);					
			}
			
			bezierSurface_set(&bs, &bezPts[0]);
			Module* inner = module_create();
			module_bezierSurface(inner, &bs, divisions-1, solid);
			module_module(md, inner);
		}
	}
}

/*
* make a unit sphere
*/
static void module_sphere_face(Module *mod, Point *pts, int division){
	//printf("doing face with division %d\n");
	if(division == 0){
		Polygon p;
		polygon_init(&p);
		/*
		int i;
		for(i = 0; i < 3; i++){
			point_print(&pts[i], stdout);
		}
		*/
		//printf("init polygon\n");
		polygon_set(&p, 3, pts);
		polygon_setNormals(&p, 3, pts);
		//printf("set polygon\n");
		module_polygon(mod, &p);
		polygon_clear(&p);
		//printf("freed polygon\n");
	}
	else{
 
		Point pts0[3];
		Point pts1[3];
		Point pts2[3];
		Point pts3[3];
		
		Point mid0;
		Point mid1;
		Point mid2;
		
		float mid0x = (pts[0].val[X] + pts[1].val[X])/2;
		float mid0y = (pts[0].val[Y] + pts[1].val[Y])/2;
		float mid0z = (pts[0].val[Z] + pts[1].val[Z])/2;
		point_set3D(&mid0, mid0x, mid0y, mid0z);
		//make point a unit distance from the center
		vector_normalize(&mid0);
		
		float mid1x  = (pts[1].val[X] + pts[2].val[X])/2;
		float mid1y  = (pts[1].val[Y] + pts[2].val[Y])/2;
		float mid1z  = (pts[1].val[Z] + pts[2].val[Z])/2;
		point_set3D(&mid1, mid1x, mid1y, mid1z);
		vector_normalize(&mid1);
	
		float mid2x  = (pts[2].val[X] + pts[0].val[X])/2;
		float mid2y  = (pts[2].val[Y] + pts[0].val[Y])/2;
		float mid2z  = (pts[2].val[Z] + pts[0].val[Z])/2;
		point_set3D(&mid2, mid2x, mid2y, mid2z);
		vector_normalize(&mid2);
		
		//do the lower left triangle
		point_copy(&pts0[0], &pts[0]);
		point_copy(&pts0[1], &mid0);
		point_copy(&pts0[2],  &mid2);
		Module* md = module_create();
		module_sphere_face(md, &pts0[0], division-1);
		module_module(mod, md);
		//lower right
		point_copy(&pts1[0], &mid0);
		point_copy(&pts1[1], &pts[1]);
		point_copy(&pts1[2], &mid1);
		md = module_create();
		module_sphere_face(md, &pts1[0], division-1);
		module_module(mod, md);
		//center triangle
		point_copy(&pts2[0], &mid0);
		point_copy(&pts2[1], &mid1);
		point_copy(&pts2[2], &mid2);
		md = module_create();
		module_sphere_face(md, &pts2[0], division-1);
		module_module(mod, md);
		//top triangle
		point_copy(&pts3[0], &mid2);
		point_copy(&pts3[1], &mid1);
		point_copy(&pts3[2], &pts[2]);
		md = module_create();
		module_sphere_face(md, &pts3[0], division-1);
		module_module(mod, md);
	}
}

void module_sphere(Module *md, int division){
	Point pts[6];
	point_set3D(&pts[0], 0, 1.0, 0);
	float sqrt2 = sqrt(2)/2;
	point_set3D(&pts[1], sqrt2, 0, sqrt2);
	point_set3D(&pts[2], -sqrt2, 0, sqrt2);
	point_set3D(&pts[3], -sqrt2, 0, -sqrt2);
	point_set3D(&pts[4], sqrt2, 0,  -sqrt2);
	point_set3D(&pts[5], 0, -1.0, 0);
	
	Point tri[3];
	
	//make first trinagle 0, 1, 2
	point_copy(&tri[0], &pts[0]);
	point_copy(&tri[1], &pts[1]);	
	point_copy(&tri[2], &pts[2]);
	module_sphere_face(md, &tri[0], division);
	
	//second face 0, 2, 3
	point_copy(&tri[0], &pts[0]);
	point_copy(&tri[1], &pts[2]);	
	point_copy(&tri[2], &pts[3]);
	module_sphere_face(md, &tri[0], division);
	
	//third 0, 3, 4
	point_copy(&tri[0], &pts[0]);
	point_copy(&tri[1], &pts[3]);	
	point_copy(&tri[2], &pts[4]);
	module_sphere_face(md, &tri[0], division);
	
	
	//fourth 0, 4, 1
	point_copy(&tri[0], &pts[0]);
	point_copy(&tri[1], &pts[4]);	
	point_copy(&tri[2], &pts[1]);
	module_sphere_face(md, &tri[0], division);
		
	//fifth 5, 2, 1,
	point_copy(&tri[0], &pts[5]);
	point_copy(&tri[1], &pts[2]);	
	point_copy(&tri[2], &pts[1]);
	module_sphere_face(md, &tri[0], division);
	
	//sixth 5, 1, 4
	point_copy(&tri[0], &pts[5]);
	point_copy(&tri[1], &pts[1]);	
	point_copy(&tri[2], &pts[4]);
	module_sphere_face(md, &tri[0], division);
		
	//secenth 5, 4, 3
	point_copy(&tri[0], &pts[5]);
	point_copy(&tri[1], &pts[4]);	
	point_copy(&tri[2], &pts[3]);
	module_sphere_face(md, &tri[0], division);
		
	//eigth 5, 3, 2
	point_copy(&tri[0], &pts[5]);
	point_copy(&tri[1], &pts[3]);	
	point_copy(&tri[2], &pts[2]);
	module_sphere_face(md, &tri[0], division);
	
}