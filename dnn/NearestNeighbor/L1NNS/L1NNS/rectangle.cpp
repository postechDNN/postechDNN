#include"Rectangle.h"

Rectangle::Rectangle() {}

Rectangle::Rectangle(double x1, double x2, double x3, double x4)
{
	this->l = x1;
	this->r = x2;
	this->t = x3;
	this->b = x4;
	this->lt = new Point(x1, x3);
	this->lb = new Point(x1, x4);
	this->rt = new Point(x2, x3);
	this->rb = new Point(x2, x4);
	this->pcheck = false;
}

Rectangle::Rectangle(double x1, double x2)
{
	this->l = x1;
	this->r = x1;
	this->t = x2;
	this->b = x2;
	this->p = new Point(x1, x2);
	this->pcheck = true;
}

Rectangle::Rectangle(Point *p)
{
	this->l = p->getx();
	this->r = p->getx();
	this->t = p->gety();
	this->b = p->gety();
	this->p = p;
	this->pcheck = true;
}

double Rectangle::getl() {
	return this->l;
}

double Rectangle::getr() {
	return this->r;
}

double Rectangle::gett() {
	return this->t;
}

double Rectangle::getb() {
	return this->b;
}

int Rectangle::getid() {
	return this->id;
}

bool Rectangle::isPoint() {
	return this->pcheck;
}

Point* Rectangle::getlt() {
	return this->lt;
}

Point* Rectangle::getlb() {
	return this->lb;
}

Point* Rectangle::getrt() {
	return this->rt;
}

Point* Rectangle::getrb() {
	return this->rb;
}

Point* Rectangle::getp() {
	return this->p;
}

void Rectangle::setid(int a) {
	this->id = a;
}
