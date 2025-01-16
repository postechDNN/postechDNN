#pragma once

#include <algorithm>
#include <vector>
#include "Point.h"

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

// corr
typedef class KDtreeNode {

// variables
public:
	Point* p;
	// vector<Point*> points;
	KDtreeNode* leftChild;
	KDtreeNode* rightChild;

// methods
public:
	KDtreeNode(){}
	KDtreeNode(Point* _p, KDtreeNode* _leftChild, KDtreeNode* _rightChild) : p(_p), leftChild(_leftChild), rightChild(_rightChild) {}
	// KDtreeNode(vector<Point*> points) {}

}KDN;

class KDtree {

// variables
public:

// methods
public:	
	KDtree(){}
	KDtree(vector<Point*> points, int dim, vector<double>& minVals, vector<double>& maxVals, double eps) {
		// 0차원부터 (d-1)차원까지
		int startDim = 0;
		buildKDtree(points, dim, startDim, minVals, maxVals, eps);
	}

	// void

	// partition the region into cubes, by recursive partitioning until the sidelength is not less then eps
	// (1) rectangular box or (2) cube? for now, we use cubes.
	// whichDim is between 0 and (dim-1). 
	KDtreeNode* buildKDtree(vector<Point*> points, int dim, int whichDim, vector<double>& minVals, vector<double>& maxVals, double eps) {
		
		int sz = points.size();

		if (sz == 0) { return new KDtreeNode(nullptr, nullptr, nullptr); }

		else if (sz == 1) { return new KDtreeNode(points[0], nullptr, nullptr); }

		// 아래와 같이 정의하면 plotting 시, epsilon에 대해 continuous가 아닐텐데?
		// else if ("side length of the cell is less then (eps) = (bounding box size) / 2 ^ (depth)") {}

		else {  
			sort(points.begin(), points.end(), sortByKthAxis(whichDim));
			int midID = points.size() / 2;

			vector<Point*> leftPoints = vectorSlice(points, 0, midID-1);
			vector<Point*> rightPoints = vectorSlice(points, midID + 1, sz-1);

			// axis cycles through all valid values
			whichDim = (whichDim + 1) % dim;

			return new KDtreeNode(points[midID], buildKDtree(leftPoints, dim, whichDim, minVals, maxVals, eps), buildKDtree(rightPoints, dim, whichDim, minVals, maxVals, eps));
		}
	}

};

double distanceBetweenCenters(KDtreeNode* c1, KDtreeNode* c2);

template <typename T>
vector<T*> vectorSlice(vector<T*> inp, int a, int b);

/*
참고: https://en.wikipedia.org/wiki/K-d_tree
 
* function kdtree(list of points pointList, int depth)
{
	// Select axis based on depth so that axis cycles through all valid values
	var int axis : = depth mod k;

	// Sort point list and choose median as pivot element
	select median by axis from pointList;

	// Create node and construct subtree
	node.location : = median;
	node.leftChild : = kdtree(points in pointList before median, depth + 1);
	node.rightChild : = kdtree(points in pointList after median, depth + 1);
	return node;
}

https://www.geeksforgeeks.org/find-median-of-vector-elements-in-cpp/

// C++ Program to calculate the median of a vector of
// integers
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// Function for calculating median
double Median(vector<int> v, int n)
{
	// Sort the vector
	sort(v.begin(), v.end());

	// Check if the number of elements is odd
	if (n % 2 != 0)
		return (double)v[n / 2];

	// If the number of elements is even, return the average
	// of the two middle elements
	return (double)(v[(n - 1) / 2] + v[n / 2]) / 2.0;
}

int main()
{
	// initializing vector
	vector<int> v = { 5, 7, 4, 2, 6, 2, 8, 3 };
	// finding size of vector
	int N = v.size();
	// print the median
	cout << "Median = " << Median(v, N) << endl;
	return 0;
}
*/
