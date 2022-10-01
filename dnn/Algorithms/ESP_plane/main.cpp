
#include "CS.h"
#include <iostream>
#include <tuple>
#include <vector>
#include <random>

using namespace std;

int main() {

	vector<Point*> vec = { new Point(14.0, -9.0), new Point(15.0, -9.0), new Point(15.0, -8.0), new Point(14.0, -8.0) };
	RP* R = new RP(vec);
	makeDCEL(R);

	vector<vector<Point*>> obs;
	// vector<Point*> ob1 = { new Point(6.1, 8.1), new Point(8.1, 8.1), new Point(8.1, -8.1), new Point(6.1, -8.1) };
	// obs.push_back(ob1);
	Point* s = new Point(0.1, 0.1);
	Point* t = new Point(30.1, 2.1);

	auto CS = conforming_subdivision(s, t, obs); 
	DCEL* D = CS.build_subdivision(); // constructs vertex conforming subdivision
	// DCEL* D_ls = build_ls_subdivision(D); // constructs edge conforming subdivision

	return 0;
}
