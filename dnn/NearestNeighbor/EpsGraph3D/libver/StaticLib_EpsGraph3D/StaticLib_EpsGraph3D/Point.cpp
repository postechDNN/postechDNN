#include "pch.h"
#include "Point.h"
#include <cmath>

//this class defines a point in 2D space
#define ERR 1e-6
EPS::Point::Point() {
	this->x = 0.;
	this->y = 0.;
	this->z = 0.;
}
EPS::Point::Point(double _x, double _y, double _z) {
	this->x = _x;
	this->y = _y;
	this->z = _z;
}
EPS::Point::Point(Point* _p) {
	this->x = _p->getx();
	this->y = _p->gety();
	this->z = _p->getz();
}
EPS::Point::~Point() {}

bool EPS::Point::operator==(Point _p) {
	return std::abs(this->x - _p.getx()) < ERR && std::abs(this->y - _p.gety()) < ERR && std::abs(this->z - _p.getz()) < ERR;
}

EPS::Point EPS::Point::operator- (Point _p) {
	Point p(this->x - _p.x, this->y - _p.y, this->z - _p.z);
	return p;
}

double EPS::Point::getx() {
	return this->x;
}

void EPS::Point::setx(double _x) {
	this->x = _x;
}


double EPS::Point::gety() {
	return this->y;
}

void EPS::Point::sety(double _y) {
	this->y = _y;
}


double EPS::Point::getz() {
	return this->z;
}

void EPS::Point::setz(double _z) {
	this->z = _z;
}

double EPS::Point::distance(Point _p) {
	return sqrt(pow(_p.x - this->x, 2) + pow(_p.y - this->y, 2) + pow(_p.z - this->z, 2));
}

EPS::Free_Point::Free_Point(){

}

EPS::Free_Point::Free_Point(double _x, double _y, double _z) :Point(_x, _y, _z) {

}

EPS::Grid_Point::Grid_Point() : Point() { ind = { -1, -1, -1 }; num = -1; ip = { false, false, false, false, false, false }; encl = -1; }

EPS::Grid_Point::Grid_Point(int _x_num, int _y_num, int _z_num, double x_min, double y_min, double z_min, double eps, int eg_y, int eg_z) {
	ind.x_ind = _x_num; ind.y_ind = _y_num; ind.z_ind = _z_num;
	x = x_min + ind.x_ind * eps; y = y_min + ind.y_ind * eps; z = z_min + ind.z_ind * eps;
	num = ind.x_ind * eg_y * eg_z + ind.y_ind * eg_z + ind.z_ind;
	encl = -1;
	ip = { false, false, false, false, false, false };
}
