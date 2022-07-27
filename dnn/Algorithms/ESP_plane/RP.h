#pragma once

#include "Point.h"
#include <vector>
#include <list>
#include <deque>
#include "i_quad.h"

using namespace std;

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
RP* mymyUnion(vector<i_quad*>);
int bin_search(vector<tuple<int, int>*>,int);
int bin_insert(vector<tuple<int, int>>*, int);
int bin_delete(vector<tuple<int, int>>*, int);

int bin_insert(vector<tuple<int, int, int, tuple<int, int>>>*, int);
int bin_delete(vector<tuple<int, int, int, tuple<int, int>>>*, int);