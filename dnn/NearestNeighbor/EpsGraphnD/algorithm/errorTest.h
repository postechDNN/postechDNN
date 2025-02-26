#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <filesystem>
#include "../Polytope.h"
#include "../data_generation/data_generation.h"
#include "quadtree.h"

void querySpeedTest(std::string dir, int startID, int endID, int numSites, int numQueries);

void distanceSumTest(std::string dir, int startID, int endID, int numSites, int numQueries);

void Test(std::string dir, bool speedFlag, int useDataSetId);

void insertionTest(EpsGraphNd* qT, Point* q, int maxDepth, double EPS = 0.0000001);

void deletionTest(EpsGraphNd* qT, Point* q, double EPS = 0.0000001);

void autoTest(std::string dir, double epsilon, bool speedFlag, int useDataSetId);

pair<double, double> printErrorDijk(GridGraph& epsGraph, pVV& pr,
	vector<pair<Free_Point, double>>& prDijk, Free_Point& q, int& k, string& dir);

pair<double, double> printError(GridGraph* epsGraph1, GridGraph* epsGraph2,
	int useDataSetId,
	pVV& pr1, pVV& pr2, Free_Point* q, int& k, string& dir);

void printSpeedFinal(string dir, vector<double> avgSpeed);

void printSpeedFinal(string dir, vector<long long int> speedSum);

void printSpeedTemp(string dir, int id, long long time);

void printErrorFinal(string dir, vector<double> numErrorSumsAll, vector<double> distErrorSumsAll,
	int numQueries, int numDatasets);

bool isIn(vector<int> vec, int val);