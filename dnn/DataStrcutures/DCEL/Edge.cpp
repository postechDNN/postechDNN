#include "Edge.h"
#include "Point.h"
#include <algorithm>
#define ERR 1e-6

Edge::Edge() {
	this->s = new Point();
	this->t = new Point();
}


Edge::Edge(Point* _s, Point* _t) {
	this->s = _s;
	this->t = _t;
}

Edge::Edge(Edge* _e) {
	this->s = _e->gets();
	this->t = _e->gett();
}

Edge::~Edge() {

}

bool Edge::operator==(Edge _e) {
	return (*(this->gett()) == _e.gett()) && (*(this->gets()) == _e.gets()) || (*(this->gett()) == _e.gets()) && (*(this->gets()) == _e.gett());
}

bool Edge::on(Point* p) {
	double s_x = this->gets()->getx();
	double s_y = this->gets()->gety();
	double t_x = this->gett()->getx();
	double t_y = this->gett()->gety();
	double p_x = p->getx();
	double p_y = p->gety();
	double d = t_x - s_x;
	if (abs(d) < ERR) {
		if (abs(p_x - t_x) <= ERR && p_y <= std::max(t_y, s_y) && p_y >= std::min(t_y, s_y)) {
			return true;
		}
		else return false;
	}
	else {
		double y = ((t_y - s_y) / d) * (p_x - s_x) + s_y;
		if (abs(y - p_y) <= ERR && p_y <= std::max(t_y, s_y) && p_y >= std::min(t_y, s_y)) {
			return true;
		}
		else return false;
	}
}

Point* Edge::crossing(Edge* _e, bool closed = true) {

	double x_1 = this->gets()->getx();
	double y_1 = this->gets()->gety();
	double x_2 = this->gett()->getx();
	double y_2 = this->gett()->gety();
	double x_3 = _e->gets()->getx();
	double y_3 = _e->gets()->gety();
	double x_4 = _e->gett()->getx();
	double y_4 = _e->gett()->gety();

	double d = (y_4 - y_3) * (x_2 - x_1) - (x_4 - x_3) * (y_2 - y_1);
	if (d == 0) {
		if (t == 0 && closed) {
			if (this->on(_e->gets())) {
				return _e->gets();
			}
			else if (this->on(_e->gett())) {
				return _e->gett();
			}
			else if (_e->on(this->gets())) {
				return this->gets();
			}
			else if (_e->on(this->gett())) {
				return this->gett();
			}
			else return nullptr;
		}
		else return nullptr;
	}
	else {
		double t = (x_4 - x_3) * (y_1 - y_3) - (y_4 - y_3) * (x_1 - x_3);
		double s = (x_2 - x_1) * (y_1 - y_3) - (y_2 - y_1) * (x_1 - x_3);
		t = t / d;
		s = s / d;
		if (t > 1 || s > 1 || t < 0 || s < 0) {
			return nullptr;
		}
		else if (((t == 0 || t == 1) || (s == 0 || s == 1)) && !closed) {
			//printf("if open");
			return nullptr;
		}
		else {
			double x = (1 - t) * x_1 + t * x_2;
			double y = (1 - t) * y_1 + t * y_2;
			Point* P = new Point(x, y);
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

void Edge::sets(Point* _p) {
	this->s = _p;
}

void Edge::sett(Point* _p) {
	this->t = _p;
}

double Edge::length() {
	return this->s->distance(*this->t);
}
