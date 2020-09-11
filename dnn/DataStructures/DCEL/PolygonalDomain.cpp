#include "PolygonalDomain.h" 
#include "Point.h"
#include "SimplePolygon.h"
#include <iterator>
#include <cmath>
#include <algorithm>

PolygonalDomain::PolygonalDomain() {
	this->obstacles = new std::vector<SimplePolygon*>();
}

PolygonalDomain::PolygonalDomain(std::vector<SimplePolygon*>* obs) {
	this->obstacles = obs;
}

bool PolygonalDomain::validPoint(Point p) {
	for (std::vector<SimplePolygon*>::iterator i1 = this->obstacles->begin(); i1 != this->obstacles->end(); i1++) {
		SimplePolygon* s = *i1;
		if (s->inPolygon(p) == 1) {
			return false;
		}
	}
	return true;
}

std::vector<SimplePolygon*>* PolygonalDomain::getObstacles() {
	return this->obstacles;
}
