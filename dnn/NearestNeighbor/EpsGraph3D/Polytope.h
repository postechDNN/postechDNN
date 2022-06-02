#pragma once
#include "Point.h"

class Face {
protected:
	char* face_key;
	std::vector<Point*> points;

public:
	Face();
	Face(std::vector<Point*>);
	~Face();
	bool below(Point* p);
};

class Polytope {
public:
	int num_faces;
	int num_points;
	double x_min, x_max, y_min, y_max, z_min, z_max;
	std::vector<Face*> faces;
	std::vector<Point*> vertices;
	std::vector<Point*> encl_pts;
	int ord;

public:
	Polytope();
	Polytope(FILE*);
	~Polytope();
	bool isIn(Point* p);
};