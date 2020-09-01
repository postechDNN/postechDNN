#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <cmath>
#include "Point.h"
using namespace std;
#define PI 3.1415926535897931

class Node {
	friend class Tree;
private:
	int helper;
	point_type angle;
	point_type constant;
	Node* parent;
	Node* left;
	Node* right;
	Point point;
	int height;
public:
	Node(int _helper, point_type _angle, Point _point) {
		helper = _helper;
		angle = _angle;		
		height = 1;
		parent = NULL;
		left = NULL;
		right = NULL;
		point = _point;
		constant = 0;
		if (_angle != PI/2 || _angle != PI*1.5) {
			constant = _point.get_y() - tan(_angle)*_point.get_x();
		}
	}
	void copy(Node * _copy) {
		helper = _copy->helper;
		angle = _copy->angle;
		point = _copy->point;
		constant = _copy->constant;
	}

	int get_helper() { return helper; }
	void set_helper(int _helper) {
		helper = _helper;
	}
	point_type get_x(point_type y) {
		if (angle == PI / 2 || angle == PI*1.5) return point.get_x();
		else return (y - constant) / tan(angle);
	}
	point_type get_y(point_type x) {
		if (angle == 0 || angle == PI) return point.get_y();
		else return x * tan(angle) + constant;
	}
	int get_height() { return height; }
	//TO DELETE
	bool change_child(Node * old, Node * child) {
		if (left == old) {
			left = child;
		}
		else if (right == old) {
			right = child;
		}
		else return false;

		child->parent = this;
		return true;
	}
};