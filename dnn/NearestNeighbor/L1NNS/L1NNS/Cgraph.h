#pragma once
#include"Edge.h"
#include"Point.h"
#include<vector>
using namespace std;

class Cgraph {
public:
	vector<Edge*> elist; // list of edges in carrier
	vector<Point*> vlist; // list of vertices in carrier
//	vector<vector<double>> matrix(vertexcnt, vector<double>(vertexcnt, 0));
};