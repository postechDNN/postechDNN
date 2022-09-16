#include "Edge.h"
#include "Face.h"
#include <iostream>

Edge::Edge() {}
Edge::~Edge() {}

Edge::Edge(Point* _p1, Point* _p2) {
	p1 = _p1; p2 = _p2;
	length = sqrt(pow(p1->getX() - p2->getX(), 2) + pow(p1->getY() - p2->getY(), 2));
}

void Edge::editP1(Point* _pt) {
	*this->p1 = *_pt;
}
void Edge::editP2(Point* _pt) {
	*this->p2 = *_pt;
}

Point* Edge::getP1() {
	return p1;
}

Point* Edge::getP2() {
	return p2;
}

void Edge::setNext(Edge* _next) { next = _next; }
Edge* Edge::getNext() { return next; }
void Edge::getPrev(Edge* _prev) { prev = _prev; }
Edge* Edge::getPrev() { return prev; }

Face* Edge::getTwin(Face* f) {
	if (f == f1) { return f2; }
	else { return f1; }
}

double Edge::getLength() {
	return length;
}

void Edge::print() {
	std::cout << std::endl;
	this->p1->print();
	this->p2->print();
}

/*
bool sortbylength(Edge& a, Edge& b) {
	return (a.getLength() < b.getLength());
}
*/

bool sortbylength(Edge* a, Edge* b) {
	return (a->getLength() < b->getLength());
}