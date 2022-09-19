#pragma once
#include "SimplePolygon.h"

class PolygonalDomain {
private:
	std::vector<SimplePolygon*>* obstacles;
public:
	PolygonalDomain();
	PolygonalDomain(std::vector<SimplePolygon*>*);
	~PolygonalDomain();
	bool validPoint(Point p);
	std::vector<SimplePolygon*>* getObstacles();
};