#include <vector>
#include <cmath>
#include "dots.h"
#include "propagation.h"

// given a point x, compute d(x), the minimum euclidean distance from x to any point on the boundary of D(x)
double MinDist(Tetra* Tets, Point P) {
	return 1.0;
}

double ComputeRe(Tetra* Tets, Segment e) {
	return 1.0;
}

void MarkPoints(Segment* e) {
	// have to calculate r_e, r_v, m_e
	double d = 1;
	double r_e = 1;
	double r_v = 1;
	double m_e = 0.5;

	double er_v = EPS * r_v;

	// Tetra* Tets;

	vector<double> pts;
	double value;
	pts.push_back(er_v);
	pts.push_back(1 - er_v);

	value = m_e;
	while (value > er_v) {
		pts.push_back(value);
		// value -= EPS * MinDist(Tets, value);
	}

	value = m_e;
	while (value < 1 - er_v) {
		pts.push_back(value);
		// value += EPS * MinDist(Tets, value);
	}

	// there are two m_e's in pts 
	sort(pts.begin(), pts.end());
	
	// e.setdist(v);
}

Point Height(Point P1, Point P2, Point P3) {
	double x1 = P1.x, x2 = P2.x, x3 = P3.x;
	double y1 = P1.y, y2 = P2.y, y3 = P3.y;
	double z1 = P1.z, z2 = P2.z, z3 = P3.z;

	double c = (-1) * ((x2 - x1) * (x3 - x2) + (y2 - y1) * (y3 - y2) + (z2 - z1) * (z3 - z2)) / (pow(x3 - x2, 2) + pow(y3 - y2, 2) + pow(z3 - z2, 2));
	return P1 * (1-c) + P2 * c;
}

void MarkPoints(Tetra Tet) {
	// Tri T1 = Tet.t1, T2 = Tet.t2, T3 = Tet.t3, T4 = Tet.t4;
	Point P1 = Tet.p1, P2 = Tet.p2, P3 = Tet.p3, P4 = Tet.p4;
	Point P = (P3 + P4) / 2;

	Plane PL1(P1, P2, P3), PL2(P1, P2, P4); 
	
	double EPSvalue = sqrt(EPS / 8);
	// represents sin(\Gamma / 2)
	double AngleValue = sin(acos(PlaneAngle(PL1, PL2)) / 2);

	Point H = Height(P1, P2, P);
	double HP_i = PointsDist(P, H);

	double border = MinDist(&Tet, H);
	
	vector<double> pts;

	// steiner point
	Point P_s;


}

	/*
	while (PointsDist(H, pt) < border) {
		pts.push_back();
	}
	*/

// connected polyhedral domain D
/*
vector<PointOnSeg> MarkPoints(PolyDomain D) {

	for (auto e : D.edges) {
		MarkPoints(&e);
	}

	for (auto Tet : D.tets) {
		MarkPoints(Tet);
	}
	
}
*/

// steiner points are placed on the edges of D and on the bisectors of the diheral angles of the tetrahedra in D.

