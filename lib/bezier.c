/* bezier.c
 * Defines functions for using Bezier curves and surfaces
 *
 * Jason Gurevitch and Melody Mao
 * CS351 Spring 2018
 * Project 7
 */

#include "bezier.h"
#include "color.h"

/*** BezierCurve functions ***/

/* sets the zbuffer flag to 1 and the curve points to the X-axis between 0 and 1
 */
void bezierCurve_init( BezierCurve *b )
{
    //invalid input
    if ( b == NULL )
    {
        return;
    }
    
    b -> zBuffer = 1;
    point_set3D( &(b->pt[0]), 0.0, 0.0, 0.0 );
    point_set3D( &(b->pt[1]), 0.3, 0.0, 0.0 );
    point_set3D( &(b->pt[2]), 0.7, 0.0, 0.0 );
    point_set3D( &(b->pt[3]), 1.0, 0.0, 0.0 );
}

/* sets the control points of the BezierCurve to the four points in the vlist array
 */
void bezierCurve_set( BezierCurve *b, Point *vlist )
{
    //invalid input
    if ( b == NULL || vlist == NULL )
    {
        return;
    }
    
    //copy over points
    for ( int i = 0; i < 4; i++ )
    {
        point_copy( &(b->pt[i]), vlist + i );
    }
}

//sets the zbuffer to the flag
void bezierCurve_zBuffer( BezierCurve *p, int flag )
{
    p -> zBuffer = flag;
}

/* draws the Bezier curve, given in screen coordinates, into the image using the given color
 * assumes the control points are in screen coordinates
 */
void bezierCurve_draw( BezierCurve *b, Image *src, Color c )
{
    //invalid input
    if ( b == NULL || src == NULL )
    {
        return;
    }
    
    //find max dimension of bounding box for control points
    double diffx1 = MAX( fabs( b->pt[0].val[X] - b->pt[1].val[X] ), fabs( b->pt[2].val[X] - b->pt[3].val[X] ) );
    double diffx2 = MAX( fabs( b->pt[0].val[X] - b->pt[2].val[X] ), fabs( b->pt[1].val[X] - b->pt[3].val[X] ) );
    double diffx3 = MAX( fabs( b->pt[0].val[X] - b->pt[3].val[X] ), fabs( b->pt[1].val[X] - b->pt[2].val[X] ) );
    double diffx = MAX( diffx1, diffx2 );
    diffx = MAX( diffx, diffx3 );
    double diffy1 = MAX( fabs( b->pt[0].val[Y] - b->pt[1].val[Y] ), fabs( b->pt[2].val[Y] - b->pt[3].val[Y] ) );
    double diffy2 = MAX( fabs( b->pt[0].val[Y] - b->pt[2].val[Y] ), fabs( b->pt[1].val[Y] - b->pt[3].val[Y] ) );
    double diffy3 = MAX( fabs( b->pt[0].val[Y] - b->pt[3].val[Y] ), fabs( b->pt[1].val[Y] - b->pt[2].val[Y] ) );
    double diffy = MAX( diffy1, diffy2 );
    diffy = MAX( diffy, diffy3 );
    double diffz1 = MAX( fabs( b->pt[0].val[Z] - b->pt[1].val[Z] ), fabs( b->pt[2].val[Z] - b->pt[3].val[Z] ) );
    double diffz2 = MAX( fabs( b->pt[0].val[Z] - b->pt[2].val[Z] ), fabs( b->pt[1].val[Z] - b->pt[3].val[Z] ) );
    double diffz3 = MAX( fabs( b->pt[0].val[Z] - b->pt[3].val[Z] ), fabs( b->pt[1].val[Z] - b->pt[2].val[Z] ) );
    double diffz = MAX( diffz1, diffz2 );
    diffz = MAX( diffz, diffz3 );
    
    //if the curve is small enough, just draw
    if ( diffx < 10 && diffy < 10 && diffz < 10 )
    {
        Line l;
        line_set( &l, b->pt[0], b->pt[1] );
        line_draw( &l, src, c );
        line_set( &l, b->pt[1], b->pt[2] );
        line_draw( &l, src, c );
        line_set( &l, b->pt[2], b->pt[3] );
        line_draw( &l, src, c );
        
        return;
    }
    
    //otherwise split curve, using the de Casteljau algorithm
    //first level of subdivision
    double p10x = ( b->pt[0].val[X] + b->pt[1].val[X] ) / 2;
    double p10y = ( b->pt[0].val[Y] + b->pt[1].val[Y] ) / 2;
    double p10z = ( b->pt[0].val[Z] + b->pt[1].val[Z] ) / 2;
    double p11x = ( b->pt[1].val[X] + b->pt[2].val[X] ) / 2;
    double p11y = ( b->pt[1].val[Y] + b->pt[2].val[Y] ) / 2;
    double p11z = ( b->pt[1].val[Z] + b->pt[2].val[Z] ) / 2;
    double p12x = ( b->pt[2].val[X] + b->pt[3].val[X] ) / 2;
    double p12y = ( b->pt[2].val[Y] + b->pt[3].val[Y] ) / 2;
    double p12z = ( b->pt[2].val[Z] + b->pt[3].val[Z] ) / 2;
    
    //second level of subdivision
    double p20x = ( p10x + p11x ) / 2;
    double p20y = ( p10y + p11y ) / 2;
    double p20z = ( p10z + p11z ) / 2;
    double p21x = ( p11x + p12x ) / 2;
    double p21y = ( p11y + p12y ) / 2;
    double p21z = ( p11z + p12z ) / 2;
    
    //third level of subdivision
    double p30x = ( p20x + p21x ) / 2;
    double p30y = ( p20y + p21y ) / 2;
    double p30z = ( p20z + p21z ) / 2;
    
    //make sub-curves and draw them
    Point *pts1 = (Point *)malloc( sizeof( Point ) * 4 );
    point_set3D( pts1, b->pt[0].val[X], b->pt[0].val[Y], b->pt[0].val[Z] );
    point_set3D( pts1 + 1, p10x, p10y, p10z );
    point_set3D( pts1 + 2, p20x, p20y, p20z );
    point_set3D( pts1 + 3, p30x, p30y, p30z );
    BezierCurve *b1 = (BezierCurve *)malloc( sizeof( BezierCurve ) );
    bezierCurve_set( b1, pts1 );
    bezierCurve_draw( b1, src, c );
    
    Point *pts2 = (Point *)malloc( sizeof( Point ) * 4 );
    point_set3D( pts2, p30x, p30y, p30z );
    point_set3D( pts2 + 1, p21x, p21y, p21z );
    point_set3D( pts2 + 2, p12x, p12y, p12z );
    point_set3D( pts2 + 3, b->pt[3].val[X], b->pt[3].val[Y], b->pt[3].val[Z] );
    BezierCurve *b2 = (BezierCurve *)malloc( sizeof( BezierCurve ) );
    bezierCurve_set( b2, pts2 );
    bezierCurve_draw( b2, src, c );
    
    free( pts1 );
    free( pts2 );
    free( b1 );
    free( b2 );
}

/*** BezierSurface functions ***/

/* sets the zbuffer flag to 1 and the surface to the X-Z plane between (0, 0) and (1, 1)
 */
void bezierSurface_init( BezierSurface *b )
{
    //invalid input
    if ( b == NULL )
    {
        return;
    }
    
    b -> zBuffer = 1;
    point_set3D( &(b->pt[0][0]), 0.0, 0.0, 0.0 );
    point_set3D( &(b->pt[0][1]), 0.3, 0.0, 0.0 );
    point_set3D( &(b->pt[0][2]), 0.7, 0.0, 0.0 );
    point_set3D( &(b->pt[0][3]), 1.0, 0.0, 0.0 );
    point_set3D( &(b->pt[1][0]), 0.0, 0.3, 0.0 );
    point_set3D( &(b->pt[1][1]), 0.3, 0.3, 0.0 );
    point_set3D( &(b->pt[1][2]), 0.7, 0.3, 0.0 );
    point_set3D( &(b->pt[1][3]), 1.0, 0.3, 0.0 );
    point_set3D( &(b->pt[2][0]), 0.0, 0.7, 0.0 );
    point_set3D( &(b->pt[2][1]), 0.3, 0.7, 0.0 );
    point_set3D( &(b->pt[2][2]), 0.7, 0.7, 0.0 );
    point_set3D( &(b->pt[2][3]), 1.0, 0.7, 0.0 );
    point_set3D( &(b->pt[3][0]), 0.0, 1.0, 0.0 );
    point_set3D( &(b->pt[3][1]), 0.3, 1.0, 0.0 );
    point_set3D( &(b->pt[3][2]), 0.7, 1.0, 0.0 );
    point_set3D( &(b->pt[3][3]), 1.0, 1.0, 0.0 );
}

/* sets the control points of the BezierSurface to the 16 points in the vlist array
 */
void bezierSurface_set( BezierSurface *b, Point *vlist )
{
    //invalid input
    if ( b == NULL || vlist == NULL )
    {
        return;
    }
    
    //copy over points
    for ( int i = 0; i < 4; i++ )
    {
        for( int j = 0; j < 4; j++ )
        {
            point_copy( &(b->pt[i][j]), vlist + i*4+j );
        }
    }
}

//sets the zbuffer to the flag
void bezierSurface_zBuffer( BezierSurface *p, int flag )
{
    p -> zBuffer = flag;
}


/*
* helper bezier curve subdivide function
* takes in list of 4 points and outputs into list of 8 points
*/
void bezier_subdivide(Point *bezIn, Point *bezOut){
	double p10x = ( bezIn[0].val[X] + bezIn[1].val[X] ) / 2;
	double p10y = ( bezIn[0].val[Y] + bezIn[1].val[Y] ) / 2;
	double p10z = ( bezIn[0].val[Z] + bezIn[1].val[Z] ) / 2;
	double p11x = ( bezIn[1].val[X] + bezIn[2].val[X] ) / 2;
	double p11y = ( bezIn[1].val[Y] + bezIn[2].val[Y] ) / 2;
	double p11z = ( bezIn[1].val[Z] + bezIn[2].val[Z] ) / 2;
	double p12x = ( bezIn[2].val[X] + bezIn[3].val[X] ) / 2;
	double p12y = ( bezIn[2].val[Y] + bezIn[3].val[Y] ) / 2;
	double p12z = ( bezIn[2].val[Z] + bezIn[3].val[Z] ) / 2;
	
	
	//second level of subdivision
	double p20x = ( p10x + p11x ) / 2;
	double p20y = ( p10y + p11y ) / 2;
	double p20z = ( p10z + p11z ) / 2;
	double p21x = ( p11x + p12x ) / 2;
	double p21y = ( p11y + p12y ) / 2;
	double p21z = ( p11z + p12z ) / 2;

	//third level of subdivision
	double p30x = ( p20x + p21x ) / 2;
	double p30y = ( p20y + p21y ) / 2;
	double p30z = ( p20z + p21z ) / 2;
	
	point_set3D( &bezOut[0], bezIn[0].val[X], bezIn[0].val[Y], bezIn[0].val[Z] );
	point_set3D( &bezOut[1], p10x, p10y, p10z );
	point_set3D( &bezOut[2], p20x, p20y, p20z );
	point_set3D( &bezOut[3], p30x, p30y, p30z );
	
	point_set3D( &bezOut[4], p30x, p30y, p30z );
	point_set3D( &bezOut[5], p21x, p21y, p21z );
	point_set3D( &bezOut[6], p12x, p12y, p12z );
	point_set3D( &bezOut[7], bezIn[3].val[X], bezIn[3].val[Y], bezIn[3].val[Z] );		
}

void bezier_surfaceNormal(Point *contu, Point *contv, float u, float v, Vector *result){
	Matrix Mbez;
	Matrix MbezT;
	Vector resU;
	Vector tmpResU;
	Vector resV;
	Vector tmpResV;
	
	u = MAX(0.10, MIN(0.90, u));
	v = MAX(0.10, MIN(0.90, v));
	
	matrix_clear(&Mbez);
	Mbez.m[0][0] = -1;
	Mbez.m[0][1] = 3;
	Mbez.m[0][2] = 3;
	Mbez.m[0][3] = 1;
	
	Mbez.m[1][0] = 3;
	Mbez.m[1][1] = 6;
	Mbez.m[1][2] = 3;
	
	Mbez.m[2][0] = -3;
	Mbez.m[2][1] = 3;
	
	Mbez.m[3][0] = 1;
	
	matrix_copy(&MbezT, &Mbez);
	matrix_transpose(&MbezT);
	
	Matrix ptsu;
	
	Matrix ptsv;
	
	int i;
	int j;
	for(i = 0; i <4; i++){
		for(j = 0; j < 4; j++){
			matrix_set(&ptsu, i, j, contu[i].val[j]);
			matrix_set(&ptsv, i, j, contv[i].val[j]);
		}
	}
	
	vector_set(&resU, v*v*v, v*v, v);
	resU.val[H] = 1;
	matrix_xformVector(&MbezT, &resU, &tmpResU);
	matrix_xformVector(&ptsu, &tmpResU, &resU);
	matrix_xformVector(&Mbez, &resU, &tmpResU);
	
	
	resU.val[X] = 3 * (u * u) * tmpResU.val[X];
	resU.val[Y] = 2 * u * tmpResU.val[Y];
	resU.val[Z] = tmpResU.val[Z];
	
	//printf("final u is \n");
	//vector_print(&resU, stdout);
	
	vector_set(&resV, (3*(v*v)), 2*v, 1);
	resV.val[H] = 0;
	matrix_xformVector(&MbezT, &resV, &tmpResV);
	matrix_xformVector(&ptsv, &tmpResV, &resV);
	matrix_xformVector(&Mbez, &resV, &tmpResV);
	
	resV.val[X] = u * u * u * tmpResV.val[X];
	resV.val[Y] = u * u * tmpResV.val[Y];
	resV.val[Z] = u * tmpResV.val[Z];
	
	vector_cross(&resU, &resV, result);
}