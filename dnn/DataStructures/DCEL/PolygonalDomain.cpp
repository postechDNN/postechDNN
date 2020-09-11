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
	double min_x = 1e10;
	double min_x2 = 1e10;
	int idx = -1;
	double x = p.getx(), y = p.gety();
	bool in_polygon = false;
	for (std::vector<SimplePolygon*>::iterator i1 = this->obstacles->begin(); i1 != this->obstacles->end(); i1++) {
		SimplePolygon* s = *i1;
		std::vector<Edge*>* edges = s->getEdges();
		for (int i2 = 0; i2 < s->getEdges()->size(); i2++) {
			Edge* he = (*edges)[i2];
			Point origin = he->gets(), dest = he->gett();

			if (origin.gety() > dest.gety())
				std::swap(origin, dest);
			double x1 = origin.getx(), y1 = origin.gety();
			double x2 = dest.getx(), y2 = dest.gety();

			if (abs((y1 - y2) * (x - x1) - (y - y1) * (x1 - x2)) < 1e-6) {
				if ((std::min(y1, y2) <= y && y <= std::max(y1, y2)) && std::min(x1, x2) <= x && x <= std::max(x1, x2))
					continue;
			}

			if (y1 - 1e-6 < y && y < y2 + 1e-6 && abs(y1 - y2) > 1e-6 && (y - y1) * (x2 - x1) > (x - x1) * (y2 - y1)) {
				double tmp = (y - y1) * (x1 - x2) / (y1 - y2) + x1;
				Point tmp_org = Point(x1, y1), tmp_dest = Point(x2, y2);
				if (abs(y - y2) < 1e-6) { tmp_org = Point(x2, y2); tmp_dest = Point(x1, y1); }
				double normal_x2 = (tmp_dest.getx() - tmp_org.getx()) / tmp_dest.distance(tmp_org) + tmp_org.getx();
				if (min_x > tmp || (abs(min_x - tmp) < 1e-6 && min_x2 > x2)) {
					min_x = tmp;
					min_x2 = x2;
					idx = i2;
				}
			}
		}
		if (idx == -1) continue;
		Point org = (*edges)[idx]->gets();
		Point dest = (*edges)[idx]->gett();
		if ((dest.gety() - org.gety()) > 0)
			continue;
		else {
			in_polygon = true;
			break;
		}
	}
	return in_polygon;
}

std::vector<SimplePolygon*>* PolygonalDomain::getObstacles() {
	return this->obstacles;
}

PolygonalDomain::~PolygonalDomain(){}