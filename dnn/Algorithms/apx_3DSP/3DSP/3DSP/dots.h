#pragma once
#include <iostream>

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

	void print() {
		std::cout << this->x << " " << this->y << " " << this->z << std::endl;
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

MyVec Points2Vec(Point P1, Point P2) {
	return MyVec(P2.x - P1.x, P2.y - P1.y, P2.z - P1.z);
}

MyVec OuterProd(MyVec V1, MyVec V2) {
	double a1 = V1.x, a2 = V1.y, a3 = V1.z, b1 = V2.x, b2 = V2.y, b3 = V2.z;
	return MyVec{ a2 * b3 - a3 * b2, -(a1 * b3 - a3 * b1), a1 * b2 - a2 * b1 };
}

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