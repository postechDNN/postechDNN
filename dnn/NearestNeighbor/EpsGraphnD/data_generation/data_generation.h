#pragma once

// #include "stdafx.h"

#include "../algorithm/Point.h"
#include "../algorithm/Polytope.h"
#include "../EpsGraphnD.h"
#include "../CPolytope.h"

// 2#include "data_generation_print.h"

#define EPS_DG pow(0.1, 5)

// ------------------------------------------------------------------------------------------------------------------------
// structs and classes
typedef struct halfplane {
	int d; // dimension
	std::vector<double> vals; // (d+1) values to represent the halfplane. For example, (d=2) ax + by + c = 0, (d=3) ax + by + cz + d = 0.
	std::vector<Point*> on_points; // points on the halfplane
	std::vector<pair<double, double>> restriction;

	halfplane() {
		d = -1; vals = {}; on_points = {};
		restriction = {};
	}

	halfplane(int _d) {
		d = _d;
		// vals.assign(_d, 0.0); 
		on_points = {};
		vals = {};
		for (int i = 0; i < _d+1; i++) vals.push_back(0.0);
		restriction.assign(_d, make_pair(-DBL_MAX, DBL_MAX));
		// for (int i = 0; i < d; i++) restriction.push_back(make_pair(DBL_MIN, DBL_MAX));
		/*
		for (int i = 0; i < _d; i++) {
			auto pr = make_pair(0.0, 0.0);
			restriction.push_back(pr);
		}
		*/
	}


} halfplane;

struct graph_input {
	// std::vector<s>
};

struct cell {
	std::vector<std::pair<double, double>> bbx;
	std::vector<int> index;
	// std::vector<Point*> vertices;

	// for grid
	std::vector<pair<cell*, halfplane*>> neighbors;
	std::vector<Point*> in_points;

	int total_index;

	// 직선 정보 + 나머지는 index에 따른 bound니까
	// x axis에 대한 index가 다르면 직선은 x = a 이런 꼴이니까
	// 일단 근데 3차원 facet 위에 그럼 점을 찍는 거임?
	// 좀 이상한데
	// 상상이 되려나 그게

	bool active;

	cell() {
		active = false;
		in_points = {};
	}
	cell(int i) {
		active = false;
		in_points = {};
		total_index = i;
	}
};

// ------------------------------------------------------------------------------------------------------------------------
// enums
enum DIST_FLAG {
	UNIFORM,
	NORMAL,
};

// ------------------------------------------------------------------------------------------------------------------------
// 기타 함수들
bool isNumeric(std::string const& str);

// ------------------------------------------------------------------------------------------------------------------------
// halfplane 생성 함수들
halfplane* gen_hp_random(int dim, int index);
halfplane* gen_hp_thru_p(Point* center);
std::vector<halfplane*> x_slice(int u_bnd, int dim, int num_cuts = 1);
std::vector<halfplane*> axis_slice(int u_bnd, int dim, int my_dim, int num_cuts = 1);


// ------------------------------------------------------------------------------------------------------------------------
// 점집합 생성 함수들

// void generate_points_on_bb(std::vector<std::pair<double, double>> bbx, int dis_pts, int max_iter = 50000);
void generate_points_on_bb(cell* c, int dis_pts, int max_iter = 50000);
std::vector<Point*> gen_pts(int u_bound, halfplane* H, bool positive, int num, int max_iter = 50000);

// std::vector<Point*> brute_force(double bound, int num, halfplane* hp, bool b);
std::vector<Point*> pts_on_sphere(int num_pts, Point* center, double radius, DIST_FLAG DF);

// halfplane 위에 점 생성
void generate_points_on_bb(std::vector<std::pair<double, double>> bbx, halfplane* H,
																int dim, int con_pts, int max_iter = 50000);
void generate_points_on_restricted_hp(std::vector<std::pair<double, double>> bbx, halfplane* H, vector<int> grid_size,
	vector<int> index, int con_pts, int max_iter = 50000);

// ------------------------------------------------------------------------------------------------------------------------
// 점집합 분리 함수들

// index를 반환
// std::tuple<std::vector<int>, std::vector<int>, std::vector<int>> separate_pts(halfplane* hp, std::vector<Point*>);
// std::vector<std::vector<int>> separate_pts(halfplane* hp, std::vector<Point*>);
std::pair<std::vector<Point*>, std::vector<Point*>> separate_pts(halfplane* hp, std::vector<Point*>);

// halfplane 기준으로 점이 어느 곳에 위치하는지 테스트하는 함수 
// 리턴값은 양수, 0, 음수 중 하나. halfplane 위에 있을 경우 0을 반환
double hp_result(halfplane* hp, Point* p);

// ------------------------------------------------------------------------------------------------------------------------
// print 함수들

// halfplane 출력 함수
void print_halfplane(halfplane* H, int index);

// point 출력 함수
void print_pt(Point* p, int index);

// ------------------------------------------------------------------------------------------------------------------------
// 점 변경 함수
void scale_pt(int u_bound, Point* pt);

// ------------------------------------------------------------------------------------------------------------------------
// grid 관련 함수
vector<cell*> generate_grid(std::vector<std::pair<double, double>> bbx, vector<int> num_subspaces_over_axes);

vector<int> compute_denominators(vector<int> num_subspaces_over_axes);
vector<int> num2index(vector<int> num_subspaces_over_axes, vector<int> denominators, int num);
int index2num(vector<int> num_subspaces_over_axes, vector<int> multipliers, vector<int> index);

void activate_cells(vector<cell*> cells, int num_parts);

void disperse_pts_between_active_cells(std::vector<std::pair<double, double>> bbx, vector<int> num_subspaces_over_axes, vector<cell*> cells, int num);

// vector<Polytope*> dels2polytopes(int num_topes); 
vector<Polytope*> dels2polytopes(string dir, int num_topes);

// 파일 input을 통해, convex polytope을 만드는 함수
CPolytope* dels2cpolytope(string dir, int dim, bool isSimplex = false);

vector<Point*> generate_point_sites(std::vector<std::pair<double, double>> bbx, int dim, int num_points, int TYPE = 0);

void data_generation();

void data_generation(int dim, double u_bound, int halfplane_constraint, int num_parts, int num_subspaces, int dis_pts, int dis_pts_cell);

// std::string config = ""

void autoTest(string dir, double epsilon, bool speedFlag, int useDataSetId);

vector<Point*> makePointSet(std::string dir);
vector<Point> makePointSetNoPtr(std::string dir);
vector<Free_Point*> makeFreePointSet(std::string dir);
vector<Free_Point> makeFreePointSetNoPtr(std::string dir);

typedef pair<vector<Free_Point*>, vector<double>> pVV;

pair<double, double> printErrorDijk(Eps_Graph_nD& epsGraph, pVV& pr,
	vector<pair<Free_Point*, double>>& prDijk, Free_Point& q, int& k, string& dir);

pair<double, double> printError(Eps_Graph_nD* epsGraph1, Eps_Graph_nD* epsGraph2,
	int useDataSetId, 
	pVV& pr1, pVV& pr2, Free_Point* q, int& k, string& dir);
// double printNeighborError(pVV& pr1, pVV& pr2, Free_Point& q, int& k, string& dir);

// void printSpeedTemp(string dir, int id, long long time);

// void printSpeedFinal(string dir, vector<double> avgSpeed);