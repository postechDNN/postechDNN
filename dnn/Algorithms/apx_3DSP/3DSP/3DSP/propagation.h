#pragma once
#include <vector>
#include <set>
#include <numeric>
#include <limits>
#include <algorithm>
#include "dots.h"

using namespace std;

struct PointOnSeg;

class Segment {
private:
	MyVec a,b;
	vector<double> X;
	vector<double> dist;
	vector<PointOnSeg> prev;
	set<int> S;
	set<int> Sbar;
	vector<Tri> Tris; //Segment�� �����ϴ� �� face
	vector<pair<Segment*, int>> Adjs;
	vector<vector<pair<double, double>>> AdjDiagram;
	// k��° ���� O(log n)�� ��갡���� tree �����ؼ� �����ؾ� ��
	vector<set<pair<double,int>>> AddVoronoi; //set�� �� ���Ҵ� (vertex �����ġ, +���� cell�� site)
	vector<vector<double>> Near;
	
	//Steiner point generation �ϼ��� ���Ŀ� ���� ����
	void SetAdjs() {};
	void SetAdjDiagram();
	void SetNear();

	pair<double, double> Vinterval(int i, int i1, int lindex);
	pair<double, double> Interval(int i, MyVec& v1, MyVec& v2, MyVec& v3, Tri& f, Segment& l1);
	bool IsVvertex(int i, int i1, Segment& l1, double t);
	bool IsContain(int i, int i1, Segment& l1, double t);

public:
	Segment(MyVec _a, MyVec _b, vector<double> _X) {
		a = _a;
		b = _b;
		X = vector<double>(_X);
		S.clear();
		for (size_t i = 0; i < X.size(); i++)
			Sbar.insert(i);
		SetAdjs();
		SetAdjDiagram();
		SetNear();
	}
	int sizeX() {
		return X.size();
	}
	
	bool IsActive(int i);
	void UpdateV(int i, int lindex);
	void setdist(vector<double>);
};



struct PointOnSeg {
	Segment* l;
	int index;
	bool tetra;
	PointOnSeg() {
		l = nullptr;
		index = -1;
		tetra = false;
	}
};


struct Repr {
	PointOnSeg src;
	PointOnSeg dst;
	double val;
	bool operator<(const Repr& r2) const{
		return this->val < r2.val;
	}
};

