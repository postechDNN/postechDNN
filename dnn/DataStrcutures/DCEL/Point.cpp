#include "Point.h"
#include <math.h>

//this class defines a point in 2D space

template <typename T>
Point<T>::Point() {
	this->x = T(0.);
	this->y = T(0.);
}
template <typename T>
Point<T>::Point(T _x, T _y) {
	this->x = _x;
	this->y = _y;
}
template <typename T>
Point<T>::Point(Point<T>* _p) {
	this->x = _p->getx();
	this->y = _p->gety();
}
template <typename T>
Point<T>::~Point() {}

template <typename T>
bool Point<T>::operator==(Point<T> _p) {
	return this->x == _p.getx() && this->y == _p.gety();
}

template <typename T>
T Point<T>::getx() {
	return this->x;
}

template <typename T>
T Point<T>::gety() {
	return this->y;
}

template <typename T>
double Point<T>::distance(Point<T> _p) {
	return sqrt(pow(_p.x - this->x, 2) + pow(_p.y - this->y, 2));
}
