#pragma once
#include <algorithm>
#include <vector>
#include <utility>
#include <iostream>
#include <float.h>

#include "Point.h"
#include "Macro.h"
#include "CPolytope.h"

using namespace std;

class sortByKthAxis {

	// variables
public:
	int axis;

	// methods
public:

	sortByKthAxis(int k) : axis(k) {}

	bool operator()(Point* p1, Point* p2) {
		return p1->xs[axis] < p2->xs[axis];
	}

};

double minCellSize(vector<pair<double, double>>& _boundingBox);

double myLog2(double num);

// ex) 14 -> {1, 1, 1, 0}
// need to test
vector<int> dec2bin(int powerNum, int num);

// need to test
int bin2dec(vector<int> bin);

bool isContained(Point* p, vector<pair<double, double>> boundingBox, vector<int> binary);

typedef class kDQuadTreeNode {

	// variables
public:

	int numNodesSubtree; // number of nodes (not points) in the subtree rooted at this node

	vector<pair<double, double>> boundingBox; // rectangular region
	vector<kDQuadTreeNode*> childNodes;
	// vector<Node*> incidentCells;
	vector<Point*> points; // 리프 노드일 경우만 유효

    bool isLeaf; // 리프 노드 여부
	// bool isRoot; // 루트 노드 여부는 parent == nullptr인지를 통해서 확인 가능.

	kDQuadTreeNode* parent; // parent node
	vector<Point*> spreadPoints; // kNN을 위해 생성하는 local graph의 노드에 해당하는 점들

	vector<kDQuadTreeNode*> incidentNodes;

	// methods
public:
	kDQuadTreeNode() : isLeaf(false), parent(nullptr), numNodesSubtree(0) {} // 기본 생성자 - 사용 여부?

	kDQuadTreeNode(vector<kDQuadTreeNode*> _childNodes, kDQuadTreeNode* _parent = nullptr) // for internal node
	: childNodes(_childNodes), isLeaf(false), parent(_parent), numNodesSubtree(0) {} // numNodesSubtree(1 + _childNodes.size())
	
	kDQuadTreeNode(vector<Point*> _points, kDQuadTreeNode* _parent = nullptr) // for leaf node
	: points(_points), isLeaf(true), parent(_parent), numNodesSubtree(0) {} // numNodesSubtree(1)

	void updateNumNodesSubtree();


}Node;

class kDQuadTree {

	// variables
	public:
		int dim; // dimension
		Node* root;
		vector<CPolytope*> pols;

	// methods
	public:
		kDQuadTree(){}
		kDQuadTree(vector<Point*> _points, vector<CPolytope*> _pols, int _dim, vector<pair<double, double >> _boundingBox, double _eps) : pols(_pols), dim(_dim) {
			
			// insert polytope vertices into _points
			for (auto& pol : _pols) {
				for (auto& v : pol->vertices) {
					v.isPolytopeVertex = true;
					_points.push_back(&v);
				}
			}

			this->root = build(_points, _dim, _boundingBox, _eps, 0);
		}
		
		// CPolytope square2D(2, vertices2D, facets2D);

		Node* build(vector<Point*> _points, int _dim, vector<pair<double, double>> _boundingBox, double _eps, int _depth, kDQuadTreeNode* parent = nullptr);
		
		vector<pair<double, Point*>> kNN(Point* query, int k);
};

void buildEpsilonGraph();

Node* pointLocation(Node* node, Point* point);

Node* addPoint(Node* node, Point* point, int maxDepth);
Node* deletePoint(Node* node, Point* point);

void buildPointGraphOnQuadTree(kDQuadTree* quadtree);

// node를 root로 하는 subtree 내의 모든 node에 numPoints 개의 점을 무작위로 찍기
// void spreadPoints(Node* node, int dim, int numPoints = numSpreadPoints);
void spreadPoints(Node* node, int dim, int numPoints = 10);

void constructLocalGraph(Node* root, int dim);

Point* generateRandomPoint(int dim, vector<pair<double, double>> boundingBox);

Point* generateRandomPoint(int dim, pair<double, double> boundingBox);