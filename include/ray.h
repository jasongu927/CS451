#ifndef RAY_H

#define RAY_H

typedef struct{
	Vector ray;
	Point start;
}Ray;

typedef struct PolygonNode{
	Polygon p;
	int isEmpty; //bad for now, will fix
	struct  PolygonNode* next;
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
	int totalP;
}BoundingBoxList;

PolygonList* polygon_list_create(void);

PolygonList* polygon_node_add(PolygonList* list, Polygon *p);

void polygon_list_free(PolygonList* list);

BoundingBoxNode* bb_node_create(void);

BoundingBoxNode* bb_node_addPolygon(BoundingBoxNode* bbnode, Polygon* p);

BoundingBoxNode* bb_node_add_bbnode(BoundingBoxNode* bbnode, BoundingBoxNode* newn);

BoundingBoxList* bb_list_create(void);

BoundingBoxList* bb_list_addPolygon(BoundingBoxList* bblist, Polygon *p);

BoundingBoxList* bb_list_addBoundingBox(BoundingBoxList* bblist, BoundingBoxNode* newn);

void bb_list_free(BoundingBoxList* list);

double intersect_triangle_MT(Ray *ray, Polygon *p, Vector* baryPt);

int intersect_bounding_box(Ray* ray, BoundingBoxNode* node);

Polygon* find_closest_p(Ray *ray, PolygonList *list, double maxDist, double* dist, Vector* baryPt);

double find_closest_p_dist(Ray *ray, PolygonList *list, double maxDist);

Polygon* find_closest_p_bb(Ray *ray, BoundingBoxList* list, double maxDist, double* dist, Vector* baryPt);

double find_closest_p_bb_dist(Ray *ray, BoundingBoxList* list, double maxDist);

BoundingBoxNode* module_create_polygon_list(Module *md, Matrix *GTM, DrawState *ds);

void module_draw_ray(Module *md, Matrix *GTM, View3D *view, DrawState *ds, Lighting *lighting, Image *src);

void module_draw_ray_cel(Module *md, Matrix *GTM, View3D *view, DrawState *ds, Lighting *lighting, Image *src, Color* cel_colors, int mask_length);

#endif