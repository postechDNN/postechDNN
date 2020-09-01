#pragma once
#include "Point.h"
#include <limits>

enum TYPE {
	PATH,
	BOUNDARY,
	BEND
};

class LINE {
protected:
	Point endP[2];
	float slope;
	TYPE type;
	int v;
public:
	//LINE(TYPE type, Point p1, Point p2, int v);
};

class PATH : public LINE {
	int v2;

public:
	
};

class BOUNDARY : public LINE {
	int boundary_point;
};

class BEND : public LINE {

};