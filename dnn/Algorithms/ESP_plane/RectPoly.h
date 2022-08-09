#pragma once

#include "Edge.h"
#include <vector>
// #include "basic.h"

class RectPoly {
private:
	std::vector<Edge*> edges;
	int index;
	bool is_core; // if a Point is neither src nor dst, it is an obstacle vertex.
	int lv;

public:
	RectPoly();
	~RectPoly();
	RectPoly(std::vector<Edge*>);
	RectPoly(std::vector<Edge*>, int i);
	// friend double operator-(Point& a, Point& b);
	void addEdge_front(Edge*);
	void addEdge_back(Edge*);
	void stretchEdge(bool front, Point* pt); 
	Edge* getEdge(int i);
	RectPoly* unn(RectPoly*); // union
	std::vector<Edge*> getEdges();
	int getIndex();
};

class Square : public RectPoly {
private:
	Edge* left, * right;
	Edge* upper, * lower;
	Point* ul, * ur; // upper left, upper right

public:
	Square();
	Square(Edge*, Edge*, Edge*, Edge*);
	~Square();
	Edge* getLeft(); Edge* getRight();
	Edge* getUpper(); Edge* getLower();
	Point* getUl(); Point* getUr();
};

bool sortEventbyXIncr(std::tuple<Point*, int, bool> a, std::tuple<Point*, int, bool> b); 
bool sortEventbyYDecr(std::tuple<Point*, int, bool> a, std::tuple<Point*, int, bool> b);

std::vector<RectPoly*> merge(std::vector<Square*> sqrs); // is assumed to be all squares 