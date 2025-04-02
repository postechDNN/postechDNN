#include "Site.h"

Site::Site(Point& _p, Edge& e) {
	this->p = _p;
	this->onEdge = e;
	this->posSlope = Edge(Point(0, 0), Point(0.1, 0));
	this->negSlope = Edge(Point(0, 0), Point(0.1, 0));
}

Site::~Site() {}

Point Site::getPoint() { return this->p; }

Edge Site::getOnEdge() { return this->onEdge; }
Edge Site::getNegSlope() { return this->negSlope; }
Edge Site::getPosSlope() { return this->posSlope; }

void Site::setOnEdge(Edge& e) { this->onEdge = e; }
void Site::setNegSlope(Edge& e) { this->negSlope = e; }
void Site::setPosSlope(Edge& e) { this->posSlope = e; }