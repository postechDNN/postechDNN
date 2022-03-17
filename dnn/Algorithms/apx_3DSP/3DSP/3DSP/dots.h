#pragma once
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

const double EPS = 1e-8;
const double eps = 2.; // 0 <= eps <= 1 by the definition
const vector<vector<int>> perm = {{0, 1, 2, 3}, {0, 2, 1, 3}, {0, 3, 1, 2}, {1, 2, 0, 3}, {1, 3, 0, 2}, {2, 3, 0, 1}}; // tetrahedron vertex permutation
// const vector<int> edge_perm = {} // tetrahedron edge permutation

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
	int sur_tet; // tetrahedron surrounding the point. defined only when the point does not lie on a face of the polygonal domain

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
		p1 = p2 = p3 = -1; // a = b = c = -1;
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
	vector<tuple<int, int, int>> neighbors; // neighboring bisectors.
	                                        // int, int, int : tetrahedron number, bisector number, face number
	int i; // bi is the j-th bisector of the i-th tetrahedron in the polygonal domain
	int j;
	vector<Segment*> bi_sgs; // steiner segments on the bisector, including an edge of the polygonal domain
	                         // (if the bisector is ABC' where A, B are vertices of the polygonal domain
							 // and C' is the midpoint of C, D, both of which are also vertices,
							 // then the bisector ABC' contains AB in its bi_sgs
}bi;

// tetrahedron
class Tetra {
private:
	int index;
	// pts[0] = p0, pts[1] = p1, pts[2] = p2, pts[3] = p3
	vector<int> pts;
	// sgs[0] : p0p1, sgs[1] : p0p2, sgs[2] : p0p3, sgs[3] : p1p2, sgs[4] : p1p3, sgs[5] : p2p3 
	int sgs[6];
	vector<int> egs;
	// fcs[0] = p0p1p2, fcs[1] = p1p2p3, fcs[2] = p0p2p3, fcs[3] = p0p1p3
	vector<int> fcs;
	vector<int> itets;

	vector<bi*> bis;
	// bis[0] = p0p1p2 | p0p1p3, bis[1] = p0p2p1 | p0p2p3, bis[2] = p0p3p1 | p0p3p2
	// bis[3] = p1p2p0 | p1p2p3, bis[4] = p1p3p0 | p1p3p2, bis[5] = p2p3p0 | p2p3p1


public:
	Tetra() { 
		// a = b = c = d = -1; 
		index = -1;
	}
	Tetra(vector<int> _pts, vector<int> _egs, 
	      vector<int> _fcs, int _index, int* ordered_sgs) {
		pts = _pts;
		// p1 = _p1; p2 = _p2; p3 = _p3; p4 = _p4;
		// t1 = _t1; t2 = _t2; t3 = _t3;
		egs = _egs;
		fcs = _fcs;
		index = _index;
		copy(ordered_sgs, ordered_sgs + 6, sgs);
	}
	int getTri(int i) {
		if (0 <= i && i <= 3) { return fcs[i]; }
		else { throw "function Tetra::getTri(i) has input parameter range 0 <= i <= 3"; }
	}

	int getSg(int i) {
		if (0 <= i && i <= 5) { return sgs[i]; }
		else { throw "function Tetra::getSg(i) has input parameter range 0 <= i <= 5"; }
	}

	vector<int> get_fcs() { return fcs; }

	int getPoint(int i) {
		if (0 <= i && i <= 3) { return pts[i]; }
		else { throw "function Tetra::getPoint(i) has input parameter range 0 <= i <= 3"; }
	}

	void add_bis(bi* _bi) {
		bis.push_back(_bi);
	}

	/*
	void add_bi_sg(int i, Segment* S) {
		bis[i].bi_sgs.push_back(*S);
	}
	*/

	void update_bi(int i, tuple<int, int, int> tup) {
		((*(bis[i])).neighbors).push_back(tup);
	}

	bi* get_bi(int i) {
		return bis[i];
	}

	/*
	bi get_bi(int i) {
		return bis[i];
	}
	*/

	vector<bi*> get_bis() {
		return bis;
	}

	int getindex() {return index;}

	void add_itets(int tet) { itets.push_back(tet); }
	vector<int> get_itets() { return itets; }

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
	vector<Segment*> sgs;
	vector<Tri> fcs;
	vector<Tetra> tets;
	int num_seg;
	Point query;

public:
	PolyDomain() { pts = {}; sgs = {}; fcs = {}; tets = {};}
	PolyDomain(vector<Point>& _pts, vector<Segment*> _sgs, vector<Tri>& _fcs, vector<Tetra>& _tets) {
		pts = _pts; sgs = _sgs; fcs = _fcs; tets = _tets;
		num_seg = sgs.size();
	}
	
	void Initialize() {
		init_neigh_sgs(); // 
		MarkPoints(); // Mark Steiner pts
		ConnectSgs();
		SetSgs();
		// CountEmptySegs();
	}

	vector<Point> get_pts() const {
		return pts;
	}
	Point get_pt(int num) const {
		return pts[num];
	}
	vector<Segment*> get_sgs() const {
		return sgs;
	}
	Segment* get_sg(int num) {
		return sgs[num];
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


	// void MarkPoints(int a, int b, int c, int d, int Tet_index, int sg_num);
	void MarkPoints_Bi(int i, int j);
	void MarkPoints_Sg(int i);
	void MarkPoints();

	void ConnectSgs();
	int Ln_Search(int, pair<Segment*,int>);

	void SetSgs();
	bool inTet(int i, Point p);
	void BuildSPM(Point p0);
	double shortest(Point q);
	Point Nearest(Point q, vector<Point>& P);

	void Clear();
	void CountEmptySegs();
};

vector<pair<Segment*, int>> Remove_Dup(vector<pair<Segment*, int>> ps); // remove duplicates by sorting + linear search
Point perpen(Point A, Point B, Point P);