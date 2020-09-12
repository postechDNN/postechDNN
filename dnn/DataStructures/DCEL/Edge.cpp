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
	return  (*(this->gett()) == *(_e.gett()) && *(this->gets()) == *(_e.gets()) ) || ( *(this->gett()) == *(_e.gets()) && *(this->gets()) == *(_e.gett()) );
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
		if (abs(y - p_y) <= ERR && p_x <= std::max(t_x, s_x) && p_x >= std::min(t_x, s_x)) {
			return true;
		}
		else return false;
	}
}
double inline middle_point_of_4(double num1, double num2, double num3, double num4){
	if(num1>num2) std::swap(num1,num2);
	if(num3>num4) std::swap(num3,num4);
	if(num1>num3) std::swap(num1,num3);
	if(num2>num4) std::swap(num2,num4);
	if(num2>num3) std::swap(num2,num3);
	return (num2 + num3)/2;
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
	double t = (x_4 - x_3) * (y_1 - y_3) - (y_4 - y_3) * (x_1 - x_3);	//(x1,y1) - (x3,y3) and (x3,y3) - (x4,y4)
	double s = (x_2 - x_1) * (y_1 - y_3) - (y_2 - y_1) * (x_1 - x_3);
	if (abs(d) < ERR) {	//two line segment have same slope.
		if (abs(t) < ERR) {	//two line segment lies on same line.
			if(abs(x_1-x_2) <ERR){	//Vertical Line
				if(std::max(y_1,y_2) < std::min(y_3,y_4) || std::min(y_1,y_2) > std::max(y_3,y_4))	return nullptr;
				else if(!closed && (abs(std::max(y_1,y_2) - std::min(y_3,y_4)) <ERR ||abs(std::min(y_1,y_2) - std::max(y_3,y_4))<ERR )) return nullptr;
				else return new Point(x_1,middle_point_of_4(y_1,y_2,y_3,y_4));
			}
			else{
				if(std::max(x_1,x_2) < std::min(x_3,x_4) || std::min(x_1,x_2) > std::max(x_3,x_4))	return nullptr;
				else if(!closed && (abs(std::max(x_1,x_2) - std::min(x_3,x_4)) <ERR ||abs(std::min(x_1,x_2) - std::max(x_3,x_4))<ERR )) return nullptr;
				else{
					double mid_x = middle_point_of_4(x_1,x_2,x_3,x_4);
					return new Point(mid_x, (y_1 -y_2) / (x_1-x_2) *(mid_x - x_1) + y_1);
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
		else if (((abs(t) < ERR || t == 1) || (abs(s) < ERR || abs(s-1) < ERR)) && !closed) {
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
