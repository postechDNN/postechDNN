#pragma once
#include <algorithm>
#include <vector>
#include <utility>
// #include <pair>
// #include <tuple>
#include "Point.h"
#include <iostream>
#include <float.h>

// multilevel

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

// checks if two vectors are equal to each other
// isEqual()

// checks if a point is contained in a bounding box (cell) 
// binary.size() == boundingBox.size()
bool isContained(Point* p, vector<pair<double, double>> boundingBox, vector<int> binary);

typedef class kDQuadTreeNode {

	// variables
public:
	vector<pair<double, double>> boundingBox; // rectangular region
	vector<kDQuadTreeNode*> childNodes;
	// vector<Node*> incidentCells;
	vector<Point*> points; // 리프 노드일 경우만 유효
    bool isLeaf; // 리프 노드 여부
	kDQuadTreeNode* parent; // parent node

	// methods
public:
	// kDQuadTreeNode() { } // childNodes = {}; }
	// kDQuadTreeNode(vector<kDQuadTreeNode*> _childNodes) : childNodes(_childNodes) {}
	kDQuadTreeNode() : isLeaf(false), parent(nullptr) {}
	kDQuadTreeNode(vector<kDQuadTreeNode*> _childNodes, kDQuadTreeNode* _parent = nullptr) 
	: childNodes(_childNodes), isLeaf(false), parent(_parent) {}
	kDQuadTreeNode(vector<Point*> _points, kDQuadTreeNode* _parent = nullptr) 
	: points(_points), isLeaf(true), parent(_parent) {}


}Node;

// typedef class kDQuadTreeLeafNode  : public kDQuadTreeNode {

// 	// variables
// public:
// 	vector<Point*> points;

// 	// methods
// public:
// 	// kDQuadTreeLeafNode() {}
// 	kDQuadTreeLeafNode(vector<Point*> _points) : points(_points) {}

// }LeafNode;



class kDQuadTree {

	// variables
	public:
		int dim; // dimension
		// boundingBox
		// int maxDepth;
		Node* root;

	// methods
	public:
		kDQuadTree(){}
		kDQuadTree(vector<Point*> _points, int _dim, vector<pair<double, double >> _boundingBox, double _eps) : dim(_dim) {
			this->root = build(_points, _dim, _boundingBox, _eps, 0);
		}
			
		// vector<Point*> points = 

		Node* build(vector<Point*> _points, int _dim, vector<pair<double, double>> _boundingBox, double _eps, int _depth, kDQuadTreeNode* parent = nullptr) { // vector<Polytope*>
			
			cout << "current depth: " << _depth << ", # points:" << _points.size() << endl;

			int maxDepth = 2;

			// if (_points.empty()) { cout << "point set empty. return" << endl; return new kDQuadTreeLeafNode({}); }
			// // else if (_depth == maxDepth) { cout << "reached maximum depth of " << _depth << " achieved. return" << endl; return nullptr; }
			// else if (_depth == maxDepth) { cout << "reached maximum depth of " << _depth << " achieved. return" << endl; return new kDQuadTreeLeafNode(_points); }
			// else if (_points.size() == 1) { cout << "Single point. return" << endl; return new kDQuadTreeLeafNode(_points); }

			// 포인트가 비어 있으면 리프 노드 반환
			if (_points.empty()) {
				cout << "point set empty. return" << endl;
				kDQuadTreeNode* leafNode = new kDQuadTreeNode(vector<Point*>{}, parent);
				leafNode->isLeaf = true;
				return leafNode;
			}

			// 최대 깊이 도달
			else if (_depth == maxDepth) {
				cout << "Reached max depth. return" << endl;
				kDQuadTreeNode* leafNode = new kDQuadTreeNode(_points, parent);
				leafNode->isLeaf = true;
				return leafNode;
			}

			// 포인트가 하나만 있으면 리프 노드 반환
			if (_points.size() == 1) {
				cout << "Single point. return" << endl;
				kDQuadTreeNode* leafNode = new kDQuadTreeNode(_points, parent);
				leafNode->isLeaf = true;
				return leafNode;
			}

			vector<Node*> childNodes;

			// child nodes 생성
			int powerNum = 1;
			for (int j = 0; j < _dim; j++) { powerNum *= 2; }

			for (int j = 0; j < powerNum; j++) {
				Node* newNode = new Node();

				vector<int> nowBinary = dec2bin(powerNum, j);

				vector<Point*> nowCellPoints;
				for (auto p : _points) if (isContained(p, _boundingBox, nowBinary)) nowCellPoints.push_back(p);

				vector<pair<double, double>> newBoundingBox;

				for (int axis = 0; axis < _dim; axis++) {

					double axisMin = _boundingBox[axis].first;
					double axisMax = _boundingBox[axis].second;
					double axisMid = (axisMin + axisMax) / 2;

					// 0 for the lower part, 1 for the upper part
					if (nowBinary[axis] == 0) newBoundingBox.push_back(make_pair(axisMin, axisMid));
					else newBoundingBox.push_back(make_pair(axisMid, axisMax));

				}

				 // 재귀적으로 child node 생성
				childNodes.push_back(build(nowCellPoints, _dim, newBoundingBox, _eps, _depth+1));
			}

			kDQuadTreeNode* internalNode = new kDQuadTreeNode(childNodes, parent);
    		internalNode->isLeaf = false;
    		return internalNode;
			// return new Node(childNodes);
		}

};

void buildEpsilonGraph();
Node* PointLocation(Node* node, Point* point);
Node* PointDel(Node* node, Point* point);