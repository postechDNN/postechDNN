#pragma once
#include "SNode.h"
#include "Point.h"
using namespace std;

class Edge {
private:
	int origin;
	bool reverse;
	int dest;
	point_type len;
	int triangle[2];
	SNode * cell;
public:
	bool operator==(Edge e2) {
		if (origin == e2.get_origin() && dest == e2.get_dest())
			return true;
		else return false;
	}
	void init() {
		reverse = false;
		origin = dest = -1;
	}
	Edge() {
		init();
		cell = NULL;
	}
	Edge(int point) {
		origin = dest = point;
		reverse = false;
		len = 0;
	}
	Edge(int _origin, int _dest) {
		cell = NULL;
		if (_origin < _dest) {
			reverse = false;
			origin = _origin;
			dest = _dest;
		}
		else {
			reverse = true;
			origin = _dest;
			dest = _origin;
		}
		Point p1 = point_list[origin];
		Point p2 = point_list[dest];
		len = (point_type)sqrt(((p1.get_x() - p2.get_x())*(p1.get_x() - p2.get_x()))
			+ ((p1.get_y() - p2.get_y())*(p1.get_y() - p2.get_y())));
	}
	bool is_reverse() {
		return reverse;
	}
	bool is_point() {
		if (origin == dest) {
			return true;
		}
		else return false;
	}
	point_type get_len() {
		return len;
	}
	int set_traingle(int t1, int t2) {
		if (t1 < t2) {
			triangle[0] = t1;
			triangle[1] = t2;
		}
	}
	int * get_traingle() {
		return triangle;
	}
	int get_origin() { return origin; }
	int get_dest() { return dest; }
	int check_same_point(Edge& e) {
		if (origin == e.get_origin() || origin == e.get_dest())
			return origin;
		if (dest == e.get_origin() || dest == e.get_dest())
			return dest;
		return -1;
	}
	int check_same_point(int p) {
		if (origin == p)
			return origin;
		if (dest == p)
			return dest;
		return -1;
	}
	void set_SNode(SNode* _snode) {
		cell = _snode;
	}
	SNode* get_SNode() {
		return cell;
	}
};
