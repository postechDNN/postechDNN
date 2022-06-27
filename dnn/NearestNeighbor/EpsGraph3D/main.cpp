#include "EpsGraph3D.h"
#include "Point.h"
#include "Polytope.h"
#include <list>
#include <vector>

int main() {
	Free_Point* p1 = new Free_Point(0., 0., 0.);
	Free_Point* p2 = new Free_Point(1.5, 1.5, 0.);
	Free_Point* p3 = new Free_Point(0., 0., 1.5);
	Free_Point* p4 = new Free_Point(1000., 1000., 1000.);
	Free_Point* p5 = new Free_Point(-1000., -1000., -1000.);
	Polytope* poly = new Polytope();
	Point* v1 = new Point(1000., 1000., 1.);
	Point* v2 = new Point(1000., -1000., 1.);
	Point* v3 = new Point(-1000., -1000., 1.);
	Point* v4 = new Point(-1000., 1000., 1.);
	Point* v5 = new Point(0., 0., 1.4);
	poly->num_faces = 6;
	poly->num_points = 5;
	poly->x_min = -1000.;
	poly->x_max = 1000.;
	poly->y_min = -1000.;
	poly->y_max = 1000.;
	poly->z_min = 1.;
	poly->z_max = 1.4;
	std::vector<Point*> fv = { v1,v2,v3 };
	Face* f1 = new Face(fv);
	fv = { v1,v3,v4 };
	Face* f2 = new Face(fv);
	fv = { v1,v2,v5 };
	Face* f3 = new Face(fv);
	fv = { v1,v4,v5 };
	Face* f4 = new Face(fv);
	fv = { v2,v3,v5 };
	Face* f5 = new Face(fv);
	fv = { v3,v4,v5 };
	Face* f6 = new Face(fv);
	poly->faces = { f1,f2,f3,f4,f5,f6 };
	poly->vertices = { v1,v2,v3,v4,v5 };
	std::list<Free_Point> frpts = { *p1,*p2,*p3,*p4,*p5 };
	std::vector<Polytope> plts = {};
	Eps_Graph_3D grid(frpts, plts, 0.1);
	Free_Point* q = new Free_Point(0., 0., 0.9);
	grid.print_kNN(*q, 1);
	grid.add_pol(*poly);
	grid.print_kNN(*q, 1);
	return 0;
}
