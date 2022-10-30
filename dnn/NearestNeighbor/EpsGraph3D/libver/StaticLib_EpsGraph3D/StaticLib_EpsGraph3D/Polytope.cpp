#include "pch.h"
#include "Polytope.h"
#include <queue>
#include <assert.h>
#include <random>

#define BUFFERSIZE 1000

EPS::Edge::Edge() {
	p1 = new Point;
	p2 = new Point;
	length = 0;
}

EPS::Edge::~Edge() {
}

EPS::Edge::Edge(std::vector<Point*> vp) {
	p1 = vp[0];
	p2 = vp[1];
	length = abs(vp[0]->x - vp[1]->x) + abs(vp[0]->y - vp[1]->y) + abs(vp[0]->z - vp[1]->z);
}

EPS::Edge::Edge(Point* v1, Point* v2) {
	p1 = v1;
	p2 = v2;
	length = abs(v1->x - v2->x) + abs(v1->y - v2->y) + abs(v1->z - v2->z);
}

EPS::Edge::Edge(Grid_Point* v1, Grid_Point* v2) {
	p1 = v1;
	p2 = v2;
	length = abs(v1->x - v2->x) + abs(v1->y - v2->y) + abs(v1->z - v2->z);
}

EPS::Edge::Edge(Free_Point* v1, Grid_Point* v2) {
	p1 = v1;
	p2 = v2;
	length = abs(v1->x - v2->x) + abs(v1->y - v2->y) + abs(v1->z - v2->z);
}

EPS::Edge::Edge(Point v1, Point v2) {
	p3 = v1;
	p4 = v2;
	//length = sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y) + (v1.z - v2.z) * (v1.z - v2.z));
	length = abs(v1.x - v2.x) + abs(v1.y - v2.y) + abs(v1.z - v2.z);
}

EPS::Edge::Edge(Grid_Point v1, Grid_Point v2) {
	p3 = v1;
	p4 = v2;
	//length = sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y) + (v1.z - v2.z) * (v1.z - v2.z));
	length = abs(v1.x - v2.x) + abs(v1.y - v2.y) + abs(v1.z - v2.z);
}

EPS::Edge::Edge(Free_Point v1, Grid_Point v2) {
	p3 = v1;
	p4 = v2;
	//length = sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y) + (v1.z - v2.z) * (v1.z - v2.z));
	length = abs(v1.x - v2.x) + abs(v1.y - v2.y) + abs(v1.z - v2.z);
}

bool EPS::Edge::operator==(Edge e) {
	return (this->p1 == e.p1 && this->p2 == e.p2) || (this->p2 == e.p1 && this->p1 == e.p2);
}

bool EPS::Edge::on(Point* p) {
	double lambda = INFINITY;
	if (p->x < std::min(p1->x, p2->x) || p->x > std::max(p1->x, p2->x)) { return false; }
	if (p->y < std::min(p1->y, p2->y) || p->y > std::max(p1->y, p2->y)) { return false; }
	if (p->z < std::min(p1->z, p2->z) || p->z > std::max(p1->z, p2->z)) { return false; }
	if (p1->x == p2->x)
	{
		if (p1->y == p2->y || p1->z == p2->z) { return true; }
		if ((p->y - p2->y) / (p1->y - p2->y) == (p->z - p2->z) / (p1->z - p2->z)) { return true; }
		else return false;
	}
	else {
		if (p1->y == p2->y && p1->z == p2->z) { return true; }
		if (p1->y == p2->y) { 
			if ((p->x - p2->x) / (p1->x - p2->x) == (p->z - p2->z) / (p1->z - p2->z)) { return true; } 
			else return false;
		}
		if (p1->z == p2->z) {
			if ((p->y - p2->y) / (p1->y - p2->y) == (p->x - p2->x) / (p1->x - p2->x)) { return true; }
			else return false;
		}
		if ((p->y - p2->y) / (p1->y - p2->y) == (p->x - p2->x) / (p1->x - p2->x) && (p->y - p2->y) / (p1->y - p2->y) == (p->z - p2->z) / (p1->z - p2->z)) { return true; }
		else return false;
	}
}

bool EPS::Edge::cross(Point* p, Point* ray) {
	Point vec;
	Point normal;
	vec.x = p2->x - p1->x; vec.y = p2->y - p1->y; vec.z = p2->z - p1->z;
	normal.x = ray->y * vec.z - ray->z * vec.y; normal.y = ray->z * vec.x - ray->x * vec.z; normal.z = ray->x * vec.y - ray->y * vec.x;
	if (normal.x * (p2->x - p->x) + normal.y * (p2->y - p->y) + normal.z * (p2->z - p->z) != 0) {
		return false;
	}
	if (vec.x * ray->y - ray->x * vec.y == 0) { return false; }
	double Value = (p->x * ray->y - p->y * ray->x - ray->y * p1->x + ray->x * p1->y) / (vec.x * ray->y - ray->x * vec.y);
	double ray_Value = (vec.y * p1->x - vec.x * p1->y - p->x * vec.y + p->y * vec.x ) / (vec.x * ray->y - ray->x * vec.y);
	if (Value >0 && Value <1 && ray_Value < 0) { return true; }
	else return false;

}

EPS::Face::Face() {
	face_key = new char[10];
	points.push_back(new Point());
	points.push_back(new Point());
	points.push_back(new Point());
}

EPS::Face::Face(std::vector<Point*> vp) {
	face_key = new char[10];
	points.push_back(vp[0]);
	points.push_back(vp[1]);
	points.push_back(vp[2]);
}

EPS::Face::~Face() {
}

std::vector<EPS::Point*> EPS::Face::getpoints()
{
	return points;
}

bool EPS::Face::on(Point* p, int mode) {
	double normal[3];
	if (mode == 0) {
		normal[0] = (points[0]->gety() - points[1]->gety()) * (points[2]->getz() - points[1]->getz())
			- (points[0]->getz() - points[1]->getz()) * (points[2]->gety() - points[1]->gety());
		normal[1] = -(points[0]->getx() - points[1]->getx()) * (points[2]->getz() - points[1]->getz())
			+ (points[0]->getz() - points[1]->getz()) * (points[2]->getx() - points[1]->getx());
		normal[2] = (points[0]->getx() - points[1]->getx()) * (points[2]->gety() - points[1]->gety())
			- (points[0]->gety() - points[1]->gety()) * (points[2]->getx() - points[1]->getx());
		if (normal[0] * p->getx() + normal[1] * p->gety() + normal[2] * p->getz() !=
			normal[0] * points[0]->getx() + normal[1] * points[0]->gety() + normal[2] * points[0]->getz()) {
			return false;
		}
	}
	Point vec_to_vec[3];
	Point vec_to_p[3];
	Point out1[3];
	Point out2[3];
	double out_prod[3];
	for (int i = 0; i < 3; i++)
	{
		vec_to_vec[i].setx(points[(i + 1) % 3]->getx() - points[i]->getx());
		vec_to_vec[i].sety(points[(i + 1) % 3]->gety() - points[i]->gety());
		vec_to_vec[i].setz(points[(i + 1) % 3]->getz() - points[i]->getz());
		vec_to_p[i].setx(-points[i]->getx() + p->getx());
		vec_to_p[i].sety(-points[i]->gety() + p->gety());
		vec_to_p[i].setz(-points[i]->getz() + p->getz());
	}
	for (int i = 0; i < 3; i++)
	{
		out1[i].x = -( vec_to_p[i].y* vec_to_vec[i].z - vec_to_p[i].z * vec_to_vec[i].y);
		out1[i].y = vec_to_p[i].x * vec_to_vec[i].z - vec_to_p[i].z * vec_to_vec[i].x;
		out1[i].z = -(vec_to_p[i].x * vec_to_vec[i].y - vec_to_p[i].y * vec_to_vec[i].x);
		out2[i].x =-( vec_to_vec[i].y * vec_to_vec[(i + 2) % 3].z - vec_to_vec[i].z * vec_to_vec[(i + 2) % 3].y);
		out2[i].y = vec_to_vec[i].x * vec_to_vec[(i + 2) % 3].z - vec_to_vec[i].z * vec_to_vec[(i + 2) % 3].x;
		out2[i].z =-( vec_to_vec[i].x * vec_to_vec[(i + 2) % 3].y - vec_to_vec[i].y * vec_to_vec[(i + 2) % 3].x);
	}
	if (out1[0].x * out2[0].x + out1[0].y * out2[0].y + out1[0].z * out2[0].z > 0 &&
		out1[1].x * out2[1].x + out1[1].y * out2[1].y + out1[1].z * out2[1].z > 0 &&
		out1[2].x * out2[2].x + out1[2].y * out2[2].y + out1[2].z * out2[2].z > 0 )
	{
		return true;
	}
	else return false;
}

bool EPS::Face::cross(Point* p, Point* ray) {
	// Determine if the face is below from p along z-axis.
	bool belowness = false;
	double normal[3];
	normal[0] = (points[0]->gety()- points[1]->gety())* (points[2]->getz() - points[1]->getz())
		- (points[0]->getz() - points[1]->getz()) * (points[2]->gety() - points[1]->gety());
	normal[1] = -(points[0]->getx() - points[1]->getx()) * (points[2]->getz() - points[1]->getz())
		+ (points[0]->getz() - points[1]->getz()) * (points[2]->getx() - points[1]->getx());
	normal[2] = (points[0]->getx() - points[1]->getx()) * (points[2]->gety() - points[1]->gety())
		- (points[0]->gety() - points[1]->gety()) * (points[2]->getx() - points[1]->getx());
	double constant = normal[0] * points[0]->x + normal[1] * points[0]->y + normal[2] * points[0]->z;
	double p_value = normal[0] * p->x + normal[1] * p->y + normal[2] * p->z;
	double ray_value = normal[0] * ray->x + normal[1] * ray->y + normal[2] * ray->z;
	if (normal[0] * ray->x + normal[1] * ray->y + normal[2] * ray->z == 0) { return false; }
	if ((constant - p_value) / ray_value > 0) {
		Point* In = new Point;
		In->x = p->x + (constant - p_value) / ray_value * ray->x;
		In->y = p->y + (constant - p_value) / ray_value * ray->y;
		In->z = p->z + (constant - p_value) / ray_value * ray->z;
		return this->on(In, 1);
	}
	else return false;
}

bool EPS::Face::pass(Point* p1, Point* p2, int dir){
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

EPS::Polytope::Polytope() {
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

EPS::Polytope::Polytope(FILE* f) {
	char* buffer = new char[BUFFERSIZE];
	fgets(buffer, BUFFERSIZE, f);
	//to be implemented
}

EPS::Polytope::~Polytope() {
}

void EPS::Polytope::setpolytope(std::vector<Face*> input)
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
	std::vector<Point*> temp = F->getpoints();
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
		Edge t[3] = { {temp[0], temp[1]}, { temp[1], temp[2]},{ temp[2], temp[0]} };
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
				min_length = std::min(min_length, t[i].length);
				num_edges++;
			}
		}
	}
	for (Point* vertex : vertices)
	{
		x_min = std::min(x_min, vertex->getx());
		y_min = std::min(y_min, vertex->gety());
		z_min = std::min(z_min, vertex->getz());
		x_max = std::max(x_max, vertex->getx());
		y_max = std::max(y_max, vertex->gety());
		z_max = std::max(z_max, vertex->getz());
	}
}

// Polytope
bool EPS::Polytope::isIn(Point* p) {
	int num_below = 0;
	bool check = true;
	Point* ray = new Point;
	std::random_device rng;
	std::uniform_int_distribution<int> dist1(0, 100);
	while (check)
	{
		bool v_check = false;
		bool e_check = false;
		ray->setx(static_cast<float>(dist1(rng))/100); ray->sety(static_cast<float>(dist1(rng)) / 100); ray->setz(static_cast<float>(dist1(rng)) / 100);
		if (ray->x == 0 || ray->y == 0 || ray->z == 0)
		{
			continue;
		}
		for (auto v : vertices)
		{
			if (p == v) {
				return false;
			}
			if ((p->x - v->x) / ray->x == (p->y - v->y) / ray->y && (p->y - v->y) / ray->y == (p->z - v->z) / ray->z) {
				v_check = true;
			}
		}
		if (v_check) { continue; }
		for (auto e : edges)
		{
			if (e.on(p))	{
				return false;
			}
			if (e.cross(p, ray)) {
				e_check = true;
			}
		}
		if (e_check) { continue; }
		break;
	}
	for (auto f: faces)
	{
		if (f->on(p, 0)) {
			return false;
		}
		if (f->cross(p, ray)) {
			num_below++;
		}
	}

	if (num_below % 2 == 0)
	{
		return false;
	}
	else { return true; }
};

bool EPS::Polytope::intersect(Point p1, Point p2, int dir) {
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

bool EPS::Polytope::operator==(Polytope _p) {
	return this->ord == _p.ord;
}

std::vector<EPS::Face*> EPS::Polytope::getfaces()
{
	return faces;
}