#include"Rect.h"

Rect::Rect() {}

Rect::Rect(double x1, double x2, double x3, double x4)
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

Rect::Rect(double x1, double x2)
{
	this->l = x1;
	this->r = x1;
	this->t = x2;
	this->b = x2;
	this->p = new Point(x1, x2);
	this->pcheck = true;
}

Rect::Rect(Point *p)
{
	this->l = p->getx();
	this->r = p->getx();
	this->t = p->gety();
	this->b = p->gety();
	this->p = p;
	this->pcheck = true;
}

double Rect::getl() {
	return this->l;
}

double Rect::getr() {
	return this->r;
}

double Rect::gett() {
	return this->t;
}

double Rect::getb() {
	return this->b;
}

int Rect::getid() {
	return this->id;
}

bool Rect::isPoint() {
	return this->pcheck;
}

Point* Rect::getlt() {
	return this->lt;
}

Point* Rect::getlb() {
	return this->lb;
}

Point* Rect::getrt() {
	return this->rt;
}

Point* Rect::getrb() {
	return this->rb;
}

Point* Rect::getp() {
	return this->p;
}

void Rect::setid(int a) {
	this->id = a;
}
