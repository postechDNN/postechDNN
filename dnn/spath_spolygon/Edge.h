#pragma once
#include "SNode.h"

class Edge {
private:
	int origin;
	int dest;
	int triangle[2];
	SNode * cell;
public:
	Edge() {
		cell = NULL;
	}
	Edge(int _origin, int _dest) {
		cell = NULL;
		if (_origin < _dest) {
			origin = _origin;
			dest = _dest;
		}
		else {
			origin = _dest;
			dest = _origin;
		}
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
	void set_SNode(SNode* _snode) {
		cell = _snode;
	}
	SNode* get_SNode() {
		return cell;
	}
};
