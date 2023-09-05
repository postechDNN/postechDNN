#pragma once

// #include "stdafx.h"

#include "../algorithm/Point.h"
#include "../Polytope.h"

// 2#include "data_generation_print.h"

#define EPS_DG pow(0.1, 5)

typedef struct halfplane {
	halfplane() {
		d = -1; vals = {}; on_points = {};
	}

	halfplane(int _d) {
		d = _d; vals = {}; on_points = {};
	}

	int d; // dimension
	std::vector<double> vals; // (d+1) values to represent the halfplane. For example, (d=2) ax + by + c = 0, (d=3) ax + by + cz + d = 0.
	std::vector<Point*> on_points;

} halfplane;

enum DIST_FLAG {
	UNIFORM,
	NORMAL,
};

struct graph_input {
	// std::vector<s>
};

// ------------------------------------------------------------------------------------------------------------------------
// halfplane 생성 함수들
halfplane* gen_hp_random(int dim, int index);
halfplane* gen_hp_thru_p(Point* center);
std::vector<halfplane*> x_slice(int u_bnd, int dim, int num_cuts = 1);


// ------------------------------------------------------------------------------------------------------------------------
// 점집합 생성 함수들

std::vector<Point*> gen_pts(int u_bound, halfplane* H, bool positive, int num, int max_iter = 50000);

// std::vector<Point*> brute_force(double bound, int num, halfplane* hp, bool b);
std::vector<Point*> pts_on_sphere(int num_pts, Point* center, double radius, DIST_FLAG DF);

// halfplane 위에 점 생성
void generate_points_on_bb(std::vector<std::pair<double, double>> bbx, halfplane* H,
																		int dim, int con_pts, int max_iter = 50000);

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