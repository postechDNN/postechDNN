#include <cmath>
#include <algorithm>
#include <iostream>
#include "dots.h"
#include "propagation.h"

Point incircle_center(Tri T) {

	// 3차원에서 두 직선이 이루는 평면.
	// general position assumption for points in 'tri' : no two points have the same x-coordinate.
	Point A = T.p1, B = T.p2, C = T.p3;

	// MyVec BA = {A.x - B.x, A.y - B.y, A.z - B.z}; // A - B
	// MyVec BC = {C.x - B.x, C.y - B.y, C.z - B.z };

	// ABC_sec (bisector) = 선분 BA + 선분 BC
	MyVec ABC_sec = { (A.x - B.x) + (C.x - B.x), (A.y - B.y) + (C.y - B.y), (A.z - B.z) + (C.z - B.z) };

	// MyVec CA = { A.x - C.x, A.y - C.y, A.z - C.z };
	// MyVec CB = { B.x - C.x, B.y - C.y, B.z - C.z };

	// BCA_sec (bisector) = 선분 CA + 선분 CB
	MyVec BCA_sec = { (A.x - C.x) + (B.x - C.x), (A.y - C.y) + (B.y - C.y), (A.z - C.z) + (B.z - C.z) };

	// 직선 
	double x1 = B.x, a1 = ABC_sec.x, y1 = B.y, b1 = ABC_sec.y, z1 = B.z, c1 = ABC_sec.z;
	double x2 = C.x, a2 = BCA_sec.x, y2 = C.y, b2 = BCA_sec.y, z2 = C.z, c2 = BCA_sec.z;
	double t = (x2 - x1) / (a1 - a2);

	return Point(a1 * t + x1, b1 * t + y1, c1 * t + z1);
}

MyVec operator*(const double a, const MyVec& V)
{
	return MyVec(a * V.x, a * V.y, a * V.z);
}

double VecSize(MyVec V) {
	return sqrt(pow(V.x, 2) + pow(V.y, 2) + pow(V.z, 2));
}

double PointsDist(Point P1, Point P2) {
	return sqrt(pow(P1.x - P2.x, 2) + pow(P1.y - P2.y, 2) + pow(P1.z - P2.z, 2));
}

double InnerProd(MyVec V1, MyVec V2) {
	return V1.x * V2.x + V1.y * V2.y + V1.z * V2.z;
}

MyVec Point2Vec(Point P1) {
	return MyVec{P1.x, P1.y, P1.z, P1.index};
}

MyVec Points2Vec(Point P1, Point P2) {
	return MyVec(P2.x - P1.x, P2.y - P1.y, P2.z - P1.z);
}

MyVec OuterProd(MyVec V1, MyVec V2) {
	double a1 = V1.x, a2 = V1.y, a3 = V1.z, b1 = V2.x, b2 = V2.y, b3 = V2.z;
	return MyVec{ a2 * b3 - a3 * b2, -(a1 * b3 - a3 * b1), a1 * b2 - a2 * b1 };
}

double PointEdgeDist(Point P0p, Edge E) {
	Point P1 = E.p1;
	Point P2 = E.p2;

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

// 3D 공간에서 삼각형이 (해당 삼각형이 정의되는) 평면 위의 한 점을 포함하는지 판단. reference : 3D Distance from a Point to a Triangle
bool PointInsideTri(Tri T, Point P0) {
	Point P1 = T.p1, P2 = T.p2, P3 = T.p3;
	MyVec P1_P2 = Points2Vec(P1, P2), P1_P3 = Points2Vec(P1, P3);
	MyVec N_P = OuterProd(P1_P2, P1_P3);
	MyVec temp1 = Points2Vec(P3, P1) / PointsDist(P3, P1);
	MyVec V1 = MyVec(Points2Vec(P2, P1) / PointsDist(P2, P1)).operator+(temp1);
	MyVec temp2 = Points2Vec(P1, P2) / PointsDist(P1, P2);
	MyVec V2 = MyVec(Points2Vec(P3, P2) / PointsDist(P3, P2)).operator+(temp2);
	MyVec temp3 = Points2Vec(P2, P3) / PointsDist(P2, P3);
	MyVec V3 = MyVec(Points2Vec(P1, P3) / PointsDist(P1, P3)).operator+(temp3);

	double f1 = InnerProd(OuterProd(V1, Points2Vec(P1, P0)), N_P);
	double f2 = InnerProd(OuterProd(V2, Points2Vec(P2, P0)), N_P);
	double f3 = InnerProd(OuterProd(V3, Points2Vec(P3, P0)), N_P);

	// int outside;

	if (f1 > 0 && f2 < 0) {
		/*MyVec VA = Points2Vec(P0, P1), VB = Points2Vec(P0, P2);
		MyVec VV = OuterProd(Points2Vec(P0, P1), Points2Vec(P0, P2));
		std::cout << VA.x << " " << VA.y << " " << VA.z << std::endl;
		std::cout << VB.x << " " << VB.y << " " << VB.z << std::endl;
		std::cout << VV.x << " " << VV.y << " " << VV.z << std::endl;
		*/
		if (InnerProd(OuterProd(Points2Vec(P0, P1), Points2Vec(P0, P2)), N_P) < 0) { return false; } // point lies outside from the triangle
		else { return true; } // point lies inside the triangle
	}
	else if (f2 > 0 && f3 < 0) {
		if (InnerProd(OuterProd(Points2Vec(P0, P2), Points2Vec(P0, P3)), N_P) < 0) { return false; }
		else { return true; }
	}
	else {
		if (InnerProd(OuterProd(Points2Vec(P0, P3), Points2Vec(P0, P1)), N_P) < 0) { return false; }
		else { return true; }
	}

}

double PointPlaneDist(Point P0, Plane PL) {
	double x0 = P0.x, y0 = P0.y, z0 = P0.z;
	double a1 = PL.a, b1 = PL.b, c1 = PL.c, d1 = PL.d;
	return abs(a1 * x0 + b1 * y0 + c1 * z0 + d1) / sqrt(pow(a1, 2) + pow(b1, 2) + pow(c1, 2));
}

double PlaneAngle(Plane PL1, Plane PL2) {
	MyVec V1{ PL1.a, PL1.b, PL1.c }, V2{ PL2.a, PL2.b, PL2.c };
	return abs(InnerProd(V1, V2)) / (VecSize(V1) * VecSize(V2));
}

// 맞닿아 있는 두 tetrahedron이 주어졌을 때, 맞닿아 있는 삼각형 내에서 임의의 tetrahedron의 변까지의 길이를 최대화하는 삼각형 내의 점을 구하는 알고리즘
// T는 T1과 T2이 공유하는 triangular face. T, Tet1.t1, Tet2.t1는 동일.
// Tet1.t2와 Tet2.t2는 T의 동일한 edge(T.p1와 T.p2를 잇는 edge)를 공유. Tet1.t3과 Tet2.t3 (T.p2와 T.p3를 잇는 edge), Tet1.t4와 Tet2.t4 (T.p3와 T.p1를 잇는 edge)도 마찬가지
Point TetraCenter(Tetra Tet1, Tetra Tet2, Tri T) {
	// PL1 위의 점은 (x0, y0, (d1 + b1y0 + a1x0) * (-1/c1))로 표시할 수 있음.
	Plane PL1(T);
	Plane PL12(Tet1.t2), PL13(Tet1.t3), PL14(Tet1.t4), PL22(Tet2.t2), PL23(Tet2.t3), PL24(Tet2.t4);
	Plane PL2, PL3, PL4;
	if (PlaneAngle(PL1, PL12) < PlaneAngle(PL1, PL22)) { PL2 = PL22; }
	else { PL2 = PL12; }
	if (PlaneAngle(PL1, PL13) < PlaneAngle(PL1, PL23)) { PL3 = PL23; }
	else { PL3 = PL13; }
	if (PlaneAngle(PL1, PL14) < PlaneAngle(PL1, PL24)) { PL4 = PL24; }
	else { PL4 = PL14; }

	MyVec V2 = PL2.NormalVec(), V3 = PL3.NormalVec(), V4 = PL4.NormalVec();
	double a1 = PL1.a, b1 = PL1.b, c1 = PL1.c, d1 = PL1.d;
	double a2 = PL2.a, b2 = PL2.b, c2 = PL2.c, d2 = PL2.d;
	double a3 = PL3.a, b3 = PL3.b, c3 = PL3.c, d3 = PL3.d;
	double a4 = PL4.a, b4 = PL4.b, c4 = PL4.c, d4 = PL4.d;
	double a2_bar = a2 - ((a1 * c2) / c1), b2_bar = b2 - ((b1 * c2) / c1), c2_bar = d2 - ((d1 * c2) / c1);
	double a3_bar = a3 - ((a1 * c3) / c1), b3_bar = b3 - ((b1 * c3) / c1), c3_bar = d3 - ((d1 * c3) / c1);
	double a4_bar = a4 - ((a1 * c4) / c1), b4_bar = b4 - ((b1 * c4) / c1), c4_bar = d4 - ((d1 * c4) / c1);

	double p, q;
	p = (V3.size() * b2_bar - V2.size() * b3_bar) / (V2.size() * a3_bar - V3.size() * a2_bar);
	q = (V3.size() * c2_bar - V2.size() * c3_bar) / (V2.size() * a3_bar - V3.size() * a2_bar);
	
	double x1, y1, z1, x2, y2, z2;
	y1 = T.p2.y + 0.01; x1 = p * y1 + q; z1 = (d1 + b1 * y1 + a1 * x1) * (-1 / c1);
	y2 = T.p2.y - 0.01; x2 = p * y2 + q; z2 = (d1 + b1 * y2 + a1 * x2) * (-1 / c1);
	// need to handle degenerate cases
	/*
	std::cout << x1 << " " << y1 << " " << z1 << std::endl;
	std::cout << x2 << " " << y2 << " " << z2 << std::endl;
	std::cout << PointInsideTri(T, Point{ x1, y1, z1 }) << " " << std::endl;
	std::cout << PointInsideTri(T, Point{ x2, y2, z2 }) << std::endl;
	*/
	if (!PointInsideTri(T, Point{ x1, y1, z1 }) && !PointInsideTri(T, Point{x2, y2, z2})) {
		p = (V3.size() * b2_bar + V2.size() * b3_bar) / -(V2.size() * a3_bar + V3.size() * a2_bar);
		q = (V3.size() * c2_bar + V2.size() * c3_bar) / -(V2.size() * a3_bar + V3.size() * a2_bar);
	}

	y1 = T.p2.y + 0.01; x1 = p * y1 + q; z1 = (d1 + b1 * y1 + a1 * x1) * (-1 / c1);
	y2 = T.p2.y - 0.01; x2 = p * y2 + q; z2 = (d1 + b1 * y2 + a1 * x2) * (-1 / c1);
	/*
	std::cout << x1 << " " << y1 << " " << z1 << std::endl;
	std::cout << x2 << " " << y2 << " " << z2 << std::endl;
	std::cout << PointInsideTri(T, Point{ x1, y1, z1 }) << " " << PointInsideTri(T, Point{ x2, y2, z2 }) << std::endl;
	*/

	double x0, y0, z0;
	y0 = ((V3.size() * (a4_bar * q + c4_bar)) - (V4.size() * (a3_bar * q + c3_bar))) / ((V4.size() * (a3_bar * p + b3_bar)) - (V3.size() * (a4_bar * p + b4_bar)));
	x0 = p * y0 + q;
	z0 = (d1 + b1 * y0 + a1 * x0) * (-1 / c1);

	if (!PointInsideTri(T, Point{ x0, y0, z0 })) {
		y0 = -((V3.size() * (a4_bar * q + c4_bar)) + (V4.size() * (a3_bar * q + c3_bar))) / ((V4.size() * (a3_bar * p + b3_bar)) + (V3.size() * (a4_bar * p + b4_bar)));
		x0 = p * y0 + q;
		z0 = (d1 + b1 * y0 + a1 * x0) * (-1 / c1);
	}

	return Point{ x0, y0, z0 };
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