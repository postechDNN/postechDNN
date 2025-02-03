#pragma once
#include "Point.h"

constexpr double ERR_BOUND = 1e-6;

// Ax_1+Bx_2+Cx_3+D <= 0. 3-dim HalfPlane
class HalfPlane {
public:
	int dim;
	bool is_boundary; // true if it is hyperplane Ax_1+Bx_2+Cx_3+D == 0
	std::vector<double> coeffs;
	//double* coeffs; // dim + 1
public:
	HalfPlane(int dim, bool is_boundary, double coeffs[]);
	HalfPlane(int dim, std::vector<Point> pts, Point anc);
	int is_in(Point& p); // 1: interior 0: boundary -1: out
	double getCoeff(int index);
	void translate(Point vec);
	//~HalfPlane();
};

class CPolytope {
public:
	int dim;
	int v_n, f_n;
	std::vector<Point> vertices;
	std::vector< std::vector<int>> facets;
	std::vector< HalfPlane> half_planes;
public:
	CPolytope(int dim, std::vector<Point>, std::vector< std::vector<int>>);
	int is_in(Point p); // -1: out 0: boundary 1: in
	void translate(Point vec);
	bool is_intersect(Point p, Point q);
	~CPolytope();
};

// Function to print intersection results
void test_intersection(CPolytope& polytope, Point p, Point q, int test_num);

void print_halfplane(HalfPlane& hp, int dim);