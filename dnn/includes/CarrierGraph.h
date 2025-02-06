#pragma once
#include"DCEL/Edge.h"
#include"Rect.h"
#include<queue>
#include<vector>
#include<map>

class CarrierGraph {
protected:
	enum constant { BOUNDINGBOX = -1 }; // constant number, {index of bbox}
	const double INF = 2147483647.0; // sufficiently large value
	vector<Edge*> elist; // list of edges in carrier
	vector<Point*> vlist; // list of vertices in carrier
	vector<Rect*> rectset; // list of rectangles
	vector<Point*> pointset; // list of points
	Rect* bbox; // bounding box
	vector<vector<double>> matrix; // matrix for shortest path (temporary supports, later to be deleted)
	vector<vector<pair<int, double>>> adj; //simplified adjacent list
	void dijkstra_pointonly(int); // private method: Dijkstra with simplified adjacent list
	int vcnt; // # of vcnt
	int ecnt; // # of ecnt
	map<Point*,int> vconnect; // information mapping vertex and index
	void CarrierGraphxpos(); // construct CarrierGraph as G_x+
	void CarrierGraphxneg(); // construct CarrierGraph as G_x-
	void CarrierGraphypos(); // construct CarrierGraph as G_y+
	void CarrierGraphyneg(); // construct CarrierGraph as G_y-
	void rightraydivide(Point*); // Edge seperation by right ray from point
	void leftraydivide(Point*); // Edge seperation by left ray from point
	void upraydivide(Point*); // Edge seperation by upper ray from point
	void downraydivide(Point*); // Edge seperation by lower ray from point
	bool ishit(double, double, double); // simple method for detect hitting
	bool isclosest(double, double, double); // simple method for computing closest edge
public:
	CarrierGraph(); // constructor
	CarrierGraph(vector<Rect*>, vector<Point*>, Rect*, int); // constructor with rectangles, points, and direction of carrier graph.
	~CarrierGraph(); // destructor
	vector<vector<double>> getmatrix(); // get shortest path matrix (temporary supports, later to be deleted)
};
