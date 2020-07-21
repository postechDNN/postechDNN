#include "Point.h"
#include <cmath>

//this class defines a point in 2D space

Point::Point() {
	this->x = 0.;
	this->y = 0.;
}
Point::Point(double _x, double _y) {
	this->x = _x;
	this->y = _y;
}
Point::Point(Point* _p) {
	this->x = _p->getx();
	this->y = _p->gety();
}
Point::~Point() {}

bool Point::operator==(Point _p) {
	return this->x == _p.getx() && this->y == _p.gety();
}

double Point::getx() {
	return this->x;
}

void Point::setx(double _x) {
	this->x = _x;
}


double Point::gety() {
	return this->y;
}

void Point::sety(double _y) {
	this->y = _y;
}

double Point::distance(Point _p) {
	return sqrt(pow(_p.x - this->x, 2) + pow(_p.y - this->y, 2));
}
