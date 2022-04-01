#pragma once
#include "Point.h"
//#include "Edge.h"


class Vector : public Point{
public:
	Vector();
	Vector(Point );
	Vector(Point , Point);
	//Vector(Edge );	//??
	~Vector();
	bool operator==(Vector );
	Vector operator- (Vector );
	double innerProdct(Vector );
	double outerProdct(Vector );
	//Point getOrigin(void);
	//Point getDest(void);
	double norm();
};