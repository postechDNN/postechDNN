#include "Polytope.h"
#include <queue>
#include <assert.h>
#define BUFFERSIZE 1000


using namespace std;
Edge::Edge() {
	p1 = new Point;
	p2 = new Point;
	length = 0;
}

Edge::~Edge() {
}

Edge::Edge(std::vector<Point*> vp) {
	p1 = vp[0];
	p2 = vp[1];
	length = sqrt((vp[0]->x - vp[1]->x) * (vp[0]->x - vp[1]->x) + (vp[0]->y - vp[1]->y) * (vp[0]->y - vp[1]->y) + (vp[0]->z - vp[1]->z) * (vp[0]->z - vp[1]->z));
}

Edge::Edge(Point* v1, Point* v2) {
	p1 = v1;
	p2 = v2;
	length = sqrt((v1->x - v2->x)* (v1->x - v2->x)+ (v1->y- v2->y)* (v1->y - v2->y)+ (v1->z - v2->z)* (v1->z - v2->z));
}

bool Edge::operator==(Edge e) {
	return (this->p1 == e.p1 && this->p2 == e.p2) || (this->p2 == e.p1 && this->p1 == e.p2);
}

bool Edge::below(Point* p) {
	if (p->z > p1->z || p->z > p2->z)
	{
		if ((p->x - p2->x) * (p1->y - p2->y) == (p->y - p2->y) * (p1->x - p2->x))
		{
			if (p1->x == p2->x && p1->y == p2->y) { return false; }
			else if (p1->x == p2->x)
			{
				if((p->y - p2->y) / (p1->y - p2->y) > 0 && (p->y - p2->y) / (p1->y - p2->y) < 1)
				{
					if (p->z > (p->y - p2->y) / (p1->y - p2->y) * p1->z + (1 - (p->y - p2->y) / (p1->y - p2->y)) * p2->z)
					{
						return true;
					}
				}
			}
			else 
			{
				if ((p->x - p2->x) / (p1->x - p2->x) > 0 && (p->x - p2->x) / (p1->x - p2->x) < 1)
				{
					if (p->z > (p->x - p2->x) / (p1->x - p2->x) * p1->z + (1 - (p->x - p2->x) / (p1->x - p2->x)) * p2->z)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

Face::Face() {
	face_key = new char[10];
	points.push_back(new Point());
	points.push_back(new Point());
	points.push_back(new Point());
}

Face::Face(std::vector<Point*> vp) {
	face_key = new char[10];
	points.push_back(vp[0]);
	points.push_back(vp[1]);
	points.push_back(vp[2]);
}

Face::~Face() {
}

std::vector<Point*> Face::getpoints()
{
	return points;
}

bool Face::below(Point* p) {
	// Determine if the face is below from p along z-axis.
	bool belowness = false;
	double normal[3];
	normal[0] = (points[0]->gety()- points[1]->gety())* (points[2]->getz() - points[1]->getz())
		- (points[0]->getz() - points[1]->getz()) * (points[2]->gety() - points[1]->gety());
	normal[1] = -(points[0]->getx() - points[1]->getx()) * (points[2]->getz() - points[1]->getz())
		+ (points[0]->getz() - points[1]->getz()) * (points[2]->getx() - points[1]->getx());
	normal[2] = (points[0]->getx() - points[1]->getx()) * (points[2]->gety() - points[1]->gety())
		- (points[0]->gety() - points[1]->gety()) * (points[2]->getx() - points[1]->getx());
	if (normal[2] < 0)
	{
		normal[0] = -normal[0];
		normal[1] = -normal[1];
		normal[2] = -normal[2];
	}
	if (normal[0] * p->getx() + normal[1] * p->gety() + normal[2] * p->getz() >= 
		normal[0] * points[0]->getx() + normal[1] * points[0]->gety() + normal[2] * points[0]->getz()) {
		belowness = true;
	}
	if (normal[2] == 0)
	{
		Edge temp[3] = {{ points[0], points[1] }, { points[1], points[2] }, { points[2], points[0] }	};
		if ((temp[0].below(p) || temp[1].below(p) || temp[2].below(p))&& belowness)
		{
			return true;
		}
	}
	// Check the face contains intersection of z-line contining the point and the plain expanded by the face
	bool includeness = false;
	Point proj_vec[3];
	Point p_to_vec[3];
	double out_prod[3];
	for (int i = 0; i < 3; i++)
	{
		proj_vec[i].setx(points[(i + 1)%3]->getx() - points[i]->getx());
		proj_vec[i].sety(points[(i + 1) % 3]->gety() - points[i]->gety());
		proj_vec[i].setz(0);
		p_to_vec[i].setx(points[i]->getx() - p->getx());
		p_to_vec[i].sety(points[i]->gety() - p->gety());
		p_to_vec[i].setz(0);		
	}
	for (int i = 0; i < 3; i++)
	{
		out_prod[i] = proj_vec[i].getx() * p_to_vec[i].gety() - proj_vec[i].gety() * p_to_vec[i].getx();
	}
	if (out_prod[0] * out_prod[1] > 0 && out_prod[1] * out_prod[2] > 0)
	{
		includeness = true;
	}
	if (includeness && belowness)
	{
		return true;
	}
	else {
		return false;
	}
}

bool Face::pass(Point* p1, Point* p2, int dir){
	signed int check_p1 = 1;
	signed int check_p2 = 1;
	bool possibility = false;
	double normal[3];
	double cons;
	Point proj_vec[3];
	Point p_to_vec[3];
	double out_prod[3];
	normal[0] = (points[0]->gety() - points[1]->gety()) * (points[2]->getz() - points[1]->getz())
		- (points[0]->getz() - points[1]->getz()) * (points[2]->gety() - points[1]->gety());
	normal[1] = -(points[0]->getx() - points[1]->getx()) * (points[2]->getz() - points[1]->getz())
		+ (points[0]->getz() - points[1]->getz()) * (points[2]->getx() - points[1]->getx());
	normal[2] = (points[0]->getx() - points[1]->getx()) * (points[2]->gety() - points[1]->gety())
		- (points[0]->gety() - points[1]->gety()) * (points[2]->getx() - points[1]->getx());
	if (normal[2] < 0)
	{
		normal[0] = -normal[0];
		normal[1] = -normal[1];
		normal[2] = -normal[2];
	}
	if (normal[0] * p1->getx() + normal[1] * p1->gety() + normal[2] * p1->getz() >=
		normal[0] * points[0]->getx() + normal[1] * points[0]->gety() + normal[2] * points[0]->getz()) {
		check_p1 = -1;
	}
	if (normal[0] * p2->getx() + normal[1] * p2->gety() + normal[2] * p2->getz() >=
		normal[0] * points[0]->getx() + normal[1] * points[0]->gety() + normal[2] * points[0]->getz()) {
		check_p2 = -1;
	}
	if (check_p1 * check_p2 < 0) {
		possibility = true; // Check if the plane containing a face cross the line connecting two points 
	}
	if (possibility)
	{
		switch (dir) // Check if the face cross the line connecting two points
		{
		case 1:
			for (int i = 0; i < 3; i++)
			{
				proj_vec[i].setx(0);
				proj_vec[i].sety(points[(i + 1) % 3]->gety() - points[i]->gety());
				proj_vec[i].setz(points[(i + 1) % 3]->getz() - points[i]->getz());
				p_to_vec[i].setx(0);
				p_to_vec[i].sety(points[i]->gety() - p1->gety());
				p_to_vec[i].setz(points[i]->getz() - p1->getz());
			}
			for (int i = 0; i < 3; i++)
			{
				out_prod[i] = proj_vec[i].getz() * p_to_vec[i].gety() - proj_vec[i].gety() * p_to_vec[i].getz();
			}
			if (out_prod[0] * out_prod[1] >= 0 && out_prod[1] * out_prod[2] >= 0)
			{
				return true;
			}
			break;
		case 2:
			for (int i = 0; i < 3; i++)
			{
				proj_vec[i].setx(points[(i + 1) % 3]->getx() - points[i]->getx());
				proj_vec[i].sety(0);
				proj_vec[i].setz(points[(i + 1) % 3]->getz() - points[i]->getz());
				p_to_vec[i].setx(points[i]->getx() - p1->getx());
				p_to_vec[i].sety(0);
				p_to_vec[i].setz(points[i]->getz() - p1->getz());
			}
			for (int i = 0; i < 3; i++)
			{
				out_prod[i] = proj_vec[i].getx() * p_to_vec[i].getz() - proj_vec[i].getz() * p_to_vec[i].getx();
			}
			if (out_prod[0] * out_prod[1] >= 0 && out_prod[1] * out_prod[2] >= 0)
			{
				return true;
			}
			break;
		case 3:
			for (int i = 0; i < 3; i++)
			{
				proj_vec[i].setx(points[(i + 1) % 3]->getx() - points[i]->getx());
				proj_vec[i].sety(points[(i + 1) % 3]->gety() - points[i]->gety());
				proj_vec[i].setz(0);
				p_to_vec[i].setx(points[i]->getx() - p1->getx());
				p_to_vec[i].sety(points[i]->gety() - p1->gety());
				p_to_vec[i].setz(0);
			}
			for (int i = 0; i < 3; i++)
			{
				out_prod[i] = proj_vec[i].getx() * p_to_vec[i].gety() - proj_vec[i].gety() * p_to_vec[i].getx();
			}
			if (out_prod[0] * out_prod[1] >= 0 && out_prod[1] * out_prod[2] >= 0)
			{
				return true;
			}
			break;
		}
	}
	return false;
}

Polytope::Polytope() {
	num_faces = 0;
	num_points = 0;
	num_edges = 0;
	encl_pts = {};
	x_min = INFINITY;
	y_min = INFINITY;
	z_min = INFINITY;
	x_max = -INFINITY;
	y_max = -INFINITY;
	z_max = -INFINITY;
}

Polytope::Polytope(FILE* f) {
	char* buffer = new char[BUFFERSIZE];
	fgets(buffer, BUFFERSIZE, f);
	//to be implemented
}

Polytope::~Polytope() {
}

void Polytope::setpolytope(std::vector<Face*> input)
{
	num_faces = 0;
	num_points = 0;
	num_edges = 0;
	encl_pts = {};
	min_length = INFINITY;
	x_min = INFINITY;
	y_min = INFINITY;
	z_min = INFINITY;
	x_max = -INFINITY;
	y_max = -INFINITY;
	z_max = -INFINITY;
	for (auto F : input)
	{
		faces.push_back(F);
		num_faces++;
		std:vector<Point*> temp = F->getpoints();
		for (auto p : temp)
		{
			bool same = false;
			for (auto v : vertices)
			{
				if (p == v)
				{
					same = true;
					break;
				}
			}
			if (!same)
			{
				vertices.push_back(p);
				num_points++;
			}
		}
		Edge t[3] = { {temp[0], temp[1]}, { temp[1], temp[2]},{ temp[2], temp[0]}};
		for (int i = 0; i < 3; i++)
		{
			bool same = false;
			for (auto e : edges)
			{
				if (e == t[i])
				{
					same = true;
					break;
				}
			}
			if (!same)
			{
				edges.push_back(t[i]);
				min_length = min(min_length, t[i].length);
				num_edges++;
			}
		}
	}
	for (Point* vertex : vertices)
	{
		x_min = min(x_min, vertex->getx());
		y_min = min(y_min, vertex->gety());
		z_min = min(z_min, vertex->getz());
		x_max = max(x_max, vertex->getx());
		y_max = max(y_max, vertex->gety());
		z_max = max(z_max, vertex->getz());
	}
}

// Polytope
bool Polytope::isIn(Point* p) {
	int num_below = 0;
	for (int i = 0; i < num_faces; i++)
	{
		if (this->faces[i]->below(p)) {
			num_below++;
		}
	}
	for (auto e : edges)
	{
		if (e.below(p))
		{
			num_below++;
		}
	}
	for (auto v : vertices)
	{
		if (p->getx() == v->getx() && p->gety() == v->gety() && p->z > v->z)
		{
			num_below++;
		}
	}
	if (num_below % 2 == 0)
	{
		return false;
	}
	else { return true; }
};

bool Polytope::intersect(Point p1, Point p2, int dir) {
	for (int i = 0; i < num_faces; i++)
	{
		Point temp = p1;
		Point temp2 = p2;
		double eps = min_length / 10;
		int count = 0;
		switch (dir) // Check if the face cross the line connecting two points
		{
		case 1:
			if (this->faces[i]->pass(&temp, &temp2, dir)) { count++; }
			temp.sety(p1.y - eps); temp2.sety(p2.y - eps);
			if (this->faces[i]->pass(&temp, &temp2, dir)) {count++;}
			temp.sety(p1.y + eps); temp2.sety(p2.y + eps);
			if (this->faces[i]->pass(&temp, &temp2, dir)) { count++; }
			temp.sety(p1.y); temp2.sety(p2.y);
			temp.setz(p1.z - eps); temp2.setz(p2.z - eps);
			if (this->faces[i]->pass(&temp, &temp2, dir)) { count++; }
			temp.setz(p1.z + eps); temp2.setz(p2.z + eps);
			if (this->faces[i]->pass(&temp, &temp2, dir)) { count++; }
			if (count == 5) { return true; }
			else { return false; }
			break;
		case 2:
			if (this->faces[i]->pass(&temp, &temp2, dir)) { count++; }
			temp.setx(p1.x - eps); temp2.setx(p2.x - eps);
			if (this->faces[i]->pass(&temp, &temp2, dir)) { count++; }
			temp.setx(p1.x + eps); temp2.setx(p2.x + eps);
			if (this->faces[i]->pass(&temp, &temp2, dir)) { count++; }
			temp.setx(p1.x); temp2.setx(p2.x);
			temp.setz(p1.z - eps); temp2.setz(p2.z - eps);
			if (this->faces[i]->pass(&temp, &temp2, dir)) { count++; }
			temp.setz(p1.z + eps); temp2.setz(p2.z + eps);
			if (this->faces[i]->pass(&temp, &temp2, dir)) { count++; }
			if (count == 5) { return true; }
			else { return false; }
			break;
		case 3:
			if (this->faces[i]->pass(&temp, &temp2, dir)) { count++; }
			temp.sety(p1.y - eps); temp2.sety(p2.y - eps);
			if (this->faces[i]->pass(&temp, &temp2, dir)) { count++; }
			temp.sety(p1.y + eps); temp2.sety(p2.y + eps);
			if (this->faces[i]->pass(&temp, &temp2, dir)) { count++; }
			temp.sety(p1.y); temp2.sety(p2.y);
			temp.setz(p1.x - eps); temp2.setz(p2.x - eps);
			if (this->faces[i]->pass(&temp, &temp2, dir)) { count++; }
			temp.setz(p1.x + eps); temp2.setz(p2.x + eps);
			if (this->faces[i]->pass(&temp, &temp2, dir)) { count++; }
			if (count == 5) { return true; }
			else { return false; }
			break;
		}
	}
	return false;
}

bool Polytope::operator==(Polytope _p) {
	return this->ord == _p.ord;
}