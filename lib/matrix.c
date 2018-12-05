#include "matrix.h"
#include <math.h>


//matrix
void matrix_print(Matrix *m, FILE *fp){
	int i;
	fprintf(fp, "matrix: \n [");
	for(i = 0; i < 3; i++){
		fprintf(fp, "%f, ", m -> m[0][i]);
	}
	fprintf(fp, "%f ]\n",m -> m[0][3]);
	
	fprintf(fp, "|");
	for(i = 0; i < 3; i++){
		fprintf(fp, "%f, ", m -> m[1][i]);
	}
	fprintf(fp, "%f |\n", m -> m[1][3]);
	
	
	fprintf(fp, "|");
	for(i = 0; i < 3; i++){
		fprintf(fp, "%f, ", m -> m[2][i]);
	}
	fprintf(fp, "%f |\n", m -> m[2][3]);
	
	
	fprintf(fp, "[");
	for(i = 0; i < 3; i++){
		fprintf(fp, "%f, ", m -> m[3][i]);
	}
	fprintf(fp, "%f ]\n", m -> m[3][3]);
}

void matrix_clear(Matrix *m){
	int i, j;
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			m -> m[i][j] = 0;
		}
	}
}

void matrix_identity(Matrix *m){
	int i, j;
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			m -> m[i][j] = i == j ? 1 : 0;
		}
	}
}

double matrix_get(Matrix *m, int r, int c){
	return m -> m[r][c];
}


//Do this as a macro?
void matrix_set(Matrix *m, int r, int c, double v){
	m -> m[r][c] = v;
}

void matrix_copy(Matrix *dest, Matrix *src){
	int i, j;
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			dest -> m[i][j] = src -> m[i][j];
		}
	}
}

//square matrix transpose algorithm taken from wikipedia psudeocode
void matrix_transpose(Matrix *m){
	double tmp;
	int i, j;
	for(i = 0; i < 4-1; i++){
		for(j = i; j < 4; j++){
			tmp = m -> m[i][j];
			m -> m[i][j] = m -> m[j][i];
			m -> m[j][i] = tmp;
		}
	}
}

//make sure that this can be done with result being left/right
void matrix_multiply(Matrix *left, Matrix *right, Matrix *m){
	double result[4][4];
	
	int i, j, k;
	
	double sum;
	
	for(i = 0; i< 4; i++){
		for(j = 0; j < 4; j++){
		sum = 0;
			for(k = 0; k < 4; k++){
				sum+= left -> m[i][k] * right -> m[k][j];
			}
		result[i][j] = sum;
		} 
	}
	
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			m -> m[i][j] = result[i][j];
		}
	}
}

//no loops for speed
void matrix_xformPoint(Matrix *m, Point *p, Point *q){
	q -> val[X] = m->m[0][0] * p -> val[X] + m->m[0][1] * p -> val[Y] + m->m[0][2] * p -> val[Z] + m->m[0][3] * p -> val[H]; 
	q -> val[Y] = m->m[1][0] * p -> val[X] + m->m[1][1] * p -> val[Y] + m->m[1][2] * p -> val[Z] + m->m[1][3] * p -> val[H];
	q -> val[Z] = m->m[2][0] * p -> val[X] + m->m[2][1] * p -> val[Y] + m->m[2][2] * p -> val[Z] + m->m[2][3] * p -> val[H];
	q -> val[H] = m->m[3][0] * p -> val[X] + m->m[3][1] * p -> val[Y] + m->m[3][2] * p -> val[Z] + m->m[3][3] * p -> val[H];		
}

//shouldn't I not need this becasue typedef
void matrix_xformVector(Matrix *m, Vector *p, Vector *q){
	q -> val[X] = m->m[0][0] * p -> val[X] + m->m[0][1] * p -> val[Y] + m->m[0][2] * p -> val[Z] + m->m[0][3] * 0; 
	q -> val[Y] = m->m[1][0] * p -> val[X] + m->m[1][1] * p -> val[Y] + m->m[1][2] * p -> val[Z] + m->m[1][3] * 0;
	q -> val[Z] = m->m[2][0] * p -> val[X] + m->m[2][1] * p -> val[Y] + m->m[2][2] * p -> val[Z] + m->m[2][3] * 0;
	q -> val[H] = m->m[3][0] * p -> val[X] + m->m[3][1] * p -> val[Y] + m->m[3][2] * p -> val[Z] + m->m[3][3] * 0;
}

void matrix_xformPolygon(Matrix *m, Polygon *p){
	int i;
	Point tmp;
	Vector tmpv;
	for(i = 0; i < p -> numVertex; i++){
		matrix_xformPoint(m, &p->vertex[i], &tmp);
		p->vertex[i].val[X] = tmp.val[X];
		p->vertex[i].val[Y] = tmp.val[Y];
		p->vertex[i].val[Z] = tmp.val[Z];
		p->vertex[i].val[H] = tmp.val[H];
		
		if(p->normal != NULL){
			matrix_xformVector(m, &p->normal[i], &tmpv);
			vector_copy(&p->normal[i], &tmpv);
		}
	}
}

void matrix_xformPolyline(Matrix *m, Polyline *p){
	int i;
	Point tmp;
	for(i = 0; i < p -> numVertex; i++){
		matrix_xformPoint(m, &p->vertex[i], &tmp);
		p->vertex[i].val[X] = tmp.val[X];
		p->vertex[i].val[Y] = tmp.val[Y];
		p->vertex[i].val[Z] = tmp.val[Z];
		p->vertex[i].val[H] = tmp.val[H];
	}
}

void matrix_xformLine(Matrix *m, Line *line){
	Point tmp;

	matrix_xformPoint(m, &line->a, &tmp);
	line->a.val[X] = tmp.val[X];
	line->a.val[Y] = tmp.val[Y];
	line->a.val[Z] = tmp.val[Z];
	line->a.val[H] = tmp.val[H];
	
	matrix_xformPoint(m, &line->b, &tmp);
	line->b.val[X] = tmp.val[X];
	line->b.val[Y] = tmp.val[Y];
	line->b.val[Z] = tmp.val[Z];
	line->b.val[H] = tmp.val[H];
}


//2D matrix functions
void matrix_scale2D(Matrix *m, double sx, double sy){
	Matrix scale;
	matrix_identity(&scale);
	scale.m[0][0] = sx;
	scale.m[1][1] = sy;
	matrix_multiply(&scale, m, m);
}

void matrix_rotateZ(Matrix *m, double cth, double sth){
	Matrix rotate;
	matrix_identity(&rotate);
	rotate.m[0][0] = cth;
	rotate.m[0][1] = -sth;
	rotate.m[1][0] = sth;
	rotate.m[1][1] = cth;
	matrix_multiply(&rotate, m, m);
}

void matrix_translate2D(Matrix *m, double tx, double ty){
	Matrix t;
	matrix_identity(&t);
	t.m[0][3] = tx;
	t.m[1][3] = ty;
	matrix_multiply(&t, m, m);
}

void matrix_shear2D(Matrix *m, double shx, double shy){
	Matrix sh;
	matrix_identity(&sh);
	sh.m[0][1] = shx;
	sh.m[1][0] = shy;
	matrix_multiply(&sh, m, m);
}

//3D matrix functions
void matrix_translate(Matrix *m, double tx, double ty, double tz){
	Matrix t;
	matrix_identity(&t);
	t.m[0][3] = tx;
	t.m[1][3] = ty;
	t.m[2][3] = tz;
	matrix_multiply(&t, m, m);
}


void matrix_scale(Matrix *m, double sx, double sy, double sz){
	Matrix scale;
	matrix_identity(&scale);
	scale.m[0][0] = sx;
	scale.m[1][1] = sy;
	scale.m[2][2] = sz;
	matrix_multiply(&scale, m, m);
}

void matrix_rotateX(Matrix *m, double cth, double sth){
	Matrix rotate;
	matrix_identity(&rotate);
	rotate.m[1][1] = cth;
	rotate.m[1][2] = -sth;
	rotate.m[2][1] = sth;
	rotate.m[2][2] = cth;
	matrix_multiply(&rotate, m, m);
}


void matrix_rotateY(Matrix *m, double cth, double sth){
	Matrix rotate;
	matrix_identity(&rotate);
	rotate.m[0][0] = cth;
	rotate.m[0][2] = sth;
	rotate.m[2][0] = -sth;
	rotate.m[2][2] = cth;
	matrix_multiply(&rotate, m, m);
}


void matrix_rotateXYZ(Matrix *m, Vector *u, Vector *v, Vector *w){
	Matrix rotate;
	matrix_identity(&rotate);
	rotate.m[0][0] = u -> val[X];
	rotate.m[0][1] = u -> val[Y];
	rotate.m[0][2] = u -> val[Z];
	
	rotate.m[1][0] = v -> val[X];
	rotate.m[1][1] = v -> val[Y];
	rotate.m[1][2] = v -> val[Z];
	
	rotate.m[2][0] = w -> val[X];
	rotate.m[2][1] = w -> val[Y];
	rotate.m[2][2] = w -> val[Z];
	matrix_multiply(&rotate, m, m);
}

void matrix_shearZ(Matrix *m, double shx, double shy){
	Matrix sh;
	matrix_identity(&sh);
	sh.m[0][2] = shx;
	sh.m[1][2] = shy;
	matrix_multiply(&sh, m, m);
}


void matrix_perspective(Matrix *m, double d){
	Matrix p;
	matrix_identity(&p);
	p.m[3][2] = 1/d;
    p.m[3][3] = 0;
	matrix_multiply(&p, m, m);
}
