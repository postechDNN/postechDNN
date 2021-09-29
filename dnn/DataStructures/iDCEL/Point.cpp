#include "Point.h"

Point::Point(long long int _x, long long int _y) {
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

Point Point::operator- (Point _p) {
	Point p(this->x - _p.x, this->y - _p.y);
	return p;
}

bool Point::operator<=(Point _p) {
	if (x == _p.x)
		return y <= _p.y;
	else
		return x <= _p.x;
}

long long int Point::getx() {
	return this->x;
}

long long int Point::gety() {
	return this->y;
}


