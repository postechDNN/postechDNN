#pragma once
#include <iostream>
#include <vector>

using namespace std;

const double EPS = 1e-8;

class Segment;

struct Line {
	double m;
	double n;
};

class MyVec {
public:
	double x, y, z;
	int index;
public:
	MyVec() {
		x = y = z = index = 0;
	}

	MyVec(double x_, double y_, double z_) {
		x = x_;
		y = y_;
		z = z_;
		index = -1;
	}

	MyVec(double x_, double y_, double z_, int index_) {
		x = x_;
		y = y_;
		z = z_;
		index = index_;
	}

	double size() {
		return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
	}

	MyVec operator + (const MyVec& V) const{
		return MyVec(x + V.x, y + V.y, z + V.z);
	}

	MyVec operator - (const MyVec& V) const{
		return MyVec(x - V.x, y - V.y, z - V.z);
	}

	MyVec operator * (const double a) const{
		return MyVec(a * x, a * y, a * z);
	}

	MyVec operator / (const double a) const{
		return MyVec(x / a, y / a, z / a);
	}

	friend MyVec operator*(const double a, const MyVec& V);
};


class Point {
public:
	double x, y, z;
	int index;

public:
	Point() {
		x = y = z = index = 0;
	}

	Point(double x_, double y_, double z_) {
		x = x_;
		y = y_;
		z = z_;
		index = -1;
	}

	Point(double x_, double y_, double z_, int index_) {
		x = x_;
		y = y_;
		z = z_;
		index = index_;
	}

	operator MyVec() { return MyVec(x, y, z); }

	Point operator - (const Point& P) const{
		return Point(x - P.x, y - P.y, z - P.z);
	}

	Point operator + (const MyVec& V) const{
		return Point(x + V.x, y + V.y, z + V.z);
	}

	friend bool operator == (const Point& P1, const Point& P2) {
		return P1.x == P2.x && P1.y == P2.y && P1.z == P2.z;
	}

	Point operator* (const double c) const {
		return Point(c * x, c * y, c * z);
	}

	Point operator/ (const double c) const {
		return Point(x / c, y / c, z / c);
	}

	void print() {
		std::cout << this->x << " " << this->y << " " << this->z << " " << this->index << std::endl;
	}
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
	MyVec v1, v2, v3;
public:
	Tri() {};
	Tri(Point P1_, Point P2_, Point P3_) {
		p1 = P1_; p2 = P2_; p3 = P3_;
		v1 = MyVec(p1.x, p1.y, p1.z); v2 = MyVec(p2.x, p2.y, p2.z); v3 = MyVec(p3.x, p3.y, p3.z);
	}

	friend bool operator == (const Tri& T1, const Tri& T2) {
		return T1.p1 == T2.p1 && T1.p2 == T2.p2 && T1.p3 == T2.p3;
	}
};

// tetrahedron
class Tetra {
public:
// t1 : <p1, p2, p3>, t2 : <p2, p3, p4>, t3: <p3, p4, p1>, t4 : <p4, p1, p2>
	Tri t1, t2, t3, t4;
	Point p1, p2, p3, p4;
public:
	Tetra() {};
	Tetra(Tri T1_, Tri T2_, Tri T3_, Tri T4_) {
		t1 = T1_; t2 = T2_, t3 = T3_, t4 = T4_;
		p1 = t1.p1; p2 = t2.p1; p3 = t3.p1; p4 = t4.p1;
	}
	Tetra(Point P1_, Point P2_, Point P3_, Point P4_) {
		p1 = P1_; p2 = P2_; p3 = P3_; p4 = P4_;
		t1 = Tri(p1, p2, p3); t2 = Tri(p2, p3, p4); t3 = Tri(p3, p4, p1); t4 = Tri(p4, p1, p2);
	}

	friend bool operator == (const Tetra& Tet1, const Tetra& Tet2) {
		return Tet1.t1 == Tet2.t1 && Tet1.t2 == Tet2.t2 && Tet1.t3 == Tet2.t3 && Tet1.t4 == Tet2.t4;
	}
};

MyVec Point2Vec(Point P1);
Point Vec2Point(MyVec V1);
MyVec Points2Vec(Point P1, Point P2);

MyVec OuterProd(MyVec V1, MyVec V2);

// ax + by + cz + d = 0
class Plane {
public:
	double a, b, c, d;
public:
	Plane() {
		a = b = c = d = 0;
	}
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


Point incircle_center(Tri T);
double VecSize(MyVec V);
double PointsDist(Point P1, Point P2);
double InnerProd(MyVec V1, MyVec V2);


double PointEdgeDist(Point P0p, Edge E);
bool PointInsideTri(Tri T, Point P0);
double PointPlaneDist(Point P0, Plane PL); 
double PlaneAngle(Plane PL1, Plane PL2);
Point TetraCenter(Tetra Tet1, Tetra Tet2, Tri T);

class PolyDomain {
public:
	// Segment* Ss;
	vector<Point> pts;
	vector<Segment> edges;
	vector<Tri> faces;
	vector<Tetra> tets;
public:
	PolyDomain() { pts = {}; edges = {}; faces = {}; tets = {};}
	PolyDomain(vector<Point> pts_, vector<Segment> edges_, vector<Tri> faces_, vector<Tetra> tets_) {
		pts = pts_; edges = edges_; faces = faces_; tets = tets_;
	}
};

