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
	this->isPoint = false;
}

Rectangle::Rectangle(double x1, double x2)
{
	this->l = x1;
	this->r = x1;
	this->t = x2;
	this->b = x2;
	this->p = new Point(x1, x2);
	this->isPoint = true;
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

void Rectangle::setid(int a) {
	this->id = a;
}