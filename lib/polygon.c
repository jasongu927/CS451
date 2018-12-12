/* polygon.c
 * Defines functions for using Polygons
 * Polygons do connect the last vertex back to the first
 * Origin is in upper left corner
 * Pixel coordinates correspond to lower left corner of pixel
 *
 * Melody Mao
 * CS351 Spring 2018
 * Project 4
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "polygon.h"
#include "list.h"

/*** Polygon functions ***/

/* returns an allocated Polygon pointer initialized
 * so that numVertex is 0 and vertex is NULL
 */
Polygon *polygon_create()
{
    Polygon *p = (Polygon *)malloc( sizeof(Polygon) );
    p->oneSided = 1;
    p->numVertex = 0;
    p->vertex = NULL;
	p->color = NULL;
	p -> normal = NULL;
	p -> zBuffer = 1;
	
	p -> orgVertex = NULL;
	p -> orgNormal = NULL;
	p -> reflectance = 0.0;
	p -> transmittance = 0.0;
	p -> tex_ptr = NULL;
    p -> project = ProjectUV;
	
    return p;
}

/* returns an allocated Polygon pointer with the vertex list initialized
 * to the points in vlist
 */
Polygon *polygon_createp( int numV, Point *vlist )
{
    //invalid input
    if ( vlist == NULL )
    {
        return NULL;
    }
    
    Polygon *p = (Polygon *)malloc( sizeof(Polygon) );
    p->oneSided = 1;
    p->numVertex = numV;
	p->color = NULL;
	p -> normal = NULL;
	p -> zBuffer = 1;
    p->vertex = malloc( sizeof(Point) * numV );
    
	p -> orgVertex = NULL;
	p -> orgNormal = NULL;
	p -> reflectance = 0.0;
	p -> transmittance = 0.0;
	p -> tex_ptr = NULL;
	p -> project = ProjectUV;
    //copy over points
    for ( int i = 0; i < numV; i++ )
    {
        point_copy( (p->vertex) + i, vlist + i );
    }
    
    return p;
}

/* frees the internal data and the Polygon pointer
 */
void polygon_free( Polygon *p )
{
    //invalid input
    if ( p == NULL )
    {
        return;
    }
    
    if ( p->vertex != NULL )
    {
		//printf("freeing vertecies\n");
        free( p->vertex );
    }
	
	if( p -> color != NULL){
		//printf("freeing colors\n");
		free(p -> color);
	}
	if( p -> normal != NULL){
		//printf("freeing normals\n");
		free(p -> normal);
	}
	if(p->orgNormal !=NULL){
		free(p->orgNormal);
	}
	if(p->orgVertex !=NULL){
		free(p->orgVertex);
	}
    free( p );
}

/* initializes the pre-existing Polygon to an empty Polygon
 */
void polygon_init( Polygon *p )
{
    //invalid input
    if ( p == NULL )
    {
        return;
    }
    //if vertex had data before, deallocate
    /*
	if ( p->vertex != NULL )
    {
		printf("trying to dealoc data\n");
        free( p->vertex );
    }*/
    
    p->oneSided = 1;
    p->numVertex = 0;
    p->vertex = NULL;
	p->color = NULL;
	p -> normal = NULL;
	p -> zBuffer = 1;
	
	p -> orgVertex = NULL;
	p -> orgNormal = NULL;
	p -> reflectance = 0.0;
	p -> transmittance = 0.0;
	p -> tex_ptr = NULL;
	p -> project = ProjectUV;
}

/* initializes the vertex list to the points in vlist
 * de-allocates/allocates the vertex list for p, as necessary
 */
void polygon_set( Polygon *p, int numV, Point *vlist )
{
    //invalid input
    if ( p == NULL || vlist == NULL )
    {
        return;
    }
    
    //if vertices were already set, de-allocate
    if ( p->vertex != NULL )
    {
        free( p->vertex );
    }
    
    //allocate for new points
    p->numVertex = numV;
    p->vertex = malloc( sizeof(Point) * numV );
    
    //copy over points
    for ( int i = 0; i < numV; i++ )
    {
        point_copy( (p->vertex) + i, vlist + i );
    }
}

/* frees the internal data for a polygon, if necessary,
 * and sets numVertex to 0 and vertex to NULL
 */
void polygon_clear( Polygon *p )
{
    //invalid input
    if ( p == NULL )
    {
        return;
    }
    
    //if vertices were already set, de-allocate
    if ( p->vertex != NULL )
    {
        free( p->vertex );
    }
	if( p -> color != NULL){
		free(p -> color);
	}
	if( p -> normal != NULL){
		free(p -> normal);
	}
	if(p->orgNormal !=NULL){
		free(p->orgNormal);
	}
	if(p->orgVertex !=NULL){
		free(p->orgVertex);
	}
    
    p->numVertex = 0;
    p->vertex = NULL;
    p->oneSided = 1;
	p->color = NULL;
	p -> normal = NULL;
	p -> zBuffer = 1;
	
	p -> orgVertex = NULL;
	p -> orgNormal = NULL;
	p -> reflectance = 0.0;
	p -> transmittance = 0.0;
	p -> tex_ptr = NULL;
	p -> project = ProjectUV;
}

/* sets the oneSided field to the value
 */
void polygon_setSided( Polygon *p, int oneSided )
{
    //invalid input
    if ( p == NULL )
    {
        return;
    }
    
    p->oneSided = oneSided;
}

//sets the colors
void polygon_setColors(Polygon *p, int numV, Color *clist){
	//printf("setting colors\n");
	    //invalid input
	//printf("setting colors\n");
	
    if ( p == NULL || clist == NULL || numV != p -> numVertex)
    {
        return;
    }
    
    //if vertices were already set, de-allocate
    if ( p->color != NULL )
    {
		//printf("freeing already allocated data\n");
        free( p->color );
    }
    
    //allocate for new points
    p->color = malloc( sizeof(Color) * numV );
    
    //copy over points
    for ( int i = 0; i < numV; i++ )
    {
        color_copy( &(p->color)[i], &clist[i] );
    }
	//printf("successfully set colors\n");
}

//sets the normals
void polygon_setNormals(Polygon *p, int numV, Vector *normals){
	//printf("setting normals\n");
	if ( p == NULL || normals == NULL || numV != p -> numVertex)
    {
        return;
    }
    
    //if vertices were already set, de-allocate
    if ( p->normal != NULL )
    {
        free( p->normal );
    }
    
    //allocate for new points
    p->normal = malloc( sizeof(Vector) * numV );
    
    //copy over points
    for ( int i = 0; i < numV; i++ )
    {
        vector_copy( &(p->normal)[i], &normals[i] );
    }
}

//sets all the fields
void polygon_setAll(Polygon *p, int numV, Point *vlist, Color *clist, Vector *normals){
	if ( p == NULL || vlist == NULL || clist == NULL || normals == NULL)
    {
        return;
    }
    
    //if vertices were already set, de-allocate
    if ( p->vertex != NULL )
    {
        free( p->vertex );
    }
    
    //allocate for new points
    p->numVertex = numV;
    p->vertex = malloc( sizeof(Point) * numV );
    
    //copy over points
    for ( int i = 0; i < numV; i++ )
    {
        point_copy( (p->vertex) + i, vlist + i );
    }
	
	//if vertices were already set, de-allocate
    if ( p->color != NULL )
    {
        free( p->color );
    }
    
    //allocate for new points
    p->color = malloc( sizeof(Color) * numV );
    
    //copy over points
    for ( int i = 0; i < numV; i++ )
    {
        color_copy( &(p->color)[i], &clist[i] );
    }
	
	    //if vertices were already set, de-allocate
    if ( p->normal != NULL )
    {
        free( p->normal );
    }
    
    //allocate for new points
    p->normal = malloc( sizeof(Vector) * numV );
    
    //copy over points
    for ( int i = 0; i < numV; i++ )
    {
        vector_copy( &(p->normal)[i], &normals[i] );
    }
}

//sets the zbuffer to the flag
void polygonZbuffer(Polygon *p, int flag){
	p -> zBuffer = flag;
}

void polygon_setOrgNormals(Polygon *p, int numV, Vector *normals){
	if ( p == NULL || normals == NULL || numV != p -> numVertex)
    {
        return;
    }
    
    //if vertices were already set, de-allocate
    if ( p->orgNormal != NULL )
    {
        free( p->orgNormal );
    }
    
    //allocate for new points
    p->orgNormal = malloc( sizeof(Vector) * numV );
    
    //copy over points
    for ( int i = 0; i < numV; i++ )
    {
        vector_copy( &(p->orgNormal)[i], &normals[i] );
    }
}

void polygon_setOrgVertex(Polygon *p, int numV, Point *vertex){
	if ( p == NULL || vertex == NULL || numV != p -> numVertex)
    {
        return;
    }
    
    //if vertices were already set, de-allocate
    if ( p->orgVertex != NULL )
    {
        free( p->orgVertex );
    }
    
    //allocate for new points
    p->orgVertex = malloc( sizeof(Point) * numV );
    
    //copy over points
    for ( int i = 0; i < numV; i++ )
    {
        point_copy( &(p->orgVertex)[i], &vertex[i] );
    }
}
void polygon_setReflectance(Polygon *p, float r){
	p->reflectance = r;
}
void polygon_setTransmittance(Polygon *p, float t){
	p->transmittance = t;
}
void polygon_setTexture(Polygon*p, Tex_cube_map* t, Projection_type proj){
	p->tex_ptr = t;
	p -> project = proj;
}

/* De-allocates/allocates space as necessary in the dest polygon data structure
 * and copies the vertex data from the source polygon (from) to the destination (to)
 */
void polygon_copy( Polygon *to, Polygon *from )
{
    //invalid input
    if ( to == NULL || from == NULL )
    {
        return;
    }
    
    //if dest already contains data, deallocate
    if ( to->vertex != NULL )
    {
        free( to->vertex );
    }
    
    //if source is empty, empty dest
    if ( from->vertex == NULL )
    {
        to->oneSided = from->oneSided;
        to->numVertex = 0;
        to->vertex = NULL;
        return;
    }
    
    //allocate for new points
    int numV = from->numVertex;
    
	to -> zBuffer = from -> zBuffer;
	
	to->numVertex = numV;
    
	
	to->vertex = malloc( sizeof(Point) * numV );
    //copy over points
	
    for ( int i = 0; i < numV; i++ )
    {
        point_copy( (to->vertex) + i, (from->vertex) + i );
    }
	
	//this handles all cases
	if(to->normal){
		free(to->normal);
	}
	polygon_setNormals(to, numV, from -> normal);
	polygon_setColors(to, numV, from -> color);
	//if these are null it will do nothing!
	polygon_setOrgNormals(to, numV, from->orgNormal);
	polygon_setOrgVertex(to, numV, from->orgVertex);
	polygon_setReflectance(to, from->reflectance);
	polygon_setTransmittance(to, from->transmittance);
	polygon_setTexture(to, from->tex_ptr, from->project);
}

/* prints Polygon data to the stream designated by the FILE pointer
 */
void polygon_print( Polygon *p, FILE *fp )
{
    //invalid input
    if ( p == NULL || fp == NULL )
    {
        return;
    }
    
    int numV = p->numVertex;
    fprintf( fp, "polygon: oneSided = %d, %d vertices...\n", p->oneSided, numV );
    
    //print out vertices if there are any
    if ( numV > 0 )
    {
        for ( int i = 0; i < numV; i++ )
        {
            fprintf( fp, " " );
            point_print( p->vertex + i, fp );
        }
		
		fprintf(fp, "normals are\n");
		if (p->normal){
			vector_print(&p->normal[0], fp);
		}
    }
	
}

/* normalize the x and y values of each vertex by the homogeneous coordinate
 */
void polygon_normalize( Polygon *p )
{
    //invalid input
    if ( p == NULL )
    {
        return;
    }
    
    for ( int i = 0; i < p->numVertex; i++ )
    {
        point_normalize( p->vertex + i );
    }
}

/* draw the polygon using color c
 */
void polygon_draw( Polygon *p, Image *src, Color c )
{
    //invalid input
    if ( p == NULL || src == NULL || p->vertex == NULL )
    {
        return;
    }
    
    Line line; //used for all line segments
    
    //loop through to draw line segments between pairs of vertices
    int lastIDX = p->numVertex - 1;
    for ( int i = 0; i < lastIDX; i++ )
    {
        line_set( &line, p->vertex[i], p->vertex[i + 1] );
		line_zBuffer(&line, p->zBuffer);
        line_draw( &line, src, c );
    }
    
    //draw line segment from last vertex to first
    line_set( &line, p->vertex[lastIDX], p->vertex[0] );
	line_zBuffer(&line, p->zBuffer);
	line_draw(&line, src, c);
}

/********************
 Scanline Fill Algorithm (based on code skeleton from Bruce Maxwell)
 ********************/

// define the struct here, because it is local to only this file
typedef struct Edge {
    float x0, y0;                   /* start point for the edge */
    float x1, y1;                   /* end point for the edge */
    int yStart, yEnd;               /* start row and end row */
    float xIntersect, dxPerScan;    /* where the edge intersects the current scanline and how it changes */
	float z0, z1;                   //all z values are actually 1/z
	Color c0, c1;
	float zIntersect, dzPerScan;
	Color cIntersect, dcPerScan;
    /* we'll add more here later */
    struct Edge *next;
} Edge;


/*
	This is a comparison function that returns a value < 0 if a < b, a
	value > 0 if a > b, and 0 if a = b.  It uses the yStart field of the
	Edge structure.  It is used to sort the overall edge list.
 */
static int compYStart( const void *a, const void *b ) {
    Edge *ea = (Edge *)a;
    Edge *eb = (Edge *)b;
    
    return(ea->yStart - eb->yStart);
}


/*
	This is a comparison function that returns a value < 0 if a < b, a
	value > 0 if a > b, and 0 if a = b.  It uses the xIntersect field of the
	Edge structure.  It is used to sort the active edge list.
 */
static int compXIntersect( const void *a, const void *b ) {
    Edge *ea = (Edge *)a;
    Edge *eb = (Edge *)b;
    
    if( ea->xIntersect < eb->xIntersect )
        return(-1);
    else if(ea->xIntersect > eb->xIntersect )
        return(1);
    
    return(0);
}

/*
	Allocates, creates, fills out, and returns an Edge structure given
	the inputs.
 
	Current inputs are just the start and end location in image space.
 */
static Edge *makeEdgeRec( Point start, Point end, Image *src, Color c1, Color c2, DrawState *ds )
{
    //Edge *edge;
    float dscan = end.val[1] - start.val[1];
	Color tmp;
    
    //skip edge if completely outside image
    if ( start.val[1] > src->rows || end.val[1] < 0 )
    {
        return NULL;
    }
    
    // allocate and initialize edge structure
    Edge *e = (Edge *)malloc( sizeof(Edge) );
    e->x0 = start.val[0];
    e->y0 = start.val[1];
    e->x1 = end.val[0];
    e->y1 = end.val[1];
    
    e->yStart = (int) (e->y0 + 0.5);
    e->yEnd = (int) (e->y1 + 0.5) - 1;
    
    // Clip yEnd to the number of rows-1.
    if ( e->yEnd > src->rows )
    {
        e->yEnd = src->rows - 1;
    }
    
    // Calculate the slope, dxPerScan
    float dx = end.val[0] - start.val[0];
    e->dxPerScan = dx / dscan;
    
    //calculate zbuffer info
    e->z0 = 1.0 / start.val[Z];
    e->z1 = 1.0 / end.val[Z];
    e->zIntersect = 1.0 / start.val[Z]; //reciprocal for perspective projection
    float dz = 1.0/end.val[Z] - 1.0/start.val[Z];
    e->dzPerScan = dz / dscan;
	
	if(ds->shade == ShadeGouraud){
		color_multiply_float(&c1, (1.0/start.val[Z]), &e->cIntersect);
		color_copy(&e->c0, &e->cIntersect);
		
		Color endC;
		
		color_multiply_float(&c2, (1.0/end.val[Z]), &endC);
		color_copy(&e->c1, &endC);
		
		
		Color dc;
		
		dc.c[RED] = endC.c[RED] - e->cIntersect.c[RED]; 
		dc.c[GREEN] = endC.c[GREEN] - e->cIntersect.c[GREEN]; 
		dc.c[BLUE] = endC.c[BLUE] - e->cIntersect.c[BLUE]; 
		
		color_multiply_float(&dc, (1.0/dscan), &e->dcPerScan);
	}
    // Calculate xIntersect, adjusting for the fraction of the point in the pixel.
    // Scanlines go through the middle of pixels
    float frac = e->y0 - (int)( e->y0 ); //get fractional part of start y-coordinate
    e->xIntersect = e->x0; //start from unmodified start x
    if ( frac <= 0.5 )
    {
        //move up to scanline in same row
        e->xIntersect += (0.5 - frac) * e->dxPerScan;
        
        //move edge endpoint
        e->x0 = e->xIntersect;
        e->y0 -= (0.5 - frac);
        
        //adjust zbuffer info
        e->zIntersect += (0.5 - frac) * e->dzPerScan;
		if(ds->shade == ShadeGouraud){
			Color fracC;
			color_multiply_float(&e->dcPerScan, (0.5 - frac), &fracC);
			color_add(&e->cIntersect, &fracC, &tmp);
			color_copy(&e->cIntersect, &tmp);
		}
	}
    else
    {
        //move up to scanline above
        e->xIntersect += ((1.0 - frac) + 0.5) * e->dxPerScan;
        
        //move edge endpoint
        e->x0 = e->xIntersect;
        e->y0 -= (1.0 - frac) + 0.5;
        
        //adjust zbuffer info
        e->zIntersect += ((1.0 - frac) + 0.5) * e->dzPerScan;
		if(ds->shade == ShadeGouraud){		
			Color fracC;
			color_multiply_float(&e->dcPerScan, (1.0 - frac + 0.5), &fracC);
			color_add(&e->cIntersect, &fracC, &tmp);
			color_copy(&e->cIntersect, &tmp);
		}
    }
    // adjust if the edge starts above the image
    // move the intersections down to scanline zero
    if ( e->y0 < 0 )
    {
        float adjust = - e->y0;
        e->xIntersect += adjust * e->dxPerScan;
        e->y0 = 0;
        e->x0 = e->xIntersect;
        e->yStart = 0;
        
        //adjust zbuffer info
        e->zIntersect += adjust * e->dzPerScan;
		if(ds->shade == ShadeGouraud){
			Color fracC;
			color_multiply_float(&e->dcPerScan, adjust, &fracC);
			color_add(&e->cIntersect, &fracC, &tmp);
			color_copy(&e->cIntersect, &tmp);
		}
    }
    
    // check for really bad cases with steep slopes where xIntersect has gone beyond the end of the edge
	// because these are straight lines all we need to do is chceck the bounds of the line
    if ( e -> dxPerScan > 0 ){
		if(e -> xIntersect > e -> x1){
			return NULL; // we can do thsi because it checks to make sure its not NULL
		}
	}
	else{
		if (e -> xIntersect < e -> x0){
			return NULL;
		}
	}
    
    // return the newly created edge data structure
    return( e );
}


/*
	Returns a list of all the edges in the polygon in sorted order by
	smallest row.
 */
static LinkedList *setupEdgeList( Polygon *p, Image *src, DrawState *ds ) {
    LinkedList *edges = NULL;
    Point v1, v2;
	Color c1, c2;
    int i;
    
    // create a linked list
    edges = ll_new();
	
	//printf("colors are\n");
	//for(i = 0; i < 3; i++){
	//	color_print(&p->color[i], stdout);
	//}
	
    // walk around the polygon, starting with the last point
    v1 = p->vertex[p->numVertex-1];
	if(ds->shade == ShadeGouraud){
		color_copy(&c1, &p->color[p->numVertex-1]);
	}
	//printf("starting new polygon\n");
    for(i=0;i<p->numVertex;i++) {
        
        // the current point (i) is the end of the segment
        v2 = p->vertex[i];
		if(ds->shade == ShadeGouraud){
			color_copy(&c2, &p->color[i]);
        }
		//printf("c1 is\n");
		//color_print(&c1, stdout);
		//printf("c2 is\n");
		//color_print(&c2, stdout);

		
        // if it is not a horizontal line
        if( (int)(v1.val[1]+0.5) != (int)(v2.val[1]+0.5) ) {
            Edge *edge;
            
            // if the first coordinate is smaller (top edge)
            if( v1.val[1] < v2.val[1] )
                edge = makeEdgeRec( v1, v2, src, c1, c2, ds);
            else
                edge = makeEdgeRec( v2, v1, src, c2, c1, ds );
            
            // insert the edge into the list of edges if it's not null
            if( edge )
                ll_insert( edges, edge, compYStart );
        }
		if(ds->shade == ShadeGouraud){
			color_copy(&c1, &c2);
		}
        v1 = v2;
    }
    
    // check for empty edges (like nothing in the viewport)
    if( ll_empty( edges ) ) {
        ll_delete( edges, NULL );
        edges = NULL;
    }
    
    return(edges);
}

/*
	Draw one scanline of a polygon given the scanline, the active edges,
	a DrawState, the image, and some Lights (for Phong shading only).
 */
static void fillScan( int scan, LinkedList *active, Image *src, DrawState *ds ) {
    Edge *p1, *p2;
	//printf("in fillscan\n");
    //int i, f;
    
    // loop over the list
    p1 = ll_head( active );
	int i = 0;
    while(p1) {
        
		// the edges have to come in pairs, draw from one to the next
        p2 = ll_next( active );
        if( !p2 ) {
            printf("bad bad bad (your edges are not coming in pairs)\n");
            break;
        }
        
        // if the xIntersect values are the same, don't draw anything.
        // Just go to the next pair.
        if( p2->xIntersect == p1->xIntersect ) {
            p1 = ll_next( active );
            continue;
        }
        
        int startCol = (int) p1->xIntersect;
        // clip to the left side of the image
        if ( startCol < 0 )
        {
            startCol = 0;
        }
        
        int endCol = (int) p2->xIntersect;
        // clip to the right side of the image
        if ( endCol > src->cols)
        {
            endCol = src->cols;
        }
        
        //zbuffer stuff
        float curZ = p1->zIntersect;
        float dzPerCol = (p2->zIntersect - curZ) / (endCol - startCol);
		
		Color curC;
		Color tmp;
        Color dcPerCol;
		Color colorF;
		if(ds -> shade == ShadeGouraud){
			color_copy(&curC, &p1 -> cIntersect);
			
			Color dc;
	
			dc.c[RED] = p2 -> cIntersect.c[RED] - curC.c[RED]; 
			dc.c[GREEN] = p2 -> cIntersect.c[GREEN] - curC.c[GREEN]; 
			dc.c[BLUE] = p2 -> cIntersect.c[BLUE] - curC.c[BLUE]; 
			
			color_multiply_float(&dc, 1.0/(endCol - startCol), &dcPerCol);
			//printf("dc per col is %.2f %.2f %.2f\n", dcPerCol.c[0], dcPerCol.c[1], dcPerCol.c[2] );

/*			
			printf("intersect is \n");
			color_print(&p1->cIntersect, stdout);
			
			printf("final color is \n");
			color_print(&p2->cIntersect, stdout);
			
			printf("dc is \n");
			color_print(&dc, stdout);
			
			printf("dcPerCol is per %d cols \n", (endCol - startCol));
			color_print(&dcPerCol, stdout);
			*/
		}
        //color in the pixels
        for ( int col = startCol; col < endCol; col++ )
        {
            //check zbuffer
				if( curZ > src -> data[scan][col].z ){
					src -> data[scan][col].z = curZ; //update z-buffer
				}
               // printf("shading with type %d\n", ds->shade);
                switch(ds->shade){
                    case ShadeFrame: //should not occur here (handled in drawShade)
                        break;
                    case ShadeConstant:
						if( curZ > src -> data[scan][col].z ){
							image_setColor( src, col, scan, ds->color );
						}
						break;
                    case ShadeDepth:
                    {
                        float scaleZ = cos(1/curZ * M_PI/2); //uninvert z to value in [0, 1]
						if( curZ > src -> data[scan][col].z ){
							image_setColor(src, col, scan, ((Color){{ds->color.c[RED] * (scaleZ),
                                                                ds->color.c[GREEN] * (scaleZ),
                                                                ds->color.c[BLUE] * (scaleZ)}}));
						}
						break;
                    }
					case ShadeFlat:
					//	printf("shading flat with color\n");
						//printf("adding color %.2f %.2f %.2f\n", ds -> flatColor.c[0], ds -> flatColor.c[1], ds -> flatColor.c[2] );
						if( curZ > src -> data[scan][col].z ){
							image_setColor(src, col, scan, ds -> flatColor);
						}
						break;
					case ShadeGouraud:
					{
						color_multiply_float(&curC, 1.0/curZ, &tmp);
						//color_print(&tmp, stdout);
						color_copy_bounds(&colorF, &tmp);
						if( curZ > src -> data[scan][col].z ){
							image_setColor(src, col, scan, colorF);
						}
						color_add(&curC, &dcPerCol, &tmp);
						color_copy(&curC, &tmp);
						break;
					}
                }

            
            curZ += dzPerCol;
        }
        
        // move ahead to the next pair of edges
        p1 = ll_next( active );
    }
    
    return;
}

/*
 Process the edge list, assumes the edges list has at least one entry
 */
static int processEdgeList( LinkedList *edges, Image *src, DrawState *ds ) {
    LinkedList *active = NULL;
    LinkedList *tmplist = NULL;
    LinkedList *transfer = NULL;
    Edge *current;
    Edge *tedge;
    int scan = 0;
    
    active = ll_new( );
    tmplist = ll_new( );
    
    // get a pointer to the first item on the list and reset the current pointer
    current = ll_head( edges );
    
    // start at the first scanline and go until the active list is empty
    for(scan = current->yStart;scan < src->rows;scan++ ) {
        
        // grab all edges starting on this row
        while( current != NULL && current->yStart == scan ) {
            ll_insert( active, current, compXIntersect );
            current = ll_next( edges );
        }
        // current is either null, or the first edge to be handled on some future scanline
        
        if( ll_empty(active) ) {
            break;
        }
        
        // if there are active edges
        // fill out the scanline
        fillScan( scan, active, src, ds );
        Color tmp;
        // remove any ending edges and update the rest
        for( tedge = ll_pop( active ); tedge != NULL; tedge = ll_pop( active ) ) {
            
            // keep anything that's not ending
            if( tedge->yEnd > scan ) {
                float a = 1.0;
                
                // update the edge information with the dPerScan values
                tedge->xIntersect += tedge->dxPerScan;
                tedge->zIntersect += tedge->dzPerScan; //added for zbuffer
				if(ds -> shade == ShadeGouraud){
					color_add(&tedge->cIntersect, &tedge->dcPerScan, &tmp);
					color_copy(&tedge -> cIntersect, &tmp);
					//printf("updated Edge is %.2f %.2f %.2f\n", tedge -> cIntersect.c[0], tedge -> cIntersect.c[1], tedge -> cIntersect.c[2] );
				}
                
                // adjust in the case of partial overlap
                if( tedge->dxPerScan < 0.0 && tedge->xIntersect < tedge->x1 ) {
                    a = (tedge->xIntersect - tedge->x1) / tedge->dxPerScan;
                    tedge->xIntersect = tedge->x1;
					if(ds -> shade == ShadeGouraud){
						color_copy(&tedge -> cIntersect, &tedge -> c1);
					}
				}
                else if( tedge->dxPerScan > 0.0 && tedge->xIntersect > tedge->x1 ) {
                    a = (tedge->xIntersect - tedge->x1) / tedge->dxPerScan;
                    tedge->xIntersect = tedge->x1;
					if(ds -> shade == ShadeGouraud){
						color_copy(&tedge -> cIntersect, &tedge -> c1);
					}
				}
                
                ll_insert( tmplist, tedge, compXIntersect );
            }
        }
        
        transfer = active;
        active = tmplist;
        tmplist = transfer;
        
    }
    
    // get rid of the lists, but not the edge records
    ll_delete(active, NULL);
    ll_delete(tmplist, NULL);
    
    return(0);
}

/*
	Draws a filled polygon with the specified draw state into the image src.
 */
void polygon_drawFill(Polygon *p, Image *src, DrawState *ds ) {
    LinkedList *edges = NULL;
    
    // set up the edge list
    edges = setupEdgeList( p, src, ds );
    if( !edges )
        return;
    
    // process the edge list (should be able to take an arbitrary edge list)
    processEdgeList( edges, src, ds );
    
    // clean up
    ll_delete( edges, (void (*)(const void *))free );
    
    return;
}

/****************************************
 End Scanline Fill
 *****************************************/

void polygon_shade(Polygon *p, Lighting *lighting, DrawState *ds){
	Color *cList = malloc(sizeof(Color) * p->numVertex);
	
	//printf("setting polygon colors\n");
	for(int i = 0; i < p-> numVertex; i++){
		Vector viewing;
		
		viewing.val[X] = ds->viewer.val[X] - p->vertex[i].val[X];
		viewing.val[Y] = ds->viewer.val[Y] - p->vertex[i].val[Y];
		viewing.val[Z] = ds->viewer.val[Z] - p->vertex[i].val[Z];
		

		vector_normalize(&viewing);
		//printf("viewing angle is \n");
		//vector_print(&viewing, stdout);
		
		
		Point pt;
		point_set(&pt, p->vertex[i].val[X], p->vertex[i].val[Y], p->vertex[i].val[Z], p->vertex[i].val[H]);
		
		//printf("point is\n");
		//point_print(&pt, stdout);
		
		lighting_shading( lighting, &p->normal[i], &viewing, &pt, &ds->body, &ds->surface, ds->surfaceCoeff, p->oneSided, &cList[i]);
		//printf("color is : %.2f %.2f %.2f\n", cList[i].c[0], cList[i].c[1], cList[i].c[2] );
		//printf("normal is\n");
		//vector_print(&p->normal[i], stdout);
		//printf("color is \n");
		//color_print(&cList[i], stdout);
	}
	//printf("setting colors inside shade\n");
	
	polygon_setColors(p, p->numVertex, &cList[0]);
	free(cList);
}
 
void polygon_drawFillB(Polygon *p, Image *src, Color c){
	if( p -> numVertex != 3){
		printf("you need to draw triangles\n");
		return;
	}
	int maxX = MAX(MAX(p -> vertex[0].val[X], p -> vertex[1].val[X]), p -> vertex[2].val[X]);
	int maxY = MAX(MAX(p -> vertex[0].val[Y], p -> vertex[1].val[Y]), p -> vertex[2].val[Y]);
	int minX = MIN(MIN(p -> vertex[0].val[X], p -> vertex[1].val[X]), p -> vertex[2].val[X]);
	int minY = MIN(MIN(p -> vertex[0].val[Y], p -> vertex[1].val[Y]), p -> vertex[2].val[Y]);
	
	int x;
	int y;
	float beta;
	float gamma;
	float alpha;
	for(y = minY; y < maxY; y++){
		for(x = minX; x < maxX; x++){
			beta = ((p -> vertex[2].val[Y] - p  -> vertex[0].val[Y]) * (x + 0.5) // tried adding 0.5 to these but it looked wrong
					+ (p -> vertex[0].val[X] - p ->vertex[2].val[X]) * (y + 0.5)  
					+ p -> vertex[2].val[X] * p -> vertex[0].val[Y] 
					- p -> vertex[0].val[X] * p -> vertex[2].val[Y]);
			
			beta /= ((p -> vertex[2].val[Y] - p  -> vertex[0].val[Y]) * p -> vertex[1].val[X] 
					+ (p -> vertex[0].val[X] - p ->vertex[2].val[X]) * p -> vertex[1].val[Y] 
					+ p -> vertex[2].val[X] * p -> vertex[0].val[Y] 
					- p -> vertex[0].val[X] * p -> vertex[2].val[Y]);
					
			gamma = ((p -> vertex[0].val[Y] - p  -> vertex[1].val[Y]) * (x + 0.5)
					+ (p -> vertex[1].val[X] - p ->vertex[0].val[X]) * (y + 0.5)  
					+ p -> vertex[0].val[X] * p -> vertex[1].val[Y] 
					- p -> vertex[1].val[X] * p -> vertex[0].val[Y]);
			
			gamma /= ((p -> vertex[0].val[Y] - p  -> vertex[1].val[Y]) * p -> vertex[2].val[X] 
					+ (p -> vertex[1].val[X] - p ->vertex[0].val[X]) * p -> vertex[2].val[Y] 
					+ p -> vertex[0].val[X] * p -> vertex[1].val[Y] 
					- p -> vertex[1].val[X] * p -> vertex[0].val[Y]);
			alpha = 1 - beta - gamma;
			
			if((alpha >= 0.0 && alpha < 1.0) && (beta >= 0.0 && beta < 1.0) && (gamma >= 0.0 && gamma < 1.0)){
				image_setColor(src, x, y, c);
			}
		}
	}	
 }
 
void polygon_drawFillBGrad(Polygon *p, Image *src, Color cA, Color cB, Color cC){
	printf("why the fuck am I in here\n");
	if( p -> numVertex != 3){
		printf("you need to draw triangles\n");
		return;
	}
	int maxX = MAX(MAX(p -> vertex[0].val[X], p -> vertex[1].val[X]), p -> vertex[2].val[X]);
	int maxY = MAX(MAX(p -> vertex[0].val[Y], p -> vertex[1].val[Y]), p -> vertex[2].val[Y]);
	int minX = MIN(MIN(p -> vertex[0].val[X], p -> vertex[1].val[X]), p -> vertex[2].val[X]);
	int minY = MIN(MIN(p -> vertex[0].val[Y], p -> vertex[1].val[Y]), p -> vertex[2].val[Y]);
	
	int x;
	int y;
	float beta;
	float gamma;
	float alpha;
	for(y = minY; y < maxY; y++){
		for(x = minX; x < maxX; x++){
			beta = ((p -> vertex[2].val[Y] - p  -> vertex[0].val[Y]) * (x + 0.5)
					+ (p -> vertex[0].val[X] - p ->vertex[2].val[X]) * (y + 0.5) 
					+ p -> vertex[2].val[X] * p -> vertex[0].val[Y] 
					- p -> vertex[0].val[X] * p -> vertex[2].val[Y]);
			
			beta /= ((p -> vertex[2].val[Y] - p  -> vertex[0].val[Y]) * p -> vertex[1].val[X] 
					+ (p -> vertex[0].val[X] - p ->vertex[2].val[X]) * p -> vertex[1].val[Y] 
					+ p -> vertex[2].val[X] * p -> vertex[0].val[Y] 
					- p -> vertex[0].val[X] * p -> vertex[2].val[Y]);
					
			gamma = ((p -> vertex[0].val[Y] - p  -> vertex[1].val[Y]) * (x + 0.5)
					+ (p -> vertex[1].val[X] - p ->vertex[0].val[X]) * (y + 0.5) 
					+ p -> vertex[0].val[X] * p -> vertex[1].val[Y] 
					- p -> vertex[1].val[X] * p -> vertex[0].val[Y]);
			
			gamma /= ((p -> vertex[0].val[Y] - p  -> vertex[1].val[Y]) * p -> vertex[2].val[X] 
					+ (p -> vertex[1].val[X] - p ->vertex[0].val[X]) * p -> vertex[2].val[Y] 
					+ p -> vertex[0].val[X] * p -> vertex[1].val[Y] 
					- p -> vertex[1].val[X] * p -> vertex[0].val[Y]);
			alpha = 1 - beta - gamma;
			
			if((alpha >= 0.0 && alpha < 1.0) && (beta >= 0.0 && beta < 1.0) && (gamma >= 0.0 && gamma < 1.0)){
				float r = cA.c[RED] * alpha + cB.c[RED] * beta + cC.c[RED] * gamma;
				float g = cA.c[GREEN] * alpha + cB.c[GREEN] * beta + cC.c[GREEN] * gamma;
				float b = cA.c[BLUE] * alpha + cB.c[BLUE] * beta + cC.c[BLUE] * gamma;
				
				SET_RGB(src -> data[y][x], r, g, b);
			}
		}
	}
	
 }
  
void polygon_drawFillBGradSave(Polygon *p, Image *src, Color cA, Color cB, Color cC){
	if( p -> numVertex != 3){
		printf("you need to draw triangles\n");
		return;
	}
	int maxX = MAX(MAX(p -> vertex[0].val[X], p -> vertex[1].val[X]), p -> vertex[2].val[X]);
	int maxY = MAX(MAX(p -> vertex[0].val[Y], p -> vertex[1].val[Y]), p -> vertex[2].val[Y]);
	int minX = MIN(MIN(p -> vertex[0].val[X], p -> vertex[1].val[X]), p -> vertex[2].val[X]);
	int minY = MIN(MIN(p -> vertex[0].val[Y], p -> vertex[1].val[Y]), p -> vertex[2].val[Y]);
	
	int x;
	int y;
	float beta;
	float gamma;
	float alpha;
	int it =0;
	Color color;
	for(y = minY; y < maxY; y++){
		for(x = minX; x < maxX; x++){
			beta = ((p -> vertex[2].val[Y] - p  -> vertex[0].val[Y]) * (x + 0.5)
					+ (p -> vertex[0].val[X] - p ->vertex[2].val[X]) * (y + 0.5) 
					+ p -> vertex[2].val[X] * p -> vertex[0].val[Y] 
					- p -> vertex[0].val[X] * p -> vertex[2].val[Y]);
			
			beta /= ((p -> vertex[2].val[Y] - p  -> vertex[0].val[Y]) * p -> vertex[1].val[X] 
					+ (p -> vertex[0].val[X] - p ->vertex[2].val[X]) * p -> vertex[1].val[Y] 
					+ p -> vertex[2].val[X] * p -> vertex[0].val[Y] 
					- p -> vertex[0].val[X] * p -> vertex[2].val[Y]);
					
			gamma = ((p -> vertex[0].val[Y] - p  -> vertex[1].val[Y]) * (x + 0.5)
					+ (p -> vertex[1].val[X] - p ->vertex[0].val[X]) * (y + 0.5) 
					+ p -> vertex[0].val[X] * p -> vertex[1].val[Y] 
					- p -> vertex[1].val[X] * p -> vertex[0].val[Y]);
			
			gamma /= ((p -> vertex[0].val[Y] - p  -> vertex[1].val[Y]) * p -> vertex[2].val[X] 
					+ (p -> vertex[1].val[X] - p ->vertex[0].val[X]) * p -> vertex[2].val[Y] 
					+ p -> vertex[0].val[X] * p -> vertex[1].val[Y] 
					- p -> vertex[1].val[X] * p -> vertex[0].val[Y]);
			alpha = 1 - beta - gamma;
			
			if((alpha >= 0.0 && alpha < 1.0) && (beta >= 0.0 && beta < 1.0) && (gamma >= 0.0 && gamma < 1.0)){
				color.c[RED] = cA.c[RED] * alpha + cB.c[RED] * beta + cC.c[RED] * gamma;
				color.c[GREEN] = cA.c[GREEN] * alpha + cB.c[GREEN] * beta + cC.c[GREEN] * gamma;
				color.c[BLUE] = cA.c[BLUE] * alpha + cB.c[BLUE] * beta + cC.c[BLUE] * gamma;
				
				image_setColorSave(src, x, y, color, it);
				it++;
			}
		}
	}
	
 }
 
void polygon_drawShade(Polygon *p, Image *src, DrawState *ds, Lighting *light){
											  
	//printf("drawstate is %d\n", ds->shade);
	switch(ds -> shade){
		case ShadeFrame:
			polygon_draw(p, src, ds->color);
		break;
		case ShadeConstant: //both constant & depth handled by same function
        case ShadeDepth:
			polygon_drawFill(p, src, ds);
		break;
		case ShadeFlat:
			polygon_drawFill(p, src, ds);
		break;
		case ShadeGouraud:
			polygon_drawFill(p, src, ds);
		break;
	}
}
												  

