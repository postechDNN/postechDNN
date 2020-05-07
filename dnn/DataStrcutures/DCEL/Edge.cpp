#include <algorithm>
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
Point* Edge::crossing(Edge* _e, bool closed = true)
{
	//P(t) = (1-t)*this->gets + t*this->gett
	//P(s) = (1-s)*_e->gets + s*_e->gett
	double d = (_e->gett()->gety() - _e->gets()->gety()) * (this->t->getx() - this->s->getx()) - (_e->gett()->getx() - _e->gets()->getx()) * (this->t->gety() - this->s->gety());
	if (d == 0) {//2 lines are parellel
		return nullptr;
	}
	else {
		double t = (_e->gett()->getx() - _e->gets()->getx()) * (this->s->gety() - _e->gets()->gety()) - (_e->gett()->gety() - _e->gets()->gety()) * (this->s->getx() - _e->gets()->getx());
		double s = (this->t->getx() - this->s->getx()) * (this->s->gety() - _e->gets()->gety()) - (this->t->gety() - this->s->gety()) * (this->s->getx() - _e->gets()->getx());
		t = t / d;
		s = s / d;
		if (t > 1 || s > 1) {//no intersection
			return nullptr;
		}
		else if (((t == 0 || t == 1) || (s == 0 || s == 1)) && !closed) {//open 
			return nullptr;
		}
		else {
			double x = (1 - t) * this->s->getx() + t * this->t->getx();
			double y = (1 - t) * this->s->gety() + t * this->t->gety();
			Point *P = new Point(x, y);
			return P;
		}
	}
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
