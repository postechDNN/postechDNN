#pragma once
// #include "Edge.h"
#include "Graph.h"
#include "i_box.h"
// #include "i_quad.h"
// #include <vector>
#include "RP.h"
#include <list>
#include "./../../DataStructures/Delaunay_Linf/delaunay.h"
// #include "basic.h"

using namespace std;
typedef struct Subdivision Subdivision;

/*
double operator-(const Point& a, const Point& b);
bool sortbylength(Edge& a, Edge& b);
bool sortbyXIncr(i_quad& a, i_quad& b);
bool sortbyYIncr(i_quad& a, i_quad& b);
int my_find(int x, int* root, int count);
int my_union(int x, int y, int* root, int* rank, int count);
*/

/*
struct Subdivision{
	vector<Face*> FCs; // faces
	vector<Edge*> EGs; // segments
};
*/

class conforming_subdivision {
public: // 변수
	list<i_quad*> Q_pointer;
	Point* src; Point* dst;
	vector<Point*> obstacle_vertices;
	vector<i_quad*> Q_old, Q_i; // set of i-quads // Q_im4
	vector <Graph*> MSF_old, MSF_i;
	// Subdivision S;
	int rp_index; // RectPoly_index

public: // 함수
	conforming_subdivision();
	~conforming_subdivision();
	conforming_subdivision(Point*, Point*, vector<Point*>);
	bool contained(i_quad*, i_quad*);
	i_quad* bigger_quad(i_quad*, i_quad*);
	i_quad* bigger_quad(i_quad*);
	vector<pair<i_quad*, i_quad*>> construct_graph(vector<i_quad*>);
	pair<vector<pair<i_quad*, i_quad*>>, vector<i_quad*>> maximal_matching(vector<i_quad*>, vector<pair<i_quad*, i_quad*>>);
	vector<i_quad*> growth(vector<i_quad*>); // growth(S)
	bool overlap(i_quad*, i_quad*);
	int interior(i_quad*, char);
	bool is_simple(i_quad*);
	bool about_to_merge(i_quad*); // about to merge
	void draw_SPBD(i_quad*);
	// bool contained(i_box, vector<core>);
	bool contained(i_box, vector<i_quad>);
	void draw_CXBD(i_box);
	// void fill_R1R2(vector<core>, vector<i_quad>);
	// void fill_R1S(vector<core>, vector<i_quad>);
	void build_subdivision(); // subdivision : set of line segments. datatype : vector<line_segment>
	pair<vector<vector<int>>, vector<i_quad*>> equiv_classes(vector<i_quad*>);
	vector<tuple<int, int, double>> delaunay(Graph*);
};

int myPow(int x, unsigned int p);
int my_find(int x, int* root, int count);
int my_union(int x, int y, int* root, int* rank, int count);
double my_log2(double x);