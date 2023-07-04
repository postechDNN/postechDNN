#pragma once
#include "DCEL/Edge.h"
#include "DCEL/DCEL.h"

double euc_dist_edges(Edge &a, Edge &b);
bool is_left(HEdge& he, Point& p);

//Defintion of strong alpha-conforming subdivision of free space
bool check_c1(std::vector<Point>& pts, DCEL& dcel);
bool check_c2(DCEL& dcel, int alpha);
bool check_c3(std::vector<Point>& pts, DCEL& dcel);

//Additional properties for strong alpha-conforming subdivision
bool check_a1(DCEL& dcel);
bool check_a2(DCEL& dcel);
bool check_a3(DCEL& dcel);
bool check_a4(DCEL& dcel, int alpha);
bool check_a5(DCEL& dcel);

//Properties of well-covered regions with parameter alpha
bool check_w1(DCEL& dcel);
bool check_w2(DCEL& dcel, int alpha);
bool check_w3(DCEL& dcel, int alpha);
