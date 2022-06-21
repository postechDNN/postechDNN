#include "Point.h"
#include <cmath>

//this class defines a point in 2D space
#define ERR 1e-6
Point::Point() {
	this->x = 0.;
	this->y = 0.;
	this->z = 0.;
	this->encl = -1;
}
Point::Point(double _x, double _y, double _z) {
	this->x = _x;
	this->y = _y;
	this->z = _z;
	this->encl = -1;
}
Point::Point(Point* _p) {
	this->x = _p->getx();
	this->y = _p->gety();
	this->z = _p->getz();
	this->encl = -1;
}
Point::~Point() {}

bool Point::operator==(Point _p) {
	return std::abs(this->x - _p.getx()) < ERR && std::abs(this->y - _p.gety()) < ERR && std::abs(this->z - _p.getz()) < ERR;
}

Point Point::operator- (Point _p) {
	Point p(this->x - _p.x, this->y - _p.y, this->z - _p.z);
	return p;
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


double Point::getz() {
	return this->z;
}

void Point::setz(double _z) {
	this->z = _z;
}

double Point::distance(Point _p) {
	return sqrt(pow(_p.x - this->x, 2) + pow(_p.y - this->y, 2) + pow(_p.z - this->z, 2));
}

Grid_Point::Grid_Point() : Point() { ind = { -1, -1 }; num = -1; ip = { false, false, false, false, false, false }; encl = -1; }

Grid_Point::Grid_Point(int _row_num, int _col_num, int _lay_num, double x_min, double y_max, double z_min, double eps, int eg_colnum, int eg_layer) {
	ind.row = _row_num; ind.column = _col_num; ind.layer = _lay_num; 
	x = x_min + ind.column * eps; y = y_max - ind.row * eps; z = z_min + ind.layer * eps;
	num = ind.row * eg_colnum * eg_layer + ind.column * eg_layer + ind.layer;
	encl = -1;

	ip.right = ip.lower = ip.left = ip.upper = ip.higher = ip.deeper = false;
}
