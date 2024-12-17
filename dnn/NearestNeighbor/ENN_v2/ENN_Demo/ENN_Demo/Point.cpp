#include "pch.h"
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
Point::Point(const Point& _p) {
	this->x = _p.x;
	this->y = _p.y;
}
Point::~Point() {}

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

void Point::operator=(Point _p) {
	this->setx(_p.getx());
	this->sety(_p.gety());
}