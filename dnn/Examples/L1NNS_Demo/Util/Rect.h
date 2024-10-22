#pragma once
//#include"../../DataStructures/DCEL/Point.h"
#include "Point.h"
using namespace std;

class Rect {
protected:
	double l,r,u,d;
	Point *ld, *lu, *rd, *ru;
	int id;
public:
	Rect(double, double, double, double);
	~Rect();
	double getl(void);
	double getr(void);
	double getu(void);
	double getd(void);
	const bool operator==(Rect&);
	Point* getld(void);
	Point* getlu(void);
	Point* getrd(void);
	Point* getru(void);
	int getid();
	void setid(int);
};
