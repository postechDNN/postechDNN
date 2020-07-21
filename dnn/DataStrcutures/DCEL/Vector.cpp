#include "Vector.h"
#include <math.h>

//this class defines a vector in 2D space

Vector::Vector() {
	this->origin = new Point(0.,0.);
	this->dest = new Point(0., 0.);
}

Vector::Vector(Point * _origin, Point * _dest) {
	this->origin = _origin;
	this->dest = _dest;
}

Vector::Vector(Edge * _e) {
	this->origin = _e->gets();
	this->dest = _e->gett();
}

Vector::~Vector() {}


bool Vector::operator==(Vector * _v) {
	return this->origin == _v->getOrigin() && this->dest == _v->getDest();
}

Vector Vector::operator- (Vector * _v) {
	Point * _origin = new Point(this->origin - _v->getOrigin());
	Point * _dest = new Point(this->dest - _v->getDest());
	Vector _ret = Vector(_origin, _dest);
	return _ret;

};

double Vector::innerProdct(Vector * _v) {
	return ((this->getDest()-this->getOrigin()).getx()* (_v->getDest() - _v->getOrigin()).getx() + (this->getDest() - this->getOrigin()).gety()* (_v->getDest() - _v->getOrigin()).gety());
};

double Vector::outerProdct(Vector * _v) {
	Point p1 = this->dest - this->origin;
	Point p2 = _v->getDest() - _v->getOrigin();
	return p1.getx() * p2.gety() - p1.gety() * p2.getx();
};

Point Vector::getOrigin() {
	return this->origin;
}


Point Vector::getDest() {
	return this->dest;
}

double Vector::norm() {
	Point p = this->dest - this->origin;
	return sqrt(p.getx() * p.getx() + p.gety() * p.gety());
}
