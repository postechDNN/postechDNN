#include <cmath>
#include <algorithm>

struct Line {
	double m;
	double n;
};

class MyVec {
public:
	double x, y, z;
public:
	MyVec() {};

	MyVec(double x_, double y_, double z_) {
		x = x_;
		y = y_;
		z = z_;
	}

	double size() {
		return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
	}

	MyVec operator + (MyVec& V) {
		x = x + V.x;
		y = y + V.y;
		z = z + V.z;
		return MyVec(x, y, z);
	}

	MyVec operator - (MyVec& V) {
		x = x - V.x;
		y = y - V.y;
		z = z - V.z;
		return MyVec(x, y, z);
	}

	MyVec operator * (double a) {
		x = a * x;
		y = a * y;
		z = a * z;
		return MyVec(x, y, z);
	}

	MyVec operator / (double a) {
		x = x / a;
		y = y / a;
		z = z / a;
		return MyVec(x, y, z);
	}
};

// https://yeolco.tistory.com/119
class Point {
public:
	double x, y, z;

public:
	Point() {};
	Point(double x_, double y_, double z_) {
		x = x_;
		y = y_;
		z = z_;
	}

	Point operator - (Point& P) {
		x = x - P.x;
		y = y - P.y;
		z = z - P.z;
		return Point(x, y, z);
	}

	Point operator + (MyVec& V) {
		x = x + V.x;
		y = y + V.y;
		z = z + V.z;
		return Point(x, y, z);
	}

	friend bool operator == (const Point& P1, const Point& P2) {
		return P1.x == P2.x && P1.y == P2.y && P1.z == P2.z;
	}
	/*
	Point operator + (Point& P) {
		x = x + P.x;
		y = y + P.y;
		z = z + P.z;
		return Point(x, y, z);
	}
	*/
};

class Edge {
public:
	Point p1;
	Point p2;

public:
	Edge() {};
	Edge(Point P1_, Point P2_) {
		p1 = P1_; p2 = P2_;
	}
};

// triangular face of a tetrahedron
class Tri {
public:
	Point p1, p2, p3;
public:
	Tri() {};
	Tri(Point P1_, Point P2_, Point P3_) {
		p1 = P1_; p2 = P2_; p3 = P3_;
	}

	friend bool operator == (const Tri& T1, const Tri& T2) {
		return T1.p1 == T2.p1 && T1.p2 == T2.p2 && T1.p3 == T2.p3;
	}
};

// tetrahedron
class Tetra {
public:
	Tri t1, t2, t3, t4;
public:
	Tetra() {};
	Tetra(Tri T1_, Tri T2_, Tri T3_, Tri T4_) {
		t1 = T1_; t2 = T2_, t3 = T3_, t4 = T4_;
	}

	friend bool operator == (const Tetra& Tet1, const Tetra& Tet2) {
		return Tet1.t1 == Tet2.t1 && Tet1.t2 == Tet2.t2 && Tet1.t3 == Tet2.t3 && Tet1.t4 == Tet2.t4;
	}
};

// ax + by + cz + d = 0
class Plane {
public:
	double a, b, c, d;
public:
	Plane() {};
	Plane(double a_, double b_, double c_, double d_) {
		a = a_; b = b_; c = c_; d = d_;
	}
	Plane(Point P1, Point P2, Point P3) {
		MyVec P1_P2 = Points2Vec(P1, P2), P1_P3 = Points2Vec(P1, P3);
		MyVec N_P = OuterProd(P1_P2, P1_P3);
		a = N_P.x; b = N_P.y; c = N_P.z;
		d = -(a * P1.x + b * P1.y + c * P1.z);
	}
	Plane(Tri T) {
		Point P1 = T.p1, P2 = T.p2, P3 = T.p3;
		MyVec P1_P2 = Points2Vec(P1, P2), P1_P3 = Points2Vec(P1, P3);
		MyVec N_P = OuterProd(P1_P2, P1_P3);
		a = N_P.x; b = N_P.y; c = N_P.z;
		d = -(a * P1.x + b * P1.y + c * P1.z);
	}
	MyVec NormalVec() {
		return MyVec(a, b, c);
	}
};

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

double VecSize(MyVec V) {
	return sqrt(pow(V.x, 2) + pow(V.y, 2) + pow(V.z, 2));
}

double PointsDist(Point P1, Point P2) {
	return sqrt(pow(P1.x - P2.x, 2) + pow(P1.y - P2.y, 2) + pow(P1.z - P2.z, 2));
}

MyVec Points2Vec(Point P1, Point P2) {
	return MyVec(P2.x - P1.x, P2.y - P1.y, P2.z - P1.z);
}

double InnerProd(MyVec V1, MyVec V2) {
	return V1.x * V2.x + V1.y * V2.y + V1.z * V2.z;
}

MyVec OuterProd(MyVec V1, MyVec V2) {
	double a1 = V1.x, a2 = V1.y, a3 = V1.z, b1 = V2.x, b2 = V2.y, b3 = V2.z;
	return MyVec{ a2 * b3 - a1 * b2, -(a1 * b3 - a3 * b1), a1 * b2 - a2 * b1 };
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
	MyVec temp = Points2Vec(P3, P1) / PointsDist(P3, P1);
	MyVec V1 = MyVec(Points2Vec(P2, P1) / PointsDist(P2, P1)).operator+(temp);
	MyVec temp = Points2Vec(P1, P2) / PointsDist(P1, P2);
	MyVec V2 = MyVec(Points2Vec(P3, P2) / PointsDist(P3, P2)).operator+(temp);
	MyVec temp = Points2Vec(P2, P3) / PointsDist(P2, P3);
	MyVec V3 = MyVec(Points2Vec(P1, P3) / PointsDist(P1, P3)).operator+(temp);

	double f1 = InnerProd(OuterProd(V1, Points2Vec(P1, P0p)), N_P);
	double f2 = InnerProd(OuterProd(V2, Points2Vec(P2, P0p)), N_P);
	double f3 = InnerProd(OuterProd(V3, Points2Vec(P3, P0p)), N_P);

	int outside;

	if (f1 > 0 && f2 < 0) {
		if (InnerProd(OuterProd(Points2Vec(P0p, P1), Points2Vec(P0p, P2)), N_P)) { return false; } // point lies outside from the triangle
		else { return true; } // point lies inside the triangle
	}
	else if (f2 > 0 && f3 < 0) {
		if (InnerProd(OuterProd(Points2Vec(P0p, P2), Points2Vec(P0p, P3)), N_P)) { return false; }
		else { return true; }
	}
	else {
		if (InnerProd(OuterProd(Points2Vec(P0p, P3), Points2Vec(P0p, P1)), N_P)) { return false; }
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
// Tet1.t2와 Tet2.t2는 T의 동일한 edge를 공유. Tet1.t3과 Tet2.t3, Tet1.t4와 Tet2.t4도 마찬가지
Point tetra_center(Tetra Tet1, Tetra Tet2, Tri T, int depth) {
	// PL1 위의 점은 (x0, y0, (d1 + b1y0 + a1x0) * (-1/c1))로 표시할 수 있음.
	Plane PL1(T), PL12(Tet1.t2), PL13(Tet1.t3), PL14(Tet1.t4), PL22(Tet2.t2), PL23(Tet2.t3), PL24(Tet2.t4);
	Plane PL2, PL3, PL4;
	if (PlaneAngle(PL1, PL12) > PlaneAngle(PL1, PL22)) { PL2 = PL22; }
	else { PL2 = PL12; }
	if (PlaneAngle(PL1, PL13) > PlaneAngle(PL1, PL23)) { PL3 = PL23; }
	else { PL3 = PL13; }
	if (PlaneAngle(PL1, PL14) > PlaneAngle(PL1, PL24)) { PL4 = PL24; }
	else { PL4 = PL14; }

	MyVec V2 = PL2.NormalVec(), V3 = PL3.NormalVec(), V4 = PL4.NormalVec();
	double a1 = PL1.a, b1 = PL1.b, c1 = PL1.c, d1 = PL1.d;
	double a2 = PL2.a, b2 = PL2.b, c2 = PL2.c, d2 = PL2.d;
	double a3 = PL3.a, b3 = PL3.b, c3 = PL3.c, d3 = PL3.d;
	double a4 = PL4.a, b4 = PL4.b, c4 = PL4.c, d4 = PL4.d;
	double a2_bar = a2 - ((a1 * c2) / c1), b2_bar = b2 - ((b1 * c2) / c1), c2_bar = d2 - ((d1 * c2) / c1);
	double a3_bar = a3 - ((a1 * c3) / c1), b3_bar = b3 - ((b1 * c3) / c1), c3_bar = d3 - ((d1 * c3) / c1);
	double a4_bar = a4 - ((a1 * c4) / c1), b4_bar = b4 - ((b1 * c4) / c1), c4_bar = d4 - ((d1 * c4) / c1);

	double p = (V3.size() * b2_bar - V2.size() * b3_bar) / (V2.size() * a3_bar - V3.size() * a2_bar);
	double q = (V3.size() * c2_bar - V2.size() * c3_bar) / (V2.size() * a3_bar - V3.size() * a2_bar);

	double x1 = x0 - 1, y1 =
	double x2 =
	if (!PointInsideTri(Point{ x0 - 1, }) && !PointInsideTri(, Point{ x0 + 1,)) {

	}

	double y0 = ((V3.size() * (a4_bar * q + c4_bar)) - (V4.size() * (a3_bar * q + c3_bar))) / ((V4.size() * (a3_bar * p + b3_bar)) - (V3.size() * (a4_bar * p + b4_bar)));
	double x0 = p * y0 + q;

	return Point{ x0, y0, (d1 + b1y0 + a1x0) * (-1 / c1) };
	}
}


int main() {

	// tetra_center()
	// Tetra Tet1, Tet2;
	// T는 T1과 T2이 공유하는 triangular face
	// Tri T;
	Point P1{0, 1, 0}, P2{-0.5, 0, 0}, P3{0.5, 0, 0};
	Tri T{P1, P2, P3};
	Point P4{0, 2, 1};
	Point P5{0.4, 0.4, 0.4};
	Point P6{ 0.2, 0.2, 0.2 };
	Point P7{ 0.4, 0.1, 0.4 };
	Point P8{ 0, 1, 1 };
	Point P9{-0.2, -0.2, -0.2};
	// line 위에 있는 case
	// vertex 위에 있는 case
	// 둔각삼각형인 경우에도 잘 되나?

	Point arr[] = { P4, P5, P6, P7, P8, P9};
	Point* it;

	for (it = &arr[0]; it != &arr[6]; it++) {
		std::cout << FacePointDist(T, *it) << std::endl;
		
	}


	// double ans = FacePointDist(T, P0);
	

	// Point center = tetra_center(Tet1, Tet2, T, 0);
}