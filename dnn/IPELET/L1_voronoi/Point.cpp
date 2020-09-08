#include "Point.h"
#include <iostream>
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

Point::Point(Point *_p) {
	this->x = _p->getx();
	this->y = _p->gety();
}

Point::~Point() {}

double Point::getx() {
	return this->x;
}

double Point::gety() {
	return this->y;
}

double Point::distance(Point _p) {
	return sqrt(pow(_p.x - this->x, 2) + pow(_p.y - this->y, 2));
}

//Jaehoon
Point Point::operator- (Point operand) {
	Point ret(this->x - operand.x, this->y - operand.y);

	return ret;
}

bool Point::operator == (Point operand) {
	if (abs(this->x - operand.x) + abs(this->y - operand.y) < 1e-6)
		return true;
	else
		return false;
}

bool Point::operator !=(Point operand) {
	return !(*this == operand);
}


double Point::dot_product(Point operand) {
	return this->x * operand.x + this->y * operand.y;
}


Point Point::operator *(double x) {	//Scalar Multiplication
	return Point(x * this->x, x * this->y);
}

Point Point::operator+ (Point operand) {
	return Point(this->x + operand.x, this->y+operand.y);
}

bool Point::operator< (Point operand) {
	if (this->x + 1e-6 < operand.x)
		return true;
	else if (abs(this->x - operand.x) < 1e-6) return this->y+1e-6 < operand.y;

	return false;

}

double Point::operator *(Point op) {
	return this->x * op.x + this->y * op.y;
}

double Point::norm() {
	return std::sqrt(this->x * this->x + this->y * this->y);
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
	std::cout << "( " << p.x << ", " << p.y << " )";
	return os;
}