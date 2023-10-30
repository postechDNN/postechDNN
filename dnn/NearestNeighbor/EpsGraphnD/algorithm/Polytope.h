#pragma once
#include "Point.h"
#include<Eigen/Dense>

using namespace std;
using Eigen::MatrixXd;

class simplex {
protected:
	int d; // dimension
	std::vector<Point*> vertices;
	MatrixXd A; // Each column indicates coordinate of each vertex.
public:
	simplex();
	simplex(int, vector<Point*>);
	~simplex();
	MatrixXd getmatrix();
	bool intersect(simplex);
	bool intersect(Point* p, Point* q);
	bool isIn(Point* p);
};

class Polytope {
protected:
	//std::vector<Face*> faces;
	vector<simplex> simplices;
	std::vector<Point*> vertices;
	int num_simplices;
	int num_points;
	int d; // dimension
	double min_length;
public:
	std::vector <double> xs_max, xs_min;
	vector<Point*> encl_pts;
	int ord;

public:
	Polytope();
	Polytope(FILE*);
	Polytope(vector<simplex>, std::vector<Point*>);
	~Polytope();
	int get_num_point();
	std::vector<Point*> get_vertices();
	bool isIn(Point* p);
	bool intersect(Polytope P);
	bool intersect(Point* p, Point* q);
	bool operator==(Polytope P);
	void set_vertices(std::vector<Point*>);
	void set_vertices_size();
	void set_simplices(std::vector<simplex>&);
	void set_maxmin();
};