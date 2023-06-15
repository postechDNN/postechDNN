#include "Point.h"
#include <cmath>
#define ERR 1e-6

Point::Point(int _n) {
	this->n = _n;
	this->xs = std::vector<double>(_n, 0.);
}
Point::Point(std::vector<double> _xs) {
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
		if (std::abs(this->xs[i] - _p.getx(i)) > ERR) return false;
	}
	return true;
}

Point Point::operator- (Point _p) {
	int _n = this->n;
	std::vector<double> v;
	v = std::vector<double>(_n, 0.);
	for (int i=0;i<this->n;i++){
		v[i] = this->xs[i] - _p.getx(i);
	}
	Point p(this->xs = v);
	return p;
}

double Point::getx(int _i) {
	return this->xs[_i];
}

void Point::setx(int _i, double _x) {
	this->xs[_i] = _x;
}

std::vector<double> Point::getxs() {
	return this->xs;
}

void Point::setxs(std::vector<double> _xs) {
	this->xs = _xs;
}

int Point::getsize() {
	return this->n;
}


double Point::distance(Point _p) {
	int a = 0;
	for (int i=0;i<this->n;i++){
		a += pow(this->xs[i] - _p.getx(i), 2);
	}
	return sqrt(a);
}

Free_Point::Free_Point(std::vector<double> _xs) :Point(xs) {
	host = -1;
}

Grid_Point::Grid_Point(int n) : Point(n) {
	this->n = n;
	ind = vector<long long int>(n, -1);
	num = -1;
	topmost = vector<bool>(n, false);
	bottommost = vector<bool>(n, false);
	encl = -1; 
}

Grid_Point::Grid_Point(int n, vector<long long int> index, vector<double> location, vector<int> max_ind) : Point(location) {
	this->n = n;
	topmost = vector<bool>(n, false);
	bottommost = vector<bool>(n, false);
	ind = index;
	for (int i = 0; i < n; i++) {
		if (index[i] == 0) bottommost[i] = true;
		else if (index[i] == max_ind[i]) topmost[i] == true;
	}
	long long int help_num = 0;
	long long int count_num = 1;
	for (int i = 0; i < n; i++) {
		help_num += index[i] * count_num;
		count_num = max_ind[i] * count_num;
	}
	num = help_num;
	encl = -1;
}
