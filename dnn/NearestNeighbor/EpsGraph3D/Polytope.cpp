#include "Polytope.h"
#include <queue>
#include <assert.h>



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
	normal[0] = (vertices[0]->gety()- vertices[1]->gety())* (vertices[2]->getz() - vertices[1]->getz())
		- (vertices[0]->getz() - vertices[1]->getz()) * (vertices[2]->gety() - vertices[1]->gety());
	normal[1] = -(vertices[0]->getx() - vertices[1]->getx()) * (vertices[2]->getz() - vertices[1]->getz())
		+ (vertices[0]->getz() - vertices[1]->getz()) * (vertices[2]->getx() - vertices[1]->getx());
	normal[2] = (vertices[0]->getx() - vertices[1]->getx()) * (vertices[2]->gety() - vertices[1]->gety())
		- (vertices[0]->gety() - vertices[1]->gety()) * (vertices[2]->getx() - vertices[1]->getx());
	if (normal[0] * p->getx() + normal[1] * p->gety() + normal[2] * p->getz() >= 
		normal[0] * vertices[0]->getx() + normal[1] * vertices[0]->gety() + normal[2] * vertices[0]->getz()) {
		belowness = true;
	}
	// Check the face contains intersection of z-line contining the point and the plain expanded by the face
	bool includeness = false;
	Point proj_vec[3];
	Point p_to_vec[3];
	double out_prod[3];
	for (int i = 0; i < 3; i++)
	{
		proj_vec[i].setx(vertices[(i + 1)%3]->getx() - vertices[i]->getx());
		proj_vec[i].sety(vertices[(i + 1) % 3]->gety() - vertices[i]->gety());
		proj_vec[i].setz(0);
		p_to_vec[i].setx(vertices[i]->getx() - p->getx());
		p_to_vec[i].sety(vertices[i]->gety() - p->gety());
		p_to_vec[i].setz(0);		
	}
	for (int i = 0; i < 3; i++)
	{
		out_prod[i] = proj_vec[i].getx() * p_to_vec[i].gety() - proj_vec[i].gety() * p_to_vec[i].getx();
	}
	if (out_prod[0] * out_prod[1] >= 0 && out_prod[1] * out_prod[2] >= 0)
	{
		// Exception
		if (proj_vec[0].getx()* proj_vec[1].gety()- proj_vec[0].gety()* proj_vec[1].getx() != 0) 
		{
			includeness = true;
		}
	}
	if (includeness && belowness)
	{
		return true;
	}
	else {
		return false;
	}
}

Polytope::Polytope() {
	num_faces = 0;
	num_points = 0;
}

Polytope::Polytope(FILE* f) {
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