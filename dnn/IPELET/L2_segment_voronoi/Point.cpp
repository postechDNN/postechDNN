#include "Point.h"
#include "MACRO.h"

#include <cmath>

Point::Point() {
	pos[0] = 0;
	pos[1] = 0;
}

Point::Point(double x, double y) {
	pos[0] = x;
	pos[1] = y;
}

Point::Point(Point* _p) {
	pos[0] = _p->getX();
	pos[1] = _p->getY();
}

Point::~Point() {};

void Point::setX(double x) {
	pos[0] = x;
}
void Point::setY(double y) {
	pos[1] = y;
}
void Point::setPos(double x, double y) {
	pos[0] = x;
	pos[1] = y;
}

double Point::getX() {
	return pos[0];
}
double Point::getY() {
	return pos[1];
}

//Jaehoon
Point Point::operator- (Point operand) {
	Point ret(pos[0] - operand.getX(), pos[1] - operand.getY());

	return ret;
}

bool Point::operator == (Point operand) {
	if (std::abs(pos[0] - operand.getX()) + std::abs(pos[1] - operand.getY()) < 1e-6)
		return true;
	else
		return false;
}

bool Point::operator !=(Point operand) {
	return !(*this == operand);
}


double Point::dot_product(Point operand) {
	return pos[0] * operand.getX() + pos[1] * operand.getY();
}


Point Point::operator *(double x) {	//Scalar Multiplication
	return Point(x * pos[0], x * pos[1]);
}

Point Point::operator+ (Point operand) {
	return Point(pos[0] + operand.getX(), pos[1] + operand.getY());
}

bool Point::operator< (Point operand) {
	if (pos[0] + 1e-6 < operand.getX())
		return true;
	else if (std::abs(pos[0] - operand.getX()) < 1e-6) return pos[1] + 1e-6 < operand.getY();

	return false;

}

double Point::operator *(Point op) {
	return pos[0] * op.getX() + pos[1] * op.getY();
}

double Point::norm() {
	return std::sqrt(pos[0] * pos[0] + pos[1] * pos[1]);
}

double Point::computeDist(Point p) { // Distance between another point p
	return sqrt(pow(pos[0] - p.getX(), 2) + pow(pos[1] - p.getY(), 2));
}

void Point::translation(double origin[]) {
	pos[0] -= origin[0];
	pos[1] -= origin[1];
}

void Point::rotation(double radian) {
	double tempX, tempY;
	tempX = pos[0] * std::cos(radian) + pos[1] * std::sin(radian);
	tempY = pos[1] * std::cos(radian) - (pos[0] * std::sin(radian));
	pos[0] = tempX;
	pos[1] = tempY;
}

int Point::ccw(Point b, Point c) { // -1 == CW, 0 == colinear, 1 == CCW
	double result = pos[0] * b.getY() + b.getX() * c.getY() + c.getX() * pos[1];
	result = result - pos[1] * b.getX() - b.getY() * c.getX() - c.getY() * pos[0];

	if (result > 0) return CCW;
	else if (result < 0) return CW;
	else return CO;
}

Point* Point::getMedian(Point p) {
	Point* result = new Point((pos[0] - p.getX()) / 2, (pos[1] - p.getY()) / 2);
	return result;
}

Point Point::operator =(Point p) {
	pos[0] = p.getX();
	pos[1] = p.getY();
	return this;
}