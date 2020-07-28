#include "Edge.h"

Edge::Edge() :s(Point()), t(Point()) {}
Edge::Edge(Point s, Point t) :s(s), t(t) {}
Edge::~Edge() {};

Point Edge::getOrigin() const{
	return s;
}
Point Edge::getDest() const{
	return t;
}
Point Edge::getDir() {
	return t - s;
}

double Edge::turn_direction(Point op) {	//right : negative , left : positive
	double p_x = s.getx(), p_y = s.gety();
	double q_x = t.getx(), q_y = t.gety();
	double r_x = op.getx(), r_y = op.gety();
	double t = (p_x * q_y - p_y * q_x);
	t -= (p_x * r_y - p_y * r_x);
	t += (q_x * r_y - q_y * r_x);

	return t;
}


int Edge::intersect(Edge& op, double* i_x , double* i_y) {

	double p0_x, p0_y, p1_x, p1_y, p2_x, p2_y, p3_x, p3_y;
	double s1_x, s1_y, s2_x, s2_y;

	p0_x = this->s.getx(), p0_y = this->s.gety();
	p1_x = this->t.getx(), p1_y = this->t.gety();
	p2_x = op.getOrigin().getx(), p2_y = op.getOrigin().gety();
	p3_x = op.getDest().getx(), p3_y = op.getDest().gety();

	//https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
	s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
	s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

	if (abs(-s2_x * s1_y + s1_x * s2_y) < 1e-6) { // same slope
		return false;
		/*
if (abs(s1_x) < 1e-6) {
	if (std::max(p0_y, p1_y) < std::min(p2_y, p3_y) ||
		std::max(p2_y, p3_y) < std::min(p0_y, p1_y))
		return false;


}
else {}*/
	}

	float s, t;
	s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
	t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		// Collision detected
		if (i_x != NULL)
			*i_x = p0_x + (t * s1_x);
		if (i_y != NULL)
			*i_y = p0_y + (t * s1_y);
		return true;
	}

	return false;
}

bool Edge::contain(Point p) {
	;
}

std::ostream& operator<<(std::ostream& os,const Edge& op) {
	std::cout << op.getOrigin() << op.getDest();
	return os;
}

bool Edge::operator <(Edge& operand) {
	Point s = this->s, t = this->t;
	Point op_s = operand.s, op_t = operand.t;

	if (!(s < t)) std::swap(s, t);
	if (!(op_s < op_t)) std::swap(op_s, op_t);
	
	if (s < op_s) return true;
	else if (s == op_s) return t < op_t;

	return false;
}

bool Edge::operator ==(Edge& operand) {
	Point s = this->s, t = this->t;
	Point op_s = operand.s, op_t = operand.t;

	if (!(s < t)) std::swap(s, t);
	if (!(op_s < op_t)) std::swap(op_s, op_t);
	return s == op_s && t == op_t;
}