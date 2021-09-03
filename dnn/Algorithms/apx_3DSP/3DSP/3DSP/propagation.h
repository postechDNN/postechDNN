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
	vector<int> incid_fcs;
	vector<int> incid_tets;
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
	vector<int> Revs; //Adjs[lindex].first.Adjs[Revs[lindex]].first == this, Adjs[lindex].first���� Representative update�ϱ� ���� �ʿ�
	vector<vector<pair<double, double>>> AdjDiagram;
	vector<AVLTree<pair<double,int>>> AddVoronoi; //tree�� �� ���Ҵ� (vertex �����ġ, -���� cell�� site)
	vector<vector<double>> Near; //The nearest positions from each point to other segments

	pair<double, double> Vinterval(int i, int i1, int lindex);
	pair<double, double> Interval(int i, MyVec& v1, MyVec& v2, MyVec& v3, Tri& f, Segment* l1);
	bool IsVvertex(int i, int i1, Segment& l1, double t);
	bool IsContain(int i, int i1, Segment& l1, double t);
	void UpdateSeg(int i, int lindex, priority_queue<Repr, vector<Repr>, greater<Repr>>& Reprs);
	pair<int, double> FindRepr(int i, int lindex, pair<double, double> intv);
	void SetRepr(int i, int lindex, pair<double,double> intv, priority_queue<Repr, vector<Repr>, greater<Repr>>& Reprs);
	void SetReprInv(int i, int lindex, priority_queue<Repr, vector<Repr>, greater<Repr>>& Reprs); //Sbar���� vertex�� ����� representative�� �����ؾ� �ϴ� ���

public:
	//Steiner point generation �ϼ��� ���Ŀ� ���� ����, ���Ŀ� private�� �ű��
	void SetAdjs() {}; //�������� AddVoronoi�� �ʱ�ȭ�ؾ���
	void SetRevs() {}; //How to implement?
	void SetAdjDiagram();
	void SetNear();
	Segment() {a_ind = b_ind = -1;}
	//Constructor using two points and vector
	Segment(MyVec _a, MyVec _b, vector<double> _X) {
		a = _a;
		b = _b;
		X = vector<double>(_X);
		S.clear();
		for (size_t i = 0; i < X.size(); i++)
		{
			Sbar[X[i]] = i;
		}
		dist.assign(X.size(), 0);
		SetAdjs();
		SetRevs();
		SetAdjDiagram();
		SetNear();
	}
	Segment(MyVec _a, MyVec _b, vector<double> _X, int _a_ind, int _b_ind) {
		a = _a;
		b = _b;
		X = vector<double>(_X);
		a_ind = _a_ind;
		b_ind = _b_ind;
		S.clear();
		for (size_t i = 0; i < X.size(); i++)
		{
			Sbar[X[i]]=i;
		}
		dist.assign(X.size(), 0);
		/*
		* SetAdjs();
		SetRevs();
		SetAdjDiagram();
		SetNear();
		*/
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

	vector<int> get_incid_fcs() {
		return incid_fcs;
	}

	void set_incid_tets(vector<int> _incid_tets) {
		incid_tets = _incid_tets;
	}

	vector<int> get_incid_tets() {
		return incid_tets;
	}

	int geta_ind() {
		return a_ind;
	}

	int getb_ind() {
		return b_ind;
	}

	void add_fc(int fc_num) {
		incid_fcs.push_back(fc_num);
	}

	void add_tet(int tet_num) {
		incid_tets.push_back(tet_num);
	}

	Tetra* get_tets() {
		return tets;
	}

	bool IsTetra() {
		return tetra;
	}
	   
	bool IsActive(int i);
	void Update(int i, double val, priority_queue<Repr, vector<Repr>, greater<Repr>>& Reprs);

	//Debug
	void AddAdjs(Segment* s, int i);
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

