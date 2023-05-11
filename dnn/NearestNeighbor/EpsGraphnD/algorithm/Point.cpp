#include "Point.h"
#include <cmath>

//this class defines a point in 2D space
#define ERR 1e-6
Point::Point(int _n) {
	this->n = _n;
	this->xs = std::vector(_n, 0.);
	//this->x = 0.;
	//this->y = 0.;
	//this->z = 0.;
}
Point::Point(std::vector<double> _xs) {
	this->xs = _xs;
	this->n = _xs.size();
	//this->x = _x;
	//this->y = _y;
	//this->z = _z;
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
	std::vector(this->n,0.) v;
	for (int i=0;i<this->n;i++){
		v[i] = this->xs[i] - _p.getx(i);
	}
	Point p(this->xs = v, this->n = _p.getsize());
	return p;
}

double Point::getx(int _i) {
	return this->x[_i];
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

}

Grid_Point::Grid_Point() : Point() { ind = { -1, -1, -1 }; num = -1; ip = { false, false, false, false, false, false }; encl = -1; }

//Grid_Point::Grid_Point(int _x_num, int _y_num, int _z_num, double x_min, double y_min, double z_min, double eps, int eg_y, int eg_z) {
//	ind.x_ind = _x_num; ind.y_ind = _y_num; ind.z_ind = _z_num;
//	x = x_min + ind.x_ind * eps; y = y_min + ind.y_ind * eps; z = z_min + ind.z_ind * eps;
//	num = ind.x_ind * eg_y * eg_z + ind.y_ind * eg_z + ind.z_ind;
//	encl = -1;
//	ip = { false, false, false, false, false, false };
//}
