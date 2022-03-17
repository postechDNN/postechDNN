#pragma once
#include <vector>
#include <set>
#include <numeric>
#include <limits>
#include <algorithm>
#include <queue>
#include <map>
#include "dots.h"
#include "..\..\..\..\DataStructures\AVLTree\AVLTree.h"

using namespace std;

struct PointOnSeg;
struct Repr;

class Segment {
private:
	int a_ind, b_ind;
	int index;
	vector<int> ifcs;
	vector<int> itets;
	bool tetra;	            //Whether segment is an edge of tetrahedron or not
	MyVec a,b;              //A:start point, b:end point
	Tetra* tets;            //A tetrahedron which the segment contained in
	vector<double> X;       //Relative position of each point(start:0, end:1)
	vector<double> dist;    //Distance from origin to each point on the segment
	//vector<PointOnSeg> prev;
	map<double, int> S;     //Active elements in the segment
	map<double, int> Sbar;  //Inactive elements in the segment
	vector<Tri> Tris;       //Two faces which define the segment
	vector<pair<Segment*, int>> Adjs;  
	vector<int> Revs; //Adjs[lindex].first.Adjs[Revs[lindex]].first == this, Adjs[lindex].first에서 Representative update하기 위해 필요
	vector<vector<pair<double, double>>> AdjDiagram;
	vector<AVLTree<pair<double,int>>> AddVoronoi; //tree의 각 원소는 (vertex 상대위치, -방향 cell의 site)
	vector<vector<double>> Near; //The nearest positions from each point to other segments
	
	int i, j; // when tetra == false, the segment is a steiner segment on the j-th bisector 
	          // of the i-th tetrahedron in the polygonal domain

	pair<double, double> Vinterval(int i, int i1, int lindex);
	pair<double, double> Interval(int i, MyVec& v1, MyVec& v2, MyVec& v3, Tri& f, Segment* l1);
	bool IsVvertex(int i, int i1, Segment& l1, double t);
	bool IsContain(int i, int i1, Segment& l1, double t);
	void UpdateSeg(int i, int lindex, priority_queue<Repr, vector<Repr>, greater<Repr>>& Reprs);
	pair<int, double> FindRepr(int i, int lindex, pair<double, double> intv);
	void SetRepr(int i, int lindex, pair<double,double> intv, priority_queue<Repr, vector<Repr>, greater<Repr>>& Reprs);
	void SetReprInv(int i, int lindex, priority_queue<Repr, vector<Repr>, greater<Repr>>& Reprs); //Sbar에서 vertex가 사라져 representative를 수정해야 하는 경우

public:
	//Steiner point generation 완성된 이후에 구현 예정, 추후에 private로 옮기기
	//void SetAdjs() {}; //마지막에 AddVoronoi도 초기화해야함
	//void SetRevs() {}; //How to implement?
	void SetAdjDiagram(PolyDomain &D);
	void SetNear();
	Segment() {
		a_ind = b_ind = -1;
		tetra = false;
	}
	//Constructor using two points and vector
	Segment(MyVec _a, MyVec _b, vector<double> _X, int _index, bool _tetra, Tetra* _tets, int _i, int _j) {
		a = _a;
		b = _b;
		X = vector<double>(_X);
		index = _index;
		tetra = _tetra;
		tets = _tets;
		i = _i;
		j = _j;
		S.clear();
		
	}

	Segment(MyVec _a, MyVec _b, vector<double> _X, int _a_ind, int _b_ind, int _index, bool _tetra, Tetra* _tets) {
		a = _a;
		b = _b;
		X = vector<double>(_X);
		a_ind = _a_ind;
		b_ind = _b_ind;
		index = _index;
		tetra = _tetra;
		tets = _tets;

		S.clear();


		
		
		//SetAdjs();
		//SetRevs();
		//SetAdjDiagram();
		//SetNear();
		
	}
	int sizeX() {
		return X.size();
	}

	void setX(vector<double> _X) {
		this->X = _X;
	}

	vector<double> getX() {
		return X;
	}

	void seta(MyVec _a) {
		a = _a;
	}

	MyVec geta() {
		return a;
	}

	void setb(MyVec _b) {
		b = _b;
	}

	MyVec getb() {
		return b;
	}

	MyVec getPoint(int i) {
		return (1 - X[i]) * a + X[i] * b;
	}

	double getDist(int i) {
		return dist[i];
	}

	vector<int> get_ifcs() {
		return ifcs;
	}

	void set_itets(vector<int> _itets) {
		itets = _itets;
	}

	void add_itet(int tet_num) {
		itets.push_back(tet_num);
	}

	vector<int> get_itets() {
		return itets;
	}

	int geta_ind() {
		return a_ind;
	}

	int getb_ind() {
		return b_ind;
	}

	int getind() {
		return index;
	}

	void add_ifc(int fc_num) {
		ifcs.push_back(fc_num);
	}

	Tetra* get_tets() {
		return tets;
	}

	bool IsTetra() {
		return tetra;
	}
	   
	bool IsActive(int i);
	void Update(int i, double val, priority_queue<Repr, vector<Repr>, greater<Repr>>& Reprs);

	void SetSbar() {
		for (size_t i = 0; i < X.size(); i++)
		{
			Sbar[X[i]] = i;
		}
		
	}

	void initDist() {
		dist.assign(X.size(), 0.);
	}

	void initVoronoi() {
		AddVoronoi.assign(Adjs.size(), AVLTree<pair<double, int>>());
	}

	//Debug
	void AddAdjs(Segment* s, int i);
	void AddAdj(pair<Segment*, int> p) {
		Adjs.push_back(p);
	}

	int getAdjSize() {
		return Adjs.size();
	}

	pair<Segment*, int> getAdj(int i) {
		return Adjs[i];
	}

	void AddRev(int i) {
		this->Revs.push_back(i);
	}

	int Ln_Search(int i, int f);
};

struct PointOnSeg {
	Segment* l;
	int index;
	PointOnSeg() {
		l = nullptr;
		index = -1;
	}
};


struct Repr {
	PointOnSeg src;
	PointOnSeg dst;
	double val;
	bool operator>(const Repr& r2) const{
		return this->val > r2.val;
	}
};

