#ifndef RAY_H

#define RAY_H

typedef struct{
	Vector ray;
	Point start;
}Ray;

typedef struct{
	Polygon p;
	int isEmpty; //bad for now, will fix
	void* next;
}PolygonNode;

typedef struct{
	PolygonNode* head;
	int length;
}PolygonList;


typedef struct BoundingBoxNode{
	float maxX;
	float minX;
	float maxY;
	float minY;
	float maxZ;
	float minZ;
	PolygonList* polygons;
	struct BoundingBoxList* bblist;
	struct BoundingBoxNode* next;
}BoundingBoxNode;

typedef struct BoundingBoxList{
	BoundingBoxNode* head;
	int length;
}BoundingBoxList;

PolygonList* polygon_list_create(void);

PolygonList* polygon_node_add(PolygonList* list, Polygon *p);

void polygon_list_free(PolygonList* list);

BoundingBoxNode* bb_node_create(void);

BoundingBoxNode* bb_node_add_polygon(BoundingBoxNode* bbnode, Polygon* p);

BoundingBoxNode* bb_node_add_bbnode(BoundingBoxNode* bbnode, BoundingBoxNode* newn);

BoundingBoxList* bb_list_create(void);

BoundingBoxList* bb_list_addPolygon(BoundingBoxList* bblist, Polygon *p);

BoundingBoxList* bb_list_addBoundingBox(BoundingBoxList* bblist, BoundingBoxNode* newn);

void bb_list_list_free(PolygonList* list);

double intersect_triangle_MT(Ray *ray, Polygon *p, Vector* baryPt);

double intersect_triangle(Ray *ray, Polygon *p, Point *result);

Polygon* find_closest_p(Ray *ray, PolygonList *list, double maxDist, double* dist, Vector* baryPt);

PolygonList* create_polygon_list(Module *md, Matrix *GTM, DrawState *ds);

BoundingBoxList* module_create_polygon_list(Module *md, Matrix *GTM, DrawState *ds);

void module_draw_ray(Module *md, Matrix *GTM, View3D *view, DrawState *ds, Lighting *lighting, Image *src);

#endif