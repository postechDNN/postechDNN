#include "Point.h"


Point::Point(){
	this->n = 1;
	this->xs = std::vector<double>(1, 0);
}
Point::Point(int _n){
    this->n=_n;
    this->xs=std::vector<double>(_n, 0);
}

Point::Point(std::vector<double> _xs){
    this->xs = _xs;
	this->n = _xs.size();
}

Point::Point(int _n, double _range) {
	this->n = _n;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dis(-_range, _range);

	this->xs = std::vector<double>(_n, 0);
	for (int i = 0; i < _n; i++) {
		this->xs[i] = dis(gen);
	}
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

double Point::dist(Point x) {
	double sum = 0;
	for (int i = 0; i < this->n; i++) {
		sum += pow(this->xs[i] - x.getx(i), 2);
	}
	return sqrt(sum);
}