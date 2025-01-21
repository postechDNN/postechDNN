#pragma once
#include <algorithm>
#include <vector>
#include <utility>
// #include <pair>
// #include <tuple>
#include "Point.h"
#include <iostream>

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

typedef class kDQuadTreeNode {

	// variables
public:
	vector<pair<double, double>> boundingBox; // rectangular region
	vector<kDQuadTreeNode*> childNodes;
	// vector<Node*> incidentCells;

	// methods
public:
	kDQuadTreeNode() { } // childNodes = {}; }
	kDQuadTreeNode(vector<kDQuadTreeNode*> _childNodes) : childNodes(_childNodes) {}

}Node;

typedef class kDQuadTreeLeafNode  : public kDQuadTreeNode {

	// variables
public:
	vector<Point*> points;

	// methods
public:
	// kDQuadTreeLeafNode() {}
	kDQuadTreeLeafNode(vector<Point*> _points) : points(_points) {}

}LeafNode;

// checks if a point is contained in a bounding box (cell) 
// binary.size() == boundingBox.size()
bool isContained(Point* p, vector<pair<double, double>> boundingBox, vector<int> binary);

class kDQuadTree {

	// variables
	public:
		int dim; // dimension
		// boundingBox
		Node* root;

	// methods
	public:
		kDQuadTree(){}
		kDQuadTree(vector<Point*> _points, int _dim, vector<pair<double, double >> _boundingBox, double _eps) : dim(_dim) {
			this->root = build(_points, _dim, _boundingBox, _eps, 0);
		}
			
		// vector<Point*> points = 

		Node* build(vector<Point*> _points, int _dim, vector<pair<double, double>> _boundingBox, double _eps, int _depth) { // vector<Polytope*>
			
			cout << "current depth: " << _depth << ", # points:" << _points.size() << endl;

			int maxDepth = 2;

			// 구역 내에 point가 없으면 빈 노드를 반환
			if (_points.empty()) { cout << "point set empty. return" << endl; return new kDQuadTreeLeafNode({}); }
			else if (_depth == maxDepth) { cout << "reached maximum depth of " << _depth << " achieved. return" << endl; return nullptr; }

			vector<Node*> childNodes;

			// child nodes들 먼저 만듬
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

				 // 순서대로 잘 넣어야 함
				childNodes.push_back(build(nowCellPoints, _dim, newBoundingBox, _eps, _depth+1));
			}

			return new Node(childNodes);
		}

};

void buildEpsilonGraph();