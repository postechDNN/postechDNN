#include "PolygonalDomain.h"
#include "SimplePolygon.h"
#include <iterator>
#include <math.h>
#define MAX 1000 //¼öÁ¤

bool PolygonalDomain::validPoint(Point p) {

	Point q = Point(MAX, p.gety());
	Edge e = Edge(&p, &q);
	int count = 0;

	for (std::vector<SimplePolygon*>::iterator i1 = this->obstacles->begin(); i1 != this->obstacles->end(); i1++) {
		SimplePolygon* s = *i1;
		for (std::vector<Edge*>::iterator i2 = s->edges->begin; i2 != s->edges->end(); i2++) {
			Edge* _e = *i2;
			if (_e.crossing(&e, true)) {
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
