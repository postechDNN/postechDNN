#pragma once
#include <iostream>
#include <vector>
using namespace std;

class SNode {
private:
	int diagonal;
	vector<int> polygon;
	SNode * parent;
	SNode * children[2];
	//vector<SNode> star_list;
public:
	void init() {
		parent = NULL;
		children[0] = NULL;
		children[1] = NULL;
		//star_list = vector<SNode>();
	}
	SNode() {
		init();
		diagonal = -1;
	}
	SNode(vector<int>& _polygon) {
		init();
		diagonal = -1;
		polygon.assign(_polygon.begin(), _polygon.end());
	}
	SNode(int _diagonal, vector<int>& _polygon) {
		init();
		diagonal = _diagonal;
		polygon.assign(_polygon.begin(), _polygon.end());

	}
	void set_children(SNode * _children) {
		if (children[0] != NULL) children[0] = _children;
		else children[1] = _children;
	}
	void set_parent(SNode * _parent) {
		parent = _parent;
	}
	SNode * get_parent() {
		return parent;
	}
	int get_diagonal() {
		return diagonal;
	}
	vector<int>& get_polygon_with_edge() {
		return polygon;
	}
	SNode ** get_children() {
		return children;
	}
};