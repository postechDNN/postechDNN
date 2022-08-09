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
	list<Point*> vers; // vertices

public:
	RP();
	~RP();
	// RP(deque<Point*>);
	RP(list<Point*>);
	// RP(vector<Point*>);
	void print();
	void addPt(Point*);
};
// RP* myUnion(vector<List*>,int);
bool exist(Elem*, Dir);
void extend(RP*, Elem*, Dir); // left : 0, right : 1, lower : 2, upper : 3
RP* traverseUnion(vector<ElemList*>, vector<ElemList*>, int);

RP* myUnion(vector<i_quad*>);
int bin_search(vector<tuple<int, int>*>,int);
int bin_insert(vector<tuple<int, int>>*, int);
int bin_delete(vector<tuple<int, int>>*, int);

RP* mymyUnion(vector<i_quad*>);
int mySearch(vector<quadruple>*, int, bool);
int lnSearch(vector<quadruple>*, int, bool);
void myInsert(vector<quadruple>*, int, int, int);
void myDelete(vector<quadruple>*, int, int, int);
vector<int> overlap(vector<quadruple>*, int, int);