#include "Polytope.h"
#include <queue>
#include <assert.h>
#define BUFFERSIZE 1000


using namespace std;

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
	if (normal[0] * p->getx() + normal[1] * p->gety() + normal[2] * p->getz() >= 
		normal[0] * points[0]->getx() + normal[1] * points[0]->gety() + normal[2] * points[0]->getz()) {
		belowness = true;
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
			if (out_prod[0] * out_prod[1] > 0 && out_prod[1] * out_prod[2] > 0)
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
			if (out_prod[0] * out_prod[1] > 0 && out_prod[1] * out_prod[2] > 0)
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
			if (out_prod[0] * out_prod[1] > 0 && out_prod[1] * out_prod[2] > 0)
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
	encl_pts = {};
	x_min = -INFINITY;
	y_min = -INFINITY;
	z_min = -INFINITY;
	x_max = INFINITY;
	y_max = INFINITY;
	z_max = INFINITY;
	for (Point* vertex : vertices)
	{
		x_min = min(x_min, vertex->getx());
		y_min = min(y_min, vertex->gety());
		z_min = min(z_min, vertex->getz());
		x_max = max(x_max, vertex->getx());
		y_max = min(y_max, vertex->gety());
		z_max = min(z_max, vertex->getz());
	}
}

Polytope::Polytope(FILE* f) {
	char* buffer = new char[BUFFERSIZE];
	fgets(buffer, BUFFERSIZE, f);
	//to be implemented
}

Polytope::~Polytope() {
}


// Polytope
bool Polytope::isIn(Point* p) {
	int num_belowface = 0; 
	for (int i = 0; i < num_faces; i++)
	{
		if (this->faces[i]->below(p)) {
			num_belowface++;
		}
	}
	if (num_belowface % 2 == 1) {
		return true;
	}
	else { return false; }
};

bool Polytope::intersect(Point p1, Point p2, int dir) {
	for (int i = 0; i < num_faces; i++)
	{
		if (this->faces[i]->pass(&p1, &p2, dir))
		{
			return true;
		}
	}
	return false;
}

bool Polytope::operator==(Polytope _p) {
	return this->ord == _p.ord;
}