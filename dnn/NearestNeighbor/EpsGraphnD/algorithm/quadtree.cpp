#include "quadtree.h"

vector<int> dec2bin(int powerNum, int num) {
	vector<int> ret;

	double size = int(myLog2(double(powerNum)));
	ret.assign(size, 0);

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