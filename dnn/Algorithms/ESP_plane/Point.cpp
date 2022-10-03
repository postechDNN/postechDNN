#include "Point.h"
#include <iostream>
#define ERR 1e-6

Point::Point() {}
Point::~Point() {}
Point::Point(double _x, double _y) {
	x = _x; y = _y; index = -1; edge_type = -1;
}
Point::Point(double _x, double _y, int i) {
	x = _x; y = _y; index = i; edge_type = -1;
}
Point::Point(const Point& _p) {
	this->x = _p.x;
	this->y = _p.y;
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

void Point::print() { std::cout << "Point " << x << " " << y << std::endl; }

void Point::setEdgeType(int type) {edge_type = type;}
int Point::getEdgeType() {return edge_type;}

bool Point::operator==(Point _p) {
	return std::abs(this->x - _p.getx()) < ERR && std::abs(this->y - _p.gety()) < ERR;
}

Point Point::operator- (Point _p) {
	Point p(this->x - _p.x, this->y - _p.y);
	return p;
}

double Point::getx() const {
	return this->x;
}

void Point::setx(double _x) {
	this->x = _x;
}

double Point::gety() const {
	return this->y;
}

void Point::sety(double _y) {
	this->y = _y;
}

double Point::distance(Point _p) {
	return sqrt(pow(_p.x - this->x, 2) + pow(_p.y - this->y, 2));
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
	os << '(' << p.getx() << ", " << p.gety() << ')';
	return os;
}

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