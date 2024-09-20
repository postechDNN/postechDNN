#pragma once

#include <vector>
#include <string>
#include <map>
#include "DCEL/DCEL.h"

class Hyperbola {
private:
	Point s, t; // endpoint
	Point translation;
	double rotation;
	double a, b; // x^2/a^2 - y^2/b^2 = 1
public:
	Hyperbola() {}
	Hyperbola(Point F1, Point F2, double weight1, double weight2) {}
	Hyperbola(Point s, Point t, Point translation, double rotation, double a, double b) 
		: s(s), t(t), translation(translation), rotation(rotation), a(a), b(b) {}
	Hyperbola(Hyperbola* HB) {
		this->s = HB->gets();
		this->t = HB->gett();
		this->translation = HB->getTranslation();
		this->rotation = HB->getRotation();
		this->a = HB->geta();
		this->b = HB->getb();
	}
	~Hyperbola() {}

	void sets(Point s) { this->s = s; }
	void sett(Point t) { this->t = t; }
	void setTranslation(Point translation) { this->translation = translation; }
	void setRotation(double rotation) { this->rotation = rotation; }
	void seta(double a) { this->a = a; }
	void setb(double b) { this->b = b; }

	Point gets() const { return this->s; }
	Point gett() const { return this->t; }
	Point getTranslation() const { return this->translation; }
	double getRotation() const { return this->rotation; }
	double geta() const { return this->a; }
	double getb() const { return this->b; }

	std::vector<Point> intersectionPoints(Hyperbola HB) {}

	void flip() {
		Point temp = this->s;
		this->s = this->t;
		this->t = temp;
	}
};
