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

class Eps_Graph_nD {

private:

public: // variables
	int n;
	double eps;
	std::vector<double> xs_min;
	std::vector<double> xs_max;
	std::vector<long long int> xs_num;
	long long int tot_num;
	//double x_min; double x_max;
	//double y_min; double y_max;
	//double z_min; double z_max;
	//long long int x_num, y_num, z_num;
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
	Eps_Graph_nD();
	Eps_Graph_nD(int, list<Free_Point>, vector<Polytope>, double); // lexicographic order�� ������ �� binary search�� insertion/deletion ������ �ͱ����� ���� ��(arbitrary order)
	void init_grid();
	// Grid_Point get_gridpt(indices);
	Grid_Point get_gridpt(std::vector<long long int>);

	// long long int ind2num(indices);
	long long int ind2num(std::vector<long long int>);
	long long int ind2num(long long int, long long int, long long int);
	// indices num2ind(long long int);
	std::vector<long long int> num2ind(long long int);

	// void add_edge(indices, indices); // add grid edges
	void add_edge(std::vector<long long int>, std::vector<long long int>);
	void delete_edge(std::vector<long long int>, std::vector<long long int>);
	// void delete_edge(indices, indices);
	//bool cmpNadd(indices, int);
	bool cmpNadd(std::vector<long long int>, int);
	bool cmpNadd_SinPol(std::vector<long long int>, int, int);
	// bool cmpNadd_SinPol(indices, int, int);

	void add_freepts(Free_Point*);
	void add_freepts(vector<Free_Point> p_vec);
	void delete_freept(int);

	void anchor(Free_Point&);	// �߰��� ������ ����, ���� ������
	void query_anchor(Grid_Point&);

	void add_pol(Polytope);
	void delete_pol(int);
	std::vector<long long int>* eff_region(Polytope);
	// indices* eff_region(Polytope); // effective region of the given polygon. In other words, the rectangular range for checking grid edges again

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
