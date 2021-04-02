#pragma once
#include<vector>

class dPoint{
protected:
	double x, y, z;
public:
	dPoint();
	dPoint(double,double,double);
	dPoint(std::vector<double>*);
	dPoint(dPoint*);
	~dPoint();

	bool operator==(dPoint);
	double getx();
	double gety();
	double getz();
}