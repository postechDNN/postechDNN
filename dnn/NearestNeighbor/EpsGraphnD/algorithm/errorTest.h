#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <filesystem>
#include "../Polytope.h"
#include "../data_generation/data_generation.h"
#include "quadtree.h"


void autoTest(std::string dir, double epsilon, bool speedFlag, int useDataSetId);

pair<double, double> printErrorDijk(Eps_Graph_nD& epsGraph, pVV& pr,
	vector<pair<Free_Point, double>>& prDijk, Free_Point& q, int& k, string& dir);

pair<double, double> printError(Eps_Graph_nD* epsGraph1, Eps_Graph_nD* epsGraph2,
	int useDataSetId,
	pVV& pr1, pVV& pr2, Free_Point* q, int& k, string& dir);

void printSpeedFinal(string dir, vector<double> avgSpeed);

void printSpeedFinal(string dir, vector<long long int> speedSum);

void printSpeedTemp(string dir, int id, long long time);

void printErrorFinal(string dir, vector<double> numErrorSumsAll, vector<double> distErrorSumsAll,
	int numQueries, int numDatasets);

bool isIn(vector<int> vec, int val);