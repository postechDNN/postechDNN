#include "EpsGraph3D.h"
#include "Point.h"
#include "Polytope.h"
#include <list>
#include <vector>

int main() {
	Free_Point* p1 = new Free_Point(0., 0., 20.);
	Free_Point* p2 = new Free_Point(0, 0, -65.);
	Free_Point* p3 = new Free_Point(0., 0., 1.5);
	Free_Point* p4 = new Free_Point(100., 100., 100.);
	Free_Point* p5 = new Free_Point(-100., -100., -100.);
	Polytope* poly = new Polytope();
	Point* v1 = new Point(99., 99., -1.);
	Point* v2 = new Point(99., -99., -1.);
	Point* v3 = new Point(-99., -99., -1.);
	Point* v4 = new Point(-99., 99., -1.);
	Point* v5 = new Point(0., 0., 1.4);

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
	poly->setpolytope( { f1, f2, f3, f4, f5, f6 });
	std::list<Free_Point> frpts = { *p1,*p2,*p3,*p4,*p5 };
	std::vector<Polytope> plts = {};
	Eps_Graph_3D grid(frpts, plts, 10);
	grid.print_anchor();
	Free_Point* q = new Free_Point(0., 0., -20.);
	//grid.print_kNN(*q, 3);
	grid.add_freepts(q);
	grid.add_pol(*poly);
	//grid.print_edges();
	//grid.print_kNN(*q, 3);
	grid.delete_pol(0);
	grid.print_free_point();
	grid.delete_freept(3);
	grid.print_free_point();
	grid.delete_freept(3);
	grid.print_free_point();
	grid.print_anchor();
	//grid.print_edges();
	//grid.print_kNN(*q, 3);

	return 0;
}
