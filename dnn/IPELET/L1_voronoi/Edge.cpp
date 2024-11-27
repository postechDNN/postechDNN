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
bool Edge::on(Point p) {
	double s_x = this->getOrigin().getx();
	double s_y = this->getOrigin().gety();
	double t_x = this->getDest().getx();
	double t_y = this->getDest().gety();
	double p_x = p.getx();
	double p_y = p.gety();
	double d = t_x - s_x;
	if (std::abs(d) < 1e-6) {
		if (std::abs(p_x - t_x) <= 1e-6 && p_y <= std::max(t_y, s_y) && p_y >= std::min(t_y, s_y)) {
			return true;
		}
		else return false;
	}
	else {
		double y = ((t_y - s_y) / d) * (p_x - s_x) + s_y;
		if (std::abs(y - p_y) <= 1e-6 && p_x <= std::max(t_x, s_x) && p_x >= std::min(t_x, s_x)) {
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

bool Edge::crossing(Edge& op,Point *P, bool closed){

	double x_1 = this->getOrigin().getx();
	double y_1 = this->getOrigin().gety();
	double x_2 = this->getDest().getx();
	double y_2 = this->getDest().gety();
	double x_3 = op.getOrigin().getx();
	double y_3 = op.getOrigin().gety();
	double x_4 = op.getDest().getx();
	double y_4 = op.getDest().gety();

	double d = (y_4 - y_3) * (x_2 - x_1) - (x_4 - x_3) * (y_2 - y_1);	// (x1,y1) ~ (x2,y2) and (x3,y3) ~ (x4,y4)
	double t = (x_4 - x_3) * (y_1 - y_3) - (y_4 - y_3) * (x_1 - x_3);	// (x1,y1) ~ (x3,y3) and (x3,y3) ~ (x4,y4)
	double s = (x_2 - x_1) * (y_1 - y_3) - (y_2 - y_1) * (x_1 - x_3);	// (x1,y1) ~ (x2,y2) and (x1,y1) ~ (x3,y3)
	if (std::abs(d) < 1e-6) {	//two line segment have same slope.
		if (std::abs(t) < 1e-6) {	//two line segment lies on same line.
			if(std::abs(x_1-x_2) <1e-6){	//Vertical Line
				if(std::max(y_1,y_2) < std::min(y_3,y_4) || std::min(y_1,y_2) > std::max(y_3,y_4))	return false;
				else if(!closed && (std::abs(std::max(y_1,y_2) - std::min(y_3,y_4)) <1e-6 ||std::abs(std::min(y_1,y_2) - std::max(y_3,y_4))<1e-6 )) return false;
				else {
					if(P) *P= Point(x_1,middle_point_of_4(y_1,y_2,y_3,y_4));
					return true;
				}
			}
			else{
				if(std::max(x_1,x_2) < std::min(x_3,x_4) || std::min(x_1,x_2) > std::max(x_3,x_4))	return false;
				else if(!closed && (std::abs(std::max(x_1,x_2) - std::min(x_3,x_4)) <1e-6 ||std::abs(std::min(x_1,x_2) - std::max(x_3,x_4))<1e-6 )) return false;
				else{
					double mid_x = middle_point_of_4(x_1,x_2,x_3,x_4);
					if(P) *P = Point(mid_x, (y_1 -y_2) / (x_1-x_2) *(mid_x - x_1) + y_1);
					return true;
				}
			}
		}
		else return false;
	}
	else {
		t = t / d;
		s = s / d;
		if (t > 1 || s > 1 || t < 0 || s < 0) {
			return false;
		}
		else if ((std::abs(t) < 1e-6 || std::abs(t - 1.) < 1e-6 || std::abs(s) < 1e-6 || std::abs(s - 1.) < 1e-6) && !closed) {
			return false;
		}
		else {
			double x = (1 - t) * x_1 + t * x_2;
			double y = (1 - t) * y_1 + t * y_2;
			if(P) *P = Point(x, y);
			return true;
		}
	}
}

//return true if two edges are parallel, otherwise return false
//if dir variable is true, direction of edge is considered, that is (1,2) != (-1,-2)  
bool Edge::parallel(Edge &op, bool dir){
	Point p1 = this->getOrigin(), q1 = this->getDest();
	Point p2 = op.getOrigin(), q2 = op.getDest();
	double p1_x = p1.getx(), p1_y = p1.gety(), q1_x = q1.getx(), q1_y = q1.gety();
	double p2_x = p2.getx(), p2_y = p2.gety(), q2_x = q2.getx(), q2_y = q2.gety();
	
	double s = (p1_y - q1_y) * (p2_x - q2_x);
	double t = (p2_y - q2_y) * (p1_x - q1_x);
	
	if(dir){
		if(std::abs(s-t) < 1e-6){
			Point v1 = q1-p1, v2 = q2-p2;
			if(std::abs(v1.getx()) < 1e-6) //Vertical
				return v1.gety() * v2.gety() > 0 ? true : false;
			else return v1.getx() * v2.getx() > 0 ? true : false;
		}
		else return false;
	}
	else return (std::abs(s-t) < 1e-6) ? true:false;
}

//union two edges if they are parallel to and intersect with each other
Edge Edge::Union(Edge &op){
	Point p1 = this->getOrigin(), q1 = this->getDest();
	Point p2 = op.getOrigin(), q2 = op.getDest();
	//swap according to lexicographic order
	if(p1.getx() > q1.getx()) std::swap(p1,q1);
	else if(std::abs(p1.getx() - q1.getx())<1e-6 && p1.gety() > q1.gety()) std::swap(p1,q1);
	if(p2.getx() > q2.getx()) std::swap(p2,q2);
	else if(std::abs(p2.getx() - q2.getx())<1e-6 && p2.gety() > q2.gety()) std::swap(p2,q2);

	Point p,q;
	
	if (std::abs(p1.getx() - q1.getx()) < 1e-6){	//Vertical
		if(p1.gety() < p2.gety()) p = p1;
		else p = p2;
		if(q1.gety() > q2.gety()) q = q1;
		else q = q2;
	}
	else{
		if(p1.getx() < p2.getx()) p = p1;
		else p = p2;
		if(q1.getx() > q2.getx()) q = q1;
		else q = q2;
	}

	return Edge(p,q);
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