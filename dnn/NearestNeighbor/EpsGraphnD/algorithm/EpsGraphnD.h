#pragma once

#include "polytope.h"
#include <tuple>
#include <list>

using namespace std;

struct edge
{
	Point p1;
	Point p2;
};

class Eps_Graph_3D {

private:

public: // variables
	double eps;
	double x_min; double x_max;
	double y_min; double y_max;
	double z_min; double z_max;
	long long int x_num, y_num, z_num;
	int ord_pol;

	Point upper_left;
	// int row_num, col_num, layer_num; // (# of points in the eps_graph) = (row_num) * (col_num) * (layer_num) // index�� 0���� n-1����

	vector<Grid_Point> grid; // Grid edges
	list<Free_Point> fr_pts; // �ʱ�ȭ�� �� �̹� anchor�� ��Ű�Ƿ�, add_freept������ anchor��Ű�� �κ��� ���Եž� ��.
	vector<Polytope> pols; // Set of polytope

	vector<int> dist;	// BFS distance
	vector<bool> visited;
	vector<int> closest; // gridpt indices in nondecreasing order of distance
	vector<int> NN_dist;

public:	// functions
	Eps_Graph_3D();
	Eps_Graph_3D(list<Free_Point>, vector<Polytope>, double); // lexicographic order�� ������ �� binary search�� insertion/deletion ������ �ͱ����� ���� ��(arbitrary order)
	void init_grid();
	Grid_Point get_gridpt(indices);

	long long int ind2num(indices);
	long long int ind2num(long long int, long long int, long long int);
	indices num2ind(long long int);

	void add_edge(indices, indices); // add grid edges
	void delete_edge(indices, indices);
	bool cmpNadd(indices, int);
	bool cmpNadd_SinPol(indices, int, int);

	void add_freepts(Free_Point*);
	void add_freepts(vector<Free_Point> p_vec);
	void delete_freept(int);

	void anchor(Free_Point&);	// �߰��� ������ ����, ���� ������
	void query_anchor(Grid_Point&);

	void add_pol(Polytope);
	void delete_pol(int);
	indices* eff_region(Polytope); // effective region of the given polygon. In other words, the rectangular range for checking grid edges again

	vector<Free_Point> kNN(Free_Point, int); // kNN point query
	vector<Edge> path_kNN(Free_Point p, int k);

	void print_grid();
	void print_encl();
	void print_free_point();
	void print_edges();
	void print_anchor();
	void print_dist();
	void print_kNN(Free_Point, int);

	Free_Point get_free_point(int);
	list<Free_Point> get_free_points();
	vector<Polytope> get_Polytope();
	vector<Grid_Point> get_grid();
	vector<Edge> get_path(Free_Point, int);

};
