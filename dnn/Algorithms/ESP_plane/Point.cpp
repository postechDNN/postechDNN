#include "Point.h"
#include <iostream>

using namespace std;

Point::Point() {}
Point::~Point() {}
Point::Point(double _x, double _y) {
	x = _x; y = _y; index = -1; edge_type = -1;
}
Point::Point(double _x, double _y, int i) {
	x = _x; y = _y; index = i; edge_type = -1;
}
Point::Point(int _x, int _y, int lv) {
	x = double(pow(_x, lv)); y = double(pow(_y, lv)); index = -1;
	edge_type = -1;
}
double operator-(Point& a, Point& b) {
	return sqrt(pow(abs(a.getX() - b.getX()), 2) + pow(abs(a.getY() - b.getY()), 2));
}
double Point::getX() { return x; }
double Point::getY() { return y; }

void Point::setIndex(int _index) { index = _index; }
int Point::getIndex() { return index; }

void Point::print() { cout << "Point " << x << " " << y << endl; }
void Point::setEdgeType(int type) {edge_type = type;}
int Point::getEdgeType() {return edge_type;}

double distance(Point* a, Point* b) {
	return sqrt(pow(a->getX() - b->getX(), 2) + pow(a->getY() - b->getY(), 2));
}

bool sortPtbyXIncr(Point& a, Point& b) {
	return (a.getX() < b.getX());
}

bool sortPtbyYDecr(Point& a, Point& b) {
	return (a.getY() > b.getY());
}

int getGridIndex(double val, int lv) {
	int powered = pow(2, lv);
	double unit_val = val / double(powered);
	double up_diff = ceil(val) - val;
	double down_diff = val - floor(val);
	if (up_diff > down_diff) {
		return floor(unit_val);
	}
	else {
		return ceil(unit_val);
	}
}