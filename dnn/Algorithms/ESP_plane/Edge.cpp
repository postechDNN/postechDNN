#include "Edge.h"
#include "Face.h"
#include <iostream>
#include <algorithm>

// 

Edge::Edge() {
	this->s = Point();
	this->t = Point();
}
Edge::~Edge() {}

Edge::Edge(Point* _p1, Point* _p2) {
	p1 = _p1; p2 = _p2;
	Length = sqrt(pow(p1->getX() - p2->getX(), 2) + pow(p1->getY() - p2->getY(), 2));
	Length_inf = max(abs(p1->x - p2->x), abs(p1->y - p2->y));
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

/*
Face* Edge::getTwin(Face* f) {
	if (f == f1) { return f2; }
	else { return f1; }
}
*/

double Edge::getLength_inf() {
	return Length_inf;
}

double Edge::getLength() {
	return Length;
}

void Edge::print() {
	std::cout << std::endl;
	this->p1->print();
	this->p2->print();
}

/// Jaehoon code

Edge::Edge(const Point& _s, const Point& _t) {
	this->s = _s;
	this->t = _t;
}

Edge::Edge(const Edge& _e) {
	this->s = _e.s;
	this->t = _e.t;
}

// Edge::~Edge() {}

bool Edge::operator==(Edge& _e) {
	return ((this->gett()) == _e.gett()) && ((this->gets()) == _e.gets()) || ((this->gett()) == _e.gets()) && ((this->gets()) == _e.gett());
}

bool Edge::on(Point& p) {
	double s_x = this->gets().getx();
	double s_y = this->gets().gety();
	double t_x = this->gett().getx();
	double t_y = this->gett().gety();
	double p_x = p.getx();
	double p_y = p.gety();
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
/*double inline middle_point_of_4(double num1, double num2, double num3, double num4){
	if(num1>num2) std::swap(num1,num2);
	if(num3>num4) std::swap(num3,num4);
	if(num1>num3) std::swap(num1,num3);
	if(num2>num4) std::swap(num2,num4);
	if(num2>num3) std::swap(num2,num3);
	return (num2 + num3)/2;
}*/

std::pair<double, double> inline find_mid_points(double num1, double num2, double num3, double num4) {
	if (num1 > num2) std::swap(num1, num2);
	if (num3 > num4) std::swap(num3, num4);
	if (num1 > num3) std::swap(num1, num3);
	if (num2 > num4) std::swap(num2, num4);
	if (num2 > num3) std::swap(num2, num3);
	return std::pair<double, double>(num2, num3);
}

//If there is no crossing, return nullptr
//Else, return a pointer of the point crossed. 
Edge* Edge::crossing(Edge& _e, bool closed = true) {
	double x_1 = this->gets().getx();
	double y_1 = this->gets().gety();
	double x_2 = this->gett().getx();
	double y_2 = this->gett().gety();
	double x_3 = _e.gets().getx();
	double y_3 = _e.gets().gety();
	double x_4 = _e.gett().getx();
	double y_4 = _e.gett().gety();

	double d = (y_4 - y_3) * (x_2 - x_1) - (x_4 - x_3) * (y_2 - y_1);	// (x1,y1) ~ (x2,y2) and (x3,y3) ~ (x4,y4)
	double t = (x_4 - x_3) * (y_1 - y_3) - (y_4 - y_3) * (x_1 - x_3);	// (x1,y1) ~ (x3,y3) and (x3,y3) ~ (x4,y4)
	double s = (x_2 - x_1) * (y_1 - y_3) - (y_2 - y_1) * (x_1 - x_3);	// (x1,y1) ~ (x2,y2) and (x1,y1) ~ (x3,y3)
	if (std::abs(d) < ERR) {	//two line segment have same slope.
		if (std::abs(t) < ERR) {	//two line segment lies on same line.
			if (std::abs(x_1 - x_2) < ERR) {	//Vertical Line
				if (std::max(y_1, y_2) < std::min(y_3, y_4) || std::min(y_1, y_2) > std::max(y_3, y_4))	return nullptr;
				else if (!closed && (std::abs(std::max(y_1, y_2) - std::min(y_3, y_4)) < ERR || std::abs(std::min(y_1, y_2) - std::max(y_3, y_4)) < ERR)) return nullptr;
				else {	//Intersect!
					//return new Point(x_1,middle_point_of_4(y_1,y_2,y_3,y_4));
					std::pair<double, double> mid_pts = find_mid_points(y_1, y_2, y_3, y_4);
					return new Edge(Point(x_1, mid_pts.first), Point(x_1, mid_pts.second));
				}
			}
			else {
				if (std::max(x_1, x_2) < std::min(x_3, x_4) || std::min(x_1, x_2) > std::max(x_3, x_4))	return nullptr;
				else if (!closed && (std::abs(std::max(x_1, x_2) - std::min(x_3, x_4)) < ERR || std::abs(std::min(x_1, x_2) - std::max(x_3, x_4)) < ERR)) return nullptr;
				else {
					//double mid_x = middle_point_of_4(x_1,x_2,x_3,x_4);
					//return new Point(mid_x, (y_1 -y_2) / (x_1-x_2) *(mid_x - x_1) + y_1);
					std::pair<double, double> mid_pts = find_mid_points(x_1, x_2, x_3, x_4);
					return new Edge(Point(mid_pts.first, (y_1 - y_2) / (x_1 - x_2) * (mid_pts.first - x_1) + y_1),
						Point(mid_pts.second, (y_1 - y_2) / (x_1 - x_2) * (mid_pts.second - x_1) + y_1));
				}
			}
		}
		else return nullptr;
	}
	else {
		t = t / d;
		s = s / d;
		if (t > 1 || s > 1 || t < 0 || s < 0) {
			return nullptr;
		}
		else if ((std::abs(t) < ERR || std::abs(t - 1.) < ERR || std::abs(s) < ERR || std::abs(s - 1.) < ERR) && !closed) {
			return nullptr;
		}
		else {
			double x = (1 - t) * x_1 + t * x_2;
			double y = (1 - t) * y_1 + t * y_2;
			//Point* P = new Point(x, y);
			return new Edge(Point(x, y), Point(x, y));
		}
	}
}

Point Edge::gets() {
	return this->s;
}

Point Edge::gett() {
	return this->t;
}

void Edge::sets(Point& _p) {
	this->s = _p;
}

void Edge::sett(Point& _p) {
	this->t = _p;
}

double Edge::length() {
	return this->s.distance(this->t);
}


/*
bool sortbylength(Edge& a, Edge& b) {
	return (a.getLength() < b.getLength());
}
*/

bool sortbylength(Edge* a, Edge* b) {
	return (a->getLength() < b->getLength());
}

// sort edges in increasing order, l_inf distance
bool sortbylength_inf(Edge* a, Edge* b) {
	auto P1 = a->p1, P2 = a->p2, P3 = b->p1, P4 = b->p2;
	return max(abs(P1->x - P2->x), abs(P1->y - P2->y)) < max(abs(P3->x - P4->x), abs(P3->y - P4->y));
}