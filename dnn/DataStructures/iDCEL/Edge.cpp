#include "Edge.h"
#include <algorithm>

Edge::Edge(Point* _s, Point* _t) {
	this->s = _s;
	this->t = _t;
}

Edge::~Edge() {

}

bool Edge::crossing(Edge* _e) {
	if (std::min(this->s->getx(), this->t->getx()) > std::max(_e->s->getx(), _e->t->getx()))
		return false;
	if (std::min(this->s->gety(), this->t->gety()) > std::max(_e->s->gety(), _e->t->gety()))
		return false;
	if (std::max(this->s->getx(), this->t->getx()) < std::min(_e->s->getx(), _e->t->getx()))
		return false;
	if (std::max(this->s->gety(), this->t->gety()) < std::min(_e->s->gety(), _e->t->gety()))
		return false;
	if (this->on(_e->gets()))
		return true;
	if (this->on(_e->gett()))
		return true;
	if (_e->on(this->s))
		return true;
	if (_e->on(this->t))
		return true;
	Point p1 = *(this->t) - *(this->s);
	Point p2 = *(_e->s) - *(this->s);
	Point p3 = *(_e->t) - *(this->s);
	long long int dir1 = p1.getx()*p2.gety() - p2.getx()*p1.gety();
	long long int dir2 = p1.getx()*p3.gety() - p3.getx()*p1.gety();
	if ((dir1 > 0 && dir2 < 0) || (dir1 < 0 && dir2 > 0)) {
		Point p1 = *(_e->t) - *(_e->s);
		Point p2 = *(this->s) - *(_e->s);
		Point p3 = *(this->t) - *(_e->s);
		long long int dir1 = p1.getx()*p2.gety() - p2.getx()*p1.gety();
		long long int dir2 = p1.getx()*p3.gety() - p3.getx()*p1.gety();
		if ((dir1 > 0 && dir2 < 0) || (dir1 < 0 && dir2 > 0))
			return true;
		else
			return false;
	}
	else
		return false;
}

bool Edge::on(Point* _p) {
	if (*_p == *(this->t) || *_p == *(this->s))
		return false;
	Point p1 = *(this->t) - *(this->s);
	Point p2 = *_p - *(this->s);
	long long int dir = p1.getx()*p2.gety() - p2.getx()*p1.gety();
	if (dir == 0) {
		if (p1.getx() * p2.getx() < 0)
			return false;
		if (p1.gety() * p2.gety() < 0)
			return false;
		if (std::abs(p1.getx()) < std::abs(p2.getx()))
			return false;
		if (std::abs(p1.gety()) < std::abs(p2.gety()))
			return false;
		return true;
	}
	return false;
}

Point* Edge::gets() {
	return this->s;
}

Point* Edge::gett() {
	return this->t;
}