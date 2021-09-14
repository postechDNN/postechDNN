#pragma once
#include<vector>

class dPoint{
protected:
	int d;
	std::vector<double>* cds;
public:
	dPoint();
	dPoint(double,double,double);
	dPoint(std::vector<double>*);
	dPoint(dPoint*);
	~dPoint();

	bool operator==(dPoint);
	int getd();
	double getx();
	double gety();
	double getz();
	double getc(int i);
	
	auto cbegin() const
	{return this->cds->cbegin();}
	auto cend() const
	{return this->cds->cend();}	
};