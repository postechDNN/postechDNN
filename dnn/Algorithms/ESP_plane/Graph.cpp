#include "Graph.h"
#include <vector>

Graph::Graph() {}
Graph::~Graph() {}
Graph::Graph(vector<Point*> _vertices, vector<Edge*> _edges, vector<i_quad*> Qs, int _index) {
	vertices = _vertices;
	edges = _edges;
	num_vertices = vertices.size();
	if (num_vertices > 1) { isin_N = false; }
	else { isin_N = true; }
	num_edges = edges.size();
	Q_iT = Qs;
	index = _index;
}

/*
Graph* operator+(Graph& G1, Graph& G2) {

}
*/

Graph* Graph::operator+(Graph* G) {
	this->vertices.insert(this->vertices.end(), G->vertices.begin(), G->vertices.end());
	this->edges.insert(this->edges.end(), G->edges.begin(), G->edges.end());
	this->num_vertices += G->num_vertices;
	this->num_edges += G->num_edges;
	return this;
}

Point* Graph::getVertex(int i) {
	return vertices[i];
}

int Graph::getNumVertices() {
	return num_vertices;
}

int Graph::getNumEdges() { return num_edges; }

vector<i_quad*> Graph::getQ_ioldT() { return Q_ioldT; }
void Graph::setQ_ioldT(vector<i_quad*> _q) { Q_ioldT = _q; }
i_quad* Graph::getQ_ioldT(int i) { return Q_ioldT[i]; }

vector<i_quad*> Graph::getQ_iT() { return Q_iT; }
void Graph::setQ_iT(vector<i_quad*> _q) { Q_iT = _q; }
void Graph::addQ_iT(vector<i_quad*> _q) { Q_iT.insert(Q_iT.end(), _q.begin(), _q.end()); }
i_quad* Graph::getQ_iT(int i) { return Q_iT[i]; }
void Graph::setIndex(int _index) { index = _index; }
int Graph::getIndex() { return index; }

void Graph::setIsin_N(bool _b) { isin_N = _b; }
bool Graph::getIsin_N() { return isin_N; }

void Graph::addEdge(Edge* E) { this->edges.push_back(E); this->num_edges += 1; }