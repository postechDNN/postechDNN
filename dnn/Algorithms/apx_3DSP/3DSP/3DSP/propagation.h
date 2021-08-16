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
	bool tetra;
	MyVec a,b;
	Tetra* tets;
	vector<double> X;
	vector<double> dist;
	vector<PointOnSeg> prev;
	map<double, int> S;
	map<double, int> Sbar;
	vector<Tri> Tris; //Segment�� �����ϴ� �� face
	vector<pair<Segment*, int>> Adjs;
	vector<int> Revs; //Adjs[lindex].first.Adjs[Revs[lindex]].first == this, Adjs[lindex].first���� Representative update�ϱ� ���� �ʿ�
	vector<vector<pair<double, double>>> AdjDiagram;
	vector<AVLTree<pair<double,int>>> AddVoronoi; //tree�� �� ���Ҵ� (vertex �����ġ, -���� cell�� site)
	vector<vector<double>> Near;
	
	

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
	void SetAdjs() {};//�������� AddVoronoi�� �ʱ�ȭ�ؾ���
	void SetRevs() {};
	void SetAdjDiagram();
	void SetNear();

	Segment(MyVec _a, MyVec _b, vector<double> _X) {
		a = _a;
		b = _b;
		X = vector<double>(_X);
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

	MyVec geta() {
		return a;
	}

	MyVec getb() {
		return b;
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

