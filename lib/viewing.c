/* viewing.c
 * Defines functions for transforming points in views defined by View2D and View3D structs
 *
 * Melody Mao and Jason Gurevitch
 * CS351 Spring 2018
 * Project 5
 */

#include "viewing.h"
#include <math.h>

/* Sets vtm to be the view transformation defined by the 2DView structure
 */
void matrix_setView2D( Matrix *vtm, View2D *view )
{
    //invalid input
    if ( vtm == NULL || view == NULL )
    {
        return;
    }
    
    //init as identity
    matrix_identity( vtm );
    
    //translate anchor to origin
    matrix_translate2D( vtm, - view->vrp.val[X], - view->vrp.val[Y] );
    
    //rotate view using x-axis orientation
    matrix_rotateZ( vtm, view->x.val[0], view->x.val[1] );
    
    //scale to image size and flip y-axis
    double scaleX = view->screenx / view->dx;
    double dy = view->dx * view->screeny / view->screenx;
    double scaleY = - view->screeny / dy;
    matrix_scale2D( vtm, scaleX, scaleY );
    
    //put anchor back
    double tX = ((double) view->screenx) / 2;
    double tY = ((double) view->screeny) / 2;
    matrix_translate2D( vtm, tX, tY );
}

/* Sets vtm to be the perspective view transformation defined by the 3DView structure
 */
void matrix_setView3D( Matrix *vtm, View3D *view )
{
    //invalid input
    if ( vtm == NULL || view == NULL )
    {
        return;
    }
    
    //init as identity
    matrix_identity( vtm );
    
    //translate VRP to origin
    matrix_translate( vtm, - view->vrp.val[X], - view->vrp.val[Y], - view->vrp.val[Z] );
    
    //printf( "after VRP translation\n" );
    //matrix_print( vtm, stdout );
    
    Vector u, vup2, vpn2;
    vector_copy( &vpn2, &(view->vpn) );
    vector_cross( &(view->vup), &vpn2, &u );
    vector_cross( &vpn2, &u, &vup2 ); //get corrected VUP
    //normalize axis vectors
    vector_normalize( &u );
    vector_normalize( &vup2 );
    vector_normalize( &vpn2 );
    //orient axes
    matrix_rotateXYZ( vtm, &u, &vup2, &vpn2 );
    
    //printf( "after Rxyz\n" );
    //matrix_print( vtm, stdout );
    
    //translate COP (center of projection) to origin
    matrix_translate( vtm, 0, 0, view->d );
    
    //printf( "after translating COP to origin\n" );
    //matrix_print( vtm, stdout );
    
    //adjust VRP
    Point vrpAdjusted; // updated for CVV
    matrix_xformPoint( vtm, &(view->vrp), &vrpAdjusted );
    
    //squish to canonical view volume
    double bprime = view->d + view->b;
    double scaleXcvv = 2 * view->d / ( bprime * view->du );
    double scaleYcvv = 2 * view->d / ( bprime * view->dv );
    double scaleZcvv = 1.0 / bprime;
    matrix_scale( vtm, scaleXcvv, scaleYcvv, scaleZcvv );
    
    //printf( "after scaling to CVV\n" );
    //matrix_print( vtm, stdout );
    
    //project onto view plane
    double dAdjusted = vrpAdjusted.val[Z] / bprime;
    matrix_perspective( vtm, dAdjusted );
    
    //printf( "after perspective\n" );
    //matrix_print( vtm, stdout );
    
    //scale to image size, flip axes
    double scaleX = - ((double) view->screenx) / ( 2 * dAdjusted );
    double scaleY = - ((double) view->screeny) / ( 2 * dAdjusted );
    matrix_scale( vtm, scaleX, scaleY, 1 );
    
    //printf( "after scale to image coords\n" );
    //matrix_print( vtm, stdout );
    
    //translate origin to center
    double tX = ((double) view->screenx) / 2;
    double tY = ((double) view->screeny) / 2;
    matrix_translate( vtm, tX, tY, 0 );
    
    //printf( "after final translation\n" );
    //matrix_print( vtm, stdout );
}

