#pragma once

#include "../algorithm/Point.h"

#define EPS_DG pow(0.1, 5)

typedef struct halfplane {

	int d; // dimension
	std::vector<double> vals; // (d+1) values to represent the halfplane. For example, (d=2) ax + by + c = 0, (d=3) ax + by + cz + d = 0.

} halfplane;

enum DIST_FLAG {
	UNIFORM,
	NORMAL,
};

// ------------------------------------------------------------------------------------------------------------------------
// halfplane 생성 함수들
halfplane* hp_random(int dim);
halfplane* hp_thru_p(Point* center);
std::vector<halfplane*> x_slice(int num_cuts = 1);

// ------------------------------------------------------------------------------------------------------------------------
// 점집합 생성 함수들
std::vector<Point*> brute_force(Point* center, double bound, int num, halfplane* hp, bool b);
std::vector<Point*> pts_on_sphere(int num_pts, Point* center, double radius, DIST_FLAG DF);

// ------------------------------------------------------------------------------------------------------------------------
// 점집합 분리 함수들

// index를 반환
// std::tuple<std::vector<int>, std::vector<int>, std::vector<int>> separate_pts(halfplane* hp, std::vector<Point*>);
std::vector<std::vector<int>> separate_pts(halfplane* hp, std::vector<Point*>);

// halfplane 기준으로 점이 어느 곳에 위치하는지 테스트하는 함수 
// 리턴값은 양수, 0, 음수 중 하나. halfplane 위에 있을 경우 0을 반환
double hp_result(halfplane* hp, Point* p);