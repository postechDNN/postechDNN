#pragma once
#include "Point.h"
#include "pch.h"
//#include "Edge.h"


class Vector : public Point {
public:
	Vector();
	Vector(double x, double y);
	Vector(const Point&);
	Vector(const Point&, const Point&);
	~Vector();
	bool operator==(Vector&);
	Vector operator/(double);
	Vector operator- (Vector&);
	double innerProduct(const Vector&);
	double crossProduct(const Vector&);
	double norm();
};