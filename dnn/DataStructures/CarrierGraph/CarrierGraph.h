#pragma once
#include"../../DataStrcutures/DCEL/Edge.h"
#include"Rect.h"
#include<queue>
#include<vector>
#include<map>

class CarrierGraph {
protected:
	enum constant { BOUNDINGBOX = -1 };
	const double INF = 2147483647.0;
	vector<Edge*> elist; // list of edges in carrier
	vector<Point*> vlist; // list of vertices in carrier
	vector<Rect*> rectset; 
	vector<Point*> pointset; 
	Rect* bbox;
	vector<vector<double>> matrix;
	vector<vector<pair<int, double>>> adj; //adj list
	void dijkstra_pointonly(int);
	int vcnt;
	int ecnt;
	map<Point*,int> vconnect;
	void CarrierGraphxpos();
	void CarrierGraphxneg();
	void CarrierGraphypos();
	void CarrierGraphyneg();
	void rightraydivide(Point*);
	void leftraydivide(Point*);
	void upraydivide(Point*);
	void downraydivide(Point*);
	bool ishit(double, double, double);
	bool isclosest(double, double, double);
public:
	CarrierGraph();
	CarrierGraph(vector<Rect*>, vector<Point*>, Rect*, int);
	~CarrierGraph();
	vector<vector<double>> getmatrix();
};
