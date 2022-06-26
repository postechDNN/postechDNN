#include "Point.h"
#include <cmath>

//this class defines a point in 2D space
#define ERR 1e-6
Point::Point() {
	this->x = 0.;
	this->y = 0.;
	this->z = 0.;
}
Point::Point(double _x, double _y, double _z) {
	this->x = _x;
	this->y = _y;
	this->z = _z;
}
Point::Point(Point* _p) {
	this->x = _p->getx();
	this->y = _p->gety();
	this->z = _p->getz();
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

Free_Point::Free_Point(double _x, double _y, double _z) :Point(_x, _y, _z) {

}

Grid_Point::Grid_Point() : Point() { ind = { -1, -1, -1 }; num = -1; ip = { false, false, false, false, false, false }; encl = -1; }

Grid_Point::Grid_Point(int _x_num, int _y_num, int _z_num, double x_min, double y_max, double z_min, double eps, int eg_y, int eg_z) {
	ind.x = _x_num; ind.y = _y_num; ind.z = _z_num; 
	x = x_min + ind.x * eps; y = y_max - ind.y * eps; z = z_min + ind.z * eps;
	num = ind.x * eg_y * eg_z + ind.z * eg_z + ind.z;
	encl = -1;

	ip.right = ip.lower = ip.left = ip.upper = ip.higher = ip.deeper = false;
}
