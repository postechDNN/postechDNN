#include "Edge.h"
#include "Point.h"

Edge::Edge() {
	this->s = new Point();
	this->t = new Point();
}

Edge::Edge(Point *_s, Point *_t) {
	this->s = new Point(_s);
	this->t = new Point(_t);
}

Edge::Edge(Edge *_e) {
	this->s = new Point(_e->gets());
	this->t = new Point(_e->gett());
}

Edge::~Edge() {
	delete(this->s);
	delete(this->t);
}

bool Edge::operator==(Edge _e) {
	return (*this->s == *_e.gets() && *this->t == *_e.gett()) || (*this->s == *_e.gett() && *this->t == *_e.gets());
}

Point* Edge::gets() {
	return this->s;
}

Point* Edge::gett() {
	return this->t;
}

double Edge::length() {
	return this->s->distance(*this->t);
}