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
    //Assume that every vertex and point are far from each other as much as epsilon
    double epsilon = 1e-6;
    double x = p.getx(), y = p.gety();
    int count = 0;
    for (std::vector<Edge*>::iterator i = this->edges->begin(); i != this->edges->end(); i++) {
        Edge *he = *i;
        Point origin = he->gets(), dest = he->gett();

        if (origin.gety() > dest.gety())   //y1 <= y2
            std::swap(origin, dest);
        double x1 = origin.getx(), y1 = origin.gety();
        double x2 = dest.getx(), y2 = dest.gety();

        //Point is on boundary line
        if (abs((y1 - y2) * (x - x1) - (y - y1) * (x1 - x2)) < 1e-6) {
            if (std::min(y1, y2) - 1e-6 < y && y < std::max(y1, y2) + 1e-6 && std::min(x1, x2) - 1e-6 < x && x < std::max(x1, x2) + 1e-6) {
                //std::cout << x << ' ' << y << ' ' << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << std::endl;
                return 0;
            }
        }
        //y1 != y2
        if (abs(y1 - y2) > 1e-6 && y1 < y - 1e-6 && y - 1e-6 < y2) {
            double tmp = (y - y1) * (x1 - x2) / (y1 - y2) + x1;
            if (tmp > x) count++;
        }
    }
    //std::cout << count << std::endl;
    if (count % 2) return 1;
    else return -1;
}

std::vector<Edge*>* SimplePolygon::getEdges() {
	return this->edges;
}
