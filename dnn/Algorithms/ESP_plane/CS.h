#pragma once
// #include "Edge.h"
#include "Graph.h"
#include "i_box.h"
// #include "i_quad.h"
// #include <vector>
#include "RP.h"
#include <list>
#include "./../../DataStructures/Delaunay_Linf/delaunay.h"
#include "./../../DataStructures/DCEL/DCEL_operation.h"
#include "./../../DataStructures/DCEL/DCEL.h"
#include "./../../DataStructures/PointLocation/Location.h"
#include "./../../DataStructures/DCEL/Vector.h"
// #include "basic.h"

using namespace std;

// typedef struct Subdivision Subdivision;

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
	public: // ����
		list<i_quad*> Q_pointer;
		vector<Point*> sources; // Point* dst;
		vector<vector<Point*>> obstacles;
		vector<i_quad*> Q_old, Q_i; // set of i-quads // Q_im4
		vector <Graph*> MSF_old, MSF_i;
		// Subdivision S;
		int rp_index; // RectPoly_index

	public: // �Լ�
		conforming_subdivision();
		~conforming_subdivision();
		conforming_subdivision(vector<Point*>, vector<vector<Point*>>);

		// for build_subdivision
		bool contained(i_quad*, i_quad*);
		i_quad* bigger_quad(i_quad*, i_quad*);
		i_quad* bigger_quad(i_quad*);
		vector<pair<i_quad*, i_quad*>> construct_graph(vector<i_quad*>);
		pair<vector<pair<i_quad*, i_quad*>>, vector<i_quad*>> maximal_matching(vector<i_quad*>, vector<pair<i_quad*, i_quad*>>);
		vector<i_quad*> growth(vector<i_quad*>); // growth(S)
		bool overlap(i_quad*, i_quad*);
		bool about_to_merge(i_quad*); // about to merge
		pair<vector<vector<int>>, vector<i_quad*>> equiv_classes(vector<i_quad*>);


		// for buildSPM
		void compute_aw(HEdge*);
		double compute_sdist(HEdge*);
		//double compute_eg(HEdge*);
		void update_covertime(HEdge*);
		void marking(DCEL*); // 
		//Vertex* claim(HEdge*);
		//Vertex* claim(Vertex*);
		bool is_endpoint_claim(Vertex*, HEdge*);
		bool is_part_claim(Vertex*, HEdge*);

		DCEL* build_subdivision(); // subdivision : set of line segments. datatype : vector<line_segment>
		DCEL* build_ls_subdivision(DCEL*);
		
		priority_queue<pair<double, Vertex*>> multisource_init(DCEL*, vector<Point*>);
		void propagation(DCEL*,  priority_queue<pair<double, Vertex*>>); // wavefront propagation phase
		void buildSPM(DCEL*); // map computation phase. when constructing it, it uses infos obtained from propagation

		// 
	};

int myPow(int x, unsigned int p);
int my_find(int x, int* root, int count);
int my_union(int x, int y, int* root, int* rank, int count);
double my_log2(double x);

