#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <filesystem>
#include "../Polytope.h"
#include "../data_generation/data_generation.h"
#include "quadtree.h"

void visualizeTXT(EpsGraphNd* qT);

void visualize(std::string dir);

void querySpeedTest(std::string dir, int startID, int endID, int numSites, int numQueries);

void distanceSumTest(std::string dir, int startID, int endID, int numSites, int numQueries);

void insertionTest(EpsGraphNd* qT, Point* q, int maxDepth, double EPS = 0.0000001);

void deletionTest(EpsGraphNd* qT, Point* q, double EPS = 0.0000001);
bool isIn(vector<int> vec, int val);

vector<Point*> makePointSet(std::string dir, int start_idx = 0);

vector<Free_Point*> makeFreePointSet(std::string dir);

// 파일 input을 통해, convex polytope을 만드는 함수
CPolytope* dels2cpolytope(string dir, int dim, bool isSimplex = false);