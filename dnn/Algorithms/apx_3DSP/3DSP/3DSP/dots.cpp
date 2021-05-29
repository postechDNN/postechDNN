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

	friend bool operator == (const Tri& T1, const Tri& T2 ) {
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
	return sqrt(pow(V.x, 2) + pow(V.y ,2) + pow(V.z, 2));
}

double PointsDist(Point P1, Point P2) {
	return sqrt(pow(P1.x- P2.x, 2) + pow(P1.y - P2.y, 2) + pow(P1.z - P2.z, 2));
}

MyVec Points2Vec(Point P1, Point P2) {
	return MyVec(P2.x - P1.x, P2.y - P1.y, P2.z - P1.z);
}

double InnerProd(MyVec V1, MyVec V2) {
	return V1.x * V2.x + V1.y * V2.y + V1.z * V2.z;
}

MyVec OuterProd(MyVec V1, MyVec V2) {
	double a1 = V1.x, a2 = V1.y, a3 = V1.z, b1 = V2.x, b2 = V2.y, b3 = V2.z;
	return MyVec{a2 * b3 - a1 * b2, -(a1 * b3 - a3 * b1), a1 * b2 - a2 * b1};
}

float PointEdgeDist(Point P0p, Edge E) {
	Point P1 = E.p1;
	Point P2 = E.p2;
	
	MyVec R = OuterProd(OuterProd(Points2Vec(P0p, P2), Points2Vec(P0p, P1)), Points2Vec(P1, P2));
	double cos_gamma = InnerProd(Points2Vec(P0p, P1), R) / (PointsDist(P0p, P1) * VecSize(R));
	double P0p_P0pp_len = PointsDist(P0p, P1) * cos_gamma;
	MyVec P0p_P0pp = R * (P0p_P0pp_len) / VecSize(R);

	Point P0pp = P0p.operator+(P0p_P0pp);

	double t = VecSize(Points2Vec(P0pp, P1)) / VecSize(Points2Vec(P2, P1));

	if (0 <= t && t <= 1) { return P0p_P0pp_len;} // P0pp is between P1 and P2
	else if (t < 0) { return 2; } // P0 is closest to P1
	else { return 3; } // P0 is closest to P2
		
}

// 점과 tetrahedron 사이의 거리. reference : 3D Distance from a Point to a Triangle
double FacePointDist(Tri T, Point P0) {
	Point P1 = T.p1, P2 = T.p2, P3 = T.p3;
	MyVec P1_P2 = Points2Vec(P1, P2), P1_P3 = Points2Vec(P1, P3);
	MyVec N_P = OuterProd(P1_P2, P1_P3);
	double cos_alpha = InnerProd(Points2Vec(P1, P0) , N_P) / (PointsDist(P1, P0) * VecSize(N_P));
	double P0_P0p_len = PointsDist(P0, P1) * cos_alpha;
	MyVec P0_P0p = N_P * (-1 * P0_P0p_len / VecSize(N_P));
	Point P0p = P0 + P0_P0p;

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
		if (InnerProd(OuterProd(Points2Vec(P0p, P1), Points2Vec(P0p, P2)), N_P)) { outside = 1; }
		else { outside = -1; }
	}
	else if(f2 > 0 && f3 < 0) {
		if (InnerProd(OuterProd(Points2Vec(P0p, P2), Points2Vec(P0p, P3)), N_P)) { outside = 2; }
		else { outside = -1; }
	}
	else {
		if (InnerProd(OuterProd(Points2Vec(P0p, P3), Points2Vec(P0p, P1)), N_P)) { outside = 3; }
		else { outside = -1; }
	}
	
	Edge E;
	switch (outside) {
		case -1 :
			return P0_P0p_len;
		case 1:
			E.p1 = P1, E.p2 = P2;
		case 2:
			E.p1 = P2, E.p2 = P3;
		case 3:
			E.p1 = P3, E.p2 = P1;
	}

	double PointEdgeRelation = PointEdgeDist(P0p, E);
	if (PointEdgeRelation == 1) { // t < 0 in PointEdgeDistance()
		return VecSize(Points2Vec(E.p1, P0));
	}
	else if (PointEdgeRelation == 2) { // t > 1 in PointEdgeDistance()
		return VecSize(Points2Vec(E.p2, P0));
	}
	else //  0 <= t <= 1 in PointEdgeDistance()
	{
		return sqrt(pow(PointEdgeRelation, 2) + pow(P0_P0p_len, 2));
	}

	// still need to handle degenerate cases
	
}



// 맞닿아 있는 두 tetrahedron이 주어졌을 때, 맞닿아 있는 삼각형 내에서 임의의 tetrahedron의 변까지의 길이를 최대화하는 삼각형 내의 점을 구하는 재귀 알고리즘
// T는 T1과 T2이 공유하는 triangular face
Point tetra_center(Tetra Tet1, Tetra Tet2, Tri T, int depth) {
	Point center = incircle_center(T);

	// T = Tet1.t4 = Tet2.t4
	double s11 = FacePointDist(Tet1.t1, center), s12 = FacePointDist(Tet1.t2, center), s13 = FacePointDist(Tet1.t3, center);
	double s21 = FacePointDist(Tet2.t1, center), s22 = FacePointDist(Tet2.t2, center), s23 = FacePointDist(Tet2.t3, center);

	double recur_1, recur_2;
	double s_min = std::min({s11, s12, s13, s21, s22, s23});
	if (s_min == s11 || s_min == s21) {
		Tri T1(T.p2, T.p3, center);
		Tri T2(T.p3, T.p1, center);
		recur_1 = tetra_center(Tet1, Tet2, T1);
		recur_2 = tetra_center(Tet1, Tet2, T2);
		
	}
	if (s_min == s12 || s_min == s22) {
		
	}
	if (s_min == s13 || s_min == s23) {

	}

	if (recur_1 < recur_2) {}

	if (depth > 5) {return ...;}
}


int main() {

	// tetra_center()
	Tetra Tet1, Tet2;
	// T는 T1과 T2이 공유하는 triangular face
	Tri T;
	
	Point center = tetra_center(Tet1, Tet2, T, 0);
}