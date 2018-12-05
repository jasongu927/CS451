#include <math.h>
#include <stdlib.h>
#include "polyline.h"




//polylines

Polyline *polyline_create(){
	Polyline* p = (Polyline*)malloc(sizeof(Polyline));
	p -> zBuffer = 0;
	p -> numVertex = 0;
	p -> vertex = NULL;
	return p;
}

Polyline *polyline_createp(int numV, Point *vlist){
	Polyline* p = (Polyline*)malloc(sizeof(Polyline));
	p -> zBuffer = 0;
	p -> numVertex = numV;
	p -> vertex = vlist;
	return p;
}


void polyline_free(Polyline *p){
	if(p -> vertex == NULL){
		free(p -> vertex);
	}
	free(p);
}

void polyline_init(Polyline *p){
	p -> zBuffer = 0;
	p -> numVertex = 0;
	p -> vertex = NULL;
}

void polyline_set(Polyline *p, int numV, Point *vlist){
	p -> numVertex = numV;
	p -> vertex = malloc(sizeof(Point) * numV);
	int i;
	for(i = 0; i < numV; i++){
		p -> vertex[i] = vlist[i];
	}
}

void polyline_clear(Polyline *p){
	//I guess I don't need to free this?????
	printf("trying to clear polyline\n");
	if(p -> vertex != NULL){
		free(p -> vertex);
	}
	p -> vertex = NULL;
	p -> numVertex = 0;
}


void polyline_zBuffer(Polyline *p, int flag){
	p -> zBuffer = flag;
}

void polyline_copy(Polyline *to, Polyline *from){
	int i;
	for(i = 0; i < to -> numVertex; i++){
		point_copy(&to -> vertex[i], &from -> vertex[i]);
	}
}

void polyline_print(Polyline *p, FILE *fp){
	int i;
	for(i = 0; i < p -> numVertex; i++){
		point_print(&p -> vertex[i], fp);
	}
}

void polyline_normalize(Polyline *p){
	int i;
	for(i = 0; i < p -> numVertex; i++){
		point_normalize(&p -> vertex[i]);
	}
}

void polyline_draw(Polyline *p, Image *src, Color c){
	int i;
	Line line;
	//need to subtract 2, 1 for indexing, and 1 more because we draw v-1 lines
	for(i = 0; i < p -> numVertex -1; i++){
		line_set2D(&line, p -> vertex[i].val[X], p -> vertex[i].val[Y],
					p -> vertex[i + 1].val[X], p -> vertex[i + 1].val[Y]);
		line_draw(&line, src, c);
	}
}

//need to add bounds checking
void scanline_flood_fill(Image* src, int startx, int starty, Color c){
	Point stack[src -> rows * 10]; //just make a huge stack
	int top = 0;
	
	int x, y;
	int topBord = 0;
	int botBord = 0;
	stack[top].val[X] = startx;
	stack[top].val[Y] = starty;
	top++;
	while(top > 0){
		top--;
		//printf("top is %d\n", top);
		x = stack[top].val[X];
		y = stack[top].val[Y];
		//printf("got top val\n");
		//check the top
		if(y < src -> rows - 1){
			if(COLOR_EQUAL_PIXEL(c, src -> data[y+1][x])){
				topBord = 1;
			}
			else{
				stack[top].val[X] = x;
				stack[top].val[Y] = y + 1;
				top++;
				topBord = 0;
			}
		}
		//printf("checked top border\n");
		//check the bottom
		if(y > 1){
			if(COLOR_EQUAL_PIXEL(c, src -> data[y-1][x])){
				botBord = 1;
			}
			else{
				stack[top].val[X] = x;
				stack[top].val[Y] = y - 1;
				top++;
				botBord = 0;
			}
		}
		//printf("checked bottom border\n");
		
		int midx = x - 1;
		// loop right
		while(x < src -> cols){
			if(COLOR_EQUAL_PIXEL(c, src -> data[y][x])){
				break;
			}
			image_setColor(src, x, y, c);
			
			//check the top row
			if(y < src -> rows - 1){
				if(COLOR_EQUAL_PIXEL(c, src -> data[y+1][x])){
					topBord = 1;
				}
				else{
					if(topBord == 1){
						stack[top].val[X] = x;
						stack[top].val[Y] = y + 1;
						top++;
						topBord = 0;
					}
				}
			}
			if(y > 1){
				if(COLOR_EQUAL_PIXEL(c, src -> data[y-1][x])){
					botBord = 1;
				}
				else{
					if(botBord == 1){
						stack[top].val[X] = x;
						stack[top].val[Y] = y - 1;
						top++;
						botBord = 0;
					}
				}
			}
			x++;
		}
		//loop left
		x = midx;
		while(x > 0){
			if(COLOR_EQUAL_PIXEL(c, src -> data[y][x])){
				break;
			}
			image_setColor(src, x, y, c);
			
			//check the top row
			if(y < src -> rows - 1){
				if(COLOR_EQUAL_PIXEL(c, src -> data[y+1][x])){
					topBord = 1;
				}
				else{
					if(topBord == 1){
						stack[top].val[X] = x;
						stack[top].val[Y] = y + 1;
						top++;
						topBord = 0;
					}
				}
			}
			if(y > 1){
				if(COLOR_EQUAL_PIXEL(c, src -> data[y-1][x])){
					botBord = 1;
				}
				else{
					if(botBord == 1){
						stack[top].val[X] = x;
						stack[top].val[Y] = y - 1;
						top++;
						botBord = 0;
					}
				}
			}
			x--;
		}
	}
}