#include "Point.h"


Point::Point(){
}
Point::Point(int _n){
    this->n=_n;
    this->xs=std::vector<double>(_n, 0);
}

Point::Point(std::vector<double> _xs){
    this->xs = _xs;
	this->n = _xs.size();
}

Point::Point(Point* _p) {
	this->xs = _p->getxs();
	this->n = this->xs.size();
}
Point::~Point() {}

bool Point::operator==(Point _p) {
	for (int i=0;i<this->n;i++){
		if (this->xs[i] != _p.getx(i)) return false;
	}
	return true;
}

double Point::getx(int _i) {
	return this->xs[_i];
}

std::vector<double> Point::getxs() {
	return this->xs;
}

void Point::setxs(std::vector<double> _xs) {
	this->xs = _xs;
}