#pragma once
#include <vector>
#include <set>
#include <numeric>
#include <limits>
#include <algorithm>
#include "dots.h"

using namespace std;

class PointOnSeg;

class Segment {
private:
	MyVec a,b;
	vector<double> X;
	vector<double> dist;
	vector<PointOnSeg> prev;
	set<int> S;
	set<int> Sbar;
	vector<Tri> Tris; //Segment를 정의하는 두 face
	vector<pair<Segment*, int>> Adjs;
	vector<vector<pair<double, double>>> AdjDiagram;
	// k번째 원소 O(log n)에 계산가능한 tree 구현해서 수정해야 함
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
	bool IsVvertex(int i, int i1, Segment& l1, double t);
	bool IsContain(int i, int i1, Segment& l1, double t);
	void UpdateV(int i, int lindex);
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

