#pragma once
#include"Edge.h"
#include"Point.h"
#include"Rectangle.h"
#include<queue>
#include<vector>
using namespace std;

class Cgraph {
public:
	Cgraph();
	Cgraph(vector<Rectangle>, Rectangle, int);
	int edgecnt();
	int vertexcnt();
	Edge* getedge(int);
	Point* getvertex(int);
	vector<vector<double>> getmatrix();
private:
	vector<Edge*> elist; // list of edges in carrier
	vector<Point*> vlist; // list of vertices in carrier
	vector<vector<double>> matrix;
//	vector<vector<double>> adj; //adj mat
	vector<vector<pair<int,double>>> adj; //adj list
	vector<double> Dijkstra(int);
	int vcnt;
	void Cgraphxpos(vector<Rectangle>, Rectangle);
	void Cgraphxneg(vector<Rectangle>, Rectangle);
	void Cgraphypos(vector<Rectangle>, Rectangle);
	void Cgraphyneg(vector<Rectangle>, Rectangle);
};
