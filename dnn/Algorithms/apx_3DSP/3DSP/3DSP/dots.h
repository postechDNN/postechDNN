#pragma once
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

const double EPS = 1e-8;
const double eps = 0.5; // 0 <= eps <= 1 by the definition

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
	int index;

	double x, y, z;
	
	vector<int> iegs;
	vector<int> ifcs;
	vector<int> itets;
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

	vector<int> get_iegs() const { return iegs; }
	void add_iegs(int eg) { iegs.push_back(eg); }
	vector<int> get_ifcs() const { return ifcs; }
	void add_ifcs(int fc) { ifcs.push_back(fc); }
	vector<int> get_itets() const { return itets; }
	void add_itets(int tet) { itets.push_back(tet); }

	int get_sur_tet() const { return sur_tet; }

	operator MyVec() { return MyVec(x, y, z); }

	Point operator - (const Point& P) const{
		return Point(x - P.x, y - P.y, z - P.z);
	}

	Point operator + (const Point& P) const {
		return Point(x + P.x, y + P.y, z + P.z);
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

// triangular face of a tetrahedron
class Tri {
private:
	int index; // index of the Tri considered among the entire Tris in the PD

	// Point p1, p2, p3;
	// MyVec v1, v2, v3;
	int p1, p2, p3; // indices of the points which are vertices of the triangular face

	vector<int> itets;

public:
	Tri() {
		// a = b = c = -1;
		index = -1;
	}
	Tri(int _p1, int _p2, int _p3, int _index) {
		index = _index;

		p1 = _p1; p2 = _p2; p3 = _p3;
	}
	/*
	Tri(Point P1_, Point P2_, Point P3_) {
		p1 = P1_; p2 = P2_; p3 = P3_;
		v1 = MyVec(p1.getx(), p1.gety(), p1.getz()); v2 = MyVec(p2.getx(), p2.gety(), p2.getz()); v3 = MyVec(p3.getx(), p3.gety(), p3.getz());
	}
	Tri(Point P1_, Point P2_, Point P3_, int _a, int _b, int _c, int _index) {
		index = _index;

		p1 = P1_; p2 = P2_; p3 = P3_;
		v1 = MyVec(p1.getx(), p1.gety(), p1.getz()); v2 = MyVec(p2.getx(), p2.gety(), p2.getz()); v3 = MyVec(p3.getx(), p3.gety(), p3.getz());
		a = _a; b = _b; c = _c;
	}
	*/

	void set_index(int i) { index = i; }
	int get_index() { return index; }

	/*
	int geta() {return a;}
	int getb() { return b; }
	int getc() { return c; }

	Point getp1() const { return p1; }
	Point getp2() const { return p2; }
	Point getp3() const { return p3; }
	*/

	int getPoint(int i) {
		if (i == 1) { return p1; }
		else if (i == 2) { return p2; }
		else if (i == 3) { return p3; }
		else { throw "function Tri::getPoint(i) has input parameter range 1 <= i <= 3"; }
	}

	void add_tet(int i) {
		itets.push_back(i);
	}

	/*
	friend bool operator == (const Tri& T1, const Tri& T2) {
		return T1.p1 == T2.p1 && T1.p2 == T2.p2 && T1.p3 == T2.p3;
	}
	*/
};

typedef struct bi { // bisector
	int f1; // the two faces whose dihedral angle is divided by the bisector 
	int f2;
	vector<tuple<int, int>> neighbors; // neighboring bisectors
}bi;

// tetrahedron
class Tetra {
private:
	int index;

	// t1 : <p1, p2, p3>, t2 : <p2, p3, p4>, t3: <p3, p4, p1>, t4 : <p4, p1, p2>
	// Tri t1, t2, t3, t4;
	// int t1, t2, t3, t4;
	// Point p1, p2, p3, p4;
	int p1, p2, p3, p4; // indices of the four points which are vertices of the tetrahedron 
	// sgs[0] : p1p2, sgs[1] : p1p3, sgs[2] : p1p4, sgs[3] : p2p3, sgs[4] : p2p4, sgs[5] : p3p4 
	int sgs[6];
	vector<int> egs;
	vector<int> fcs;
	vector<int> itets;

	vector<bi> bis;
	/*
	vector<vector<tuple<int, int>>> bis; 
	// bisectors. each bisector is defined as follows: bis[0] = p1p2p3 | p1p2p4, bis[1] = p1p3p2 | p1p3p4, ...
    // each bisector stores its neighbors.	                       
	*/


public:
	Tetra() { 
		// a = b = c = d = -1; 
		index = -1;
	}
	Tetra(int _p1, int _p2, int _p3, int _p4, vector<int> _egs, 
	      vector<int> _fcs, int _index, vector<int> nds_num, int* ordered_sgs) {
		p1 = _p1; p2 = _p2; p3 = _p3; p4 = _p4;
		// t1 = _t1; t2 = _t2; t3 = _t3;
		egs = _egs;
		fcs = _fcs;
		index = _index;
		copy(ordered_sgs, ordered_sgs + 6, sgs);
	}
	int getTri(int i) {
		if (i == 1) return fcs[0];
		else if (i == 2) return fcs[1];
		else if (i == 3) return fcs[2];
		else if (i == 4) return fcs[3];
		else { throw "function Tetra::getTri(i) has input parameter range 1 <= i <= 4"; }
	}

	int getPoint(int i) {
		if (i == 1) return p1;
		else if (i == 2) return p2;
		else if (i == 3) return p3;
		else if (i == 4) return p4;
		else { throw "function Tetra::getPoint(i) has input parameter range 1 <= i <= 4"; }
	}

	/*
	Tetra(Tri T1_, Tri T2_, Tri T3_, Tri T4_) {
		t1 = T1_; t2 = T2_, t3 = T3_, t4 = T4_;
		p1 = t1.getp1(); p2 = t2.getp1(); p3 = t3.getp1(); p4 = t4.getp1();

		vector<int> neigh_face_indices = { 1, 4, 1, 3, 4, 3, 1, 2, 4, 2, 3, 2 }; // neighboring face indices

		vector<tuple<int, int>> temp = {};
		for (int i = 0; i < neigh_face_indices.size(); i++) {
			temp.push_back(make_tuple(i / 2, getTri(neigh_face_indices[i]).get_index()));
			if (i % 2 == 1) {
				bis.push_back(temp);
				temp.clear();
			}
		}
	}
	Tetra(Point P1_, Point P2_, Point P3_, Point P4_, vector<int> _egs, vector<int> _fcs, int _index, 
	      vector<int> nds_num, int* ordered_sgs) {
		p1 = P1_; p2 = P2_; p3 = P3_; p4 = P4_;
		t1 = Tri(p1, p2, p3); t2 = Tri(p2, p3, p4); t3 = Tri(p3, p4, p1); t4 = Tri(p4, p1, p2);
		egs = _egs;
		fcs = _fcs;
		index = _index;
		a = nds_num[0]; b = nds_num[1]; c = nds_num[2]; d = nds_num[3];
		copy(ordered_sgs, ordered_sgs + 6, sgs);

		vector<int> neigh_face_indices = { 1, 4, 1, 3, 4, 3, 1, 2, 4, 2, 3, 2 }; // neighboring face indices

		vector<tuple<int, int>> temp = {};
		for (int i = 0; i < neigh_face_indices.size(); i++) {
			cout << t1.get_index() << " " << t2.get_index() << endl;
			temp.push_back(make_tuple(i / 2, getTri(neigh_face_indices[i]).get_index()));
			if (i % 2 == 1) {
				bis.push_back(temp);
				temp.clear();
			}
		}
	}

	Tri getTri(int i) {
		if (i == 1) return t1;
		else if (i == 2) return t2;
		else if (i == 3) return t3;
		else if (i == 4) return t4;
		else { throw "function Tetra::getTri(i) has input parameter range 1 <= i <= 4"; }
	}

	Point getPoint(int i) { 
		if (i == 1) return p1;
		else if (i == 2) return p2;
		else if (i == 3) return p3;
		else if (i == 4) return p4;
		else { throw "function Tetra::getPoint(i) has input parameter range 1 <= i <= 4"; }
	}
	*/

	void add_bis(bi _bi) {
		bis.push_back(_bi);
	}

	vector<bi> get_bis() {
		return bis;
	}

	int getsg(int num) { return sgs[num]; }
	vector<int> get_fcs() {return fcs;}
	int getindex() {return index;}

	void add_itets(int tet) { itets.push_back(tet); }

	friend bool operator == (const Tetra& Tet1, const Tetra& Tet2) {
		return includes(Tet1.fcs.begin(), Tet1.fcs.end(), 
		                     Tet2.fcs.begin(), Tet2.fcs.end());
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
	/*
	Plane(Tri T) {
		Point P1 = T.getp1(), P2 = T.getp2(), P3 = T.getp3();
		MyVec P1_P2 = Points2Vec(P1, P2), P1_P3 = Points2Vec(P1, P3);
		MyVec N_P = OuterProd(P1_P2, P1_P3);
		a = N_P.getx(); b = N_P.gety(); c = N_P.getz();
		d = -(a * P1.getx() + b * P1.gety() + c * P1.getz());
	}
	*/
	double geta() {return a;}
	double getb() { return b; }
	double getc() { return c; }
	double getd() { return d; }
	MyVec NormalVec() {
		return MyVec(a, b, c);
	}
};

double PlaneAngle(Plane PL1, Plane PL2);

double VecSize(MyVec V);
double PointsDist(Point P1, Point P2);
double InnerProd(MyVec V1, MyVec V2);

double PointSegDist(Point P0p, Segment S);

// bool PointInsideTri(Tri T, Point P0);
// double PointPlaneDist(Point P0, Plane PL); 
// double PlaneAngle(Plane PL1, Plane PL2);
// Point TetraCenter(Tetra Tet1, Tetra Tet2, Tri T);

class PolyDomain {
private:
	// Segment* Ss;
	vector<Point> pts;
	vector<Segment> sgs;
	vector<Tri> fcs;
	vector<Tetra> tets;

public:
	PolyDomain() { pts = {}; sgs = {}; fcs = {}; tets = {};}
	PolyDomain(vector<Point>& _pts, vector<Segment>& _sgs, vector<Tri>& _fcs, vector<Tetra>& _tets) {
		pts = _pts; sgs = _sgs; fcs = _fcs; tets = _tets;
	}
	
	vector<Point> get_pts() const {
		return pts;
	}
	Point get_pt(int num) const {
		return pts[num];
	}
	vector<Segment> get_sgs() const {
		return sgs;
	}
	Segment* get_sg(int num) {
		return &(sgs[num]);
	}
	vector<Tri> get_fcs() const {
		return fcs;
	}
	Tri get_fc(int num) const {
		return fcs[num];
	}
	vector<Tetra> get_tets() const{
		return tets;
	}
	Tetra get_tet(int num) const {
		return tets[num];
	}

	double FacePointDist(Tri T, Point P0);
	Point incircle_center(Tri T);

	void init_neigh_sgs();

	// all the 'dist' functions compute d(x) for a given point x, 
	// the minimum euclidean distance from x to any point on the boundary of D(x).
	double dist(int pt_num); // when the input point is a vertex
	double dist(Point P, int num, bool onSeg); // when the input point is not a vertex 
	// (it either lies on the interior of a segment or on the interior of a face)

	double radius(int pt_num);
	double radius(Point P, int sg_num, bool onSeg);
	
	double* radius_e(Segment S);

	vector<Segment> MarkPoints(int a, int b, int c, int d, int Tet_indx, int sg_num);
	vector<Segment> MarkPoints();
	// double dist();
};

Point perpen(Point A, Point B, Point P);