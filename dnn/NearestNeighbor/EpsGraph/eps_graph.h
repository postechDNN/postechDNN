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

private:

public: // variables
	double eps;
	double x_min; double x_max;
	double y_min; double y_max;

	Point upper_left;
	int row_num, col_num; // (# of points in the eps_graph) = (row_num) * (col_num) // index는 0부터 n-1까지

	vector<Grid_Point> grid;
	vector<edge> edges;

	list<Free_Point> fr_pts; // 초기화할 때 이미 anchor를 시키므로, add_freept에서도 anchor시키는 부분이 포함돼야 함.
	vector<Polygon> pols;

	vector<int> dist;	// BFS distance
	vector<bool> visited;
	vector<int> closest; // gridpt indices in nondecreasing order of distance

public:	// functions
	Eps_Graph(list<Free_Point>, vector<Polygon>, double); // lexicographic order로 정렬한 뒤 binary search로 insertion/deletion 구현할 것까지는 없을 듯(arbitrary order)
	void init_grid();
	Grid_Point get_gridpt(indices);

	int ind2num(indices);
	int ind2num(int, int);
	indices num2ind(int);

	void add_edge(indices, indices); // add grid edges
	void add_edge(int, int, bool);
	void delete_edge(indices, indices);
	bool cmpNadd(indices, bool);
	bool cmpNadd_SinPol(indices, bool, int);

	void add_freepts(vector<Free_Point>);
	void delete_freept(int);

	void anchor(Free_Point&);	// 중간에 있으면 왼쪽, 위로 가도록
	Grid_Point query_anchor(Free_Point);
	
	void add_pol(Polygon);
	void delete_pol(int);
	indices* eff_region(Polygon); // effective region of the given polygon. In other words, the rectangular range for checking grid edges again

	void BFS(Grid_Point); // BFS on grid
	vector<Free_Point> kNN(Free_Point, int); // kNN point query

	void print_grid();
	void print_edges();
	void print_dist();
	void print_kNN(Free_Point, int);

	Eps_Graph();
};

