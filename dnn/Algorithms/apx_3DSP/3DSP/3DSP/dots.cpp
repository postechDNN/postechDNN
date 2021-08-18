#include <cmath>
#include <algorithm>
#include <iostream>
#include "dots.h"
#include "propagation.h"

Point incircle_center(Tri T) {

	// 3차원에서 두 직선이 이루는 평면.
	// general position assumption for points in 'tri' : no two points have the same x-coordinate.
	Point A = T.getp1(), B = T.getp2(), C = T.getp3();

	// MyVec BA = {A.x - B.x, A.y - B.y, A.z - B.z}; // A - B
	// MyVec BC = {C.x - B.x, C.y - B.y, C.z - B.z };

	// ABC_sec (bisector) = 선분 BA + 선분 BC
	MyVec ABC_sec = { (A.getx() - B.getx()) + (C.getx() - B.getx()), (A.gety() - B.gety()) + (C.gety() - B.gety()), 
	(A.getz() - B.getz()) + (C.getz() - B.getz()) };

	// MyVec CA = { A.x - C.x, A.y - C.y, A.z - C.z };
	// MyVec CB = { B.x - C.x, B.y - C.y, B.z - C.z };

	// BCA_sec (bisector) = 선분 CA + 선분 CB
	MyVec BCA_sec = { (A.getx() - C.getx()) + (B.getx() - C.getx()), (A.gety() - C.gety()) + (B.gety() - C.gety()), 
	(A.getz() - C.getz()) + (B.getz() - C.getz()) };

	// 직선 
	double x1 = B.getx(), a1 = ABC_sec.getx(), y1 = B.gety(), b1 = ABC_sec.gety(), z1 = B.getz(), c1 = ABC_sec.getz();
	double x2 = C.getx(), a2 = BCA_sec.getx(), y2 = C.gety(), b2 = BCA_sec.gety(), z2 = C.getz(), c2 = BCA_sec.getz();
	double t = (x2 - x1) / (a1 - a2);

	return Point(a1 * t + x1, b1 * t + y1, c1 * t + z1);
}

MyVec operator*(const double a, const MyVec& V)
{
	return MyVec(a * V.x, a * V.y, a * V.z);
}

double VecSize(MyVec V) {
	return sqrt(pow(V.getx(), 2) + pow(V.gety(), 2) + pow(V.getz(), 2));
}

double PointsDist(Point P1, Point P2) {
	return sqrt(pow(P1.getx() - P2.getx(), 2) + pow(P1.gety() - P2.gety(), 2) + pow(P1.getz() - P2.getz(), 2));
}

double InnerProd(MyVec V1, MyVec V2) {
	return V1.getx() * V2.getx() + V1.gety() * V2.gety() + V1.getz() * V2.getz();
}

MyVec Point2Vec(Point P1) {
	return MyVec{P1.getx(), P1.gety(), P1.getz(), P1.getindex()};
}

Point Vec2Point(MyVec V1) {
	return Point{ V1.getx(), V1.gety(), V1.getz(), V1.getindex() };
}

MyVec Points2Vec(Point P1, Point P2) {
	return MyVec(P2.getx() - P1.getx(), P2.gety() - P1.gety(), P2.getz() - P1.getz());
}

MyVec OuterProd(MyVec V1, MyVec V2) {
	double a1 = V1.getx(), a2 = V1.gety(), a3 = V1.getz(), b1 = V2.getx(), b2 = V2.gety(), b3 = V2.getz();
	return MyVec{ a2 * b3 - a3 * b2, -(a1 * b3 - a3 * b1), a1 * b2 - a2 * b1 };
}

double PlaneAngle(Plane PL1, Plane PL2) {
	MyVec V1{ PL1.geta(), PL1.getb(), PL1.getc() }, V2{ PL2.geta(), PL2.getb(), PL2.getc() };
	return abs(InnerProd(V1, V2)) / (VecSize(V1) * VecSize(V2));
}

double PointEdgeDist(Point P0p, Segment S) {
	Point P1 = Vec2Point(S.geta());
	Point P2 = Vec2Point(S.getb());

	MyVec R = OuterProd(OuterProd(Points2Vec(P0p, P2), Points2Vec(P0p, P1)), Points2Vec(P1, P2));
	double cos_gamma = InnerProd(Points2Vec(P0p, P1), R) / (PointsDist(P0p, P1) * VecSize(R));
	double P0p_P0pp_len = PointsDist(P0p, P1) * cos_gamma;
	MyVec P0p_P0pp = R * (P0p_P0pp_len) / VecSize(R);

	Point P0pp = P0p.operator+(P0p_P0pp);

	double t = VecSize(Points2Vec(P0pp, P1)) / VecSize(Points2Vec(P2, P1));

	if (0 <= t && t <= 1) { return P0p_P0pp_len; } // P0pp is between P1 and P2
	else if (t < 0) { return 2; } // P0 is closest to P1
	else { return 3; } // P0 is closest to P2

}

// 점과 triangle 사이의 거리. reference : 3D Distance from a Point to a Triangle
double FacePointDist(Tri T, Point P0) {
	Point P1 = T.getp1(), P2 = T.getp2(), P3 = T.getp3();
	MyVec P1_P2 = Points2Vec(P1, P2), P1_P3 = Points2Vec(P1, P3);
	MyVec N_P = OuterProd(P1_P2, P1_P3);
	double cos_alpha = InnerProd(Points2Vec(P1, P0), N_P) / (PointsDist(P1, P0) * VecSize(N_P));
	double P0_P0p_len = PointsDist(P0, P1) * cos_alpha;
	MyVec P0_P0p = N_P * (-1 * P0_P0p_len / VecSize(N_P));
	Point P0p = P0 + P0_P0p;

	MyVec temp = Points2Vec(P3, P1) / PointsDist(P3, P1);
	MyVec V1 = MyVec(Points2Vec(P2, P1) / PointsDist(P2, P1)).operator+(temp);
	MyVec temp2 = Points2Vec(P1, P2) / PointsDist(P1, P2);
	MyVec V2 = MyVec(Points2Vec(P3, P2) / PointsDist(P3, P2)).operator+(temp2);
	MyVec temp3 = Points2Vec(P2, P3) / PointsDist(P2, P3);
	MyVec V3 = MyVec(Points2Vec(P1, P3) / PointsDist(P1, P3)).operator+(temp3);

	double f1 = InnerProd(OuterProd(V1, Points2Vec(P1, P0p)), N_P);
	double f2 = InnerProd(OuterProd(V2, Points2Vec(P2, P0p)), N_P);
	double f3 = InnerProd(OuterProd(V3, Points2Vec(P3, P0p)), N_P);

	int outside;

	if (f1 > 0 && f2 < 0) {
		if (InnerProd(OuterProd(Points2Vec(P0p, P1), Points2Vec(P0p, P2)), N_P)) { outside = 1; }
		else { outside = -1; }
	}
	else if (f2 > 0 && f3 < 0) {
		if (InnerProd(OuterProd(Points2Vec(P0p, P2), Points2Vec(P0p, P3)), N_P)) { outside = 2; }
		else { outside = -1; }
	}
	else {
		if (InnerProd(OuterProd(Points2Vec(P0p, P3), Points2Vec(P0p, P1)), N_P)) { outside = 3; }
		else { outside = -1; }
	}

	Segment S;
	switch (outside) {
	case -1:
		return P0_P0p_len;
	case 1:
		S.seta(Point2Vec(P1)), S.setb(Point2Vec(P2));
	case 2:
		S.seta(Point2Vec(P2)), S.setb(Point2Vec(P3));
	case 3:
		S.seta(Point2Vec(P3)), S.setb(Point2Vec(P1));
	}

	double PointEdgeRelation = PointEdgeDist(P0p, S);
	if (PointEdgeRelation == 1) { // t < 0 in PointEdgeDistance()
		return VecSize(Points2Vec(Vec2Point(S.geta()), P0));
	}
	else if (PointEdgeRelation == 2) { // t > 1 in PointEdgeDistance()
		return VecSize(Points2Vec(Vec2Point(S.getb()), P0));
	}
	else //  0 <= t <= 1 in PointEdgeDistance()
	{
		return sqrt(pow(PointEdgeRelation, 2) + pow(P0_P0p_len, 2));
	}

	// still need to handle degenerate cases

}


/*
int main() {

	
	Point P1{ -3.0, 0.0, 0.0 }, P2{ 0.0, -3.0, 0.0 }, P3{ 2.0, 2.0, 0.0 }, P4{ -2.0, 0.3, 1.0 }, P5{ 1.0, 1.0, -0.3 };
	Tri T1{ P1, P2, P3 }, T2{ P1, P2, P4 }, T3{ P2, P3, P4 }, T4{ P3, P1, P4 };
	Tri T5{ P1, P2, P5 }, T6{ P2, P3, P5 }, T7{ P3, P1, P5 };

	// PointInsideTri(T1, Point{ -1.6, -1.6, 0.0 });
	// PointInsideTri(T1, Point{ -10.0, -10.0, 0.0 });

	Tetra Tet1{ T1, T2, T3, T4 }, Tet2{ T1, T5, T6, T7 };
	Point c = TetraCenter(Tet1, Tet2, T1);
	c.print();
	

	// Point center = tetra_center(Tet1, Tet2, T, 0);
}
*/