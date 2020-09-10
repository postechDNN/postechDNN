#include "Edge.h"
#include "MACRO.h"

Edge::Edge() :s(Point()), t(Point()) {}
Edge::Edge(Point s, Point t) : s(s), t(t) { type = 0; }
Edge::Edge(Point s, Point t, Point f, Point dO, Point dR) :s(s), t(t) {
	focus = f; dOrigin = dO; dRight = dR; type = 1;
}
Edge::~Edge() {};

Point Edge::getOrigin() const {
	return s;
}
Point Edge::getDest() const {
	return t;
}
Point Edge::getDir() {
	return t - s;
}

double Edge::turn_direction(Point op) {	//right : negative , left : positive
	double p_x = s.getX(), p_y = s.getY();
	double q_x = t.getX(), q_y = t.getY();
	double r_x = op.getX(), r_y = op.getY();
	double t = (p_x * q_y - p_y * q_x);
	t -= (p_x * r_y - p_y * r_x);
	t += (q_x * r_y - q_y * r_x);

	return t;
}


bool Edge::intersect(Edge& op, double* i_x, double* i_y) {

	// Intersection between two segments
	if (type + op.type == 0) {
		double p0_x, p0_y, p1_x, p1_y, p2_x, p2_y, p3_x, p3_y;
		double s1_x, s1_y, s2_x, s2_y;

		p0_x = this->s.getX(), p0_y = this->s.getY();
		p1_x = this->t.getX(), p1_y = this->t.getY();
		p2_x = op.getOrigin().getX(), p2_y = op.getOrigin().getY();
		p3_x = op.getDest().getX(), p3_y = op.getDest().getY();

		//https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
		s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
		s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

		if (abs(-s2_x * s1_y + s1_x * s2_y) < 1e-6) { // same slope
			return false;
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

	// Intersection between parabola and segment
	else if (type + op.type == 1) {
		Edge *para, *seg;
		if (type == 1) {
			para = this;
			seg = &op;
		}
		else {
			para = &op;
			seg = this;
		}

	}

	// Intersection between two parabolas
	else {

	}
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