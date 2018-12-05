/*
	Bruce A. Maxwell
	CS 351 Computer Graphics
	Spring 2018

  This creates an X Window and shows how to put an image into the
  Window.  It allows for real time animation with your graphics
  package.

	You must have X-windows installed on your system.

	Compile command:
	
	Code edited by Jason Gurevitch Spring 2018 to add rotation and translation based on keystrokes

	gcc -o xdisp -I../include -I/opt/local/include xdisp.c -L../lib -L/opt/local/lib -limageIO -lX11 -lm
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "graphics.h"

#define LCL_DEFAULT_ROWS 480
#define LCL_DEFAULT_COLS 640

/*
  Global Variables determining window and display
*/
Display *curDisplay;
Window   curWindow;
GC       curGC; /* graphics context */
XImage  **ximp = NULL;
char    *ximd = NULL;
int     ximrows = 0;
int     ximcols = 0;
int     defdepth;
int     bytesPerPixel;
Pixmap  gblPixmap;

/*
  Global Variables for userland stuff
*/
int waitingForImage = 0;

int initWindow(void);
int initWindow(void) {
   int defScreen;
   XSetWindowAttributes wAttr;
   XGCValues gcValues;
   char buffer[64] = "Live Rendering";
   XTextProperty xtp = {(unsigned char*)buffer, 125, 8, strlen(buffer)};
   
   /*
    * connect to the X server.  uses the server specified in the
    * DISPLAY environment variable
    */
   curDisplay = XOpenDisplay((char *) NULL);
   if ((Display *) NULL == curDisplay) {
      fprintf(stderr, "dispipc:  could not open display.\n");
      exit(-1);
   }
   
   /*
    * begin to create a window
    */
   printf("creating window\n");
   defdepth = DefaultDepth(curDisplay,0);
   bytesPerPixel = defdepth/8;
   bytesPerPixel = bytesPerPixel == 3 ? 4 : bytesPerPixel;
   printf("default depth %d\n", defdepth);

   defScreen = DefaultScreen(curDisplay);
   printf("Default screen %d\n", defScreen);

   curWindow = XCreateWindow(curDisplay, 
			     DefaultRootWindow(curDisplay),
			     10, 
			     10, 
			     LCL_DEFAULT_COLS, 
			     LCL_DEFAULT_ROWS, 
			     0, 
			     defdepth, 
			     InputOutput, 
			     DefaultVisual(curDisplay, defScreen),
			     0, 
			     &wAttr);

   printf("Using depth %d\n", DefaultDepth(curDisplay, 0));

   /*
    * request mouse button and keypress events
    */
   wAttr.event_mask = ButtonPressMask | KeyPressMask | ExposureMask;
   XChangeWindowAttributes(curDisplay, 
			   curWindow, 
			   CWEventMask, 
			   &wAttr);

   /*
    * force it to appear on the screen
    */
   XSetWMName(curDisplay, curWindow, &xtp);
   XMapWindow(curDisplay, curWindow);

   /*
    * create a graphics context.  this stores a drawing state; stuff like
    * current color and line width.  this gc is for drawing into our
    * window. 
    */
   printf("creating graphics context\n");
   curGC = XCreateGC(curDisplay, curWindow, 0, &gcValues);

   printf("setting color map to default\n");
   XSetWindowColormap(curDisplay, 
		      curWindow,
		      DefaultColormapOfScreen(DefaultScreenOfDisplay(curDisplay)));

   /*
    * create an XImage for holding image data
    */



   printf("exiting init window\n");
   return(bytesPerPixel);
}

void refreshWindow(void);
void refreshWindow(void)
{
  // refresh the window here
  if( *ximp != NULL )
    XPutImage(curDisplay, curWindow, curGC, *ximp, 0, 0, 0, 0, ximcols, ximrows);

  return;
}

void closeWindow(void);
void closeWindow(void)
{
  //  XDestroyImage( *ximp );
  XCloseDisplay(curDisplay);

  if(ximd)
    free(ximd);

  if(ximp)
    free(ximp);
   
   return;
}



/*
	Moves the contents of an Image into the XWindow to be displayed
*/
void image2window(Image *src, XImage **lclximp);
void image2window(Image *src, XImage **lclximp) {
  long i, j;

  if( (src->rows != ximrows || src->cols != ximcols) && *lclximp != NULL ) {
    printf(" Freeing ximp\n");
    free( *lclximp );
    *lclximp = NULL;
  }

  if( *lclximp == NULL ) {
    int defdepth, bytesPerPixel, bitmap_pad;
    int defScreen;

    printf("Allocating XImage memory\n");

    defdepth = DefaultDepth(curDisplay,0); // lies
    defScreen = DefaultScreen(curDisplay);

    printf("%d %d\n", defdepth, defScreen);

    bytesPerPixel = defdepth/8;
    bytesPerPixel = bytesPerPixel == 3 ? 4 : bytesPerPixel;
    bitmap_pad = defdepth == 24 ? 32 : defdepth;
    printf("  default depth %d\n", defdepth);

    ximd = (char *)malloc(bytesPerPixel * src->cols * src->rows);
    memset( ximd, 0, bytesPerPixel * src->cols * src->rows );

    printf("  resizing window\n");
    XResizeWindow(curDisplay, curWindow, src->cols, src->rows);

    printf("  xcreateimage\n");
    *lclximp = XCreateImage(curDisplay, 
			    DefaultVisual(curDisplay, defScreen), 
			    defdepth, 
			    ZPixmap, 
			    0, 
			    (char *)ximd, 
			    src->cols,
			    src->rows,
			    bitmap_pad, 
			    0);

    ximrows = src->rows;
    ximcols = src->cols;
  }


	// copy the Image into the lclximp (which is using the memory in ximd)
	for(i=0;i<src->rows;i++) {
		for(j=0;j<src->cols;j++) {
			switch( bytesPerPixel ) {
			case 1:
			case 2:
				// not handled 
				break;
			case 4:
				// ordering is BGRA
				ximd[4*(i*src->cols + j) + 2] = (unsigned char)( 255 * image_getc(src, i, j, 0));
				ximd[4*(i*src->cols + j) + 1] = (unsigned char)( 255 * image_getc(src, i, j, 1));
				ximd[4*(i*src->cols + j) + 0] = (unsigned char)( 255 * image_getc(src, i, j, 2));
				break;
			default:
				break;
			}
		}
	}

  XPutImage(curDisplay, curWindow, curGC, *lclximp, 0, 0, 0, 0, ximcols, ximrows);

  return;
}


void rotateVRC(View3D *view, Vector *u, float angleVUP, float angleU);
void rotateVRC(View3D *view, Vector *u, float angleVUP, float angleU){
	Matrix t1;
	matrix_identity(&t1);
	
	Vector vpnN;
	Vector vupN;
	
	vector_copy(&vpnN, &view -> vpn);
	//vector_normalize(&vpnN);
	
	vector_cross(&vpnN, u, &vupN);
	//vector_normalize(&vupN);
	
	matrix_translate(&t1, -(view->vrp.val[X] + vpnN.val[X] * 0.5), 
							-(view->vrp.val[Y] + vpnN.val[Y]* 0.5), 
							-(view->vrp.val[Z] + vpnN.val[Z]* 0.5));
	
	//printf("t1 is\n");
	//matrix_print(&t1, stdout);
	
	Matrix rxyz;
	matrix_identity(&rxyz);
	matrix_rotateXYZ(&rxyz, u, &vupN, &vpnN);
	
	Matrix rxyzt;
	matrix_identity(&rxyzt);
	matrix_rotateXYZ(&rxyzt, u, &vupN, &vpnN);
	matrix_transpose(&rxyzt);
	
	Matrix test;
	matrix_identity(&test);
	matrix_multiply(&rxyzt, &rxyz, &test);
	
	//printf("rxyz is \n");
	//matrix_print(&rxyz, stdout);
	
	//printf("rxyzt is \n");
	//matrix_print(&rxyzt, stdout);
	
	
	//printf("please work \n");
	//matrix_print(&test, stdout);
		
	Matrix r1;
	matrix_identity(&r1);
	matrix_rotateY(&r1, cos(angleVUP), sin(angleVUP));
	
	//printf("r1 is \n");
	//matrix_print(&r1, stdout);
	
	Matrix r2;
	matrix_identity(&r2);
	matrix_rotateX(&r2, cos(angleU), sin(angleU));

	//printf("r2 is \n");
	//matrix_print(&r2, stdout);
	
	Matrix t2;
	matrix_identity(&t2);
	matrix_translate(&t2, (view->vrp.val[X] + vpnN.val[X] * 0.5), 
							(view->vrp.val[Y] + vpnN.val[Y]* 0.5), 
							(view->vrp.val[Z] + vpnN.val[Z]* 0.5));
	
	//printf("t2 is \n");
	//matrix_print(&t2, stdout);
	
	Matrix tvrc;
	matrix_identity(&tvrc);
	matrix_rotateXYZ(&tvrc, &view->vrp, u, &view -> vup);
	
	tvrc.m[0][3] = 1;
	tvrc.m[3][3] = 0;
	for(int i = 0; i < 3; i++){
		tvrc.m[3][i] = vpnN.val[i];
	}
	
	//printf("tvrc is \n");
	//matrix_print(&tvrc, stdout);
	
	Matrix result;
	matrix_identity(&result);
	matrix_transpose(&tvrc);
	//printf("new tvrc is \n");
	//matrix_print(&tvrc, stdout);
	
	
	//printf("\nmultyplying matricies \n\n");
	matrix_multiply(&tvrc, &result, &result);
	//printf("after multiplying by tvrc.T\n");
	//matrix_print(&result, stdout);
	matrix_multiply(&t1, &result, &result);
	//printf("after multiplying by t1\n");
	//matrix_print(&result, stdout);
	matrix_multiply(&rxyz, &result, &result);
	//printf("after multiplying by rxyz\n");
	//matrix_print(&result, stdout);
	matrix_multiply(&r1, &result, &result);
	//printf("after multiplying by r1\n");
	//matrix_print(&result, stdout);
	matrix_multiply(&r2, &result, &result);
	//printf("after multiplying by r2\n");
	//matrix_print(&result, stdout);
	
	matrix_multiply(&rxyzt, &result, &result);
	//printf("after multiplying by rxyz.t\n");
	//matrix_print(&result, stdout);
	matrix_multiply(&t2, &result, &result);	
	//printf("after multiplying by t2\n");
	//matrix_print(&result, stdout);
	matrix_transpose(&result);
	
	/*
	printf("after taking the transpose\n");
	matrix_print(&result, stdout);
	
	
	printf("origonal vrp is \n");
	vector_print(&view -> vrp, stdout);
	*/
	
	point_set3D(&view -> vrp, result.m[0][0], result.m[0][1], result.m[0][2]);	
		
	/*
	printf("new vrp is \n");
	vector_print(&view -> vrp, stdout);

	printf("origonal vup is \n");
	vector_print(&view -> vup, stdout);
	*/
	
	vector_set(&view -> vup, result.m[2][0], result.m[2][1], result.m[2][2]); 
	vector_normalize(&view -> vup);
	/*
	printf("new vup is \n");
	vector_print(&view -> vup, stdout);
	
	
	printf("origonal vpn is \n");
	vector_print(&view -> vpn, stdout);
	*/
	vector_set(&view -> vpn, result.m[3][0], result.m[3][1], result.m[3][2]); 	
	vector_normalize(&view -> vpn);
	/*
	printf("new vpn is \n");
	vector_print(&view -> vpn, stdout);
	
	printf("final result is\n");
	matrix_print(&result, stdout);
	*/
}

int main(int argc, char *argv[])
{
  Bool   done = False;
  XEvent curEvent;
  char c;
  long event_mask = 0xFFFFFFFF;

	// user variables
	Image *src;
	int frame = 0;
	int freeze = 1;
	int divisions = 3;
	if(argc > 1) {
		int tmp = atoi(argv[1]);
		if( tmp >= 0 && tmp < 10 )
			divisions = tmp;
	}

  // initialize variables
	src = image_create( 20, 20 );

  /* initialize the window and video device */
  initWindow();

  // ximp is a pointer to a pointer
  ximp = (XImage **)malloc( sizeof( XImage *) );
  *ximp = NULL;
	float xoff = 0.0;
	float yoff = 0.0;
	float zoff = 0.0;
	float rotx = 0.0;
	float roty = 0.0;
	Emitter *e;
	Emitter *ew;
	Emitter *ex;


	Module *cube;
	Color blue;
	color_set(&blue, .1, .2, .8);

	Color Grey = {{0.6, 0.62, 0.64}};
	Color Green = {{0.2, 0.7, 0.2}};
	Color Dark = {{0.2, 0.2, 0.2}};
	Color red = {{0.7, 0.2, 0.2}};
	Color Yellow = {{0.8, 0.8, 0.2}};
	Color Purple = {{ 0.6, 0.1, 0.7 }};

	Color White;
		color_set( &White, 1.0, 1.0, 1.0 );

	DrawState *ds;
	View3D view;
	Matrix VTM;
	Matrix GTM;
	float angle;

	// set up a view
	matrix_identity( &VTM );
	matrix_identity( &GTM );
	point_set3D( &(view.vrp),  4 + xoff,  2 + yoff, 6 + zoff );
	vector_set(  &(view.vpn), -4.0, -2.0,  -6.0);
	vector_set(  &(view.vup), 0.0, 1.0, 0.0);
	view.d = 2.0;
	view.du = 1.6;
	view.dv = src->rows * view.du / src->cols;
	view.f = 0.0;
	view.b = 15;
	view.screenx = src->cols;
	view.screeny = src->rows;
	matrix_setView3D( &VTM, &view );

	Module* scene = module_create();
	// set up a cube
	cube = module_create();
	
	Polygon p;
	Point pt[3];
	Vector norms[3];
	for(int i = 0; i < 3; i++){
		vector_set(&norms[i], 0.0, 0.0, 1.0);
	}
	polygon_init(&p);
	point_set2D(&pt[0], 0.1, 0.1);
	point_set2D(&pt[1], -3, 0);
	point_set2D(&pt[2], 0, -3);
	polygon_set( &p, 3, pt );
	polygon_setNormals(&p, 3, &norms);

	module_polygon(cube, &p);
	module_bodyColor( cube, &Grey );
	module_module(scene, cube);

	// set the shading to Gouraud
	ds = drawstate_create();
	point_copy(&(ds->viewer), &(view.vrp));
	//ds->shade = ShadeGouraud;
	ds -> surfaceCoeff = 20;
	ds->shade 	= ShadeGouraud;

  printf("Starting main loop\n");
  while(!done) {
		// sleep for a certain amount of time (100Hz)
		usleep(10);

		// generate the image to display here
		image_reset(src);

		{ // replace this block of code with your own, or use it to test your cube
			matrix_identity( &GTM );
			
			//just move the vrp
			//point_set3D( &(view.vrp),  3.0 + xoff,  3.0 + yoff, -7.0 +zoff );
			//matrix_setView3D( &VTM, &view );

			//move vrp along the vpn

			Vector u;
			vector_cross(&view.vup, &view.vpn, &u);
			vector_normalize(&u);
			//project x,y,z off to the proper vector
			point_set3D( &(view.vrp),  4 + xoff * u.val[X] + yoff * view.vup.val[X] + zoff * view.vpn.val[X]
									,   2 + xoff * u.val[Y] + yoff * view.vup.val[Y] + zoff * view.vpn.val[Y]
									,	6 + xoff * u.val[Z] + yoff * view.vup.val[Z] + zoff * view.vpn.val[Z]);

			if(rotx != 0 || roty !=0){
				rotateVRC(&view, &u, rotx, roty);
			}
			
			vector_normalize(&view.vpn);
			vector_normalize(&view.vup);
			
			matrix_setView3D( &VTM, &view );
			point_copy(&(ds->viewer), &(view.vrp));


			
			

			rotx = 0.0;
			roty = 0.0;
			
			// adjust the angle of the cube based on frame number
			angle = (frame % 1000) * 2 * M_PI / 1000.0;
			matrix_rotateY( &GTM, cos(angle), sin(angle)  );
			// draw the module
			//printf("drawing ray\n");
			module_draw_ray(cube, &GTM, &view, ds, NULL, src);

			/*
			Lighting* light = lighting_create();
			lighting_add( light, LightPoint, &White, NULL, &(view.vrp), 0, 0 );
			lighting_add(light, LightAmbient, &Dark, NULL, NULL, 0, 0);
			module_draw(cube, &VTM, &GTM, ds, light, src);
			free(light);
			*/
			
			//printf("finished draw\n");
			//module_draw(scene, &VTM, &GTM, ds, src);
			
		}
		// set the image to the XWindow
		image2window( src, ximp );

		// check for events
		if(XCheckWindowEvent(curDisplay, curWindow, event_mask, &curEvent)) {

			switch (curEvent.type) {

			case ButtonPress:
				/*
					access button by curEvent.xbutton.button (1,2,3)
					access position by curEvent.xbutton.x and .y
				*/
				if(curEvent.xbutton.button == 1) {
					// do something if the user clicks button 1
					printf("Button 1\n");
				}
				else if(curEvent.xbutton.button == 2) {
					// do something if the user clicks button 2
					printf("Button 2\n");
				}
				else if(curEvent.xbutton.button == 3) { // right button
					printf("Button 3\n");
				}
				break;

			case KeyPress:
				/*
					access string using XLookupString(*event,*char,numChars,NULL,NULL)
				*/
				if(XLookupString((XKeyEvent *)&curEvent, &c, 1, NULL, NULL) == 1) {
					switch(c) {

					case 'q': // user pressed a 'q'
						done = True;
						break;

					case ' ':
						freeze = !freeze;
						break;
					case 'w':
						yoff -= 0.1;
						break;
					case 'a':
						xoff -= 0.1; 
						break;
					case 's':
						yoff += 0.1;
						break;
					case 'd':
						xoff  += 0.1;
						break;
					case 'z':
						zoff -= 0.1;
						break;
					case 'c':
						zoff += 0.1;
						break;
					case 'p':
						rotx = 0.02/M_PI;
						break;
					case 'l':
						rotx = -0.05/M_PI;
						break;
					case 'o':
						roty = 0.05/M_PI;
						break;
					case 'k':
						roty = -0.05/M_PI;
						break;	
					case ',':
						emitter_reset(e, 0);
						emitter_reset(ew, 0);
						emitter_reset(ex, 1);
						break;
						
					default:
						break;
					}
				}
				break;

			case Expose:
				refreshWindow();

			default: // some other unhandled event
				break;
			}
		}

		// increment the frame number
		if(!freeze)
			frame++;
  }

  closeWindow();

	image_free(src);

  return(0);
}

