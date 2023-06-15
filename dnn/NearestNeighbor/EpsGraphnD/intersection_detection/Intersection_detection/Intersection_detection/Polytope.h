#pragma once
#include "Point.h"
#include<Eigen/Dense>

using namespace std;
using Eigen::MatrixXd;

class simplices {
protected:
	int d; // dimension
	std::vector<Point*> vertices;
	MatrixXd A; // Each column indicates coordinate of each vertex.
public:
	simplices();
	simplices(int, vector<Point*>);
	~simplices();
	MatrixXd getmatrix();
	bool intersect(simplices);
	bool isIn(Point* p);
};

class Polytope {
protected:
	//std::vector<Face*> faces;
	vector<simplices> component;
	std::vector<Point*> vertices;
	int num_simplices;
	int num_points;
	int d; // dimension
	double min_length;
public:
	double* min;
	double* max;
	vector<Point*> encl_pts;
	int ord;

public:
	Polytope();
	Polytope(FILE*);
	~Polytope();
	bool isIn(Point* p);
	bool intersect(Polytope P);
	bool operator==(Polytope P);
};