#include "PolygonalDomain.h"
#include "SimplePolygon.h"

#define MAX 1000 //¼öÁ¤

PolygonalDomain::PolygonalDomain() {
	this->obstacles = new std::vector<SimplePolygon*>();
}

PolygonalDomain::PolygonalDomain(std::vector<SimplePolygon*>* obs) {
	this->obstacles = obs;
}

bool PolygonalDomain::validPoint(Point p) {

	Point q = Point(MAX, p.gety());
	Edge e = Edge(&p, &q);
	int count = 0;

	for (std::vector<SimplePolygon*>::iterator i1 = this->obstacles->begin(); i1 != this->obstacles->end(); i1++) {
		SimplePolygon* s = *i1;
		for (std::vector<Edge*>::iterator i2 = s->getEdges()->begin; i2 != s->getEdges()->end(); i2++) {
			Edge* _e = *i2;
			if (_e->crossing(&e, true)) {
				count++;
			}
		}
		if (count % 2 == 1) {
			return false;
		}
		count = 0;
	}
	return true;

}

std::vector<SimplePolygon*>* PolygonalDomain::getObstacles() {
	return this->obstacles;
}