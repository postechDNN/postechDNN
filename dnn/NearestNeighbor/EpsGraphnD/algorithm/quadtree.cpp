#include "quadtree.h"

vector<int> dec2bin(int powerNum, int num) {
	vector<int> ret;

	int size = int(myLog2(double(powerNum)));
	for (int i = 0; i < size; i++) ret.push_back(0);
	// ret.assign(size, 0);
	// ret.assign(4, 0);

	int nowDigit = 0;
	while (num > 0) {
		ret[nowDigit] = num % 2;
		num = num / 2;
		nowDigit += 1;
	}

	reverse(ret.begin(), ret.end());

	return ret;
}

// need to test
int bin2dec(vector<int> bin) {
	int ret = 0;

	reverse(bin.begin(), bin.end());

	int factor = 1;

	for (int i = 0; i < bin.size(); i++) {
		ret += bin[i] * factor;
		factor *= 2;
	}
}

// minimum side length of the 
double minCellSize(vector<pair<double, double>>& _boundingBox) {
	double ret = DBL_MAX;
	for (auto slab : _boundingBox) {
		if (slab.second - slab.first < DBL_MAX)
			ret = slab.second - slab.first;
	}

	return ret;
}

bool isContained(Point* p, vector<pair<double, double>> boundingBox, vector<int> binary) {

	for (int axis = 0; axis < binary.size(); axis++) {
		double axisMin = boundingBox[axis].first;
		double axisMax = boundingBox[axis].second;
		double axisMid = (axisMin + axisMax) / 2;

		double axisVal = p->xs[axis];

		if (binary[axis] == 0 && !(axisMin <= axisVal && axisVal <= axisMid)) return false;
		else if (binary[axis] == 1 && !(axisMid < axisVal && axisVal <= axisMax)) return false;
	}

	return true;

}

double myLog2(double num) {
	return log(num) / log(2.0);
}

void buildEpsilonGraph() {
	// debug: check if some nodes are of depth 2, while others are of depth 1

	int dim = 4;
	double val = 128.0;
	// double middleVal = val / 2.0;
	double middleVal = 70.0;

	int pointNum = 1;
	for (int i = 0; i < dim; i++) pointNum *= 2; // 2 cases for each axis

	vector<Point*> points;
	for (int i = 0; i < pointNum; i++) points.push_back(new Point(dim, true));

	for (auto i = 0; i < pointNum; i++) {
		if (i == 0) continue;

		vector<int> nowBinary = dec2bin(pointNum, i);
		for (int j = 0; j < nowBinary.size(); j++) {
			if (nowBinary[j] == 0) points[i]->xs[j] = -1 * middleVal;
			else points[i]->xs[j] = middleVal;
		}
	}

	vector<pair<double, double >> boundingBox;
	for (int i = 0; i < dim; i++) boundingBox.push_back(make_pair(-val, val));

	double eps = 64;

	auto qT = new kDQuadTree(points, 4, boundingBox, eps);

	exit(1);

}