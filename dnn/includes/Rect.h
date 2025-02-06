#pragma once
#include"DCEL/Point.h"
using namespace std;

class Rect {
protected:
	double l,r,u,d; // four double values indicating coordinates of each sides of rectangle
	Point *ld, *lu, *rd, *ru; // four corners
	int id; // id of rectangle
public:
	Rect(double, double, double, double); // constructor with four sides, {left, bottom, right, top}
	~Rect(); // destructor
	double getl(void); // get left value of rectangle
	double getr(void); // get right value of rectangle
	double getu(void); // get top value of rectangle
	double getd(void); // get bottom value of rectangle
	const bool operator==(Rect&); // return true if two rectangles are same
	Point* getld(void); // get the left-bottom corner of rectangle
	Point* getlu(void); // get the left-top corner of rectangle
	Point* getrd(void); // get the right-bottom corner of rectangle
	Point* getru(void); // get the right-top corner of rectangle
	int getid(); // get id of rectangle
	void setid(int); //set id of rectangle
};
