#include "SimplePolygon.h"
#include "Point.h"
#include <algorithm>

SimplePolygon::SimplePolygon() {
	this->edges = new std::vector<Edge*>;
}

SimplePolygon::SimplePolygon(std::vector<Edge*>* _e) {
	this->edges = new std::vector<Edge*>;
	for (std::vector<Edge*>::iterator i = _e->begin(); i != _e->end(); i++) {
		this->edges->push_back(*i);
	}
}

SimplePolygon::SimplePolygon(std::vector<Point*>* _p) {
	this->edges = new std::vector<Edge*>;
	for (int i = 0; i < _p->size()-1; i++) {
		this->edges->push_back(new Edge((*_p)[i], (*_p)[i + 1]));
	}
	this->edges->push_back(new Edge((*_p)[_p->size() - 1], (*_p)[0]));
}



//The point p is inside polygon return 1
//The point p is on boundary return 0
//The point p is outside polygon return -1
int SimplePolygon::inPolygon(Point p) {

	double min_x = 1e10;
	double min_x2 = 1e10;
	int idx = -1;
	double x = p.getx(), y = p.gety();
	for (int i = 0; i < this->edges->size(); i++) {
		Edge* _e = (*(this->edges))[i];
		Point origin = _e->gets(), dest = _e->gett();

		if (origin.gety() > dest.gety())	//y1 <= y2
			std::swap(origin, dest);
		double x1 = origin.getx(), y1 = origin.gety();
		double x2 = dest.getx(), y2 = dest.gety();

		//Point is on boundary line
		if (abs((y1 - y2) * (x - x1) - (y - y1) * (x1 - x2)) < 1e-6) {
			if ((std::min(y1, y2) <= y && y <= std::max(y1, y2)) && std::min(x1, x2) <= x && x <= std::max(x1, x2))
				return 0;
		}

		if (y1 - 1e-6 < y && y < y2 + 1e-6 && abs(y1 - y2) > 1e-6 && (y - y1) * (x2 - x1) > (x - x1)* (y2 - y1)) {//edge is right to point p
			double tmp = (y - y1) * (x1 - x2) / (y1 - y2) + x1;
			Point tmp_org = Point(x1, y1), tmp_dest = Point(x2, y2);
			if (abs(y - y2) < 1e-6) { tmp_org = Point(x2, y2); tmp_dest = Point(x1, y1); }
			//Normalize x2
			double normal_x2 = (tmp_dest - tmp_org).getx() / (tmp_dest - tmp_org).distance(tmp_dest - tmp_org) + tmp_org.getx();
			if (min_x > tmp || (abs(min_x - tmp) < 1e-6 && min_x2 > x2)) {
				min_x = tmp;
				min_x2 = x2;
				idx = i;
			}
		}
	}
	if (idx == -1) return -1;
	Point org = (*(this->edges))[idx]->gets();
	Point dest = (*this->edges)[idx]->gett();
	if ((dest - org).gety() > 0)
		return -1;
	else return 1;
}

std::vector<Edge*>* SimplePolygon::getEdges() {
	return this->edges;
}
