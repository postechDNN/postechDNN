#pragma once
// #include "Point.h"
#include "Edge.h"
// #include <vector>
#include "i_quad.h"
#include <set>

using namespace std;

	class Graph {
	public:
		vector<Point*> vertices;
		vector<set<int>> adj_list;
		// vector<vector<bool>> mat;
		vector<Edge*> edges;
		int num_vertices;
		int num_edges;
		vector<i_quad*> Q_old;
		vector<i_quad*> Q_iT;
		int index;
		bool isin_N;
	public:
		Graph();
		~Graph();
		Graph(vector<Point*>, vector<Edge*>, vector<i_quad*>, int);
		Graph* operator+(Graph*);
		// friend Graph* operator+(Graph&, Graph&);
		Point* getVertex(int i);
		int getNumVertices();
		int getNumEdges();
		void setQ_old(vector<i_quad*>);
		vector<i_quad*> getQ_old();
		i_quad* getQ_old(int);
		void setQ_iT(vector<i_quad*>);
		void addQ_iT(vector<i_quad*>);
		vector<i_quad*> getQ_iT();
		i_quad* getQ_iT(int);
		void setIndex(int);
		int getIndex();
		void setIsin_N(bool);
		bool getIsin_N();
		void addEdge(Edge* E);

		Graph(vector<Point*>);
		Graph(vector<Point*>, vector<set<int>>);
		void addEdge(int,int);
		void deleteEdge(int,int);
		void print();
	};