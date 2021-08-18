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
private:
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

	double getx() const { return x; }
	double gety() const { return y; }
	double getz() const { return z; }

	double size() {
		return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
	}

	int getindex() {return index;}

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
private:
	double x, y, z;
	int index;
	vector<int> incid_egs;
	vector<int> incid_fcs;
	vector<int> incid_tets;
	int sur_tet; // tetrahedron surrounding the point

public:
	Point() {
		x = y = z = index = 0;
		sur_tet = -1;
	}

	Point(double x_, double y_, double z_) {
		x = x_;
		y = y_;
		z = z_;
		index = -1;
		sur_tet = -1;
	}

	Point(double x_, double y_, double z_, int index_) {
		x = x_;
		y = y_;
		z = z_;
		index = index_;
		sur_tet = -1;
	}

	int getindex() {return index;}
	double getx() const { return x; }
	double gety() const { return y; }
	double getz() const { return z; }

	vector<int> get_incid_egs() const { return incid_egs; }
	void add_incid_egs(int eg) { incid_egs.push_back(eg); }
	vector<int> get_incid_fcs() const { return incid_fcs; }
	void add_incid_fcs(int fc) { incid_fcs.push_back(fc); }
	vector<int> get_incid_tets() const { return incid_tets; }
	void add_incid_tets(int tet) { incid_tets.push_back(tet); }

	int get_sur_tet() const { return sur_tet; }

	operator MyVec() { return MyVec(x, y, z); }

	Point operator - (const Point& P) const{
		return Point(x - P.x, y - P.y, z - P.z);
	}

	Point operator + (const MyVec& V) const{
		return Point(x + V.getx(), y + V.gety(), z + V.getz());
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

/*
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
*/

// triangular face of a tetrahedron
class Tri {
private:
	Point p1, p2, p3;
	MyVec v1, v2, v3;
	int a, b, c; // node index
	vector<int> incid_tets;

public:
	Tri() {a = b = c = -1;}
	Tri(Point P1_, Point P2_, Point P3_) {
		p1 = P1_; p2 = P2_; p3 = P3_;
		v1 = MyVec(p1.getx(), p1.gety(), p1.getz()); v2 = MyVec(p2.getx(), p2.gety(), p2.getz()); v3 = MyVec(p3.getx(), p3.gety(), p3.getz());
	}
	Tri(Point P1_, Point P2_, Point P3_, int _a, int _b, int _c) {
		p1 = P1_; p2 = P2_; p3 = P3_;
		v1 = MyVec(p1.getx(), p1.gety(), p1.getz()); v2 = MyVec(p2.getx(), p2.gety(), p2.getz()); v3 = MyVec(p3.getx(), p3.gety(), p3.getz());
		a = _a; b = _b; c = _c;
	}

	int geta() {return a;}
	int getb() { return b; }
	int getc() { return c; }

	Point getp1() const { return p1; }
	Point getp2() const { return p2; }
	Point getp3() const { return p3; }

	void add_tet(int i) {
		incid_tets.push_back(i);
	}

	friend bool operator == (const Tri& T1, const Tri& T2) {
		return T1.p1 == T2.p1 && T1.p2 == T2.p2 && T1.p3 == T2.p3;
	}
};

// tetrahedron
class Tetra {
private:
// t1 : <p1, p2, p3>, t2 : <p2, p3, p4>, t3: <p3, p4, p1>, t4 : <p4, p1, p2>
	Tri t1, t2, t3, t4;
	Point p1, p2, p3, p4;
	int sgs[6];
	vector<int> fcs;
	vector<int> incid_tets;
	int index;
	vector<int> segs;

public:
	Tetra() {};
	Tetra(Tri T1_, Tri T2_, Tri T3_, Tri T4_) {
		t1 = T1_; t2 = T2_, t3 = T3_, t4 = T4_;
		p1 = t1.getp1(); p2 = t2.getp1(); p3 = t3.getp1(); p4 = t4.getp1();
	}
	Tetra(Point P1_, Point P2_, Point P3_, Point P4_, vector<int> _fcs, int _index) {
		p1 = P1_; p2 = P2_; p3 = P3_; p4 = P4_;
		t1 = Tri(p1, p2, p3); t2 = Tri(p2, p3, p4); t3 = Tri(p3, p4, p1); t4 = Tri(p4, p1, p2);
		fcs = _fcs;
		index = _index;
	}

	Point getp1() {return p1;}
	Point getp2() { return p2; }
	Point getp3() { return p3; }
	Point getp4() { return p4; }
	vector<int> get_fcs() {return fcs;}
	int getindex() {return index;}

	void add_incid_tets(int tet) { incid_tets.push_back(tet); }

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
private:
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
		a = N_P.getx(); b = N_P.gety(); c = N_P.getz();
		d = -(a * P1.getx() + b * P1.gety() + c * P1.getz());
	}
	Plane(Tri T) {
		Point P1 = T.getp1(), P2 = T.getp2(), P3 = T.getp3();
		MyVec P1_P2 = Points2Vec(P1, P2), P1_P3 = Points2Vec(P1, P3);
		MyVec N_P = OuterProd(P1_P2, P1_P3);
		a = N_P.getx(); b = N_P.gety(); c = N_P.getz();
		d = -(a * P1.getx() + b * P1.gety() + c * P1.getz());
	}
	double geta() {return a;}
	double getb() { return b; }
	double getc() { return c; }
	double getd() { return d; }
	MyVec NormalVec() {
		return MyVec(a, b, c);
	}
};

double PlaneAngle(Plane PL1, Plane PL2);

Point incircle_center(Tri T);
double VecSize(MyVec V);
double PointsDist(Point P1, Point P2);
double InnerProd(MyVec V1, MyVec V2);

double PointEdgeDist(Point P0p, Segment S);
double FacePointDist(Tri T, Point P0);

// bool PointInsideTri(Tri T, Point P0);
// double PointPlaneDist(Point P0, Plane PL); 
// double PlaneAngle(Plane PL1, Plane PL2);
// Point TetraCenter(Tetra Tet1, Tetra Tet2, Tri T);

class PolyDomain {
private:
	// Segment* Ss;
	vector<Point> pts;
	vector<Segment> edges;
	vector<Tri> faces;
	vector<Tetra> tets;
public:
	PolyDomain() { pts = {}; edges = {}; faces = {}; tets = {};}
	PolyDomain(vector<Point>& pts_, vector<Segment>& edges_, vector<Tri>& faces_, vector<Tetra>& tets_) {
		pts = pts_; edges = edges_; faces = faces_; tets = tets_;
	}
	
	vector<Point> get_pts() const {
		return pts;
	}
	Segment* get_edge(int num) {
		return &(edges[num]);
	}
	vector<Segment> get_edges() const {
		return edges;
	}
	vector<Tri> get_faces() const {
		return faces;
	}
	vector<Tetra> get_tets() const{
		return tets;
	}
};

