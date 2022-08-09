#pragma once

#include "Point.h"
#include <vector>
#include <list>
#include <deque>
#include "i_quad.h"

using namespace std;
typedef tuple<int, int> tii;
typedef tuple<int, int, int, vector<tuple<int, int>>*> quadruple; // quadruple: start, end, count, path

class RP {
private:
	vector<Point*> vers; // vertices

public:
	RP();
	~RP();
	RP(vector<Point*>);
	void print();
	void addPt(Point*);
};

class STS { // status
public:
	int start, end;
	int count;
	bool conn; // true if top_path and bottom_path are connected.
	vector<tii> top_path, bot_path;
public:
	STS();
	STS(int, int, int);
	STS(int, int, int, vector<tii>, vector<tii>);
};

RP* Union(vector<i_quad*>, int);
int mySearch(vector<STS*>&, int, bool);
int lnSearch(vector<STS*>&, int, bool);
void myInsert(vector<STS*>&, int, int, int);
void myDelete(vector<STS*>&, int, int, int);
vector<int> overlap(vector<STS*>&, int, int);

bool sortSTS(STS* a, STS* b);
vector<Point*> tii2Point(vector<tii>, int);