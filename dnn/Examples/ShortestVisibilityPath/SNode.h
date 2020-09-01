#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class SNode {
private:
	int diagonal;
	vector<int> polygon; // polygon with edge
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
		if (children[0] == NULL) children[0] = _children;
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
	SNode * get_left_children() {
		return children[0];
	}
	SNode * get_right_children() {
		return children[1];
	}
	bool check_inclusive(int e1) {
		return find(polygon.begin(), polygon.end(), e1) != polygon.end() ? true : false;
	}
	int other_edge(int e1, int e2) {
		int ret = -1;
		for (int i = 0; i < 3; i++) {
			if (polygon[i] != e1 && polygon[i] != e2)
				return polygon[i];
		}
		return ret;
	}
};
void delete_snodes(SNode * root) {
	SNode ** children = root->get_children();

	for (int i = 0; i < 2; i++) {
		if (children[i] != NULL)
			delete_snodes(children[i]);
	}
	delete(root);

}