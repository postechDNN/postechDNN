#pragma once

#include "polygon.h"
#include <tuple>
#include <list>

struct edge
{
	Point p1;
	Point p2;
};

class Eps_Graph {

public:
	double eps;
	double x_min; double x_max;
	double y_min; double y_max;

	Point upper_left; // its upper left gridpoint
	int row_num, col_num; // (# of points in the eps_graph) = (row_num) * (col_num) 

	vector<Grid_Point> grid;
	vector<edge> edges;

	list<Free_Point> fr_pts; 
	vector<Polygon> pols;

	vector<int> dist;	// BFS distance
	vector<bool> visited;
	vector<int> closest; // gridpt indices in nondecreasing order of dist

public:
	Eps_Graph(list<Free_Point>, vector<Polygon>, double); 
	void init_grid();
	Grid_Point get_gridpt(indices);

	int ind2num(indices);
	int ind2num(int, int);
	indices num2ind(int);

	void add_edge(indices, indices);
	void add_edge(int, int, bool);
	void delete_edge(indices, indices);
	bool cmpNadd(indices, bool); // checks if the line connecting the gridpoint and its neighboring one is blocked by any polygon. if is not, add an edge between them.
	bool cmpNadd_SinPol(indices, bool, int); // do the same with a specific polygon.

	void add_freepts(vector<Free_Point>); // add points to the point set P
	void delete_freept(int); // delete a point from P, specified by its index

	void anchor(Free_Point&); // cast anchor onto a grid point from a free point
	Grid_Point query_anchor(Free_Point);
	
	void add_pol(Polygon); // add a polygon to the set of obstacles O
	void delete_pol(int); // delete a polygon from O, specified by its index
	indices* eff_region(Polygon); // returns a range indicating orthogonal rectangle bounding the polygon (effective region)

	void BFS(Grid_Point); // BFS on grid
	vector<Free_Point> kNN(Free_Point, int); // returns k approximate nearest neighbors of p

	void print_grid();
	void print_edges();
	void print_dist();
	void print_kNN(Free_Point, int);

	Eps_Graph();
};

