#pragma once
#include "Point.h"
#include <vector>
#include <algorithm>
#include <iostream>


class BoxNN{
public:
    int n;
    long long int tot_num;
    std::vector<int> ind_num;
    std::vector<std::pair<Point,Point>> boxes;
    std::vector<Point> pts;
    std::vector<std::vector<double>> grid_val; // grid index별 실제 값
    Point query;

    std::vector<bool> valid;
    std::vector<double> dist;
    std::vector<bool> visited;
public:
    BoxNN(std::vector<std::pair<Point,Point> >, std::vector<Point>, Point);
    bool is_valid(Point);
    void make_valid();
    void init_grid();
    int grid2int(std::vector<int>);
    std::vector<int> int2grid(int);
    void Dijkstra();
    void print_knn(int);
};