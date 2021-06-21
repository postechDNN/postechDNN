#pragma once
#include <vector>
#include <set>
#include <numeric>
#include <limits>
#include <algorithm>
#include "dots.h"

using namespace std;

class Segment;

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

class Segment {
private:
	MyVec a,b;
	vector<double> X;
	vector<double> dist;
	vector<PointOnSeg> prev;
	set<int> S;
	set<int> Sbar;
	vector<Tri> Tris;
	vector<pair<Segment*, int>> Adjs;
	vector<vector<pair<double, double>>> AdjDiagram;
	vector<set<pair<double,int>>> AddVoronoi; //set의 각 원소는 (vertex 상대위치, +방향 cell의 site)

public:
	Segment(MyVec _a, MyVec _b, vector<double> _X) {
		a = _a;
		b = _b;
		X = vector<double>(_X);
		S.clear();
		for (size_t i = 0; i < X.size(); i++)
			Sbar.insert(i);
	}
	int sizeX() {
		return X.size();
	}
	pair<double, double> Vinterval(int i, int i1, int lindex);
};



struct Repr {
	PointOnSeg src;
	PointOnSeg dst;
	double val;
	bool operator<(const Repr& r2) const{
		return this->val < r2.val;
	}
};

