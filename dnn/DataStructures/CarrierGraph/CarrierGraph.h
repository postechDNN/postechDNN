#pragma once
#include"../../DataStructures/DCEL/Edge.h"
#include"Rect.h"
#include<queue>
#include<vector>
#include<map>

class CarrierGraph {
protected:
	enum constant { BOUNDINGBOX = -1 ,XPOS,XNEG,YPOS,YNEG};
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
	int dir;
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
	bool intersect(Rect*, Rect*);
	bool intersect(Rect*, Point*);
public:
	CarrierGraph();
	CarrierGraph(vector<Rect*>, vector<Point*>, Rect*, int);
	~CarrierGraph();
	int insertion(vector<Rect*>, vector<Point*>);
	int insertion(vector<Point*>);
	int insertion(vector<Rect*>);
	vector<vector<double>> getmatrix();
};
