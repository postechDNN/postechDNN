#include "RectPoly.h"
#include <tuple>
#include <algorithm>
#include <queue>

using namespace std;

RectPoly::RectPoly() {}
RectPoly::~RectPoly() {}
RectPoly::RectPoly(std::vector<Edge*> _edges) {
	edges = _edges;
}
RectPoly::RectPoly(std::vector<Edge*> _edges, int i) {
	index = i;
	edges = _edges;
}

void RectPoly::addEdge_front(Edge* _eg) {
	edges.insert(edges.begin(), _eg);
}
void RectPoly::addEdge_back(Edge* _eg) {
	edges.push_back(_eg);
}

void RectPoly::stretchEdge(bool front, Point* pt){ 
	if (front) {
		if (distance(edges[0]->getP1(), pt) > distance(edges[0]->getP2(), pt)){
			edges[0]->editP2(pt);
		}
		else {
			edges[0]->editP1(pt);
		}
	}
	else {
		if (distance(edges[edges.size()-1]->getP1(), pt) > distance(edges[edges.size()-1]->getP2(), pt)) {
			edges[edges.size() - 1]->editP2(pt);
		}
		else {
			edges[edges.size() - 1]->editP1(pt);
		}
	}
}

Edge* RectPoly::getEdge(int i) {
	return edges[i];
}
std::vector<Edge*> RectPoly::getEdges() {
	return edges;
}
RectPoly* RectPoly::unn(RectPoly* rp) { // plane sweep to compute the union of two squares. only handles the case when each 
	RectPoly* ret = new RectPoly();
	RectPoly* rp1 = new RectPoly(*this);
	RectPoly* rp2 = new RectPoly(*rp);
	
	// std::vector<Edge*> &egs1 =  
	// std::vector<Edge*> &egs2 = 
	// std::sort(rp1.begin)

	return ret;
}

int RectPoly::getIndex() {
	return index;
}

Square::Square(Edge* _left, Edge* _right, Edge* _upper, Edge* _lower) {
	left = _left; right = _right; upper = _upper; lower = _lower;
}

Edge* Square::getLeft() { return left; }
Edge* Square::getRight() { return right; }
Edge* Square::getUpper() { return upper; }
Edge* Square::getLower() { return lower; }
Point* Square::getUl() { return ul; }
Point* Square::getUr() { return ur; }

bool sortEventbyXIncr(std::tuple<Point*, int, bool> a, std::tuple<Point*, int, bool> b)
{
	return std::get<Point*>(a)->getX() < std::get<Point*>(b)->getX();
}

bool sortEventbyYDecr(std::tuple<Point*, int, bool> a, std::tuple<Point*, int, bool> b)
{
	return std::get<Point*>(a)->getY() < std::get<Point*>(b)->getY();
}




// assumptions: (1) there is no isolated vertex (2) there is no hole in the resulting rectilinear polygon
std::vector<RectPoly*> merge(std::vector<Square*> sqrs) { // plane sweep to compute the union of squares of same size.
	vector<vector<int>> adj_list = {};
	// for 

	return {};
}