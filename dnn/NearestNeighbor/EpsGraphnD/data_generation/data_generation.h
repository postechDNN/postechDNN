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
// halfplane ���� �Լ���
halfplane* hp_random(int dim);
halfplane* hp_thru_p(Point* center);
std::vector<halfplane*> x_slice(int num_cuts = 1);

// ------------------------------------------------------------------------------------------------------------------------
// ������ ���� �Լ���
std::vector<Point*> brute_force(Point* center, double bound, int num, halfplane* hp, bool b);
std::vector<Point*> pts_on_sphere(int num_pts, Point* center, double radius, DIST_FLAG DF);

// ------------------------------------------------------------------------------------------------------------------------
// ������ �и� �Լ���

// index�� ��ȯ
// std::tuple<std::vector<int>, std::vector<int>, std::vector<int>> separate_pts(halfplane* hp, std::vector<Point*>);
std::vector<std::vector<int>> separate_pts(halfplane* hp, std::vector<Point*>);

// halfplane �������� ���� ��� ���� ��ġ�ϴ��� �׽�Ʈ�ϴ� �Լ� 
// ���ϰ��� ���, 0, ���� �� �ϳ�. halfplane ���� ���� ��� 0�� ��ȯ
double hp_result(halfplane* hp, Point* p);