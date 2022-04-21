#pragma once
#include "Point.h"

class Face {
protected:
	char* face_key;
	std::vector<Point*> points;

public:
	bool below(Point* p);


};

class Polytope {
protected:
	int num_faces;
	int num_points;
	std::vector<Face*> faces;
	std::vector<Point*> vertices;

public:
	Polytope();
	Polytope(FILE*);
	~Polytope();

	bool isIn(Point* p);
};