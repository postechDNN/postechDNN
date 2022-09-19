#pragma once
#include "./..//..//Algorithms/ESP_plane/Point.h"
//#include "Edge.h"

class Vector : public Point{
public:
	Vector();
	Vector(double x, double y);
	Vector(Point& );
	Vector(Point& , Point&);
	~Vector();
	bool operator==(Vector& );
	Vector operator/(double );
	Vector operator- (Vector& );
	double innerProdct(Vector& );
	double outerProdct(Vector& );
	double norm();
};