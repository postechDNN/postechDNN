#include "Vector.h"
#include <cmath>

//this class defines a vector in 2D space

// 

Vector::Vector() {
	this->x = 0;
	this->y = 0;
	/*
	origin = new Point(0.,0.);
	this->dest = new Point(0., 0.);*/
}

Vector::Vector(double x, double y): Point(x,y){};

Vector::Vector(Point& _p){
	this->x = _p.getx();
	this->y = _p.gety();
}

Vector::Vector(Point& _origin, Point& _dest) {
	this->x = _dest.getx() - _origin.getx();
	this->y = _dest.gety() - _origin.gety();
	
	/*
	this->origin = _origin;
	this->dest = _dest;*/
}

/*
Vector::Vector(Edge * _e) {
	this->origin = _e->gets();
	this->dest = _e->gett();
}*/

Vector::~Vector() {}


bool Vector::operator==(Vector& _v) {
	return abs(this->x - _v.x) < 1e-6 && abs(this->y - _v.y) < 1e-6; 
	/*
	return this->origin == _v->getOrigin() && this->dest == _v->getDest();*/
}

Vector Vector::operator- (Vector& _v) {
	return Vector(this->x - _v.x, this->y - _v.y);
	/*
	Point * _origin = new Point(this->origin - _v->getOrigin());
	Point * _dest = new Point(this->dest - _v->getDest());
	Vector _ret = Vector(_origin, _dest);
	return _ret;*/

};

Vector Vector::operator/ (double c){
	return Vector(this->x/c, this->y/c);
}

double Vector::innerProdct(Vector& _v) {
	return this->x * _v.x + this->y * _v.y;
	/*
	return ((this->getDest()-this->getOrigin()).getx()* (_v->getDest() - _v->getOrigin()).getx() + (this->getDest() - this->getOrigin()).gety()* (_v->getDest() - _v->getOrigin()).gety());
*/
};

double Vector::outerProdct(Vector& _v) {

	return this->x * _v.y - this->y * _v.x;
	/*
	Point p1 = this->dest - this->origin;
	Point p2 = _v->getDest() - _v->getOrigin();
	return p1.getx() * p2.gety() - p1.gety() * p2.getx();*/
};

/*
Point Vector::getOrigin() {
	return this->origin;
}
Point Vector::getDest() {
	return this->dest;
}
*/
double Vector::norm() {
	return sqrt(this->x * this->x + this->y *this->y);
}